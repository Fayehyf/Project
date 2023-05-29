#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include<QLabel>
#include<QScreen>
#include <QGuiApplication>
#include<QtGui>
#include<QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <algorithm>
// define canvas size
const int CANVAS_WIDTH = 800;
const int CANVAS_HEIGHT = 600;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    // initialize currentState
    currentState = NULL;
    setFixedSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    setMinimumSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    isSelecting = false;



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addShape(const QRect& rect)
{
    shapes.push_back(rect);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.begin(this);
    QPen pen(Qt::red);
    pen.setWidth(5);
    painter.setPen(pen);

    // draw all saved points
    for (int i = 0; i < points.size(); i++)
    {
        painter.drawPoint(points[i]);
    }

    // calls the drawing function for the current state
    if (currentState != NULL)
    {
        currentState->paintEvent(nullptr, painter, points, topLeft);
    }
    if(isSelecting)
    {
        QBrush brush(QColor(0,0,0,100));
        painter.setBrush(brush);
        pen.setColor(Qt::blue);
        pen.setStyle(Qt::DashLine);
        painter.setPen(pen);
        painter.drawRect(selectionRect);

    }

    painter.end();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (currentState != NULL)
    {
        currentState->mousePressEvent(event, this);
    }
    if(event->button()==Qt::LeftButton&&event->type()==QEvent::MouseButtonPress)
    {
        isSelecting=true;
        selectionRect.setTopLeft(event->pos());
        selectionRect.setBottomRight(event->pos());
    }

    // redraw the window interface
    update();
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (isSelecting)
    {
        selectionRect.setBottomRight(event->pos());
        update();
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    isSelecting = false;
    // do something with the selected region, for example, print its coordinates
    qDebug() << "Selected region: (" << selectionRect.topLeft().x() << "," << selectionRect.topLeft().y() << ") to ("
            << selectionRect.bottomRight().x() << "," << selectionRect.bottomRight().y() << ")";
    update();
}

void MainWindow::on_actionPoint_triggered()
{
    currentState = new PointState();
}

void MainWindow::on_actionCircle_triggered()
{
    currentState = new CircleState();
}

void MainWindow::on_actionRectangle_triggered()
{
    currentState = new RectangleState();
}

void MainWindow::on_actionPolygon_triggered()
{
    currentState = new PolygonState();
}

void MainWindow::on_actionLine_triggered()
{
    currentState = new LineState();
}

void MainWindow::on_actionClear_All_triggered()
{
    points.clear();
    delete currentState;
    currentState=NULL;
    update();

}

void MainWindow::on_actionUndo_triggered()
{
    if(!points.empty())
    {
        points.pop_back();
    }
    update();
}

void MainWindow::on_actionDecide_it_triggered()
{
    // do something with the selected region, for example, print its coordinates
    qDebug() << "Selected region: (" << selectionRect.topLeft().x() << "," << selectionRect.topLeft().y() << ") to ("
            << selectionRect.bottomRight().x() << "," << selectionRect.bottomRight().y() << ")";

    //disable other drawing functions
    currentState = NULL;
    //enable selection box shadow
    isSelecting = true;

    //redraw the window interface with shadow effect
    update();
}

void MainWindow::on_actionTranslation_2_triggered()
{
    if(selectionRect.isNull())
    {
        qDebug()<<"Please select a region first";

    }
    // disable other drawing functions and selection box shadow
    currentState = NULL;
    isSelecting = false;
    // create a pixmap from the selected region
    QPixmap selectedPixmap = this->grab(selectionRect);

    // create a new label to display the selected region
    QLabel *label = new QLabel();
    label->setPixmap(selectedPixmap);
    label->show();

     // move the pixmap with mouse dragging
    bool isMoving = false;
    QPoint offset;
    QEventLoop loop;
    while (label->isVisible())
    {
        QApplication::processEvents();
        if (QApplication::mouseButtons() == Qt::LeftButton)
        {
            if (!isMoving)
            {
                isMoving = true;
                offset = label->mapFromGlobal(QCursor::pos());
            }
            QPoint newPos = QCursor::pos() - offset;
            label->move(newPos);
        }
        else
        {
            if (isMoving)
            {
                isMoving = false;
            }
        }
    }

}

void MainWindow::on_actionZoom_2_triggered()
{
}

void MainWindow::on_actionDelete_2_triggered()
{
}

void PointState::mousePressEvent(QMouseEvent *event, MainWindow *window)
{
    if (event->button() == Qt::LeftButton && event->type() == QEvent::MouseButtonPress)
    {
        window->points.push_back(event->pos());
    }
}

void PointState::paintEvent(QPaintEvent *event, QPainter &painter, std::vector<QPoint> points, QPoint topLeft)
{
    Q_UNUSED(event);
    Q_UNUSED(topLeft);
    for (int i = 0; i < points.size(); i++)
    {
        painter.drawPoint(points[i]);
    }
}

void CircleState::mousePressEvent(QMouseEvent *event, MainWindow *window)
{
    if (event->button() == Qt::LeftButton && event->type() == QEvent::MouseButtonPress)
    {
        if (window->points.size() == 0)
        {
            window->points.push_back(event->pos());
        }
        else
        {
            double x0 = window->points[0].x();
            double y0 = window->points[0].y();
            double x1 = event->pos().x();
            double y1 = event->pos().y();

            // 中点画圆算法绘制圆形
            double r = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
            int x = 0, y = r;
            int d = 1 - r;
            QPainter painter(window);
            while (x <= y) {

                if (d < 0) {
                    d += 2 * x + 3;
                }
                else {
                    d += 2 * (x - y) + 5;
                    y--;
                }
                x++;

                // 存储圆形上的所有点
                window->points.push_back(QPoint(x0 + x, y0 + y));
                window->points.push_back(QPoint(x0 + x, y0 - y));
                window->points.push_back(QPoint(x0 - x, y0 + y));
                window->points.push_back(QPoint(x0 - x, y0 - y));
                window->points.push_back(QPoint(x0 + y, y0 + x));
                window->points.push_back(QPoint(x0 + y, y0 - x));
                window->points.push_back(QPoint(x0 - y, y0 + x));
                window->points.push_back(QPoint(x0 - y, y0 - x));

                // 绘制圆形
                painter.drawPoint(x0 + x, y0 + y);
                painter.drawPoint(x0 + x, y0 - y);
                painter.drawPoint(x0 - x, y0 + y);
                painter.drawPoint(x0 - x, y0 - y);
                painter.drawPoint(x0 + y, y0 + x);
                painter.drawPoint(x0 + y, y0 - x);
                painter.drawPoint(x0 - y, y0 + x);
                painter.drawPoint(x0 - y, y0 - x);
            }
            window->update();
        }
    }
}

void CircleState::paintEvent(QPaintEvent *event, QPainter &painter, std::vector<QPoint> points, QPoint topLeft)
{
    Q_UNUSED(event);
    Q_UNUSED(topLeft);
    if (points.size() >= 2)
    {
        double x0 = points[0].x();
        double y0 = points[0].y();
        double x1 = points[1].x();
        double y1 = points[1].y();

        // 计算圆心坐标和半径，并绘制圆形
        double r = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
        painter.drawEllipse(QPointF(x0, y0), r, r);
    }
}

void RectangleState::mousePressEvent(QMouseEvent *event, MainWindow *window)
{
    if (event->button() == Qt::LeftButton && event->type() == QEvent::MouseButtonPress)
    {
        if (window->points.size() == 0)
        {
            window->points.push_back(event->pos());
        }
        else
        {
            int width = abs(event->pos().x() - window->points[0].x());
            int height = abs(event->pos().y() - window->points[0].y());
            QPoint bottomRight = QPoint((topLeft.x() + width) > topLeft.x() ? (topLeft.x() + width) : topLeft.x(),
                                         (topLeft.y() + height) > topLeft.y() ? (topLeft.y() + height) : topLeft.y());

            QRect rect(window->points[0],bottomRight);//构造矩形
            QPainter painter(window);
            painter.drawRect(rect);//绘制矩形
            window->addShape(rect);   // 存储矩形到 shapes 变量中
            window->points.clear();
        }
    }
}

void RectangleState::paintEvent(QPaintEvent *event, QPainter &painter, std::vector<QPoint> points, QPoint topLeft)
{
    Q_UNUSED(event);
    Q_UNUSED(points);

    if (!topLeft.isNull())//如果存在矩形左上角的点，则绘制预览矩形
    {
        int width = abs(currentPos.x() - topLeft.x());
        int height = abs(currentPos.y() - topLeft.y());
        QPoint bottomRight = QPoint(std::max(topLeft.x() + width, topLeft.x()), std::max(topLeft.y() + height, topLeft.y())); // 计算矩形右下角的点
        QRect rect(topLeft, bottomRight); // 构造矩形
        painter.drawRect(rect); // 绘制矩形
    }

}

void RectangleState::mouseMoveEvent(QMouseEvent *event, MainWindow *window)
{
    currentPos = event->pos();
    window->update();
}

void RectangleState::mouseReleaseEvent(QMouseEvent *event, MainWindow *window)
{
    Q_UNUSED(event);
    Q_UNUSED(window);
}

void PolygonState::mousePressEvent(QMouseEvent *event, MainWindow *window)
{
    if (event->button() == Qt::LeftButton && event->type() == QEvent::MouseButtonPress)
    {
        if (window->points.size() == 0)
        {
            window->topLeft = event->pos();
        }

        // each click adds a vertex and connects it to the previous point with a line segment
        window->points.push_back(event->pos());
        QPainter painter(window);
        painter.drawLine(window->points.back(), window->points[window->points.size() - 2]);


        // close the polygon when you press the middle or right buttons
        if (event->button() == Qt::MiddleButton || event->button() == Qt::RightButton)
        {
            // close polygon
            painter.drawLine(window->points.back(), window->topLeft);
            window->points.push_back(window->topLeft);
            edgeNum = window->points.size() - 1;
            window->topLeft = QPoint(0, 0);
        }
    }
}
void PolygonState::paintEvent(QPaintEvent *event, QPainter &painter, std::vector<QPoint> points, QPoint topLeft)
{
    if (edgeNum > 0)
    {
        QPointF pointf[100];
        for (int i = 0; i < edgeNum; i++)
        {
            pointf[i] = points[i];
        }
        painter.drawPolygon(pointf, edgeNum);
    }
}


void LineState::mousePressEvent(QMouseEvent *event, MainWindow *window)
{
    if (event->button() == Qt::LeftButton && event->type() == QEvent::MouseButtonPress)
    {
        if (window->points.size() == 0)
        {
            window->points.push_back(event->pos());
        }
        else
        {
            QPainter painter(window);
            // 记录绘制直线的起始和结束点坐标
            int startX = window->points.back().x();
            int startY = window->points.back().y();
            int endX = event->pos().x();
            int endY = event->pos().y();

            // DDA算法计算直线上的所有点，并依次绘制
            int dx = endX - startX;
            int dy = endY - startY;

            float x = startX;
            float y = startY;

            int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

            float xIncrement = dx / (float)steps;
            float yIncrement = dy / (float)steps;

            for (int i = 0; i < steps; i++) {
                x += xIncrement;
                y += yIncrement;
                painter.drawPoint(round(x), round(y));
                window->points.push_back(QPoint(x, y));
            }
            // 绘制直线
            painter.drawLine(window->points.back(), event->pos());
            window->points.push_back(event->pos());
        }
    }
}
void LineState::paintEvent(QPaintEvent *event, QPainter &painter, std::vector<QPoint> points, QPoint topLeft)
{

}
















