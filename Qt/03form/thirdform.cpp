#include "thirdform.h"
#include "ui_thirdform.h"

ThirdForm::ThirdForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ThirdForm)
{
    ui->setupUi(this);
    count = 0;
    myTimer = new QTimer(this);
    //connect(myTimer,SIGNAL(timeout()),this,SLOT(doProcessTimerOut());
      connect(myTimer,&QTimer::timeout,this,&ThirdForm::doProcessTimerOut);
      myTimer->start(100);
}

ThirdForm::~ThirdForm()
{
    delete ui;
}

void ThirdForm::doProcessTimerOut(){
    count++;
    if(count == 100){
        count = 0;

    }
    ui->lcdNumber->display(count);
}
