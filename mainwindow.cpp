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
#include <QColorDialog>

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


void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.begin(this);
    QPen pen(penColor);
    pen.setWidth(3);
    painter.setPen(pen);//设置画笔

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

void MainWindow::on_actionSector_triggered()
{
    currentState = new SectorState();
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

void MainWindow::on_actionColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::red, this,tr("颜色对话框"),QColorDialog::ShowAlphaChannel);
    if(color.isValid())
    {
        penColor = color;
    }
}

void MainWindow::on_actionDashLine_triggered()
{
    pen.setStyle(Qt::DashLine);
    update();
}


void MainWindow::on_actionDotLine_triggered()
{
    pen.setStyle(Qt::DotLine);
    update();

}
void MainWindow::on_actionDashDotLine_triggered()
{
    pen.setStyle(Qt::DashDotLine);
    update();
}

void MainWindow::on_actionDashDotDotLine_triggered()
{
    pen.setStyle(Qt::DashDotDotLine);
    update();
}

void MainWindow::on_actionCustomDashLine_triggered()
{
    pen.setStyle(Qt::CustomDashLine);
    update();
}

void MainWindow::on_actionSolidLine_triggered()
{
    pen.setStyle(Qt::SolidLine);
    update();
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
            QPoint p1 = window->points[0];
            QPoint p2 = event->pos();
            QRect rect(p1, p2);

            QPainter painter(window);
            painter.drawRect(rect);

            window->shapes.push_back(rect);
            window->points.clear();
        }
    }
}

void RectangleState::paintEvent(QPaintEvent *event, QPainter &painter, std::vector<QPoint> points, QPoint topLeft)
{
    Q_UNUSED(event);
    Q_UNUSED(points);

    if (!topLeft.isNull())
    {
        int x1 = std::min(topLeft.x(), currentPos.x());
        int y1 = std::min(topLeft.y(), currentPos.y());
        int x2 = std::max(topLeft.x(), currentPos.x());
        int y2 = std::max(topLeft.y(), currentPos.y());

        QRect rect(x1, y1, x2 - x1, y2 - y1);
        painter.drawRect(rect);
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

void PolygonState::mousePressEvent(QMouseEvent *event, MainWindow *window)//按下
{
    if (event->button() == Qt::LeftButton)
    {
        if (edgeNum == 0)
        {
            //更新左上角顶点和边数
            topLeft = event->pos();
            edgeNum++;
        }
        else
        {
            //存储多边形的点，并更新边数
            QPoint point(event->pos().x(), event->pos().y());
            window->points.push_back(point);
            edgeNum++;
        }
    }
}
void PolygonState::paintEvent(QPaintEvent *event, QPainter &painter, std::vector<QPoint> points, QPoint /*topLeft*/)//绘制
{


    //绘制已经保存的点与顶点之间的连线，准备绘制新线段
    for (unsigned int i = 1; i < points.size(); i++)
    {
        painter.drawLine(points[i - 1], points[i]);
    }

    //绘制新的线段
    if (edgeNum > 1)
    {
        painter.drawLine(points.back(), QPoint(topLeft.x(), topLeft.y()));
    }

    //如果已经存储了三个点，则将最后一个点连接到第一个点以形成完整的多边形
    if (edgeNum > 2)
    {
        painter.drawLine(points.back(), points.front());
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

void SectorState::mousePressEvent(QMouseEvent *event, MainWindow *window)
{
    if (event->button() == Qt::LeftButton)
    {
        // 记录圆心坐标
        centerPoint = event->pos();
        // 更新状态
        window->currentState = this;
    }
}

void SectorState::paintEvent(QPaintEvent *, QPainter &painter, std::vector<QPoint> points, QPoint)
{
    // 先绘制已保存点和线段
    for (unsigned int i = 1; i < points.size(); i++)
    {
        painter.drawLine(points[i - 1], points[i]);
    }

    // 绘制圆心
    painter.drawPoint(centerPoint);

    // 绘制圆弧
    if (spanAngle != 0)
    {
        QRectF rectangle(centerPoint.x() - radius, centerPoint.y() - radius, radius * 2, radius * 2);
        painter.drawPie(rectangle, startAngle, spanAngle);
    }
}

void SectorState::mouseMoveEvent(QMouseEvent *event, MainWindow *window)
{
    if (window->currentState == this)
    {
        // 计算半径和扇形角度
        qreal dx = event->pos().x() - centerPoint.x();
        qreal dy = event->pos().y() - centerPoint.y();
        radius = sqrt(dx * dx + dy * dy);
        startAngle = std::atan2(dy, dx) * 180 / M_PI;
        spanAngle = 90; // 暂时设置为 90 度
        //将当前点加入points
        QPoint currentPoint = event->pos();
        points.push_back(currentPoint);
        // 重绘窗口
        window->update();
    }
}

void SectorState::mouseReleaseEvent(QMouseEvent *, MainWindow *window)
{
    // 更新状态，保存当前的圆弧
    window->currentState = new SelectState();
    //根据起始点，结束点和圆心点来计算扇形的三个角度参数
    QPoint endPoint = points.back();
    qreal dx = endPoint.x()-centerPoint.x();
    qreal dy = endPoint.y()-centerPoint.y();
    qreal endAngle = std::atan2(dy,dx)*180/M_PI;
    if(span<0)
    {
        span+=360;
    }
    window->shapes.push_back(new ArcShape(centerPoint, radius, startAngle, spanAngle));
}

