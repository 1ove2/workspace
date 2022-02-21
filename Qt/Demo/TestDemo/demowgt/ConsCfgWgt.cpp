#include "ConsCfgWgt.h"
#include "ui_ConsCfgWgt.h"

ConsCfgWgt::ConsCfgWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConsCfgWgt)
{
    ui->setupUi(this);
}

ConsCfgWgt::~ConsCfgWgt()
{
    delete ui;
}
