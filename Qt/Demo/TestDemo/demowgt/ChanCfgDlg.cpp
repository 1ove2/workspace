#include "ChanCfgDlg.h"
#include "ui_ChanCfgDlg.h"

ChanCfgDlg::ChanCfgDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChanCfgDlg)
{
    ui->setupUi(this);
}

ChanCfgDlg::~ChanCfgDlg()
{
    delete ui;
}
