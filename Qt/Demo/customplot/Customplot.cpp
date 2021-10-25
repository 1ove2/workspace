#include "Customplot.h"
#include "ui_Customplot.h"

Customplot::Customplot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Customplot)
{
    ui->setupUi(this);
}

Customplot::~Customplot()
{
    delete ui;
}
