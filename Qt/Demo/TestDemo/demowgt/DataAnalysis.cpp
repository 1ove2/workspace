#include "DataAnalysis.h"
#include "ui_DataAnalysis.h"

DataAnalysis::DataAnalysis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataAnalysis)
{
    ui->setupUi(this);
}

DataAnalysis::~DataAnalysis()
{
    delete ui;
}
