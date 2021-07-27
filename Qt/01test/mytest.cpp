#include "mytest.h"
#include "ui_mytest.h"

mytest::mytest(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mytest)
{
    ui->setupUi(this);
    ui->line_Pwd->setEchoMode(QLineEdit::Password);
}

mytest::~mytest()
{
    delete ui;
}

//取消功能
void mytest::on_bth_Cancel_clicked()
{
    this->close();
}
//登录功能
void mytest::on_btn_Login_clicked()
{
    QString name = ui->line_Name->text();
    QString pwd = ui->line_Pwd->text();
    if(name == pwd){
        qDebug() << "登录成功！";
    }
    else{
        qDebug() << "登录失败！";
    }
}
