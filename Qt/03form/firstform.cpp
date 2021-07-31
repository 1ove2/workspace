#include "firstform.h"
#include "ui_firstform.h"

FirstForm::FirstForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FirstForm)
{
    ui->setupUi(this);
    Init();
}

FirstForm::~FirstForm()
{
    delete ui;
}

void FirstForm::Init(){
    connect(ui->btn_Next,SIGNAL(clicked(bool)),this,SLOT(doProcessClickNext(bool)));
    connect(&second,&SecondForm::SignalShowOneFrom,this,&FirstForm::doProcessSecondReq);
    //connect(&second,SIGNAL(SignalShowOneFrom(QString)),this,SLOT(doProcessSecondReq(QString)));
}

void FirstForm::doProcessClickNext(bool){
    //second = new(SecondForm);
    second.show();
    this->hide();
}

void FirstForm::doProcessSecondReq(QString str){
    qDebug()<<str;
    this->show();
}
