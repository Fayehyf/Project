#ifndef COLORCHOICE_H
#define COLORCHOICE_H
#include <QColorDialog>   //颜色对话框
#include <QWidget>
#include<QPushButton>
namespace Ui {
class Colorchoice;
}

class Colorchoice : public QWidget
{
    Q_OBJECT
private slots:
    void chooseColor();//槽函数，用于弹出颜色对话框

public:
    explicit Colorchoice(QWidget *parent = nullptr);
    ~Colorchoice();

private:
    Ui::Colorchoice *ui;
};

#endif // COLORCHOICE_H
