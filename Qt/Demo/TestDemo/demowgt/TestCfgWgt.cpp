#include "TestCfgWgt.h"
#include "ui_TestCfgWgt.h"

TestCfgWgt::TestCfgWgt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestCfgWgt)
{
    ui->setupUi(this);
}

TestCfgWgt::~TestCfgWgt()
{
    delete ui;
}
