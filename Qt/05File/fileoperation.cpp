#include "fileoperation.h"
#include "ui_fileoperation.h"

FileOperation::FileOperation(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FileOperation)
{
    ui->setupUi(this);
    myFile = new QFile(this);
    connect(ui->btn_open,&QPushButton::clicked,this,&FileOperation::doProcessOpenFile);
    connect(ui->btn_save,&QPushButton::clicked,this,&FileOperation::doProcessSaveFile);

}

FileOperation::~FileOperation()
{
    delete ui;
}

void FileOperation::doProcessOpenFile(){
    QString filename = QFileDialog::getOpenFileName(this,"open file","D:/","*.txt");
    if (filename.isEmpty()){
        QMessageBox::warning(this,"warning","file is not exit");
        return;
    }
    myFile->setFileName(filename);
    bool ret = myFile->open(QIODevice::ReadOnly|QIODevice::Text);
    if(!ret){
        QMessageBox::warning(this,"warning","open file falid!");
        return;
    }
    while(!myFile->atEnd()){
        QByteArray buf = myFile->read(1024);
        QString str = QString(buf);
        ui->textEdit->append(str);
    }
    myFile->close();
}

void FileOperation::doProcessSaveFile(){
    QString filename = QFileDialog::getSaveFileName(this,"savefile","D:/");
    if(filename.isEmpty()){
        QMessageBox::warning(this,"warning","getsave file faild");
        return;
    }
    myFile->setFileName(filename);
    bool ret = myFile->open(QIODevice::WriteOnly|QIODevice::Text);
    if(!ret){
        QMessageBox::warning(this,"waring","Open file faild");
        return;
    }
    myFile->write(ui->textEdit->toPlainText().toLocal8Bit());
    myFile->close();
}
