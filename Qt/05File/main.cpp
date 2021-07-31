#include "fileoperation.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FileOperation w;
    w.show();
    return a.exec();
}
