#ifndef CUSTOMPLOT_H
#define CUSTOMPLOT_H

#include <QWidget>
#include "Curve.h"
namespace Ui {
class Customplot;
}

class Customplot : public QWidget
{
    Q_OBJECT

public:
    explicit Customplot(QWidget *parent = 0);
    ~Customplot();

private:
    Ui::Customplot *ui;
    Curve *mcurve;
};

#endif // CUSTOMPLOT_H
