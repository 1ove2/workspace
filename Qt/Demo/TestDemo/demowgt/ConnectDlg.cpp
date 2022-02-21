#include "ConnectDlg.h"
#include "ui_ConnectDlg.h"

ConnectDlg::ConnectDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDlg)
{
    ui->setupUi(this);
}

ConnectDlg::~ConnectDlg()
{
    delete ui;
}
