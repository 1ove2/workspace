#include "secondform.h"
#include "ui_secondform.h"

SecondForm::SecondForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SecondForm)
{
    ui->setupUi(this);
    clickCount = 0;
    third = new ThirdForm();
    connect(ui->pushButton,&QPushButton::clicked,this,&SecondForm::doProcessClickgoOne);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&SecondForm::doProcessClickgoThird);
}

SecondForm::~SecondForm()
{
    delete ui;
}

void SecondForm::doProcessClickgoOne(){
    clickCount++;
    emit SignalShowOneFrom(QString::number(clickCount));
    this->hide();
}

void SecondForm::doProcessClickgoThird(){
    this->hide();
    third->show();
}
