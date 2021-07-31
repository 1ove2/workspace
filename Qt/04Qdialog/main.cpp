#include "dialoglearn.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DialogLearn w;
    w.show();
    return a.exec();
}
