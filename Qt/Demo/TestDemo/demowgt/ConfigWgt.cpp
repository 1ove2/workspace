#include "ConfigWgt.h"
#include "ui_ConfigWgt.h"

ConfigWgt::ConfigWgt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigWgt)
{
    ui->setupUi(this);
}

ConfigWgt::~ConfigWgt()
{
    delete ui;
}
