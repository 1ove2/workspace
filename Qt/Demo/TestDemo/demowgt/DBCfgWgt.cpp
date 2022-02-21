#include "DBCfgWgt.h"
#include "ui_DBCfgWgt.h"

DBCfgWgt::DBCfgWgt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBCfgWgt)
{
    ui->setupUi(this);
}

DBCfgWgt::~DBCfgWgt()
{
    delete ui;
}
