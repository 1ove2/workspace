#include "MainWindow.h"
#include <QApplication>
#include "LoginDlg.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //设置应用程序字体
    a.setFont(QFont("Microsoft Yahei", 9));

    MainWindow w;
    w.show();

    return a.exec();
}
