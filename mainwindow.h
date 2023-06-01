#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <QAbstractGraphicsShapeItem>
#include <vector>
#include <QList>
#include <algorithm>
#include <QGraphicsScene>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class State;
// 自定义功能图元 - 点
class BPointItem : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT

public:
    enum PointType {
        Center = 0, // 中心点
        Edge,       // 边缘点（可拖动改变图形的形状、大小）
        Special     // 特殊功能点
    };

    BPointItem(QAbstractGraphicsShapeItem* parent, QPointF p, PointType type);

    QPointF getPoint() { return m_point; }
    void setPoint(QPointF p) { m_point = p; }

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

public:
    QPointF m_point;
    PointType m_type;
};

// 存放点的容器
class BPointItemList: public QList<BPointItem *>
{
public:
    void setRandColor();
    void setColor(const QColor color);
    void setVisible(bool visible);
};



class BQGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    BQGraphicsScene(QObject *parent = nullptr);

    void startCreate();
    void saveItemToConfig();
    void loadItemToScene();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void updatePoint(QPointF p, QList<QPointF> list, bool isCenter);
    void createFinished();

protected:
    QList<QPointF> m_list;
    bool is_creating_BPolygon;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void setDrawFlag(bool flag); // 设置绘制标志位
    bool getDrawFlag() const;    // 获取绘制标志位

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    State *currentState;
    void setPaintFlag(bool flag);
    void setPainter(QPainter *painter);
    void addShape(const QRect& rect);  // 添加矩形到 shapes 成员变量中
    QVector<QRect> shapes;
    bool drawFlag; // 是否正在绘制矩形的标志位

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
    void on_actionColor_triggered();
    void on_actionDotLine_triggered();
    void on_actionDashLine_triggered();
    void on_actionDashDotLine_triggered();
    void on_actionDashDotDotLine_triggered();
    void on_actionCustomDashLine_triggered();
    void on_actionSolidLine_triggered();
    void on_actionSector_triggered();

public:
    Ui::MainWindow *ui;
    std::vector<QPoint> points;
    QPoint topLeft;
    bool isSelecting;//whether the box is being selected
    QRect selectionRect;//the rectangle used to store the box selection
    QColor penColor;//设置画笔颜色
    QPen pen;
    QPainter painter;
    BQGraphicsScene m_scene;


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

class ArcShape
{
public:
    QPoint center;
    qreal radius;
    int startAngle;
    int spanAngle;

    ArcShape(QPoint c, qreal r, int sa, int ea);
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
    QPoint topLeft;
public:
    PolygonState() :edgeNum(0) {}
    void mousePressEvent(QMouseEvent *event, MainWindow *window);//按下
    void paintEvent(QPaintEvent *event, QPainter &painter, std::vector<QPoint> points, QPoint topLeft);//绘制

};

class LineState : public State
{
public:
    void mousePressEvent(QMouseEvent *event, MainWindow *window);
    void paintEvent(QPaintEvent *event, QPainter &painter, std::vector<QPoint> points, QPoint topLeft);
};






class SectorState : public State
{
public:
    SectorState(qreal x, qreal y, qreal radius, qreal angle);
    void updateAngle();
protected:
    void mousePressEvent(QMouseEvent *event, MainWindow *window);
    void paintEvent(QPaintEvent *event, QPainter &painter, std::vector<QPoint> points, QPoint topLeft);
    void paint(QPainter *painter,QWidget *widget);
public:
    qreal m_angle;
    qreal m_radius;
    QPointF m_center;
    QPointF m_edge;
    BPointItemList m_pointList;
    QPen pen;
    QBrush brush;

};



#endif // MAINWINDOW_H

