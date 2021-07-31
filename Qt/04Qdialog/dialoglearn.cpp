#include "dialoglearn.h"
#include "ui_dialoglearn.h"

DialogLearn::DialogLearn(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DialogLearn)
{
    ui->setupUi(this);
    connect(ui->btn_msg,&QPushButton::clicked,this,&DialogLearn::doProcessbtn_msgClicked);
    connect(ui->btn_ColorBox,&QPushButton::clicked,this,&DialogLearn::doProcessbtn_ClorBoxClicked);
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
