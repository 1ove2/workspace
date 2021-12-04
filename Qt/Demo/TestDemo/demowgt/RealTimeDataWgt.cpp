#include "RealTimeDataWgt.h"
#include "ui_RealTimeDataWgt.h"

RealTimeDataWgt::RealTimeDataWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RealTimeDataWgt)
{
    ui->setupUi(this);
}

RealTimeDataWgt::~RealTimeDataWgt()
{
    delete ui;
}
