#include "Customplot.h"
#include "ui_Customplot.h"
using namespace  std;
Customplot::Customplot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Customplot)
{
    ui->setupUi(this);
    mcurve = qobject_cast<Curve*>(ui->wgt_plot);
    //获取数据
    for(int i =1; i < 23; i++)
    {
        mIScanResultData.append(i);
        mVScanResultData.append(sin(i));
    }
    mcurve->Append(getPointF(mIScanResultData,mVScanResultData));
}

Customplot::~Customplot()
{
    delete ui;
}

QVector<QPointF> Customplot::getPointF(QVector<double> xData, QVector<double> yData)
{
    QVector<QPointF> points;
    if(xData.length() != yData.length())
        return points;
    int size = xData.size();
    for(int i = 0; i < size; i++)
    {
        QPointF point(xData[i], yData[i]);
        points.append(point);
    }
    return points;
}
