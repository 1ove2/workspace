#include "CustomLogPlot.h"
#include <math.h>

const double Scale_Factor = 0.1;
CustomLogPlot::CustomLogPlot(QWidget *parent)
    :QCustomPlot(parent),
      mMarginGroup(new QCPMarginGroup(this))
{
    //设置可拖拽
    setInteraction(QCP::iRangeDrag);
    plotLayout()->clear();
}

CustomLogPlot::~CustomLogPlot()
{

}

void CustomLogPlot::AppendAxisRect(const ChanInfo &info)
{
    /* 将之前所有axisRect中的x轴隐藏 */
    //clearGraphs();
    foreach (auto *rect, axisRects())
    {
        rect->axis(QCPAxis::atBottom)->setVisible(false);
    }
    /* 添加新的axisRect */
    QCPAxisRect *axisRect = new QCPAxisRect(this, true);
    plotLayout()->addElement(axisRectCount(), 0, axisRect);

    /* 将它们放置在相应的层 */
    foreach (auto *rect, axisRects())
    {
        connect(rect->axis(QCPAxis::atBottom), SIGNAL(rangeChanged(QCPRange)), axisRect->axis(QCPAxis::atBottom), SLOT(setRange(QCPRange)));
        connect(axisRect->axis(QCPAxis::atBottom), SIGNAL(rangeChanged(QCPRange)), rect->axis(QCPAxis::atBottom), SLOT(setRange(QCPRange)));
        foreach (auto *axis, rect->axes())
        {
            axis->setLayer("axes");
            axis->grid()->setLayer("grid");
        }
    }
    /* 增加图线 */
    addGraph(axisRect->axis(QCPAxis::atBottom), axisRect->axis(QCPAxis::atLeft));
    /* 设置行间隔 */
    plotLayout()->setRowSpacing(5);
    /* 最后一个axisRect的x轴可见 */
    axisRect->axis(QCPAxis::atBottom)->setVisible(true);
    axisRect->axis(QCPAxis::atBottom)->grid()->setVisible(false);
    axisRect->axis(QCPAxis::atBottom)->SetTickerUnit("s");
    /* 设置margin */
    axisRect->setMinimumMargins(QMargins(0,0,0,5));
    /* 设置轴刻度 */
    axisRect->axis(QCPAxis::atLeft)->ticker()->setTickCount(3);
    axisRect->axis(QCPAxis::atLeft)->SetTickerUnit("Ω");
    /* 所有rect的y轴对齐 */
    axisRect->setMarginGroup(QCP::msLeft, mMarginGroup);
    replot();
    
    mYMaxVec.insert(info, 0);
    mYMinVec.insert(info, 0);
    mAxisRectCfg.insert(info, axisRect);
    mUnitFactorVec.insert(info, 1);
}

void CustomLogPlot::AddData(const ChanInfo &info, double x, double y)
{
    QCPGraph *graph = mAxisRectCfg[info]->graphs().value(0);
#if 1
    if (graph->data().data()->isEmpty())
    {
        mYMaxVec[info] = y;
        mYMinVec[info] = y;

        QString unitStr;
        if (qAbs(y / 1e6) >= 1)
        {
            mUnitFactorVec[info] = 1e-6;
            unitStr.append("M");
        }
        else if (qAbs(y / 1e3) >= 1)
        {
            mUnitFactorVec[info] = 1e-3;
            unitStr.append("K");
        }
        else
        {
            mUnitFactorVec[info] = 1;
        }
        mAxisRectCfg[info]->axis(QCPAxis::atLeft)->SetTickerUnit(QString("%1Ω").arg(unitStr));
    }
    graph->addData(x, y * mUnitFactorVec[info]);
    
    mYMaxVec[info] = (mYMaxVec[info] > y) ? mYMaxVec[info] : y;
    mYMinVec[info] = (mYMinVec[info] > y) ? y : mYMinVec[info];
    double rangeUp, rangeLow;
    if (mYMaxVec[info] >= 0)
    {
        rangeUp = mYMaxVec[info] * (1 + Scale_Factor);
    }
    else
    {
        rangeUp = mYMaxVec[info] * (1 - Scale_Factor);
    }
    if (mYMinVec[info] >= 0)
    {
        rangeLow = mYMinVec[info] * (1 - Scale_Factor);
    }
    else
    {
        rangeLow = mYMinVec[info] * (1 + Scale_Factor);
    }
    
    mAxisRectCfg[info]->axis(QCPAxis::atLeft)->setRange(rangeLow * mUnitFactorVec[info], rangeUp * mUnitFactorVec[info]);
    /* 此时不调用replot, 统一在最外层通过定时器刷新ui */
#endif
}

void CustomLogPlot::RemoveChan(const ChanInfo &info)
{
    for (int i = 0; i < mAxisRectCfg.count(); ++i)
    {
        if (info.key == mAxisRectCfg.keys().value(i).key)
        {
            /* 移除曲线 */
            removeGraph(mAxisRectCfg[info]->graphs().value(0));
            /* 移除axisrect */
            //QCPLayoutElement *ele = plotLayout()->element(0,0);
            //plotLayout()->remove(ele);

            plotLayout()->remove(mAxisRectCfg[info]);
            plotLayout()->simplify();
            replot();


            mAxisRectCfg.remove(info);
            mYMaxVec.remove(info);
            mYMinVec.remove(info);
        }
    }
}

void CustomLogPlot::EnableChan(const ChanInfo &info, bool isEnable)
{
//    if (!isEnable)
//    {
//        plotLayout()->take(mAxisRectCfg[info]);
//        plotLayout()->simplify();
//    }
//    else
//    {
//        plotLayout()->addElement(1, 0, mAxisRectCfg[info]);
//    }
    if (isEnable)
    {
        setInteraction(QCP::iRangeZoom, false);
        mAxisRectCfg[info]->setBackground(QBrush(QColor(Qt::white)));
    }
    else
    {
        setInteraction(QCP::iRangeZoom, false);
        mAxisRectCfg[info]->setBackground(QBrush(QColor(Qt::gray)));
    }
    replot();
}

void CustomLogPlot::ChanChecked(const ChanInfo &info, bool isChecked)
{
    QCPGraph *graph = mAxisRectCfg[info]->graphs().value(0);

    graph->setVisible(isChecked);
    replot();
}

void CustomLogPlot::ColorChanged(const ChanInfo &info, const QColor &color)
{
    QCPGraph *graph = mAxisRectCfg[info]->graphs().value(0);

    graph->setPen(QPen(color));
    replot();
}