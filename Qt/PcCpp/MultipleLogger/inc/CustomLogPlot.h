#ifndef _PSS_QT_CUSTOMLOGPLOT_H_
#define _PSS_QT_CUSTOMLOGPLOT_H_

#include "ParaDef.h"
#include "QCustomPlot.h"

class CustomLogPlot : public QCustomPlot
{
    Q_OBJECT
public:
    CustomLogPlot(QWidget * parent = 0);
    ~CustomLogPlot();
    void AppendAxisRect(const ChanInfo &info);                                                                              /* 增加矩形坐标轴 */
    void AddData(const ChanInfo &info, double x, double y);                                                      /* 增加绘图数据 */
    void RemoveChan(const ChanInfo &info);
    void EnableChan(const ChanInfo &info, bool isEnable);

public slots:
    void ChanChecked(const ChanInfo &info, bool isChecked);                                                       /* 通道勾选 */
    void ColorChanged(const ChanInfo &info, const QColor &color);                                                 /* 颜色更改 */

private:
    QMap<ChanInfo, QCPAxisRect*> mAxisRectCfg;
    QMap<ChanInfo, double> mUnitFactorVec;
    QMap<ChanInfo, double> mYMaxVec;
    QMap<ChanInfo, double> mYMinVec;
    QCPMarginGroup *mMarginGroup;
};

#endif // _PSS_QT_CUSTOMLOGPLOT_H_