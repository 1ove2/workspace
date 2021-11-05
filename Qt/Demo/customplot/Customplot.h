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
    QVector<QPointF> getPointF(QVector<double> xData, QVector<double> yData);
private:
    Ui::Customplot *ui;
    Curve *mcurve;
    QVector<double>  mIScanResultData;               /* 扫描电流结果 */
    QVector<double>  mVScanResultData;               /* 扫描电压结果 */
};

#endif // CUSTOMPLOT_H
