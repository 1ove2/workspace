#include "CSx00ReaderWorker.h"
#include "CSx00Instrument.h"
#include <QTime>
#include <QTimer>
#include <QThread>
#include <QApplication>
#include <QHostAddress>

using namespace Pss::Qt;
CSx00ReaderWorker::CSx00ReaderWorker(const QMap<int,QVector<int>> &chan,int key, const QString &ip, QObject *parent)
    :ScpiWorker(key, ip, parent),
      mUsedChan(chan),
      mCSx00(nullptr)
{

}

CSx00ReaderWorker::CSx00ReaderWorker(const QMap<int,QVector<int>> &chan,int key, const QString &comName, int comBaud, QObject *parent)
    :ScpiWorker(key, comName, comBaud, parent),
      mUsedChan(chan),
      mCSx00(nullptr)
{

}

CSx00ReaderWorker::CSx00ReaderWorker(const QMap<int,QVector<int>> &chan,int key, int cardAddr, int devAddr, QObject *parent)
    :ScpiWorker(key, cardAddr, devAddr, parent),
      mUsedChan(chan),
      mCSx00(nullptr)
{

}

CSx00ReaderWorker::~CSx00ReaderWorker()
{
    delete mCSx00;
    mCSx00 = nullptr;
}

void CSx00ReaderWorker::Init()
{
    delete mCSx00;
    mCSx00 = nullptr;

    if (LAN == mType)
    {
        mCSx00 = new CSx00Instrument(QHostAddress(mIp));
    }
    else if (RS232 == mType)
    {
        mCSx00 = new CSx00Instrument(mComName, mBaud);
    }
    else if (GPIB == mType)
    {
#ifdef _PSS_GPIB_SUPPORT_
        mSx00 = new CSx00Instrument(mCardAddr, mDevAddr);
#endif
    }
    bool isConnect = true;
    if (!mCSx00->Connect())
    {
        isConnect = false;
    }
    emit sigIsConnectSucceed(isConnect, mIp);
}

void CSx00ReaderWorker::Connect(int chanID)
{
    if (chanID != mInstKey)
    {
        return;
    }
    mCSx00->Connect();
}

void CSx00ReaderWorker::SetOutCtrl(bool isOn)
{
    /* step1 遍历所有卡 */
    int cardCnt = mUsedChan.count();
    for (int i = 0; i < cardCnt; ++i)
    {
#if 1
        /* step2 遍历卡中所有通道 */
        int chanCnt = mUsedChan.values().value(i).count();
        for (int j = 0; j < chanCnt; ++j)
        {
            int cardID = mUsedChan.keys().value(i); /* 卡号 */
            QVector<uint8_t> chanNums = {(uint8_t)mUsedChan.values().value(i).value(j)}; /* 卡内单个通道号 */
            /* 现阶段分别开或关每个通道的输出  tips:后续和软核商定是否能同时开关所有通道输出 */
            int ret = mCSx00->SetChannelGroup(cardID, chanNums);  /* 设置通道组*/
            if (0 != ret)
            {
                emit sigError(SendError);
                return;
            }
            ret = mCSx00->SetOutput(cardID, isOn);                /* 开关输出 */
            if (0 !=  ret)
            {
                emit sigError(SendError);
                return;
            }
        }
#endif
        /* step3 配置卡内响应指令的通道号 */
        QVector<uint8_t> chan;
        for (int m = 0; m < mUsedChan.values().value(i).count(); ++m)
        {
            chan.append((uint8_t)mUsedChan.values().value(i).value(m));
        }
        int ret = mCSx00->SetChannelGroup((uint8_t)mUsedChan.keys().value(i), chan);
        if (0 !=  ret)
        {
            emit sigError(SendError);
            return;
        }
    }

    /* 开输出必须读数 */
    if (isOn)
    {
        GetReadResult();
    }

}

void CSx00ReaderWorker::SetReadPara(const MeasPara &para)
{
    /* 设备标识符不一致 不响应 */
    if (para.instKey != mInstKey)
    {
        return;
    }
    /*  剔除不使用的通道 */
    if (!para.isEnable)
    {
        /* 剔除通道 */
        if (mUsedChan.keys().contains(para.cardID))
        {
            if (mUsedChan[para.cardID].contains(para.chanID))
            {
                mUsedChan[para.cardID].removeOne(para.chanID);
//                mUsedChan[para.cardID].remove(para.chanID);
            }
        }
    }
    else
    {
        /* 增加使用的通道 */
        if (!mUsedChan.keys().contains(para.cardID))
        {
            QVector<int> tmp;
            mUsedChan.insert(para.cardID, tmp);
        }
        if (!mUsedChan[para.cardID].contains(para.chanID))
        {
            mUsedChan[para.cardID].append(para.chanID);
        }
    }

    QVector<uint8_t> vec;
    vec.append((uint8_t)para.chanID);
    int ret = mCSx00->SetChannelGroup(para.cardID, vec);
    if (0 != ret)
    {
        return;
    }
    /* 设置plc */
    ret = mCSx00->SetNPLC(para.cardID, para.isSrcV, para.nplc);
    if (0 != ret)
    {
        return;
    }
    /* 二四线 */
    ret = mCSx00->Set4W(para.cardID, para.is4Wires);
    if (0 != ret)
    {
        return;
    }
    /* 设置源 */
    ret = mCSx00->SetVSrc(para.cardID, para.isSrcV);
    if (0 != ret)
    {
        return;
    }
    /* 设置源量程 */
    if (para.isSrcAuto)
    {
        ret = mCSx00->SetSrcAutoRange(para.cardID, para.isSrcV, true);
    }
    else
    {
        ret = mCSx00->SetSrcRange(para.cardID, para.isSrcV, para.srcRange);
    }
    if (0 != ret)
    {
        return;
    }
    /* 设置源值 */
    ret = mCSx00->SetSrcVal(para.cardID, para.isSrcV,para.srcVal);
    if (0 != ret)
    {
        return;
    }
    /* 设置限量程 */
    if (para.isLmtAuto)
    {
        ret = mCSx00->SetLmtAutoRange(para.cardID, para.isSrcV, true);
    }
    else
    {
        ret = mCSx00->SetLmtRange(para.cardID, para.isSrcV, para.lmtRange);
    }
    if (0 != ret)
    {
        return;
    }
    /* 设置限值 */
    ret = mCSx00->SetLmtVal(para.cardID, para.isSrcV, para.lmtVal);
    if (0 != ret)
    {
        return;
    }
}

void CSx00ReaderWorker::GetReadResult()
{
    QString str;
    QTime time;
    time.start();
    QVector<uint8_t> cardVec;
    for (int i = 0; i < mUsedChan.count(); ++i)
    {
        cardVec.append(mUsedChan.keys().value(i));
    }
    mCSx00->ReadArray(str, cardVec, MaxReadTimeout);
    mTime = time.elapsed();
    qDebug() << "Str" << str;
    ParseData(str);
}

void CSx00ReaderWorker::SetUpgradePort(const ChanInfo &info, int port)
{
    int key = info.key;
    if (key != mInstKey)
    {
        return;
    }
    mCSx00->SetNetUpgradePort(port);
}

void CSx00ReaderWorker::ParseData(const QString &str)
{
    QStringList chanStrList;
    QString chanStr;
    bool start = false;

    /* 数据格式[3-1:v,i;v,i]\r[3-2:v,i;v,i]\r\n */
    /* 先解析出[] */
    int size = str.size();
    for (int i = 0; i < size; ++i)
    {
        /* 解析到头, 开始取数据*/
        if (str[i] == '[')
        {
            start = true;
            continue;
        }
        /* 解析到尾, 重新开始解析 */
        if (str[i] == ']')
        {
            start = false;
            chanStrList.append(chanStr);
            chanStr.clear();
        }
        if (start)
        {
            chanStr.append(str[i]);
        }
    }
    QVector<RecvDataPara> para;
    
    /* 再解析出[]内的卡-通道:v,i */
    for (int i = 0; i < chanStrList.size(); ++i)
    {
        QString chanStr = chanStrList.value(i);
        QStringList list = chanStr.split(":");
        QString header = list.value(0);
        QString value = list.value(1);
    
        double valV = value.split(",").value(0).toDouble();
        double valI = value.split(",").value(1).toDouble();
    
        int card = header.split("-").value(0).toInt();
        int chan = header.split("-").value(1).toInt();
    
        RecvDataPara rDataP;
        rDataP.volt = valV;
        rDataP.curr = valI;
        rDataP.key = mInstKey;
        rDataP.card = card;
        if (3 == chan)
        {
            chan = 2;
        }
        else if ( 4 == chan)
        {
            chan = 3;
        }
        else if (2 == chan)
        {
            chan = 4;
        }
        rDataP.channel = chan;
    
        para.append(rDataP);
    }
    DataTag dataP;
    dataP.time = mTime;
    dataP.para = para;
    emit sigResultReady(mInstKey, dataP);
}