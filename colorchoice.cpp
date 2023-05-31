#include "colorchoice.h"
#include "ui_colorchoice.h"

Colorchoice::Colorchoice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Colorchoice)
{
    ui->setupUi(this);
    //将按钮与槽函数连接
    connect(ui->pushButton,&QPushButton::clicked,this,&Colorchoice::chooseColor);
}

Colorchoice::~Colorchoice()
{
    delete ui;
}
void Colorchoice::chooseColor()
{
    QColor color=QColorDialog::getColor(Qt::white,this-,"选择颜色");//弹出颜色对话框
    if(color.isValid())
    {
        //修改按钮的背景色
        QString styleSheet = QString("background-color:%1").arg(color.name());
        ui->pushButton->setStyleSheet(styleSheet);
    }
}
