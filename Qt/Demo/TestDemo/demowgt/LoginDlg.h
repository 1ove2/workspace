#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include "DBSQLite.h"
namespace Ui {
class LoginDlg;
}

class LoginDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDlg(QWidget *parent = 0);
    ~LoginDlg();
    void setWinTitle(QString strtitle,QString strLoginOrSwitch);
private slots:
    void on_btn_login_clicked();

    void on_btn_cancel_clicked();
signals:
    sigUserChange(QString UserName);
private:
    Ui::LoginDlg *ui;
    DBSQLite *m_db;
    bool mIsLoginOK;
    QString m_strUser;
public:
    QString m_strAuthority;//管理角色权限
};

#endif // LOGINDLG_H
