#include "Sx00ReaderWorker.h"
#include "Sx00Instrument.h"
#include <QTime>
#include <QTimer>
#include <QThread>
#include <QApplication>
#include <QHostAddress>

using namespace Pss::Qt;
Sx00ReaderWorker::Sx00ReaderWorker(int key, const QString &ip, QObject *parent)
    :ScpiWorker(key, ip, parent),
      mEnable(true),
      mSx00(nullptr)
{

}

Sx00ReaderWorker::Sx00ReaderWorker(int key, const QString &comName, int comBaud, QObject *parent)
    :ScpiWorker(key, comName, comBaud, parent),
      mEnable(true),
      mSx00(nullptr)
{

}

Sx00ReaderWorker::Sx00ReaderWorker(int key, int cardAddr, int devAddr, QObject *parent)
    :ScpiWorker(key, cardAddr, devAddr, parent),
      mEnable(true),
      mSx00(nullptr)
{

}

Sx00ReaderWorker::~Sx00ReaderWorker()
{
    delete mSx00;
    mSx00 = nullptr;
}

void Sx00ReaderWorker::Init()
{
    delete mSx00;
    mSx00 = nullptr;

    if (LAN == mType)
    {
        mSx00 = new Sx00Instrument(QHostAddress(mIp));
    }
    else if (RS232 == mType)
    {
        mSx00 = new Sx00Instrument(mComName, mBaud);
    }
    else if (GPIB == mType)
    {
#ifdef _PSS_GPIB_SUPPORT_
        mSx00 = new Sx00Instrument(mCardAddr, mDevAddr);
#endif
    }
    bool isConnect = true;
    if (!mSx00->Connect())
    {
        isConnect = false;
    }
    emit sigIsConnectSucceed(isConnect, mIp);
}

void Sx00ReaderWorker::Connect(int chanID)
{
    if (chanID != mInstKey)
    {
        return;
    }
    mSx00->Connect();
}

void Sx00ReaderWorker::SetOutCtrl(bool isOn)
{
    if (!mEnable)
    {
        return;
    }
    int ret = mSx00->TraceSet(isOn);
    ret = mSx00->SetOutput(isOn, false);
    if (0 != ret)
    {
        return;
    }
    if (isOn)
    {
        /* 开输出并读数 */
        GetReadResult();
    }
}

void Sx00ReaderWorker::SetReadPara(const MeasPara &para)
{
    if (para.instKey != mInstKey)
    {
        return;
    }
    mEnable = para.isEnable;
    if (!para.isEnable)
    {
        return;
    }
    /* 设置plc */
    int ret = mSx00->SetNPLC(para.isSrcV, para.nplc);
    if (0 != ret)
    {
        return;
    }
    /* 二四线 */
    ret = mSx00->Set4W(para.is4Wires);
    if (0 != ret)
    {
        return;
    }
    /* 设置源 */
    ret = mSx00->SetVSrc(para.isSrcV);
    if (0 != ret)
    {
        return;
    }
    /* 设置源量程 */
    if (para.isSrcAuto)
    {
        ret = mSx00->SetSrcAutoRange(para.isSrcV, true);
    }
    else
    {
        ret = mSx00->SetSrcRange(para.isSrcV, para.srcRange);
    }
    if (0 != ret)
    {
        return;
    }
    /* 设置源值 */
    ret = mSx00->SetSrcVal(para.isSrcV,para.srcVal);
    if (0 != ret)
    {
        return;
    }
    /* 设置限量程 */
    if (para.isLmtAuto)
    {
        ret = mSx00->SetLmtAutoRange(para.isSrcV, true);
    }
    else
    {
        ret = mSx00->SetLmtRange(para.isSrcV, para.lmtRange);
    }
    if (0 != ret)
    {
        return;
    }
    /* 设置限值 */
    ret = mSx00->SetLmtVal(para.isSrcV, para.lmtVal);
    if (0 != ret)
    {
        return;
    }
}

void Sx00ReaderWorker::GetReadResult()
{
    if (!mEnable)
    {
        return;
    }
    QTime time;
    double valV, valI;

    time.start();
    /* 如果本次为读取成功 继续读取直到5s超时 */
    int ret = mSx00->ReadOutput(valV, valI, MaxReadTimeout);
    
    /* 读取成功 发射数值 */
    if (0 != ret)
    {
        return;
    }
    
    QVector<RecvDataPara> para;
    
    RecvDataPara rDataP;
    rDataP.volt = valV;
    rDataP.curr = valI;
    rDataP.key = mInstKey;
    rDataP.card = 0;
    rDataP.channel = 0;
    para.append(rDataP);
    
    DataTag dataP;
    dataP.time = time.elapsed();
    dataP.para = para;
    
    qDebug() << "time:" << time.elapsed();
    emit sigResultReady(mInstKey, dataP);
}

void Sx00ReaderWorker::SetUpgradePort(const ChanInfo &info,int port)
{
    int key = info.key;
    if (key != mInstKey)
    {
        return;
    }
    mSx00->SetNetUpgradePort(port);
}