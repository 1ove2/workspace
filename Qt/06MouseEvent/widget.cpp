#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    count = 0;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mousePressEvent(QMouseEvent *event){
    qDebug() << "mouse Pressed!";
    qDebug() << event->windowPos();
    points[count] = event->windowPos();
    count++;
}

void Widget::mouseMoveEvent(QMouseEvent *event){
    qDebug() << "mouse Moved!";
}

void Widget::mouseReleaseEvent(QMouseEvent *event){
    qDebug() << "mouse release!";
}

void Widget::mouseDoubleClickEvent(QMouseEvent *event){
    qDebug() << "mouse Double Click";

}

void Widget::paintEvent(QPaintEvent *event){

}

void Widget::on_pushButton_clicked()
{
    QPainter painter(this);
    //用图画
    //painter.drawPixmap(0,0,this->width(),this->height(),QPixmap("D:/1.png"));

    //用笔画
    QPen pen1;
    pen1.setStyle(Qt::DotLine);
    pen1.setColor(Qt::red);
    pen1.setWidth(5);
    painter.setPen(pen1);
    painter.drawLine(QPointF(0,0),QPointF(this->width(),this->height()));

    QPen pen2;
    pen2.setStyle(Qt::SolidLine);
    pen2.setColor(Qt::green);
    pen2.setWidth(10);
    painter.setPen(pen2);
    painter.drawRect(100,100,200,200);

    //用画刷画
    QBrush brush1;
    brush1.setStyle(Qt::CrossPattern);
    brush1.setColor(Qt::gray);
    painter.setBrush(brush1);

    painter.drawPolygon(points,count);
}
