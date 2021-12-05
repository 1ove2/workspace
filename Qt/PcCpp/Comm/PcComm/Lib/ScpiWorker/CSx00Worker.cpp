#include "CSx00Worker.h"
#include <QTime>
#include <QApplication>
#include <QThread>

CSx00Worker::CSx00Worker(int key, const QString &ip, QObject *parent)
    : InstWorker(key, ip, parent),
      mCSx00(nullptr),
      isChan1Ready(true),
      isChan2Ready(true),
      isChan3Ready(true)
{

}

CSx00Worker::CSx00Worker(int key, const QString &comName, int baud, QObject *parent)
    : InstWorker(key, comName, baud, parent),
      mCSx00(nullptr),
      isChan1Ready(true),
      isChan2Ready(true),
      isChan3Ready(true)
{

}

CSx00Worker::CSx00Worker(int key, int card, int dev, QObject *parent)
    : InstWorker(key, card, dev, parent),
      mCSx00(nullptr),
      isChan1Ready(true),
      isChan2Ready(true),
      isChan3Ready(true)
{

}

CSx00Worker::~CSx00Worker()
{
    delete mCSx00;
    mCSx00 = nullptr;
}

void CSx00Worker::SetScanPara(int key, const ScanPara &para)
{
    Q_UNUSED(key);
    Q_UNUSED(para);
}

void CSx00Worker::SetScanPara(const ScanPara &para)
{   
    /* 发送24线等公共scpi指令 */
    int ret = SendCommScpiList(para);
    if (0 != ret)
    {
        return;
    }
    ret = mCSx00->ClearEvent(para.chanID);
    if (0 != ret)
    {
        return;
    }
    switch (para.type)
    {
    case ScanPara::Read:
    {
        ret = SetReadPara(para);
        break;
    }
    case ScanPara::List:
    {
        ret = SetListPara(para);
        break;
    }
    case ScanPara::Apd:
    case ScanPara::Swe:
    {
        ret = SetSwePara(para);
        break;
    }
    default:
        break;
    }
}

void CSx00Worker::SetNetUpgrade(int port)
{
    Q_UNUSED(port);
}

void CSx00Worker::Init()
{
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
        mCSx00 = new CSx00Instrument(mCardAddr, mDevAddr);
#endif
    }
    bool isConnect = true;
    if (!mCSx00->Connect())
    {
        isConnect = false;
        emit sigInstModel(false);
    }
    emit sigInstModel(true, mCSx00->GetIdn());
    emit sigIsConnectSucceed(isConnect);
}

void CSx00Worker::SetScanPara(int key, const QVector<ScanPara> &paras)
{

}

void CSx00Worker::Connect()
{

}

void CSx00Worker::SetIPConfig(bool isAuto, const QStringList &ipList)
{
    int ret = mCSx00->SetIPConfig(isAuto, ipList.value(0), ipList.value(1), ipList.value(2));
    if (0 != ret)
    {
        qDebug() << "IP信息设置失败!";
        return;
    }
    /* 需要发指令生效 */
    ret = mCSx00->SetUpdateIPConfig();
    if (0 != ret)
    {
        qDebug() << "IP信息应用失败!";
        return;
    }
}

void CSx00Worker::SetUARTConfig(int baud)
{
    int ret = mCSx00->SetUARTConfig(baud);
    if (0 != ret)
    {
        qDebug() << "UART波特率设置失败!";
        return;
    }
}

void CSx00Worker::SetGPIBConfig(int devAddr)
{
    int ret = mCSx00->SetGPIBConfig(devAddr);
    if (0 != ret)
    {
        qDebug() << "GPIB地址设置失败!";
        return;
    }
}

void CSx00Worker::SetEvent(int chanID, EventT eventTag)
{
    /* 配置前先清空事件 */
    int ret = mCSx00->ClearEvent(chanID);
    if (0 != ret)
    {
        qDebug() << "error" << ret;
        return;
    }
    /* 配置五种事件 */
    ret = mCSx00->SetStartOutputEvent(chanID, eventTag.startOutputIn,eventTag.startOutputOut,eventTag.startOutputEdge);
    if (0 != ret)
    {
        qDebug() << "error" << ret;
        return;
    }
    ret = mCSx00->SetFinishOutputEvent(chanID, eventTag.finishOutputIn, eventTag.finishOutputOut, eventTag.finishOutputEdge);
    if (0 != ret)
    {
        qDebug() << "error";
        return;
    }
    ret = mCSx00->SetStartSamplingEvent(chanID, eventTag.startSamplingIn, eventTag.startSamplingOut, eventTag.startSamplingEdge);
    if (0 != ret)
    {
        qDebug() << "error";
        return;
    }
    ret = mCSx00->SetFinishSamplingEvent(chanID, eventTag.finishSamplingIn, eventTag.finishSamplingOut, eventTag.finishSamplingEdge);
    if (0 != ret)
    {
        qDebug() << "error";
        return;
    }
    ret = mCSx00->SetStartSweEvent(chanID, eventTag.startSweIn, eventTag.startSweOut, eventTag.startSweEdge);
    if (0 != ret)
    {
        qDebug() << "error";
        return;
    }
}

void CSx00Worker::ClearEvent(int chanID)
{
    int ret = mCSx00->ClearEvent(chanID);
    if (0 != ret)
    {
        qDebug() << "error:" << ret;
    }
}

void CSx00Worker::SetTrigEvent(const TrigPara &para)
{
    /* 配置前先清空事件 */
    int ret = mCSx00->ClearEvent(para.chanID);
    if (0 != ret)
    {
        qDebug() << "error" << ret;
        return;
    }
    /* 配置五种事件 */
//    ret = mCSx00->SetStartOutputEvent(para.chanID, para.eventTag.startOutputIn,para.eventTag.startOutputOut,para.eventTag.startOutputEdge);
//    if (0 != ret)
//    {
//        qDebug() << "error" << ret;
//        return;
//    }
    ret = mCSx00->SetFinishOutputEvent(para.chanID, para.eventTag.finishOutputIn, para.eventTag.finishOutputOut, para.eventTag.finishOutputEdge);
    if (0 != ret)
    {
        qDebug() << "error";
        return;
    }
//    ret = mCSx00->SetStartSamplingEvent(para.chanID, para.eventTag.startSamplingIn, para.eventTag.startSamplingOut, para.eventTag.startSamplingEdge);
//    if (0 != ret)
//    {
//        qDebug() << "error";
//        return;
//    }
//    ret = mCSx00->SetFinishSamplingEvent(para.chanID, para.eventTag.finishSamplingIn, para.eventTag.finishSamplingOut, para.eventTag.finishSamplingEdge);
//    if (0 != ret)
//    {
//        qDebug() << "error";
//        return;
//    }
    if (0 != para.eventTag.finishOutputIn)
    {
        ret = mCSx00->SetStartSweEvent(para.chanID, para.eventTag.startSweIn, para.eventTag.startSweOut, para.eventTag.startSweEdge);
        if (0 != ret)
        {
            qDebug() << "error";
            return;
        }
    }
    ret = mCSx00->SetTrigCount(para.chanID, para.trigCount);
    if (0 != ret)
    {
        qDebug() << "error";
        return;
    }
//    qDebug() << "Trig Count:" << para.trigCount;
//    ret = mCSx00->SetOutputDelay(para.chanID, para.outputDelay);
//    if (0 != ret)
//    {
//        qDebug() << "error";
//        return;
//    }
//    ret = mCSx00->SetTrigDelay(para.chanID, para.trigDelay);
//    if (0 != ret)
//    {
//        qDebug() << "error";
//        return;
//    }

}

void CSx00Worker::SetMosPara(const QVector<ScanPara> &paraVec)
{
    /* 配置从机 */
    ScanPara masterPara = paraVec.value(0);
    ScanPara slavePara = paraVec.value(1);

    TrigPara masterTrigPara;
    TrigPara slaveTrigPara;
    
    EventT masterTag;
    EventT slaveTag;
    
    masterTag = {0,0,CSx00ScpiClient::PssCSx00TrigEdgeTypeFall,
                 0,2,CSx00ScpiClient::PssCSx00TrigEdgeTypeRise,
                 0,0,CSx00ScpiClient::PssCSx00TrigEdgeTypeFall,
                 0,0,CSx00ScpiClient::PssCSx00TrigEdgeTypeFall,
                 0,0,CSx00ScpiClient::PssCSx00TrigEdgeTypeFall};
    
    slaveTag = {0,0,CSx00ScpiClient::PssCSx00TrigEdgeTypeFall,
                2,0,CSx00ScpiClient::PssCSx00TrigEdgeTypeRise,
                0,0,CSx00ScpiClient::PssCSx00TrigEdgeTypeFall,
                0,0,CSx00ScpiClient::PssCSx00TrigEdgeTypeFall,
                0,1,CSx00ScpiClient::PssCSx00TrigEdgeTypeRise};
    
    masterTrigPara.chanID = masterPara.chanID;
    masterTrigPara.outputDelay = 0;
    masterTrigPara.trigDelay = 0;
    masterTrigPara.trigCount = 2;
    masterTrigPara.eventTag = masterTag;
    
    slaveTrigPara.chanID = slavePara.chanID;
    slaveTrigPara.outputDelay = 0;
    slaveTrigPara.trigDelay = 0;
    slaveTrigPara.trigCount = 2;
    slaveTrigPara.eventTag = slaveTag;
    
    mCSx00->ClearEvent(1);
    mCSx00->ClearEvent(2);
    mCSx00->ClearEvent(3);
    
    SetTrigEvent(masterTrigPara);
    SetTrigEvent(slaveTrigPara);
    
    int ret = SendCommScpiList(masterPara);
    if (0 != ret)
    {
        return;
    }
    ret = SendCommScpiList(slavePara);
    if (0 != ret)
    {
        return;
    }
    QVector<double> masterVecV,masterVecI;
    QVector<double> slaveVecV, slaveVecI;
    for (int i = 0; i < masterPara.pointList.count(); i++)
    {
        SendSweScpiList(i,slavePara);
        SendSweScpiList(i,masterPara);
    
        mCSx00->SetOutput(slavePara.chanID,true);
        mCSx00->SetOutput(masterPara.chanID,true);
    
        uint32_t point = masterPara.pointList.value(0);
        mCSx00->GetSweepResult(masterPara.chanID,point,masterVecV,masterVecI,MAX_TIMEOUT_MS);
        mCSx00->GetSweepResult(slavePara.chanID,point,slaveVecV,slaveVecI,MAX_TIMEOUT_MS);
    
        emit ResultReady(masterVecV, masterVecI, masterPara.chanID);
        emit ResultReady(slaveVecV, slaveVecI, slavePara.chanID);
    }

#if 0
    /* 配置前先清空事件 */
    int ret = mCSx00->ClearEvent(masterPara.chanID);
    ret = mCSx00->ClearEvent(slavePara.chanID);

    /* 配置五种事件 */
    ret = mCSx00->SetFinishOutputEvent(masterPara.chanID, 0, 1, CSx00ScpiClient::PssCSx00TrigEdgeTypeRise);
    
    ret = mCSx00->SetStartSweEvent(slavePara.chanID, 0, 2, CSx00ScpiClient::PssCSx00TrigEdgeTypeRise);
    ret = mCSx00->SetFinishOutputEvent(slavePara.chanID, 1, 0, CSx00ScpiClient::PssCSx00TrigEdgeTypeRise);
    
    ret = SendSweScpiList(0, slavePara);
    ret = SendSweScpiList(0, masterPara);
    
    mCSx00->SetOutput(slavePara.chanID,true);
    QThread::msleep(1000);
    mCSx00->SetOutput(masterPara.chanID,true);
#endif

}

void CSx00Worker::SetLivPara(const QVector<ScanPara> &paraVec)
{
    if (2 == paraVec.count())
    {
        SetMosPara(paraVec);
    }
    else if (3 == paraVec.count())
    {
        /* 配置从机 */
        ScanPara masterPara = paraVec.value(0);
        ScanPara slave1Para = paraVec.value(1);
        ScanPara slave2Para = paraVec.value(2);

        TrigPara masterTrigPara;
        TrigPara slave1TrigPara;
        TrigPara slave2TrigPara;
    
        EventT masterTag;
        EventT slave1Tag;
        EventT slave2Tag;
    
        masterTag = {0,0,CSx00ScpiClient::PssCSx00TrigEdgeTypeFall,
                     0,2,CSx00ScpiClient::PssCSx00TrigEdgeTypeRise,
                     0,0,CSx00ScpiClient::PssCSx00TrigEdgeTypeFall,
                     0,0,CSx00ScpiClient::PssCSx00TrigEdgeTypeFall,
                     0,0,CSx00ScpiClient::PssCSx00TrigEdgeTypeFall};
    
        slave1Tag = {0,0,CSx00ScpiClient::PssCSx00TrigEdgeTypeFall,
                    2,0,CSx00ScpiClient::PssCSx00TrigEdgeTypeRise,
                    0,0,CSx00ScpiClient::PssCSx00TrigEdgeTypeFall,
                    0,0,CSx00ScpiClient::PssCSx00TrigEdgeTypeFall,
                    0,1,CSx00ScpiClient::PssCSx00TrigEdgeTypeRise};
    
        slave2Tag = {0,0,CSx00ScpiClient::PssCSx00TrigEdgeTypeFall,
                    2,0,CSx00ScpiClient::PssCSx00TrigEdgeTypeRise,
                    0,0,CSx00ScpiClient::PssCSx00TrigEdgeTypeFall,
                    0,0,CSx00ScpiClient::PssCSx00TrigEdgeTypeFall,
                    0,3,CSx00ScpiClient::PssCSx00TrigEdgeTypeRise};
    
        masterTrigPara.chanID = masterPara.chanID;
        masterTrigPara.outputDelay = 0;
        masterTrigPara.trigDelay = 0;
        masterTrigPara.trigCount = 3;
        masterTrigPara.eventTag = masterTag;
    
        slave1TrigPara.chanID = slave1Para.chanID;
        slave1TrigPara.outputDelay = 0;
        slave1TrigPara.trigDelay = 0;
        slave1TrigPara.trigCount = 3;
        slave1TrigPara.eventTag = slave1Tag;
    
        slave2TrigPara.chanID = slave2Para.chanID;
        slave2TrigPara.outputDelay = 0;
        slave2TrigPara.trigDelay = 0;
        slave2TrigPara.trigCount = 3;
        slave2TrigPara.eventTag = slave2Tag;
    
        mCSx00->ClearEvent(1);
        mCSx00->ClearEvent(2);
        mCSx00->ClearEvent(3);
    
        SetTrigEvent(masterTrigPara);
        SetTrigEvent(slave1TrigPara);
        SetTrigEvent(slave2TrigPara);
    
        int ret = SendCommScpiList(masterPara);
        if (0 != ret)
        {
            return;
        }
        ret = SendCommScpiList(slave1Para);
        if (0 != ret)
        {
            return;
        }
        ret = SendCommScpiList(slave2Para);
        if (0 != ret)
        {
            return;
        }
        QVector<double> masterVecV,masterVecI;
        QVector<double> slave1VecV, slave1VecI;
        QVector<double> slave2VecV, slave2VecI;
        for (int i = 0; i < masterPara.pointList.count(); i++)
        {
            SendSweScpiList(i,slave1Para);
            SendSweScpiList(i,slave2Para);
            SendSweScpiList(i,masterPara);
    
            mCSx00->SetOutput(slave2Para.chanID,true);
            mCSx00->SetOutput(slave1Para.chanID,true);
            mCSx00->SetOutput(masterPara.chanID,true);
    
            uint32_t point = masterPara.pointList.value(0);
            mCSx00->GetSweepResult(masterPara.chanID,point,masterVecV,masterVecI, MAX_TIMEOUT_MS);
            mCSx00->GetSweepResult(slave1Para.chanID,point,slave1VecV,slave1VecI, MAX_TIMEOUT_MS);
            mCSx00->GetSweepResult(slave2Para.chanID,point,slave2VecV,slave2VecI, MAX_TIMEOUT_MS);
        }
        emit ResultReady(masterVecV, masterVecI, masterPara.chanID);
        emit ResultReady(slave1VecV, slave1VecI, slave1Para.chanID);
        emit ResultReady(slave2VecV, slave2VecI, slave2Para.chanID);
    }
}

void CSx00Worker::SetOutPutCtrl(int chanID, bool isOn)
{
    mCSx00->SetOutput(chanID, isOn);
}

void CSx00Worker::SetChannelReady(int chanID, bool isReady)
{
    switch (chanID)
    {
    case 1:
    {
        isChan1Ready = isReady;
        break;
    }
    case 2:
    {
        isChan2Ready = isReady;
        break;
    }
    case 3:
    {
        isChan3Ready = isReady;
        break;
    }
    default:
    {
        break;
    }
    }
}

int CSx00Worker::SetReadPara(const ScanPara &para)
{
    isReadReady = false;
    /* 发送测量读数scpi指令 */
    int ret = SendReadScpiList(para);
    if (0 != ret)
    {
        return ret;
    }
    QTime time;
    double valV, valI;
    /* 标识符isRun为false时 停止读取数据 */
    while (!isChan1Ready || !isChan2Ready || !isChan3Ready)
    {
        QApplication::processEvents();
        time.restart();
        QDateTime startTime = QDateTime::currentDateTime();
        /* 如果本次为读取成功 继续读取直到5s超时 */
        if (!isChan1Ready)
        {
            ret = mCSx00->ReadOutput(1,valV, valI, 5000);
            /* 读取成功 发射数值 */
            if (0 != ret)
            {
                return ret;
            }
            emit ResultReady(startTime.addMSecs(time.elapsed()).toMSecsSinceEpoch(),time.elapsed(),valV, valI, 1);
        }
        if (!isChan2Ready)
        {
            ret = mCSx00->ReadOutput(2,valV, valI, 5000);
            /* 读取成功 发射数值 */
            if (0 != ret)
            {
                return ret;
            }
            emit ResultReady(startTime.addMSecs(time.elapsed()).toMSecsSinceEpoch(),time.elapsed(),valV, valI, 2);
        }
        if (!isChan3Ready)
        {
            ret = mCSx00->ReadOutput(3,valV, valI, 5000);
            /* 读取成功 发射数值 */
            if (0 != ret)
            {
                return ret;
            }
            emit ResultReady(startTime.addMSecs(time.elapsed()).toMSecsSinceEpoch(),time.elapsed(),valV, valI, 3);
        }
    }
    //ret = mCSx00->SetOutput(para.chanID, false);                                                       /* 关闭输出 */
    if (0 != ret)
    {
        return ret;
    }
    return 0;
}

int CSx00Worker::SetListPara(const ScanPara &para)
{
    QVector<double> vecV, vecI;
    /* 发送list扫描scpi指令 */
    int ret = SendListScpiList(para);
    if (0 != ret)
    {
        return ret;
    }
    /* 开输出并收取ON */
    if ( !OutputHasRecvOn(para) )
    {
        return -99;
    }
    /* 获取并发射数据 */
    uint32_t point = para.listData.count();
    ret = mCSx00->GetSweepResult(para.chanID,point,vecV,vecI,MAX_TIMEOUT_MS);
    if (0 != ret)
    {
        return ret;
    }
    qDebug() << "ready" << vecV << vecI;
    emit ResultReady(vecV, vecI, para.chanID);
    return 0;
}

int CSx00Worker::SetSwePara(const ScanPara &para)
{
    QVector<double> vecV, vecI;
    /* 兼容一次或多次扫描 */
    for (int i = 0; i < para.startList.count(); i++)
    {
        /* 发送扫描scpi指令 */
        int ret = SendSweScpiList(i,para);
        if (0 != ret)
        {
            return ret;
        }
        /* 多台设备时主机卡住 等待从机先开OUTPUT */

        if ( !OutputHasRecvOn(para) )
        {
            return -99;
        }
        QVector<double> tmpV, tmpI;
        /* 获取数据并发射 */
        uint32_t point = para.pointList.value(i);
        ret = mCSx00->GetSweepResult(para.chanID, point, tmpV, tmpI, MAX_TIMEOUT_MS);
        if (0 != ret)
        {
            return ret;
        }
        for (int i = 0; i < tmpV.count();i ++)
        {
            vecV.append(tmpV.value(i));
            vecI.append(tmpI.value(i));
        }
    }
    emit ResultReady(vecV, vecI,para.chanID);
}

int CSx00Worker::SendCommScpiList(const ScanPara &para)
{
    /* 设置主从模式 */
    int ret = mCSx00->SetMasterMode(para.chanID,para.isMasterDevice);
    if (0 != ret)
    {
        return ret;
    }

    /* 设置缓存 */
    ret = mCSx00->TraceSet(para.chanID,para.hasCache);
    if (0 != ret)
    {
        return ret;
    }
    /* 设置2/4线 */
    ret = mCSx00->Set4W(para.chanID,para.is4Wires);
    if (0 != ret)
    {
        return ret;
    }
    /* 设置plc */
    ret = mCSx00->SetNPLC(para.chanID,para.isSourceV, para.nplc);
    if (0 != ret)
    {
        return ret;
    }
    return ret;
}

int CSx00Worker::SendSweScpiList(int i, const ScanPara &para)
{
    int ret = SendSweCommScpiList(i,para);
    if (0 != ret)
    {
        return -1;
    }
    ret = mCSx00->SetScanStartVal(para.chanID, para.isSourceV, para.startList.value(i));
    if (0 != ret)
    {
        return ret;
    }
    ret = mCSx00->SetScanStopVal(para.chanID, para.isSourceV, para.stopList.value(i));
    if (0 != ret)
    {
        return ret;
    }
    ret = mCSx00->SetScanPointNum(para.chanID, para.pointList.value(i));
    if (0 != ret)
    {
        return ret;
    }
    return 0;
}

int CSx00Worker::SendListScpiList(const ScanPara &para)
{
    int ret = SendSweCommScpiList(0, para, false);
    if (0 != ret)
    {
        return ret;
    }
    qDebug() << "mData" << para.listData;
    /* list 每次发送有长度限值 */
    QVector<double> mTmpData;
    int strLen = 0;
    for (auto &&in : para.listData)
    {
        strLen ++;
        mTmpData << in;
        if( strLen < 50 )
        {
            continue;
        }
        strLen = 0;
        ret = mCSx00->AppendCustomSweepData(para.chanID,para.isSourceV,mTmpData);
        if (0 != ret)
        {
            return ret;
        }
        mTmpData.clear();
    }
    if (strLen > 0)
    {
        ret = mCSx00->AppendCustomSweepData(para.chanID,para.isSourceV,mTmpData);
        if (0 != ret)
        {
            return ret;
        }
    }
    return 0;
}

int CSx00Worker::SendReadScpiList(const ScanPara &para)
{
    /* 进入测量界面 */
    double val;
    int ret = mCSx00->MeasureQuery(para.chanID,true,val);
    if (0 > ret)
    {
        return ret;
    }
    /* 设置源 */
    ret = mCSx00->SetVSrc(para.chanID,para.isSourceV);
    if (0 != ret)
    {
        return ret;
    }
    /* 设置源量程 */
    ret = mCSx00->SetSrcRange(para.chanID, para.isSourceV, para.srcRangeList.value(0));
    if (0 != ret)
    {
        return ret;
    }
    /* 设置源值 */
    ret = mCSx00->SetSrcVal(para.chanID, para.isSourceV,para.startList.value(0));
    if (0 != ret)
    {
        return ret;
    }
    /* 设置限值量程 */
    ret = mCSx00->SetLmtRange(para.chanID, para.isSourceV, para.lmtRangeList.value(0));
    if (0 != ret)
    {
        return ret;
    }
    /* 设置限值 */
    ret = mCSx00->SetLmtVal(para.chanID, para.isSourceV, para.lmtList.value(0));
    if (0 != ret)
    {
        return ret;
    }
    /* 开输出 并读出第一个数 */
    ret = mCSx00->SetOutput(para.chanID,true);
    if (0 != ret)
    {
        return ret;
    }
    return ret;
}

int CSx00Worker::SendSweCommScpiList(int i, const ScanPara &para,bool isSwe)
{
    CSx00ScpiClient::CSx00ScpiSweepType sweepType;
    if (isSwe)
    {
        sweepType = CSx00ScpiClient::CSx00ScpiSweepTypeLiner;
    }
    else
    {
        sweepType = CSx00ScpiClient::CSx00ScpiSweepTypeCustom;
    }
    int ret = mCSx00->SetScanType(para.chanID,para.isSourceV, sweepType);
    if (0 != ret)
    {
        return ret;
    }

    ret = mCSx00->SetVSrc(para.chanID,para.isSourceV);
    if (0 != ret)
    {
        return ret;
    }
    
    ret = SendRst(para);
    if ( 0 != ret)
    {
        return ret;
    }
    ret = mCSx00->SetSrcRange(para.chanID,para.isSourceV, para.srcRangeList.value(i));
    if (0 != ret)
    {
        return ret;
    }
    ret = mCSx00->SetLmtRange(para.chanID,para.isSourceV, para.lmtRangeList.value(i));
    if (0 != ret)
    {
        return ret;
    }
    ret = mCSx00->SetLmtVal(para.chanID, para.isSourceV, para.lmtRangeList.value(i));
    if (0 != ret)
    {
        return ret;
    }
    return 0;
}
int CSx00Worker::SendRst(const ScanPara &para)
{
    /* 手动初始化量程 */
    int ret = mCSx00->SetLmtRange(para.chanID,para.isSourceV,1e-3);
    if (0 != ret)
    {
        return ret;
    }
    ret = mCSx00->SetSrcRange(para.chanID,para.isSourceV,1e-3);
    if (0 != ret)
    {
        return ret;
    }
    return 0;
}

bool CSx00Worker::OutputHasRecvOn(const ScanPara &para)
{
    int ret = mCSx00->SetOutput(para.chanID,true);
    if (0 != ret)
    {
        return false;
    }
    return true;
}
