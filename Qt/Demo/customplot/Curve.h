#ifndef CURVE_H
#define CURVE_H
#include <QWidget>
#include "qcustomplot.h"

class Curve : public QCustomPlot
{
    Q_OBJECT
public:
    Curve(QWidget * paraent = 0);
    ~Curve();
    void Append(const QPointF &point, bool isNeedDraw = true);                                      /* 加入单点 */
    void Append(const QVector<QPointF> &points);                                                    /* 加入所有点 */

    void ClearCanvas(void);                                                                         /* 清空画布 */
    void ClearPoint(void);                                                                          /* 清空本地数据 */
    void SetXaxisLabel(QString xLabel);                                                             /* 设置X轴标签 */
    void SetYaxisLabel(QString yLabel);                                                             /* 设置Y轴标签 */
    void SetXaxisRange(double value1, double value2);                                               /* 设置X轴坐标范围 */
    void SetYaxisRange(double value1, double value2);                                               /* 设置Y轴坐标范围 */

private:
    void Draw(void);                                                                                /* 绘制 */
    QVector<double>                   mXVal;                                                        /* X轴数据 */
    QVector<double>                   mYVal;                                                        /* Y轴数据 */
    double                            mXMaxVal;                                                     /* X轴最大值 */
    double                            mXMinVal;                                                     /* X轴最小值 */
    double                            mYMaxVal;                                                     /* Y轴最大值 */
    double                            mYMinVal;                                                     /* Y轴最小值 */

    const static int                  mcNumberPrecision = 3;                                        /* 数据精度 */
};
/*********************************** 工具函数 **********************************/


#endif // CURVE_H
