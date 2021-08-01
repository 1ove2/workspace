#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    bt = new mybtn(this);
    bt->setText("hello");
    bt->setGeometry((this->width() - 120)/2,(this->height()-50)/2,120,50);
}

Widget::~Widget()
{
    delete ui;
}

