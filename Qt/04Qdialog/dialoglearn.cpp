#include "dialoglearn.h"
#include "ui_dialoglearn.h"

DialogLearn::DialogLearn(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DialogLearn)
{
    ui->setupUi(this);
    connect(ui->btn_msg,&QPushButton::clicked,this,&DialogLearn::doProcessbtn_msgClicked);
    connect(ui->btn_ColorBox,&QPushButton::clicked,this,&DialogLearn::doProcessbtn_ClorBoxClicked);
    P_dialog = NULL;
}

DialogLearn::~DialogLearn()
{
    delete ui;
}

void DialogLearn::doProcessbtn_msgClicked(){
    //about
   // QMessageBox::about(this,"关于","Message:this is about dialog");

    //Information
    QMessageBox::StandardButton ret = QMessageBox::information(this,"消息","good good study",QMessageBox::Yes|QMessageBox::No);
    if (ret == QMessageBox::Yes){
        qDebug() << "good";
    }else{
        qDebug() << "bad";
    }

    //
}


void DialogLearn::doProcessbtn_ClorBoxClicked(){
    const QColor color = QColorDialog::getColor(Qt::green,this,"Select Color",QColorDialog::ShowAlphaChannel);
    if (color.isValid()){
        QPalette palette = ui->lineEdit->palette();
        palette.setColor(QPalette::Text,color);
        ui->lineEdit->setPalette(palette);
    }
}

void DialogLearn::on_pushButton_clicked()
{

    int i = QInputDialog::getInt(this, "Input","good",25,0,100,1);
    qDebug() << i;
}

void DialogLearn::on_pushButton_2_clicked()
{

    if (P_dialog == NULL){
        step = 0;
        P_dialog = new QProgressDialog(this);
        P_dialog->setRange(0,100);
        connect(P_dialog,SIGNAL(canceled()),this,SLOT(doProcessCancel()));

        myTimer = new QTimer(this);
        connect(myTimer,SIGNAL(timeout()),this,SLOT(doProcessTimeOut()));
        myTimer->start(100);
    }
}

void DialogLearn::doProcessCancel(){
    myTimer->stop();
    delete P_dialog;
    P_dialog = NULL;
}
void DialogLearn::doProcessTimeOut(){
    P_dialog->setValue(step);
    step++;
        if(step == 101){
        myTimer->stop();
        delete P_dialog;
        P_dialog = NULL;
    }
}
