#include "MainWindow.h"
#include <QApplication>
#include "LoginDlg.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //设置应用程序字体
    a.setFont(QFont("Microsoft Yahei", 9));
    LoginDlg login;
    login.setWindowTitle("登录界面");
    if(login.exec() == QDialog::Accepted)
    {
        MainWindow w;
        w.show();

        return a.exec();
    }
    else
    {
        return 0;
    }

}
