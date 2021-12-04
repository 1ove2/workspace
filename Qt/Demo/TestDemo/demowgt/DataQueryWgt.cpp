#include "DataQueryWgt.h"
#include "ui_DataQueryWgt.h"

DataQueryWgt::DataQueryWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataQueryWgt)
{
    ui->setupUi(this);
}

DataQueryWgt::~DataQueryWgt()
{
    delete ui;
}
