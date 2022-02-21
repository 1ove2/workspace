#include "Px00Worker.h"
#include "Px00Instrument.h"
#include <QThread>
#include <QApplication>
#include <QTime>
#include <QDateTime>

Px00Worker::Px00Worker(int key, const QString &ip, QObject *parent)
    :InstWorker(key, ip, parent),
      mPx00(nullptr)
{

}

Px00Worker::Px00Worker(int key, const QString &comName, int baud, QObject *parent)
    :InstWorker(key, comName, baud, parent),
      mPx00(nullptr)
{

}

Px00Worker::Px00Worker(int key, int cardAddr, int devAddr, QObject *parent)
    :InstWorker(key, cardAddr, devAddr, parent),
      mPx00(nullptr)
{

}

Px00Worker::~Px00Worker()
{
    delete mPx00;
    mPx00 = nullptr;
}

void Px00Worker::Init()
{
    if (LAN == mType)
    {
        mPx00 = new Px00Instrument(QHostAddress(mIp));
    }
    else if (RS232 == mType)
    {
        mPx00 = new Px00Instrument(mComName, mBaud);
    }
    else if (GPIB == mType)
    {
#ifdef _PSS_GPIB_SUPPORT_
        mPx00 = new Px00Instrument(mCardAddr, mDevAddr);
#endif
    }
    bool isConnect = true;
    if (!mPx00->Connect())
    {
        isConnect = false;
    }
    emit sigIsConnectSucceed(isConnect);
}

void Px00Worker::SetScanPara(const ScanPara &para)
{
    int ret;
    isOutputReady = true;
    if (para.isMasterDevice)
    {
        isOutputReady = para.isIndependent;
    }
    switch(para.type)
    {
    case ScanPara::Swe:
    case ScanPara::Mos:
    {
        ret = SetSwePara(para);
        break;
    }
    case ScanPara::Apd:
    {
        ret = SetApdPara(para);
        break;
    }
    case ScanPara::List:
    {
        ret = SetListPara(para);
        break;
    }
    case ScanPara::Read:
    {
        ret = SetReadPara(para);
        break;
    }
    case ScanPara::Wave:
    {
        ret = SetWavePara(para);
        break;
    }
    default:
    {
        break;
    }

    }
    if (0 != ret)
    {
        qDebug() << "error:" << ret;
    }
}

void Px00Worker::SetScanPara(int key, const ScanPara &para)
{
    Q_UNUSED(key);
    Q_UNUSED(para);
}

void Px00Worker::Connect()
{
    mPx00->Connect();
}

void Px00Worker::SetNetUpgrade(int port)
{
    int ret = mPx00->SetNetUpgradePort(port);
    if (SetSuccess != ret)
    {
        emit sigFailed(mInstKey, SetNetUpgradePortError);
    }
}

void Px00Worker::SetScanPara(int key, const QVector<ScanPara> &paras)
{

}

int Px00Worker::SendRst(int chanID, bool isV)
{
#if 0
    /* 手动初始化量程 */
    int ret = mPx00->SetLmtRange(chanID,isV,1e-3);
    if (0 != ret)
    {
        return ret;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    ret = mPx00->SetSrcRange(chanID,isV,1e-3);
    if (0 != ret)
    {
        return ret;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
#endif
    return SetSuccess;
}

bool Px00Worker::OutputHasRecvOn(int chanID)
{
    int ret = mPx00->SetOutput(chanID, true);
    if (0 != ret)
    {
        return false;
    }
#if 0
    /* 判断是否收到ON\n */
    QTime time;
    time.start();
    QString str;
    bool hasRecvOn = true;
    do {
        str += mPx00->ReqVal(1000);
        if (time.elapsed() > 5000)
        {
            hasRecvOn = false;
            break;
        }
        qDebug() << "on?" << str;
    } while(!str.contains("ON"));
    qDebug() << "hasRecvOn:" << hasRecvOn;
    return hasRecvOn;
#else
    return true;
#endif
}

int Px00Worker::SendCommScpiList(const ScanPara &para)
{
    int ret = mPx00->Reset();
    if (0 != ret)
    {
        return SetTrigLineError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    /* 设置trig线方向 */
    ret = mPx00->SetTrigLine(para.dir);
    if (0 != ret)
    {
        return SetTrigLineError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    /* 设置主从模式 */
    ret = mPx00->SetMasterMode(para.chanID, para.isMasterDevice);
    if (0 != ret)
    {
        return SetMasterError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    /* 设置trig input */
    ret = mPx00->SetTrigInput(para.TrigOutput);
    if (0 != ret)
    {
        return SetTrigInputError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    if (ScanPara::Read != para.type)
    {
        /* 设置缓存 */
        ret = mPx00->TraceSet(para.chanID, para.hasCache);
        if (0 != ret)
        {
            return SetTraceError;
        }
        if (GPIB == mType)
        {
            QThread::msleep(1);
        }
    }

    /* 设置2/4线 */
    ret = mPx00->Set4W(para.chanID, para.is4Wires);
    if (0 != ret)
    {
        return Set4WError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
//    /* 设置前后面板 */
//    ret = mPx00->SetF(para.chanID, para.isFrontboard);
//    if (0 != ret)
//    {
//        return SetFError;
//    }

    /* 设置脉冲模式 */
    ret = mPx00->SetOutputMode(para.chanID, para.isPulse);
    if (0 != ret)
    {
        return SetOutputModeError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    
    if (para.isPulse)
    {
        ret = mPx00->SetPulseCount(para.chanID, para.pulseCnt);
        if (0 != ret)
        {
            return SetPulseCountError;
        }
        ret = mPx00->SetPulseSampMode(para.chanID, para.isHigh);
        if (0 != ret)
        {
            return SetPulseSampModeError;
        }
        ret = mPx00->SetPulseDelay(para.chanID, para.pulseDelay);
        if (0 != ret)
        {
            return SetPulseDelayError;
        }
        if (GPIB == mType)
        {
            QThread::msleep(1);
        }
        ret = mPx00->SetPulsePeriod(para.chanID, para.pulsePeriod);
        if (0 != ret)
        {
            return SetPulsePeriodError;
        }
        if (GPIB == mType)
        {
            QThread::msleep(1);
        }
        ret = mPx00->SetPulseWidth(para.chanID, para.pulseWidth);
        if (0 != ret)
        {
            return SetPulseWidthError;
        }
        if (GPIB == mType)
        {
            QThread::msleep(1);
        }
    }
    
    /* 设置plc */
    ret = mPx00->SetNPLC(para.chanID,para.isSourceV, para.nplc);
    if (0 != ret)
    {
        return SetNplcError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    return SetSuccess;
}

int Px00Worker::SendSweCommScpiList(int i, const ScanPara &para, bool isSwe)
{
    int ret;
    if (isSwe)
    {
        ret = mPx00->SetScanType(para.chanID,para.isSourceV, Px00ScpiClient::Px00ScpiSweepTypeLiner);
    }
    else
    {
        ret = mPx00->SetScanType(para.chanID, para.isSourceV, Px00ScpiClient::Px00ScpiSweepTypeCustom);
    }

    if (0 != ret)
    {
        return SetScanTypeError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    ret = mPx00->SetVSrc(para.chanID,para.isSourceV);
    if (0 != ret)
    {
        return SetVSrcError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    ret = SendRst(para.chanID, para.isSourceV);
    if (0 != ret)
    {
        return ret;
    }
    
    ret = mPx00->SetSrcRange(para.chanID, para.isSourceV, para.srcRangeList.value(i));
    if (0 != ret)
    {
        return SetSrcRangeError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    ret = mPx00->SetLmtRange(para.chanID, para.isSourceV, para.lmtRangeList.value(i));
    if (0 != ret)
    {
        return SetLmtRangeError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    ret = mPx00->SetLmtVal(para.chanID, para.isSourceV, para.lmtList.value(i));
    if (0 != ret)
    {
        return SetLmtValError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    return SetSuccess;
}

int Px00Worker::SendSweScpiList(int i, const ScanPara &para)
{
    int ret = SendSweCommScpiList(i, para);
    if (0 != ret)
    {
        return ret;
    }
    ret = mPx00->SetScanStartVal(para.chanID, para.isSourceV, para.startList.value(i));
    if (0 != ret)
    {
        return SetScanStartValError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    ret = mPx00->SetScanStopVal(para.chanID, para.isSourceV, para.stopList.value(i));
    if (0 != ret)
    {
        return SetScanStopValError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    ret = mPx00->SetScanPointNum(para.chanID, para.pointList.value(i));
    if (0 != ret)
    {
        return SetScanPointError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    return SetSuccess;
}

int Px00Worker::SetSwePara(const ScanPara &para)
{
#if 1
    int ret = SendCommScpiList(para);
    if (0 != ret)
    {
        return ret;
    }
    QTime time;
    QVector<double> vecV, vecI;
    /* 兼容一次或多次扫描 */
    for (int i = 0; i < para.startList.count(); i++)
    {
        /* 发送扫描scpi指令 */
        ret = SendSweScpiList(i,para);
        if (0 != ret)
        {
            return ret;
        }
        /* 多台设备时主机卡住 等待从机先开OUTPUT */
        time.start();
        while (!isOutputReady)
        {
            QThread::msleep(1);
            QApplication::processEvents();
            if (time.elapsed() > 5000)
            {
                return -99;
            }
        }
        if ( !OutputHasRecvOn(para.chanID) )
        {
            return SetOutputError;
        }
        emit sigOutputReady(true);
        /* 获取数据并发射 */
        uint32_t point = para.pointList.value(i);
        if (para.isPulse)
        {
            point = para.sampPoint * point * para.pulseCnt;
        }

        ret = mPx00->GetSweepResult(para.chanID, point,vecV,vecI,MAX_TIMEOUT_MS);
        if (0 != ret)
        {
            return SetReadError;
        }
        /* 如果是主机并且非独立运行 卡住 */
        if (para.isMasterDevice && !para.isIndependent)
        {
            isOutputReady = false;
        }
        qDebug() << "send result ready";
        emit ResultReady(vecV, vecI);
    }
    return SetSuccess;
#endif
}

int Px00Worker::SetApdPara(const ScanPara &para)
{
    int ret = SendCommScpiList(para);
    if (0 != ret)
    {
        return ret;
    }
    QTime time;
    QVector<double> vecV, vecI;
    /* 兼容一次或多次扫描 */
    for (int i = 0; i < para.startList.count(); i++)
    {
        /* 发送扫描scpi指令 */
        ret = SendSweScpiList(i, para);
        if (0 != ret)
        {
            return ret;
        }
        /* 多台设备时主机卡住 等待从机先开OUTPUT */
        time.start();

        if ( !OutputHasRecvOn(para.chanID) )
        {
            return SetOutputError;
        }
        QVector<double> tmpV, tmpI;
    
        /* 获取数据并发射 */
        uint32_t point = para.pointList.value(i);
        if (para.isPulse)
        {
            point = para.sampPoint * point * para.pulseCnt;
        }
        ret = mPx00->GetSweepResult(para.chanID, point, tmpV, tmpI, 500000);
        if (0 != ret)
        {
            return SetReadError;
        }
#if 0
        qDebug() << tmpV.count() << tmpI.count();
        if (i >= 1)
        {
            tmpV.removeFirst();
            tmpI.removeFirst();
        }
#endif
        for (int j = 0; j < tmpV.count();j++)
        {
            vecV.append(tmpV.value(j));
            vecI.append(tmpI.value(j));
        }
    }
    emit ResultReady(vecV, vecI);
    return SetSuccess;
}

int Px00Worker::SetListPara(const ScanPara &para)
{
    int ret = SendCommScpiList(para);
    if (0 != ret)
    {
        return ret;
    }
    QVector<double> vecV, vecI;
    /* 发送list扫描scpi指令 */
    ret = SendListScpiList(para);
    if (0 != ret)
    {
        return ret;
    }
    /* 开输出并收取ON */
    if ( !OutputHasRecvOn(para.chanID) )
    {
        return SetReadError;
    }
    /* 获取并发射数据 */
    uint32_t point = para.listData.count();
    if (para.isPulse)
    {
        point = para.sampPoint * point * para.pulseCnt;
    }
    ret = mPx00->GetSweepResult(para.chanID, point,vecV,vecI,MAX_TIMEOUT_MS);
    if (0 != ret)
    {
        return SetReadError;
    }
    qDebug() << "ready" << vecV << vecI;
    emit ResultReady(vecV, vecI);
    return SetSuccess;
}

int Px00Worker::SetWavePara(const ScanPara &para)
{
    mPx00->Reset();
    mPx00->Set4W(0, para.is4Wires);
    mPx00->SetSrcRange(0, para.isSourceV, para.srcRangeList.value(0));
    mPx00->SetLmtRange(0, para.isSourceV, para.lmtRangeList.value(0));
    mPx00->SetLmtVal(0, para.isSourceV, para.lmtList.value(0));
    switch (para.sweWaveType)
    {
    case Sin:
    {
        mPx00->SetSweWaveFunc(0, Px00ScpiClient::Px00ScpiSweepFuncTypeSIN);
        mPx00->SetSweWaveSinPara(0, para.ampl, para.fre, para.other, para.offset, para.wavePoint, para.waveCount, para.pulseDuty);
        break;
    }
    case Squ:
    {
        mPx00->SetSweWaveFunc(0, Px00ScpiClient::Px00ScpiSweepFuncTypeSQU);
        mPx00->SetSweWaveSquPara(0, para.ampl, para.fre, para.other, para.offset, para.wavePoint, para.waveCount, para.pulseDuty);
        break;
    }
    case Tri:
    {
        mPx00->SetSweWaveFunc(0, Px00ScpiClient::Px00ScpiSweepFuncTypeTRI);
        mPx00->SetSweWaveTriPara(0, para.ampl, para.fre, para.offset, para.wavePoint, para.waveCount, para.pulseDuty);
        break;
    }
    case Ramp:
    {
        mPx00->SetSweWaveFunc(0, Px00ScpiClient::Px00ScpiSweepFuncTypeRAMP);
        mPx00->SetSweWaveRampPara(0, para.ampl, para.fre, para.other, para.offset, para.wavePoint, para.waveCount, para.pulseDuty);
        break;
    }
    case Custom:
    {
        mPx00->SetScanType(0, para.isSourceV, Px00ScpiClient::Px00ScpiSweepTypeCustom);
        QVector<double> hAmpl = para.hAmpl;
        QVector<double> lAmpl = para.lAmpl;
        QVector<double> hTime = para.hTime;
        QVector<double> lTime = para.lTime;

        for (int i = 0; hAmpl.size() > 0; ++i)
        {
            QVector<double> hAmplData,lAmplData, hTimeData, lTimeData;
            if (hAmpl.size() > 50)
            {
                hAmplData = hAmpl.mid(0,50);
                hAmpl.remove(0,50);
    
                lAmplData = lAmpl.mid(0,50);
                lAmpl.remove(0,50);
    
                hTimeData = hTime.mid(0,50);
                hTime.remove(0,50);
    
                lTimeData = lTime.mid(0,50);
                lTime.remove(0,50);
            }
            else
            {
                hAmplData = hAmpl;
                hAmpl.remove(0, hAmpl.size());
    
                lAmplData = lAmpl;
                lAmpl.remove(0, lAmpl.size());
    
                hTimeData = hTime;
                hTime.remove(0, hTime.size());
    
                lTimeData = lTime;
                lTime.remove(0, lTime.size());
    
            }
            if (para.isPulse)
            {
                if (0 == i)
                {
                   mPx00->SetListSweepVal(para.chanID, Px00ScpiClient::Px00ScpiListSweepTypePULSE,  hAmplData, hTimeData, lAmplData, lTimeData);
                }
                else
                {
                    mPx00->SetAppendListSweepVal(para.chanID, Px00ScpiClient::Px00ScpiListSweepTypePULSE,hAmplData, hTimeData, lAmplData, lTimeData);
                }
    
                mPx00->SetVSrc(para.chanID, para.isSourceV);
    
            }
            else
            {
                Px00ScpiClient::Px00ScpiListSweepType type;
                if (para.isSourceV)
                {
                    type = Px00ScpiClient::Px00ScpiListSweepTypeVOLT;
                }
                else
                {
                    type = Px00ScpiClient::Px00ScpiListSweepTypeCURR;
                }
    
                if (0 == i)
                {
                   mPx00->SetListSweepVal(para.chanID, type,  hAmplData, hTimeData, lAmplData, lTimeData);
                }
                else
                {
                    mPx00->SetAppendListSweepVal(para.chanID, type,hAmplData, hTimeData, lAmplData, lTimeData);
                }
            }
        }
        break;
    }
    default:
        break;
    }
    mPx00->SetNPLC(0,true, para.nplc);
    mPx00->SetOutput(0, true);
    
    uint32_t point = 2000;
    QVector<double> vecV, vecI;
    mPx00->GetSweepResult(para.chanID, point, vecV, vecI, MAX_TIMEOUT_MS);
    emit ResultReady(vecV, vecI);
//    QVector<double> vecV, vecI;
//    /* 发送list扫描scpi指令 */
//    ret = SendListScpiList(para);
//    if (0 != ret)
//    {
//        return ret;
//    }
//    /* 开输出并收取ON */
//    if ( !OutputHasRecvOn(para.chanID) )
//    {
//        return SetReadError;
//    }
//    /* 获取并发射数据 */
//    uint32_t point = para.listData.count();
//    if (para.isPulse)
//    {
//        int sampPoint = 0;
//        /*us*/ /* ns */ /* ms */
//        double plcTime = 20.0;

//        sampPoint = floor((para.pulseWidth / 1e3 - para.pulseDelay / 1e6) / plcTime / para.nplc);
//        if (sampPoint < 1)
//        {
//            sampPoint = 1;
//        }
//        point = point * sampPoint;
//        qDebug() << "point!!!!:" << sampPoint;
//    }
//    ret = mPx00->GetSweepResult(para.chanID, point,vecV,vecI,MAX_TIMEOUT_MS);
//    if (0 != ret)
//    {
//        return SetReadError;
//    }
//    emit ResultReady(vecV, vecI);
    return SetSuccess;
}

int Px00Worker::SendListScpiList(const ScanPara &para)
{
    int ret = SendSweCommScpiList(0,para, false);
    if (0 != ret)
    {
        return ret;
    }
    QVector<double> listData = para.listData;
    for (int i = 0; listData.size() > 0; ++i)
    {
        QVector<double> data ;
        if (listData.size() > 50)
        {
            data = listData.mid(0,50);
            listData.remove(0,50);
        }
        else
        {
            data = listData;
            listData.remove(0, listData.size());
        }

        if (0 == i)
        {
            ret = mPx00->SetCustomScanData(para.chanID, para.isSourceV, data);
        }
        else
        {
            ret = mPx00->AppendCustomSweepData(para.chanID, para.isSourceV, data);
        }
        if (0 != ret)
        {
            return SetAppendListValError;
        }
        if (GPIB == mType)
        {
            QThread::msleep(1);
        }
    }
#if 0
    qDebug() << "list data:" << para.listData;
    /* list 每次发送有长度限值 */
    QVector<double> tmpData;
    int strLen = 0;
    int size = para.listData.size();

    for (auto &&in : para.listData)
    {
        strLen ++;
        tmpData << in;
        if( strLen < 50 )
        {
            continue;
        }
        strLen = 0;
        ret = mPx00->AppendCustomSweepData(para.chanID, para.isSourceV, tmpData);
        if (0 != ret)
        {
            return SetAppendListValError;
        }
        if (GPIB == mType)
        {
            QThread::msleep(1);
        }
        tmpData.clear();
    }
    if (strLen > 0)
    {
        ret = mPx00->AppendCustomSweepData(para.chanID, para.isSourceV, tmpData);
        if (0 != ret)
        {
            return SetAppendListValError;
        }
        if (GPIB == mType)
        {
            QThread::msleep(1);
        }
    }
#endif
    return SetSuccess;
}

int Px00Worker::SetReadPara(const ScanPara &para)
{
    /* 发送24线等公共scpi指令 */
    int ret = SendCommScpiList(para);
    if (0 != ret)
    {
        return ret;
    }
    isReadReady = false;
    /* 发送测量读数scpi指令 */
    ret = SendReadScpiList(para);
    if (0 != ret)
    {
        return ret;
    }
    QTime time;
    QVector<double> vecV, vecI;
    //double valV, valI;
    /* 标识符isRun为false时 停止读取数据 */
    QDateTime startTime = QDateTime::currentDateTime();
    time.restart();
    while (!isReadReady)
    {
        QApplication::processEvents();
        //time.restart();

        /* 如果本次为读取成功 继续读取直到5s超时 */
        //ret = mPx00->ReadOutput(para.chanID,valV, valI, 5000);
        ret = mPx00->ReadOutput(para.chanID, vecV, vecI, 10000);
        /* 读取成功 发射数值 */
        if (0 != ret)
        {
            return SetReadError;
        }
        emit sigResultReady(startTime.addMSecs(time.elapsed()).toMSecsSinceEpoch(), time.elapsed(),vecV, vecI, mInstKey);
        //emit ResultReady(startTime.addMSecs(time.elapsed()).toMSecsSinceEpoch(), time.elapsed(),valV, valI);
    }
    qDebug() << "set output off !";
    ret = mPx00->SetOutput(para.chanID, false);                                                        /* 关闭输出 */
    if(0 != ret)
    {
        return SetOutputError;
    }
    return SetSuccess;
}

int Px00Worker::SendReadScpiList(const ScanPara &para)
{
    /* 进入测量界面 */
    double val;
    int ret = mPx00->MeasureQuery(para.chanID, true,val);
    if (0 > ret)
    {
        return SetMeasureQueryError;
    }
    /* 等待设备初始化 */
    QThread::msleep(1000);

    /* 设置缓存 */
    ret = mPx00->TraceSet(para.chanID, para.hasCache);
    if (0 != ret)
    {
        return SetTraceError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    
    /* 设置源 */
    ret = mPx00->SetVSrc(para.chanID, para.isSourceV);
    if (0 != ret)
    {
        return SetVSrcError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    /* 设置源量程 */
    ret = mPx00->SetSrcRange(para.chanID, para.isSourceV, para.srcRangeList.value(0));
    if (0 != ret)
    {
        return SetSrcRangeError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    /* 设置源值 */
    ret = mPx00->SetSrcVal(para.chanID, para.isSourceV,para.startList.value(0));
    if (0 != ret)
    {
        return SetSrcValError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    /* 设置限值量程 */
    ret = mPx00->SetLmtRange(para.chanID, para.isSourceV, para.lmtRangeList.value(0));
    if (0 != ret)
    {
        return SetLmtRangeError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    /* 设置限值 */
    ret = mPx00->SetLmtVal(para.chanID, para.isSourceV, para.lmtList.value(0));
    if (0 != ret)
    {
        return SetLmtValError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    if (para.isPulse)
    {
        ret = mPx00->SetPulseCount(para.chanID, 9999);
        if (0 != ret)
        {
            return -1;
        }
        if (GPIB == mType)
        {
            QThread::msleep(1);
        }
    }
    /* 开输出 并读出第一个数 */
    ret = mPx00->SetOutput(para.chanID, true);
    if (0 != ret)
    {
        return SetOutputError;
    }
    return SetSuccess;
}
