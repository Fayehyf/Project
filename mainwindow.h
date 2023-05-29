#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <vector>
#include <algorithm>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class State;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    State *currentState;
    void setPaintFlag(bool flag);
    void setPainter(QPainter *painter);
    void addShape(const QRect& rect);  // 添加矩形到 shapes 成员变量中
    QVector<QRect> shapes;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_actionPoint_triggered();
    void on_actionCircle_triggered();
    void on_actionRectangle_triggered();
    void on_actionPolygon_triggered();
    void on_actionLine_triggered();
    void on_actionClear_All_triggered();
    void on_actionUndo_triggered();
    void on_actionDecide_it_triggered();
    void on_actionTranslation_2_triggered();
    void on_actionZoom_2_triggered();
    void on_actionDelete_2_triggered();

public:
    Ui::MainWindow *ui;
    std::vector<QPoint> points;
    QPoint topLeft;


    bool isSelecting;//whether the box is being selected
    QRect selectionRect;//the rectangle used to store the box selection
};

class State
{
public:
    enum Type
    {

        None,
        Drawing,
        Moving
    };
    virtual void mousePressEvent(QMouseEvent *event, MainWindow *window) = 0;
    virtual void paintEvent(QPaintEvent *event, QPainter &painter, std::vector<QPoint> points, QPoint topLeft)=0;

};

class PointState : public State
{
public:
    void mousePressEvent(QMouseEvent *event, MainWindow *window);
    void paintEvent(QPaintEvent *event, QPainter &painter, std::vector<QPoint> points, QPoint topLeft);
};

class CircleState : public State
{
public:
    void mousePressEvent(QMouseEvent *event, MainWindow *window);
    void paintEvent(QPaintEvent *event, QPainter &painter, std::vector<QPoint> points, QPoint topLeft);
};

class RectangleState : public State
{
public:
    void mousePressEvent(QMouseEvent *event, MainWindow *window);
    void paintEvent(QPaintEvent *event, QPainter &painter, std::vector<QPoint> points, QPoint topLeft);
    void mouseMoveEvent(QMouseEvent *event,MainWindow *window);
    void mouseReleaseEvent(QMouseEvent *event,MainWindow *window);
    QPoint topLeft;
    QPoint currentPos;
};

class PolygonState : public State
{
private:
    int edgeNum;
public:
    PolygonState() :edgeNum(0) {}
    void mousePressEvent(QMouseEvent *event, MainWindow *window);
    void paintEvent(QPaintEvent *event, QPainter &painter, std::vector<QPoint> points, QPoint topLeft);
};

class LineState : public State
{
public:
    void mousePressEvent(QMouseEvent *event, MainWindow *window);
    void paintEvent(QPaintEvent *event, QPainter &painter, std::vector<QPoint> points, QPoint topLeft);
};

#endif // MAINWINDOW_H

