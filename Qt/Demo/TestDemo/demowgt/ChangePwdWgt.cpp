#include "ChangePwdWgt.h"
#include "ui_ChangePwdWgt.h"

ChangePwdWgt::ChangePwdWgt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePwdWgt)
{
    ui->setupUi(this);
}

ChangePwdWgt::~ChangePwdWgt()
{
    delete ui;
}
