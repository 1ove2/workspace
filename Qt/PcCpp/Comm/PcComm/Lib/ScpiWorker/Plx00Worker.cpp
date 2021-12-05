#include "Plx00Worker.h"
#include "Plx00Instrument.h"
#include <QThread>
#include <QApplication>
#include <QTime>
#include <QDateTime>

Plx00Worker::Plx00Worker(int key, const QString &ip, QObject *parent)
    :InstWorker(key, ip, parent),
      mPlx00(nullptr)
{
}

Plx00Worker::Plx00Worker(int key, const QString &comName, int baud, QObject *parent)
    :InstWorker(key, comName, baud, parent),
      mPlx00(nullptr)
{

}

Plx00Worker::Plx00Worker(int key, int cardAddr, int devAddr, QObject *parent)
    :InstWorker(key, cardAddr, devAddr, parent),
      mPlx00(nullptr)
{

}

Plx00Worker::~Plx00Worker()
{
    delete mPlx00;
    mPlx00 = nullptr;
}

void Plx00Worker::SetScanPara(int key, const ScanPara &para)
{

}

void Plx00Worker::Init()
{
    if (LAN == mType)
    {
        mPlx00 = new PLx00Instrument(QHostAddress(mIp));
    }
    else if (RS232 == mType)
    {
        mPlx00 = new PLx00Instrument(mComName, mBaud);
    }
    else if (GPIB == mType)
    {
#ifdef _PSS_GPIB_SUPPORT_
        mPlx00 = new PLx00Instrument(mCardAddr, mDevAddr);
#endif
    }
    bool isConnect = true;
    if (!mPlx00->Connect())
    {
        isConnect = false;
    }
    emit sigIsConnectSucceed(isConnect);
}

void Plx00Worker::SetLivPara(const UiArgSet &para)
{
    int ret = mPlx00->SetMaxPower(para.maxPower);
    if (0 != ret)
    {
        return;
    }
    ret = mPlx00->SetOutputMode(para.isPulseMode);
    if (0 != ret)
    {
        return;
    }
    ret = mPlx00->SetOverVolProtect(para.overVolProtect);
    if (0 != ret)
    {
        return;
    }
    ret = mPlx00->SetOffsetVol(para.powerOffsetVol);
    if (0 != ret)
    {
        return;
    }
    ret = mPlx00->SetWaveLength(para.waveLen);
    if (0 != ret)
    {
        return;
    }
    ret = mPlx00->SetPulsePara(para.pulseWidth, para.pulsePeriod);
    if (0 != ret)
    {
        return;
    }
    ret = mPlx00->SetDutvCtlType(para.maxCheckVoltage);
    if (0 != ret)
    {
        return;
    }
    /* 特殊参数配置 */
    if (para.isCustomArgument)
    {
        ret = mPlx00->SetSampDelay(para.sampDelay);
        if (0 != ret)
        {
            return;
        }
        ret = mPlx00->SetSampPoint(para.sampPoint);
        if (0 != ret)
        {
            return;
        }
        ret = mPlx00->SetInitDataCtrl(para.isInitData);
        if (0 != ret)
        {
            return;
        }
    }
    ret = mPlx00->SetOutputDelay(para.trigInDelay);
    if (0 != ret)
    {
        return;
    }
    ret = mPlx00->SetTrigDelay(para.trigOutDelay);
    if (0 != ret)
    {
        return;
    }
    ret = mPlx00->SetTrigPulseWidth(para.trigOutWidth);
    if (0 != ret)
    {
        return;
    }
    /* 扫描模式 */
    if (para.isScanOutput)
    {
        ret  = mPlx00->SetScanCurrVal(para.startVal, para.stepVal, para.stopVal);
        if (0 != ret)
        {
            return;
        }
        ret = mPlx00->SetStartScan();
        if (0 != ret)
        {
            return;
        }
        QThread::msleep(100);
        bool isReady = false;
        int points = ((para.stopVal - para.startVal)/para.stepVal);
        int timeout = points*(para.pulsePeriod / 1000) + 30000;//超时时间在计算后上调300000us
        while(1)
        {
            QCoreApplication::processEvents();
            ret = mPlx00->GetSweepStatus(isReady, timeout);
            if (0 != ret)
            {
                return;
            }
            if (isReady)
            {
                break;
            }
            QThread::msleep(300);
        }
        QVector<double> vecI;
        QVector<double> vecV;
        QVector<double> vecP;
        QVector<double> vecB;
        ret = mPlx00->GetSweepResult(vecI, vecV, vecP, vecB);
        if (0 != ret)
        {
            return;
        }
        emit sigResultReady(vecI, vecV, vecP, vecB);
        return;
    }
    ret = mPlx00->SetDCScanPoint(para.pulseDcScanPoint);
    ret = mPlx00->SetDCCurrent(para.Dcurrent);
    if (para.isPulseMode && 0 != para.pulseDcScanPoint)
    {
        QThread::msleep(100);
        bool isReady = false;
        int points = ((para.stopVal - para.startVal)/para.stepVal);
        int timeout = points*(para.pulsePeriod / 1000) + 30000;//超时时间在计算后上调300000us
        while(1)
        {
            QCoreApplication::processEvents();
            ret = mPlx00->GetSweepStatus(isReady, timeout);
            if (0 != ret)
            {
                return;
            }
            if (isReady)
            {
                break;
            }
            QThread::msleep(300);
        }
        QVector<double> vecI;
        QVector<double> vecV;
        QVector<double> vecP;
        QVector<double> vecB;
        ret = mPlx00->GetSweepResult(vecI, vecV, vecP, vecB);
        if (0 != ret)
        {
            return;
        }
        emit sigResultReady(vecI, vecV, vecP, vecB);
        return;
    }
}

void Plx00Worker::DCQuery()
{
    double valV, valI, valP, valB;
    int ret = mPlx00->GetTestDC(valV, valI, valP, valB);
    if (0 != ret)
    {
        return;
    }
    emit sigResultReady({valV},{valI},{valP},{valB});
}

void Plx00Worker::SetScanPara(const ScanPara &para)
{

}

void Plx00Worker::Connect()
{
    mPlx00->Connect();
}

void Plx00Worker::SetNetUpgrade(int port)
{

}

void Plx00Worker::SetScanPara(int key, const QVector<ScanPara> &paras)
{

}
