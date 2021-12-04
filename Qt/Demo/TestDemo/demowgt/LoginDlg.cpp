#include "LoginDlg.h"
#include "ui_LoginDlg.h"
#include "QtUtil.h"
#include <QMessageBox>
LoginDlg::LoginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDlg)
{
    ui->setupUi(this);
    m_db = new DBSQLite();
    m_db->reOpenSql("testdb");
    ui->led_PassWord->setEchoMode(QLineEdit::Password);
    ui->led_UserName->setFocus();
    //从配置文件读取历史登录用户名
    QString strName = QtUtil::GetIniValue("USERNAME","userName").toString();
    ui->led_UserName->setText(strName);

}

LoginDlg::~LoginDlg()
{
    m_db->closeSql();
    delete ui;
}

void LoginDlg::on_btn_login_clicked()
{
    QList<QHash<QString,QString>> data;
    m_db->getData("LDBLogin",data);
    mIsLoginOK = false;
    for(int i = 0;i < data.size();i++)
    {
        QHash<QString,QString> node = data.at(i);
        QHash<QString,QString>::iterator iter;
        iter = node.find("UserName");
        QString strname = iter.value();
        if(strname == ui->led_UserName->text())
        {
            iter = node.find("PassWord");
            QString strpwd = iter.value();
            if(strpwd == ui->led_PassWord->text())
            {
                mIsLoginOK = true;
                iter = node.find("Authority");
                m_strAuthority = iter.value();
                m_strUser = strname;
                QString strkey = "username";
                QString strval = m_strUser;
                QtUtil::SetIniValue("USERNAME",strkey,strval);
                emit sigUserChange(strname);
                accept();
            }
        }
    }
    if(!mIsLoginOK)
    {
        QMessageBox::information(this,"提示","用户名密码错误");
        ui->led_UserName->setFocus();
    }
}

void LoginDlg::on_btn_cancel_clicked()
{
    ui->led_UserName->clear();
    ui->led_PassWord->clear();
    close();
}

void LoginDlg::setWinTitle(QString strtitle,QString strLoginOrSwitch)
{
    this->setWindowTitle(strtitle);
    ui->btn_login->setText(strLoginOrSwitch);
}
