#include "mytest.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mytest w;
    w.show();
    return a.exec();
}
