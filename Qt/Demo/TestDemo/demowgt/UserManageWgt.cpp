#include "UserManageWgt.h"
#include "ui_UserManageWgt.h"

UserManageWgt::UserManageWgt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserManageWgt)
{
    ui->setupUi(this);
}

UserManageWgt::~UserManageWgt()
{
    delete ui;
}
