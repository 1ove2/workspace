#include "Customplot.h"
#include "ui_Customplot.h"

Customplot::Customplot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Customplot)
{
    ui->setupUi(this);
    mcurve = qobject_cast<Curve*>(ui->wgt_plot);
}

Customplot::~Customplot()
{
    delete ui;
}
