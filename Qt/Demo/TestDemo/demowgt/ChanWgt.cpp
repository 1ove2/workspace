#include "ChanWgt.h"
#include "ui_ChanWgt.h"

ChanWgt::ChanWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChanWgt)
{
    ui->setupUi(this);
}

ChanWgt::~ChanWgt()
{
    delete ui;
}
