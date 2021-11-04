#include "Curve.h"
using namespace  std;
Curve::Curve(QWidget *parent) :
    QCustomPlot(parent),
    mXMaxVal(5),
    mXMinVal(0),
    mYMaxVal(500),
    mYMinVal(0)
{
    addGraph();
    graph(0);

    mXVal.clear();
    mYVal.clear();

    SetXaxisRange(mXMinVal, mXMaxVal);

    SetYaxisRange(mYMinVal, mYMaxVal);

    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(1,24,55));
    plotGradient.setColorAt(1, QColor(1,24,55));
    setBackground(plotGradient);      // 设置背景颜色

    graph(0)->setPen(QPen(::Qt::red));

    xAxis->setBasePen(QPen(::Qt::white, 1));                               /* 轴线的画笔 */
    xAxis->setTickPen(QPen(::Qt::white, 1));                               /* 轴刻度线的画笔 */
    xAxis->setTickLabelColor(::Qt::white);                                 /* 轴刻度文字颜色 */
    xAxis->setSubTickPen(QPen(::Qt::white, 1));                            /* 轴子刻度线的画笔 */
    //xAxis->setLabel("X");                                                /* 只有设置了标签，轴标签的颜色才会显示 */
    xAxis->setLabelColor(::Qt::white);                                     /* 轴标签颜色 */
    //xAxis->setTickLengthIn(3);                                           /* 轴线内刻度的长度 */
    //xAxis->setTickLengthOut(5);                                          /* 轴线外刻度的长度 */
    //xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    xAxis->ticker()->setTickCount(10);//11个主刻度
    xAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssReadability);   /*可读性优于设置*/
    xAxis->setNumberPrecision(mcNumberPrecision);                          /* 设置精度 */

    yAxis->setNumberPrecision(mcNumberPrecision);                          /* 设置精度 */
    yAxis->setBasePen(QPen(::Qt::red, 1));                                 /* 轴线的画笔 */
    yAxis->setTickPen(QPen(::Qt::red, 1));                                 /* 轴刻度线的画笔 */
    yAxis->setSubTickPen(QPen(::Qt::red, 1));                              /* 轴子刻度线的画笔 */
    yAxis->setTickLabelColor(::Qt::red);                                   /* 轴刻度文字颜色 */
    yAxis->setLabelColor(::Qt::red);                                       /* 轴标签颜色 */
    yAxis->ticker()->setTickCount(10);                                     /*11个主刻度*/
    yAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssReadability);   /*可读性优于设置*/
}



Curve::~Curve()
{
}

void Curve::Append(const QPointF &point, bool isNeedDraw)
{
    const double mx = point.x();
    const double my = point.y();

    mXVal.append(mx);
    mYVal.append(my);

    if(isNeedDraw)
    {
        Draw();
    }
}

void Curve::Append(const QVector<QPointF> &points)
{
    qDebug() << "Canvas Append Points.";
    int size = points.size();
    if(0 == size)
    {
        return;
    }

    for(int i = 0; i < size; i++)
    {
        Append(points[i], false);
    }

    Draw();
}

void Curve::Draw(void)
{
    graph(0)->setData(mXVal, mYVal);
    graph(0)->rescaleAxes();
    replot(QCustomPlot::rpQueuedReplot);
}

void Curve::ClearCanvas(void)
{
    ClearPoint();
    graph(0)->setData(mXVal,mYVal);
    replot();
    update();
}

void Curve::ClearPoint(void)
{
    mXVal.clear();
    mYVal.clear();
    mXMaxVal = 0;
    mXMinVal = 5;
    mYMaxVal = 5;
    mYMinVal = 0;
}

void Curve::SetXaxisLabel(QString xLabel)
{
    xAxis->setLabel(xLabel);
}

void Curve::SetYaxisLabel(QString xLabel)
{
    yAxis->setLabel(xLabel);
}

void Curve::SetXaxisRange(double value1, double value2)
{
    mXMinVal = min(value1, value2);
    mXMaxVal = max(value1, value2);
    xAxis->setRange(mXMinVal, mXMaxVal);
}

void Curve  ::SetYaxisRange(double value1, double value2)
{
    mYMinVal = min(value1, value2);
    mYMaxVal = max(value1, value2);
    yAxis->setRange(mYMinVal, mYMaxVal);
}

