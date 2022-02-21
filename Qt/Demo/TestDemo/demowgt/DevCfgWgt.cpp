#include "DevCfgWgt.h"
#include "ui_DevCfgWgt.h"

DevCfgWgt::DevCfgWgt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DevCfgWgt)
{
    ui->setupUi(this);
}

DevCfgWgt::~DevCfgWgt()
{
    delete ui;
}
