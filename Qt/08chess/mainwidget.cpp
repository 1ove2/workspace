#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    ui->boadLayout->addWidget(&this->chessmap);
    this->resize(700,500);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.drawPixmap(this->rect(),QPixmap(":/new/image/background.jpg"));
}
