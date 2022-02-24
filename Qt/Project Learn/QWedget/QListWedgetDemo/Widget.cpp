#include "Widget.h"
#include "ui_Widget.h"
#include <QListWidgetItem>
#include <QPushButton>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    pListWgt(nullptr)
{
    ui->setupUi(this);
    this->resize(500,400);
    initWgt();
}

Widget::~Widget()
{
    delete ui;
    if(pListWgt != nullptr)
    {
        delete pListWgt;
        pListWgt = nullptr;
    }
}

void Widget::initWgt()
{
    pListWgt = new QListWidget(this);
    pListWgt->resize(200,300);
    pListWgt->move(100,50);
    pListWgt->setFont(QFont("宋体",14));
    // 1. 添加字符串项
    pListWgt->addItem("addItem: string");

    // 2. 添加其他特殊项
    QListWidgetItem* item = new QListWidgetItem(pListWgt);
    QPushButton* btn = new QPushButton();
    QSize size = item->sizeHint();// 获取item的尺寸
    item->setSizeHint(QSize(size.width(),40));
    btn->setIcon(QIcon(":/icon/AddInst.svg"));
    btn->setIconSize(QSize(30, 30));
    // 去掉按钮边框，让按钮和背景色融为一体
    btn->setFlat(true);
    //上面setFlat表面上可以，但是点击按钮时还是会出现原来按钮背景，想要消除可以使用下面QSS
    btn->setStyleSheet("background-color:transparent");
    pListWgt->addItem(item);
    pListWgt->setItemWidget(item, btn);
}
