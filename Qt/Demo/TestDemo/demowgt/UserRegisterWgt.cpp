#include "UserRegisterWgt.h"
#include "ui_UserRegisterWgt.h"
#include <QMessageBox>
UserRegisterWgt::UserRegisterWgt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserRegisterWgt)
{
    ui->setupUi(this);
    m_db = new DBSQLite();
    m_db->reOpenSql("testdb");
//    m_db = new MysqlDb("127.0.0.1",3306,"root","1234");
//    m_db->reOpenSql("test");
    this->setWindowTitle("添加用户");
}

UserRegisterWgt::~UserRegisterWgt()
{
    m_db->closeSql();
    delete ui;
}

void UserRegisterWgt::on_btn_Add_clicked()
{
    QList<QHash<QString,QString>> data;
    m_db->getData("LDBLogin",data);
    QVector<QString> vecUserName;
    for(int i = 0;i < data.size();i++)
    {
        QHash<QString,QString> node = data.at(i);
        QHash<QString,QString>::iterator iter;
        iter = node.find("UserName");
        QString str =  iter.value();
        vecUserName.append(str);
    }
    QString strusername = ui->lineEdit_UserName->text();
    if(vecUserName.contains(strusername))
    {
        QMessageBox::information(this,"提示","用户名已存在,请更换新的用户名");
        return;
    }
    else
    {
        QString strpwd = ui->lineEdit_Pwd->text();
        QString strcompanyname = ui->lineEdit_CompanyName->text();
        QString strtelephone = ui->lineEdit_Telephone->text();
        QString strrealname = ui->lineEdit_RealName->text();
        QString strauthority = ui->m_cbAuthority->currentText();
        QHash<QString,QString> data2;
        data2.insert("UserName",strusername);
        data2.insert("PassWord",strpwd);
        data2.insert("CompanyName",strcompanyname);
        data2.insert("TelePhone",strtelephone);
        data2.insert("RealName",strrealname);
        data2.insert("Authority",strauthority);
        bool res = m_db->addData("LDBLogin",data2);
        if(!res)
        {
            QMessageBox::information(this,"提示","添加失败");
            return;
        }
        else
        {
            accept();
        }
    }
}

void UserRegisterWgt::on_btn_Cancel_clicked()
{
    this->close();
}
