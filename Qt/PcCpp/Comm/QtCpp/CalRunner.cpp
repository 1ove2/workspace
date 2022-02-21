#include "CalRunner.h"
#include "PssQtHal.h"
#ifdef _PSS_QT_SX00_
#include "PssBinAppSx00AnlgQtSetCal.h"
#endif
#include <QThread>
#include <math.h>

using namespace Pss;
using namespace Pss::Qt;

CalRunner::CalRunner(const QString &name,
        bool isCalV, double start, double end, double lmt4Instrument, double lmt4Product,
                     double nplc4Instrument,
                     PssBinAppSx00AnlgEnumRangeIndex vRange,
                     PssBinAppSx00AnlgEnumRangeIndex iRange,
                     PssBinAppSx00AnlgEnumCalIndex calIndex,
                    QObject *parent) :
    QObject(parent),

    mC04Dac(0),
    mC14Dac(0),
    mSqSum4Dac(0),
    mC04Adc(0),
    mC14Adc(0),
    mSqSum4Adc(0),
    mForCheck(true),
    
    mName(name),
    mIsCalV(isCalV),
    mStart(0),
    mEnd(0),
    mLmt4Instrument(lmt4Instrument),
    mLmt4Product(lmt4Product),
    mNplc4Instrument(nplc4Instrument),
    mVRange(vRange),
    mIRange(iRange),
    mCalIndex(calIndex),
    mErrorMsg(QString(""))
{
    /* 去掉零点和满量程 */
    double margin = (end - start) * mcMarginRate;
    mStart = start + margin;
    mEnd   = end - margin;

    /* 清理缓存 */
    Clear();
}

CalRunner::~CalRunner(void)
{
}

PssRstT CalRunner::Cal(Sx00QtAnlgBrd &anlgBrd, QtIInstrument &instrument, int calPointNums)
{
    /* step1: 采样 扫描 */
    PssRstT ret = Scan(anlgBrd, instrument, calPointNums, false);
    if(PssRstSuccess != ret)
    {
        qCritical() << "First Scan failed: " << ret;
        return ret;
    }

    /* step2: 校准计算 */
    CalibrationCompute();
    QString msg = QString("DAC c0:%1,c1:%2,sqSum:%3").arg(mC04Dac, 12, 'E', 5).arg(mC14Dac, 12, 'E', 5).arg(mSqSum4Dac, 12, 'E', 5);
    QByteArray str = msg.toUtf8();
    emit sigSendMsg(str);
    msg = QString("ADC c0:%1,c1:%2,sqSum:%3").arg(mC14Adc, 12, 'E', 5).arg(mC14Adc, 12, 'E', 5).arg(mSqSum4Adc, 12, 'E', 5);
    str = msg.toUtf8();
    emit sigSendMsg(str);
#ifdef _PSS_QT_SX00_
    /* step3: 上传校准数据 */
    PssBinAppSx00AnlgQtSetCal calData(mCalIndex, mC04Dac, mC14Dac, mC04Adc, mC14Adc);
    ret = anlgBrd.SetCalVal(calData);
    if(PssRstSuccess != ret)
    {
        qCritical() << "Set cal valued failed: " << ret;
        return ret;
    }
#endif
    /* step4: 检查 扫描 */
    ret = Scan(anlgBrd, instrument, calPointNums, true);
    if(PssRstSuccess != ret)
    {
        qCritical() << "Second scan failed: " << ret;
        return ret;
    }
    return ret;
}

const QString& CalRunner::GetName(void) const
{
    return mName;
}

void CalRunner::AppendSetVLoad(double val)
{
    mSetVLoad.append(val);
}

void CalRunner::AppendSetILoad(double val)
{
    mSetILoad.append(val);
}

void CalRunner::AppendGetVLoad(double val)
{
    mGetVLoad.append(val);
}

void CalRunner::AppendGetILoad(double val)
{
    mGetILoad.append(val);
}

void CalRunner::AppendGetVAdc(double val)
{
    mGetVAdc.append(val);
}

void CalRunner::AppendGetIAdc(double val)
{
    mGetIAdc.append(val);
}

void CalRunner::AppendGetVDac(double val)
{
    mGetVDac.append(val);
}

void CalRunner::AppendGetIDac(double val)
{
    mGetIDac.append(val);
}

void CalRunner::AppendGetVReal(double val)
{
    mVReal.append(val);
}

void CalRunner::AppendGetIReal(double val)
{
    mIReal.append(val);
}

/* DAC: DAC  = c04Dac + c14Dac * LOAD */
/* ADC: LOAD = c04Adc + c14Adc * ADC */
void CalRunner::CalibrationCompute(void)
{
    if(true == mIsCalV)
    {
        CalibrationCompute4Dac(mVReal,   mGetVDac);
        CalibrationCompute4Adc(mGetVAdc, mVReal);
    }
    else
    {
        CalibrationCompute4Dac(mIReal,   mGetIDac);
        CalibrationCompute4Adc(mGetIAdc, mIReal);
    }
}

void CalRunner::CalibrationCompute4Dac(const QVector<double> &x, const QVector<double> &y)
{
    LinearFitting(x, y, mC14Dac, mC04Dac, mSqSum4Dac);
}

void CalRunner::CalibrationCompute4Adc(const QVector<double> &x, const QVector<double> &y)
{
    LinearFitting(x, y, mC14Adc, mC04Adc, mSqSum4Adc);
}

void CalRunner::Clear(void)
{
    /* 原始数据 */
    mSetVLoad.clear();
    mSetILoad.clear();
    mGetVLoad.clear();
    mGetILoad.clear();
    mGetVAdc.clear();
    mGetIAdc.clear();
    mGetVDac.clear();
    mGetIDac.clear();
    mVReal.clear();
    mIReal.clear();

    mDacDifRate = 0;
    mAdcDifRate = 0;
}

PssRstT CalRunner::Scan(Sx00QtAnlgBrd &anlgBrd, QtIInstrument &instrument, int calPointNums, bool doCheck)
{
    double step = (mEnd - mStart) / calPointNums;
    double vSetVLoad = 0;
    double vSetILoad = 0;
#ifdef _PSS_QT_PLX00_
    PSS_UNUSED(anlgBrd);
    PSS_UNUSED(instrument);
    PSS_UNUSED(doCheck);
    PSS_UNUSED(step);
    PSS_UNUSED(vSetVLoad);
    PSS_UNUSED(vSetILoad);
#endif
    /* 状态栏信息 */
    QString msg;
    if(mIsCalV)
    {
        msg = QString("%1: %2%3 => %4%5 with %6%7......")
            .arg(mName)
            .arg(mStart)
            .arg("V")
            .arg(mEnd)
            .arg("V")
            .arg(mLmt4Product)
            .arg("A");
    }
    else
    {
        msg = QString("%1: %2%3 => %4%5 with %6%7......")
            .arg(mName)
            .arg(mStart)
            .arg("A")
            .arg(mEnd)
            .arg("A")
            .arg(mLmt4Product)
            .arg("V");
    }
    QByteArray str = msg.toUtf8();
    emit sigSendMsg(str);
    PssRstT ret = PssRstSuccess;
#ifdef _PSS_QT_SX00_
    do
    {
        /* step1: 设置模拟板量程 */
        ret = anlgBrd.SetAnlgRange(PssBinAppSx00AnlgEnumRangeTypeVMan, mVRange);
        if(ret)
        {
            break;
        }

        ret = anlgBrd.SetAnlgRange(PssBinAppSx00AnlgEnumRangeTypeIMan, mIRange);
        if(ret)
        {
            break;
        }
    
        /* step2: 设置极性 和 源 */
        msg = QString("%1").arg("  ID ", 2);
        if(mIsCalV)
        {
            ret = anlgBrd.SetAnlgVal(PssBinAppSx00AnlgEnumSetValTypeVLoad, mEnd);
            if(ret)
            {
                break;
            }
    
            ret = anlgBrd.SetAnlgVal(PssBinAppSx00AnlgEnumSetValTypeILoad, mLmt4Product);
            if(ret)
            {
                break;
            }
    
            vSetVLoad = mEnd;
            vSetILoad = mLmt4Product;
    
            ret = anlgBrd.SetSrcV(true);
            if(ret)
            {
                break;
            }
    
            msg += QString("%1,%2,%3,%4,%5,%6")
                .arg("SetILoad", 12)
                .arg("SetVLoad", 12)
                .arg("GetVLoad", 12)
                .arg("    VAdc", 12)
                .arg("    VDac", 12)
                .arg("   VReal", 12);
        }
        else
        {
            ret = anlgBrd.SetAnlgVal(PssBinAppSx00AnlgEnumSetValTypeVLoad, mLmt4Product);
            if(ret)
            {
                break;
            }
    
            ret = anlgBrd.SetAnlgVal(PssBinAppSx00AnlgEnumSetValTypeILoad, mEnd);
            if(ret)
            {
                break;
            }
    
            vSetVLoad = mLmt4Product;
            vSetILoad = mEnd;
            ret = anlgBrd.SetSrcV(false);
            if(ret)
            {
                break;
            }
    
            msg += QString("%1,%2,%3,%4,%5,%6")
                .arg("SetVLoad", 12)
                .arg("SetILoad", 12)
                .arg("GetILoad", 12)
                .arg("    IAdc", 12)
                .arg("    IDac", 12)
                .arg("   IReal", 12);
        }
        msg += QString(",%1,%2")
                .arg(" DacDiff", 12)
                .arg(" AdcDiff", 12);
        str = msg.toUtf8();
        emit sigSendMsg(str);
        /* 清空数据,避免上次校准 带来干扰 */
        Clear();
    
        /* 3: 采样循环 */
        int iMax = calPointNums + 1;
        for(int i = 0; i < iMax; i++)
        {
            double val = mStart + i * step;
    #if 0
            /* 3.1: 采样 */
            if(("1uAP" == GetName()) && (0 == i)) /* 1uAP 第一个点多采样两次 */
            {
                SampleAPoint(anlgBrd, instrument, vSetVLoad, vSetILoad, val, i);
                SampleAPoint(anlgBrd, instrument, vSetVLoad, vSetILoad, val, i);
            }
    #endif
            ret = SampleAPoint(anlgBrd, instrument, vSetVLoad, vSetILoad, val, i);
            if(ret)
            {
                break;
            }
    
            /* 3.2: 检查 */
            if(doCheck && ((fabs(mDacDifRate) >= 1) || (fabs(mAdcDifRate) >= 1)))
            {
                mErrorMsg = QString("[CalFailed]> %1 : Calibration Check Failled !\n").arg(mName);
                qCritical() << mErrorMsg;
                QByteArray str = mErrorMsg.toUtf8();
                emit sigCalCheckError(str);
                /* 此处只是提示报错，继续校准其他点数，因此返回值为成功 */
                ret = PssRstSuccess;
                //break;
            }
        }


    }while(0);
#endif
    return ret;
}

PssRstT CalRunner::SampleAPoint(Sx00QtAnlgBrd &anlgBrd, QtIInstrument &instrument, double vSetVLoad, double vSetILoad, double val, int pointIndex)
{
#if 0
    int size = 10;
    QVector<double> vals;
    vals.clear();
#endif

    double vGetVLoad = 0;
    double vGetILoad = 0;
    double vGetVAdc  = 0;
    double vGetIAdc  = 0;
    double vGetVDac  = 0;
    double vGetIDac  = 0;
    double vReal     = 0;

#ifdef _PSS_QT_PLX00_
    PSS_UNUSED(anlgBrd);
    PSS_UNUSED(instrument);
    PSS_UNUSED(vSetVLoad);
    PSS_UNUSED(vSetILoad);
    PSS_UNUSED(val);
    PSS_UNUSED(pointIndex);
    PSS_UNUSED(vGetVLoad);
    PSS_UNUSED(vGetILoad);
    PSS_UNUSED(vGetVAdc);
    PSS_UNUSED(vGetIAdc);
    PSS_UNUSED(vGetVDac);
    PSS_UNUSED(vGetIDac);
    PSS_UNUSED(vReal);
#endif
    PssRstT ret = PssRstSuccess;
#ifdef _PSS_QT_SX00_
    /* 1: 设置待测点的电压/电流 */
    if(mIsCalV)
    {
        ret = anlgBrd.SetAnlgVal(PssBinAppSx00AnlgEnumSetValTypeVLoad, val);
        vSetVLoad = val;
    }
    else
    {
        ret = anlgBrd.SetAnlgVal(PssBinAppSx00AnlgEnumSetValTypeILoad, val);
        vSetILoad = val;
    }

    if(ret)
    {
        return ret;
    }
    
    ret = ConfigureMeasure(instrument, mIsCalV, mLmt4Instrument, mNplc4Instrument);
    if(ret)
    {
        mErrorMsg = QString("Config standard(isMeasV:%1, lmtVal:%2, nplc:%3) device failed !")\
                .arg(QString::number(mIsCalV)).arg(QString::number(mLmt4Instrument).arg(QString::number(mNplc4Instrument)));
        qCritical() << mErrorMsg;
        QByteArray str = mErrorMsg.toUtf8();
        emit sigErrorMsg(str);
        return ret;
    }
    //qDebug() << i << val;
    
    /* 2: 开输出 + 延迟 */
    ret = anlgBrd.SetOutput(true);
    if(ret)
    {
        mErrorMsg = QString("Anlg Output(true) failed !");
        qCritical() << mErrorMsg;
        QByteArray str = mErrorMsg.toUtf8();
        emit sigErrorMsg(str);
        return ret;
    }
    ret = (PssRstT)instrument.SetOutput(true, false);
    if(ret)
    {
        mErrorMsg = QString("Standard Output(true) failed !");
        qCritical() << mErrorMsg;
        QByteArray str = mErrorMsg.toUtf8();
        emit sigErrorMsg(str);
        return ret;
    }
    QThread::msleep(mcUpdateValStableMs);
    
    /* 3: 采样 */
    if(mIsCalV)
    {
        ret = (PssRstT)instrument.GetMeasureVal(PssMeasureVolt, vReal);
        if(ret)
        {
            mErrorMsg = QString("Get Standard Measure value(VOLT) failed: %1").arg(ret);
            qCritical() << mErrorMsg;
            QByteArray str = mErrorMsg.toUtf8();
            emit sigErrorMsg(str);
            return ret;
        }
        QThread::msleep(50);
        ret = anlgBrd.GetAnlgVal(PssBinAppSx00AnlgEnumValRstTypeVLoad, vGetVLoad);
        if(ret)
        {
            return ret;
        }
    
        ret = anlgBrd.GetAnlgVal(PssBinAppSx00AnlgEnumValRstTypeVAdc,  vGetVAdc);
        if(ret)
        {
            return ret;
        }
    
        ret = anlgBrd.GetAnlgVal(PssBinAppSx00AnlgEnumValRstTypeVDac,  vGetVDac);
        if(ret)
        {
            return ret;
        }
    
        /* 计算偏差 */
        mDacDifRate = 1000 * (vSetVLoad - vReal) / vReal;
        mAdcDifRate = 1000 * (vGetVLoad - vReal) / vReal;
    }
    else
    {
        ret = (PssRstT)instrument.GetMeasureVal(PssMeasureCurr, vReal);
        if(ret)
        {
            mErrorMsg = QString("Get Standard Measure value(CURR) failed: %1").arg(ret);
            qCritical() << mErrorMsg;
            QByteArray str = mErrorMsg.toUtf8();
            emit sigErrorMsg(str);
            return ret;
        }
        vReal = -vReal; /* instrument为负载,故电流反向 */
    
        ret = anlgBrd.GetAnlgVal(PssBinAppSx00AnlgEnumValRstTypeILoad, vGetILoad);
        if(ret)
        {
            return ret;
        }
    
        ret = anlgBrd.GetAnlgVal(PssBinAppSx00AnlgEnumValRstTypeIAdc,  vGetIAdc);
        if(ret)
        {
            return ret;
        }
    
        ret = anlgBrd.GetAnlgVal(PssBinAppSx00AnlgEnumValRstTypeIDac,  vGetIDac);
        if(ret)
        {
            return ret;
        }
    
        /* 计算偏差 */
        mDacDifRate = 1000 * (vSetILoad - vReal) / vReal;
        mAdcDifRate = 1000 * (vGetILoad - vReal) / vReal;
    }
    
    /* 4: 关输出 */
    ret = anlgBrd.SetOutput(false);
    if(ret)
    {
        mErrorMsg = QString("Anlg Output(aflse) failed !");
        qCritical() << mErrorMsg;
        QByteArray str = mErrorMsg.toUtf8();
        emit sigErrorMsg(str);
        return ret;
    }
    
    ret = (PssRstT)instrument.SetOutput(false);
    if(ret)
    {
        mErrorMsg = QString("Standard Output(false) failed !");
        qCritical() << mErrorMsg;
        QByteArray str = mErrorMsg.toUtf8();
        emit sigErrorMsg(str);
        return ret;
    }
    
    /* 5: 回显 */
    QString msg = QString("%1:").arg(pointIndex, 4);
    if(mIsCalV)
    {
        msg += QString("%1,%2,%3,%4,%5")
            .arg(vSetILoad, 12, 'E', 5)
            .arg(vSetVLoad, 12, 'E', 5)
            .arg(vGetVLoad, 12, 'E', 5)
            .arg(vGetVAdc,  12, 'E', 5)
            .arg(vGetVDac,  12, 'E', 5);
    }
    else
    {
        msg += QString("%1,%2,%3,%4,%5")
            .arg(vSetVLoad, 12, 'E', 5)
            .arg(vSetILoad, 12, 'E', 5)
            .arg(vGetILoad, 12, 'E', 5)
            .arg(vGetIAdc,  12, 'E', 5)
            .arg(vGetIDac,  12, 'E', 5);
    }
    msg += QString("%1,%2 (*milli),%3 (*milli)")
            .arg(vReal,       12, 'E', 5)
            .arg(mDacDifRate, 11, 'F', 7)
            .arg(mAdcDifRate, 11, 'F', 7);


    QByteArray str = mErrorMsg.toUtf8();
    emit sigSendMsg(str);
    /* 3.6: 记录数据 */
    AppendSetVLoad(vSetVLoad);
    AppendSetILoad(vSetILoad);
    AppendGetVLoad(vGetVLoad);
    AppendGetILoad(vGetILoad);
    AppendGetVAdc(vGetVAdc);
    AppendGetIAdc(vGetIAdc);
    AppendGetVDac(vGetVDac);
    AppendGetIDac(vGetIDac);
    AppendGetVReal(vReal);
    AppendGetIReal(vReal);

#if 0
    for(int i = 0; i< size; i++)
    {
        qDebug() << QString("%1").arg(vals[i]);
    }
#endif
#endif
    return ret;
}

/*
 * 测量电压: 设置为电流源,输出0;
 * 测量电流: 设置为电压源,输出0;
 *
  */
 PssRstT CalRunner::ConfigureMeasure(QtIInstrument &instrument, bool isMeasV, double lmtVal, double nplc)
 {
    PssInstrumentCfgInfo info;
    if(isMeasV)
    {
        info.isVSrc = false;
    }
    else
    {
        info.isVSrc = true;
    }

    info.srcVal = 0;
    info.lmtVal = lmtVal;
    info.nplc   = nplc;
    PssRstT ret = PssRstSuccess;
    ret = (PssRstT)instrument.SetSrcInfo(&info);
    if(ret)
    {
        qCritical() << "Set cal-standard source info failed: " << ret;
        return ret;
    }
 #if 1
    /* 如果较低档位校准不过，可尝试增大分辨率，此处可用在构造时用标志表明是否要设置分辨率 */
    if((mName == "100nAP") || (mName == "100nAN"))
    {
        /* 设置最高分辨率 */
        ret = instrument.SetResolution(isMeasV, 6);
        if(ret)
        {
            return ret;
        }
    }
 #endif
    return ret;
 }