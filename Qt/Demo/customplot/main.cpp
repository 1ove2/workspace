#include "Customplot.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Customplot w;
    w.show();

    return a.exec();
}
