/******************************************************************************
 *
 * 文件名  ： CalRunner.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20191228
 * 版本号  ： v1.0 20191228
 * 文件描述： 封装校准操作
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_QT_CAL_RUNNER_H_
#define _PSS_QT_CAL_RUNNER_H_

/************************************ 头文件 ***********************************/
#include <QFile>
#include <QObject>
#include <QVector>
#include <QString>
#include "Sx00QtAnlgBrd.h"
#include "Qt2450.h"
#include "PssRstDef.h"
#include "PssBinAppSx00AnlgEnum.h"

/*********************************** 命名空间 **********************************/
namespace Pss {
namespace Qt {

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class Sx00QtAnlgBrd;
class Qt2450;
class CalRunner : public QObject
{
    Q_OBJECT
    public:
        CalRunner(const QString &name,
                bool isCalV, double start, double end, double lmt4Instrument, double lmt4Product, double nplc4Instrument,
                PssBinAppSx00AnlgEnumRangeIndex vRange, PssBinAppSx00AnlgEnumRangeIndex iRange, PssBinAppSx00AnlgEnumCalIndex calIndex,
                QObject *parent = 0);
        ~CalRunner(void);

        PssRstT Cal(Sx00QtAnlgBrd &anlgBrd, QtIInstrument &instrument, int calPointNums);               /* 校准 */
        void Clear(void);                                                                               /* 清理缓存 */
    
        const QString&  GetName(void) const;                                                            /* 获取校准量程名 */
    
    signals:
        void sigSendMsg(const QByteArray& msg);                                                             /* 发送文本消息 */
        void sigErrorMsg(const QByteArray& msg);
        void sigCalCheckError(const QByteArray& msg);                                                      /* 校准出错信号 */
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
    
        void CalibrationCompute(void);                                                                  /* 校准计算 */
        void CalibrationCompute4Dac(const QVector<double> &x, const QVector<double> &y);                /* 校准DAC */
        void CalibrationCompute4Adc(const QVector<double> &x, const QVector<double> &y);                /* 校准ADC */
        PssRstT Scan(Sx00QtAnlgBrd &anlgBrd, QtIInstrument &instrument, int calPointNums, bool doCheck);/* 扫描 */
        PssRstT SampleAPoint(Sx00QtAnlgBrd &anlgBrd, QtIInstrument &instrument, double vSetVLoad, double vSetILoade, double val, int pointIndex);/* 采样一个点 */
        PssRstT ConfigureMeasure(QtIInstrument &instrument, bool isMeasV, double lmtVal, double nplc);     /* 配置仪表 */
    
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
        PssBinAppSx00AnlgEnumRangeIndex     mVRange;                                                    /* 电压量程 */
        PssBinAppSx00AnlgEnumRangeIndex     mIRange;                                                    /* 电流量程 */
        PssBinAppSx00AnlgEnumCalIndex       mCalIndex;                                                  /* 校准系数索引 */
    
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
    
        const static int        mcUpdateValStableMs = 800;                                              /* 等待V/I更新就绪的时间 */
        constexpr static double mcMarginRate = 0.10;                                                    /* 扫描起终点的余量20% 即:10% => 90% */
        QString            mErrorMsg;                                                                   /* 最近一次错误信息 */
};
/*********************************** 工具函数 **********************************/

}
}

#endif // _PSS_QT_CAL_RUNNER_H_