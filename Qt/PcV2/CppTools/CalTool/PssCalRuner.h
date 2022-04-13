/******************************************************************************
 *
 * 文件名  ： PssCalRuner.h
 * 负责人  ： sls
 * 创建日期： 20220318
 * 版本号  ： v1.0 20220318
 * 文件描述： 封装校准操作
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_CAL_RUNER_H_
#define _PSS_CAL_RUNER_H_
/************************************ 头文件 ***********************************/
#include <QObject>

/*********************************** 命名空间 **********************************/


/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class PssCalRuner : public QObject
{
    Q_OBJECT
public:
    PssCalRuner(double start, double end,
                double stdLimt,double pssLimt, double stdNplc,int calIndex,QObject *parent = 0);

    ~PssCalRuner(void);

    bool CalCurrAdc(PssDevice &pssDev, StdDevice &stdDev, int calPoints);                            /* 校准I路Adc流程 */
    bool CalVoltAdc(PssDevice &pssDev, StdDevice &stdDev, int calPoints);                            /* 校准V路Adc流程 */
    bool CalCurrDac(PssDevice &pssDev, StdDevice &stdDev, int calPoints);                            /* 校准I路Dac流程 */
    bool CalVoltDac(PssDevice &pssDev, StdDevice &stdDev, int calPoints);                            /* 校准V路Dac流程 */

    const QString&  GetName(void) const;                                                            /* 获取校准量程名 */
    void setChannel(int channel);

public slots:
    void slotStopCal(void);
signals:


private:
    void AppendSetVLoad(double val);                                                                /* 追加 设置电压 */
    void AppendSetILoad(double val);                                                                /* 追加 设置电流 */
    void AppendGetVLoad(double val);                                                                /* 追加 模拟板负载电压 */
    void AppendGetILoad(double val);                                                                /* 追加 模拟板负载电流 */
    void AppendGetVAdc(double val);                                                                 /* 追加 模拟板V路ADC */
    void AppendGetIAdc(double val);                                                                 /* 追加 模拟板I路ADC */
    void AppendGetVDac(double val);                                                                 /* 追加 模拟板V路DAC */
    void AppendGetIDac(double val);                                                                 /* 追加 模拟板I路DAC */
    void AppendGetVReal(double val);                                                                /* 追加 负载电压实测值 */
    void AppendGetIReal(double val);                                                                /* 追加 负载电流实测值 */
    void CalibrationCompute4Dac(const QVector<double> &x, const QVector<double> &y);                /* 校准DAC */
    void CalibrationCompute4Adc(const QVector<double> &x, const QVector<double> &y);                /* 校准ADC */


    /* 校准结果 */
    double             mC04Dac;                                                                     /* DAC校准截距 */
    double             mC14Dac;                                                                     /* DAC校准斜率 */
    double             mSqSum4Dac;                                                                  /* DAC校准后的 误差(平方和) */
    double             mC04Adc;                                                                     /* ADC校准截距 */
    double             mC14Adc;                                                                     /* ADC校准斜率 */
    double             mSqSum4Adc;                                                                  /* ADC校准后的 误差(平方和) */
    bool               mForCheck;                                                                   /* 扫描模式,检查时不使用比例 */

    /* 校准参数 */
    QString            mName;                                                                       /* 校准量程名字 */
    bool               mIsCalV;                                                                     /* 校准通道V/I */
    double             mStart;                                                                      /* 校准起点 */
    double             mEnd;                                                                        /* 校准终点 */
    double             mLmt4Instrument;                                                             /* 2400测量的保护值(挡位最大值) */
    double             mLmt4Product;                                                                /* 待测设备另一路的固定值 */
    double             mNplc4Instrument;                                                            /* 仪表的NPLC */
    int                mVRange;                                                                     /* 电压量程 */
    int                mIRange;                                                                     /* 电流量程 */
    int                mCalIndex;                                                                   /* 校准系数索引 */
    bool               mStopCal;                                                                    /* 停止校准 */

    /* 原始数据 */
    QVector<double>    mSetVLoad;                                                                   /* 模拟板V路   设置电压 */
    QVector<double>    mSetILoad;                                                                   /* 模拟板I路   设置电压 */
    QVector<double>    mGetVLoad;                                                                   /* 模拟板负载  V路电压 */
    QVector<double>    mGetILoad;                                                                   /* 模拟板负载  I路电压 */
    QVector<double>    mGetVAdc;                                                                    /* 模拟板ADC   V路电压 */
    QVector<double>    mGetIAdc;                                                                    /* 模拟板ADC   I路电压 */
    QVector<double>    mGetVDac;                                                                    /* 模拟板DAC   V路电压 */
    QVector<double>    mGetIDac;                                                                    /* 模拟板DAC   I路电压 */
    QVector<double>    mVReal;                                                                      /* (2400)实测  电压 */
    QVector<double>    mIReal;                                                                      /* (2400)实测  电流 */
    double mDacDifRate;                                                                             /* DAC精度 */
    double mAdcDifRate;                                                                             /* ADC精度 */

    int                 mChannel;

/*********************************** 工具函数 **********************************/
};

#endif // _PSS_CAL_RUNER_H_
