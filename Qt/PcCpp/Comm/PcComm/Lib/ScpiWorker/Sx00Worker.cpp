#include "ESSx00Worker.h"
#include "Sx00Instrument.h"
#include <QHostAddress>
#include <QTime>
#include <QTimer>
#include <QThread>
#include <QApplication>

using namespace Pss::Qt;
ESSx00Worker::ESSx00Worker(int key, const QString &ip, QObject *parent)
    :ESInstWorker(key, ip, parent),
      mSx00(nullptr)
{

}

ESSx00Worker::ESSx00Worker(int key, const QString &comName, int comBaud, QObject *parent)
    :ESInstWorker(key, comName, comBaud, parent),
      mSx00(nullptr)
{

}

ESSx00Worker::ESSx00Worker(int key, int cardAddr, int devAddr, QObject *parent)
    :ESInstWorker(key, cardAddr, devAddr, parent),
      mSx00(nullptr)
{

}

ESSx00Worker::~ESSx00Worker()
{
    delete mSx00;
    mSx00 = nullptr;
}

void ESSx00Worker::Init()
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
    qDebug() << "init";
    bool isConnect = true;
    if (!mSx00->Connect(1000))
    {
        isConnect = false;
    }
    qDebug() << "isConnect:" << isConnect;
    emit sigIsConnectSucceed(isConnect);
}

void ESSx00Worker::Connect()
{
    /* 用来查询设备 */
    if (!mSx00->Connect())
    {
        emit sigFailed(mInstKey, ConnectError);
    }
}

int ESSx00Worker::SendRst(bool isV)
{
    /* 手动初始化量程 */
    int ret = mSx00->SetLmtRange(isV,1e-3);
    if (0 != ret)
    {
        return SetLmtRangeError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    ret = mSx00->SetSrcRange(isV,1e-3);
    if (0 != ret)
    {
        return SetSrcRangeError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    return SetSuccess;
}

int ESSx00Worker::SendCommScpiList(const ScanPara &para)
{
    /* 设置trig线方向 */
    int ret = mSx00->SetTrigLine(para.dir);
    if (0 != ret)
    {
        return SetTrigLineError;
    }
    /* GPIB需要延时 使trig线方向能被成功切换 */
    if (GPIB == mType)
    {
        QThread::msleep(500);
    }
    /* 设置主从模式 */
    ret = mSx00->SetMasterMode(para.isMasterDevice);
    if (0 != ret)
    {
        return SetMasterError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    /* 设置trig input */
    ret = mSx00->SetTrigInput(para.TrigOutput);
    if (0 != ret)
    {
        return SetTrigInputError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    /* 设置缓存 */
    ret = mSx00->TraceSet(para.hasCache);
    if (0 != ret)
    {
        return SetTraceError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    /* 设置2/4线 */
    ret = mSx00->Set4W(para.is4Wires);
    if (0 != ret)
    {
        return Set4WError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    /* 设置前后面板 */
    ret = mSx00->SetF(para.isFrontboard);
    if (0 != ret)
    {
        return SetFError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    /* 设置plc */
    ret = mSx00->SetNPLC(para.isSourceV, para.nplc);
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

int ESSx00Worker::SendSweCommScpiList(int i, const ScanPara &para, bool isSwe)
{
    int ret;
    if (isSwe)
    {
        ret = mSx00->SetScanType(para.isSourceV, Sx00ScpiClient::Sx00ScpiSweepTypeLiner);
    }
    else
    {
        ret = mSx00->SetScanType(para.isSourceV, Sx00ScpiClient::Sx00ScpiSweepTypeCustom);
    }
    if (0 != ret)
    {
        return SetScanTypeError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    ret = mSx00->SetVSrc(para.isSourceV);
    if (0 != ret)
    {
        return SetVSrcError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    ret = SendRst(para.isSourceV);
    if (0 != ret)
    {
        return ret;
    }

    ret = mSx00->SetSrcRange(para.isSourceV, para.srcRangeList.value(i));
    if (0 != ret)
    {
        return SetSrcRangeError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    ret = mSx00->SetLmtRange(para.isSourceV, para.lmtRangeList.value(i));
    if (0 != ret)
    {
        return SetLmtRangeError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    ret = mSx00->SetLmtVal(para.isSourceV, para.lmtList.value(i));
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

int ESSx00Worker::SendSweScpiList(int i, const ScanPara &para)
{
    int ret = SendSweCommScpiList(i, para);
    if (0 != ret)
    {
        return ret;
    }
    ret = mSx00->SetScanStartVal(para.isSourceV,para.startList.value(i));
    if (0 != ret)
    {
        return SetScanStartValError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    ret = mSx00->SetScanStopVal(para.isSourceV,para.stopList.value(i));
    if (0 != ret)
    {
        return SetScanStopValError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    ret = mSx00->SetScanPointNum(para.pointList.value(i));
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

int ESSx00Worker::SendListScpiList(const ScanPara &para)
{
    int ret = SendSweCommScpiList(0, para, false);
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
            ret = mSx00->SetCustomScanData(para.isSourceV, data);
        }
        else
        {
            ret = mSx00->AppendCustomSweepData(para.isSourceV, data);
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
    qDebug() << "mData" << para.listData;
    /* list 每次发送有长度限值 */
    QVector<double> mTmpData;
    int strLen = 0;
    for (auto &&in : para.listData)
    {
        strLen ++;
        mTmpData << in;
        if(strLen < 50 )
        {
            continue;
        }
        strLen = 0;
        ret = mSx00->AppendCustomSweepData(para.isSourceV,mTmpData);
        if (0 != ret)
        {
            return SetAppendListValError;
        }
        if (GPIB == mType)
        {
            QThread::msleep(1);
        }
        mTmpData.clear();
    }
    if (strLen > 0)
    {
        ret = mSx00->AppendCustomSweepData(para.isSourceV,mTmpData);
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

int ESSx00Worker::SendReadScpiList(const ScanPara &para)
{
    /* 进入测量界面 */
    double val;
    int ret = mSx00->MeasureQuery(true,val);
    if (0 > ret)
    {
        return SetMeasureQueryError;
    }

    /* 设置源 */
    ret = mSx00->SetVSrc(para.isSourceV);
    if (0 != ret)
    {
        return SetVSrcError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    if (para.isSrcAuto)
    {
        ret = mSx00->SetSrcAutoRange(para.isSourceV, true);
        if (0 != ret)
        {
            return SetSrcRangeError;
        }
        if (GPIB == mType)
        {
            QThread::msleep(1);
        }
    }
    else
    {
        /* 设置源量程 */
        ret = mSx00->SetSrcRange(para.isSourceV, para.srcRangeList.value(0));
        if (0 != ret)
        {
            return SetSrcRangeError;
        }
        if (GPIB == mType)
        {
            QThread::msleep(1);
        }
    }
    /* 设置源值 */
    ret = mSx00->SetSrcVal(para.isSourceV,para.startList.value(0));
    if (0 != ret)
    {
        return SetSrcValError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    if (para.isLmtAuto)
    {
        ret = mSx00->SetLmtAutoRange(para.isSourceV, true);
        if (0 != ret)
        {
            return SetLmtRangeError;
        }
        if (GPIB == mType)
        {
            QThread::msleep(1);
        }
    }
    else
    {
        /* 设置限值量程 */
        ret = mSx00->SetLmtRange(para.isSourceV, para.lmtRangeList.value(0));
        if (0 != ret)
        {
            return SetLmtRangeError;
        }
        if (GPIB == mType)
        {
            QThread::msleep(1);
        }
    }
    
    /* 设置限值 */
    ret = mSx00->SetLmtVal(para.isSourceV, para.lmtList.value(0));
    if (0 != ret)
    {
        return SetLmtValError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    /* 开输出 并读出第一个数 */
    ret = mSx00->SetOutput(true,false);
    if (0 != ret)
    {
        return SetOutputError;
    }
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    return SetSuccess;
}

bool ESSx00Worker::OutputHasRecvOn()
{
    int ret = mSx00->SetOutput(true, false);
    if (0 != ret)
    {
        return false;
    }
    //return true;
    /* 判断是否收到ON\n */
    QTime time;
    time.start();
    QString str;
    bool hasRecvOn = true;
    do {
        str += mSx00->ReqVal(1000);
        if (time.elapsed() > 5000)
        {
            hasRecvOn = false;
            break;
        }
    } while(!str.contains("ON"));
    return hasRecvOn;
}

void ESSx00Worker::SetScanPara(const ScanPara &para)
{
    isOutputReady = true;
    if (para.isMasterDevice)
    {
        isOutputReady = para.isIndependent;
    }
    int ret = 0;
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
    {
        ret = SetApdPara(para);
        break;
    }
    case ScanPara::Mos:
    case ScanPara::Swe:
    {
        ret = SetSwePara(para);
        break;
    }
    default:
        break;
    }
    if (SetSuccess != ret)
    {
        emit sigFailed(mInstKey,ret);
    }
}

void ESSx00Worker::SetScanPara(int key, const ScanPara &para)
{
    qDebug() << "key:" << key << "instKey:" << mInstKey;
    if (mInstKey != key)
    {
        return;
    }
    SetScanPara(para);
}

void ESSx00Worker::SetScanPara(int key, const QVector<ScanPara> &paras)
{
    if (mInstKey != key)
    {
        return;
    }
    /* 单台对应单个参数 */
    ScanPara para = paras.value(0);
    SetScanPara(para);
}

void ESSx00Worker::SetNetUpgrade(int port)
{
    int ret = mSx00->SetNetUpgradePort(port);
    if (SetSuccess != ret)
    {
        emit sigFailed(mInstKey, SetNetUpgradePortError);
    }
}
#if 0
void ESSx00Worker::SetInstPara(const InstParaTag &para)
{

    if (mInstKey != para.instKey)
    {
        return;
    }
    /* 设置trig线方向 */
    int ret = mSx00->SetTrigLine(para.desktopPara.commPara.dir);
    
    /* GPIB需要延时 使trig线方向能被成功切换 */
    if (GPIB == mType)
    {
        QThread::msleep(500);
    }
    /* 设置主从模式 */
    ret = mSx00->SetMasterMode(para.desktopPara.commPara.isMaster);
    
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    /* 设置trig input */
    ret = mSx00->SetTrigInput(para.desktopPara.commPara.isTrigOn);
    
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    /* 设置缓存 */
    ret = mSx00->TraceSet(para.desktopPara.commPara.isCache);
    
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    /* 设置2/4线 */
    ret = mSx00->Set4W(para.desktopPara.commPara.is4Wires);
    
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    /* 设置前后面板 */
    ret = mSx00->SetF(para.desktopPara.commPara.isFront);
    
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    /* 设置plc */
    ret = mSx00->SetNPLC(para.desktopPara.commPara.isSrcV, para.desktopPara.commPara.plc);
    
    if (GPIB == mType)
    {
        QThread::msleep(1);
    }
    switch (para.desktopPara.funcType)
    {
    case AppFuncTypeSwe:
    {
        int ret = mSx00->SetScanType(para.desktopPara.isSrcV, Sx00ScpiClient::Sx00ScpiSweepTypeLiner);
    
        if (GPIB == mType)
        {
            QThread::msleep(1);
        }
//        ret = mSx00->SetVSrc(para.desktopPara.swePara[0].);

//        if (GPIB == mType)
//        {
//            QThread::msleep(1);
//        }
//        ret = SendRst(para.isSourceV);
//        if (0 != ret)
//        {
//            return ret;
//        }
        ret = mSx00->SetSrcRange(para.desktopPara.isSrcV, para.desktopPara.swePara[0].srcRange);

        if (GPIB == mType)
        {
            QThread::msleep(1);
        }
        ret = mSx00->SetLmtRange(para.desktopPara.isSrcV, para.desktopPara.swePara[0].lmtRange);
    
        if (GPIB == mType)
        {
            QThread::msleep(1);
        }
        ret = mSx00->SetLmtVal(para.desktopPara.isSrcV, para.desktopPara.swePara[0].lmtVal);
    
        if (GPIB == mType)
        {
            QThread::msleep(1);
        }
        ret = mSx00->SetScanStartVal(para.desktopPara.isSrcV, para.desktopPara.swePara[0].startVal);
    
        if (GPIB == mType)
        {
            QThread::msleep(1);
        }
        ret = mSx00->SetScanStopVal(para.desktopPara.isSrcV, para.desktopPara.swePara[0].stopVal);
    
        if (GPIB == mType)
        {
            QThread::msleep(1);
        }
        ret = mSx00->SetScanPointNum(para.desktopPara.swePara[0].point);
    
        if (GPIB == mType)
        {
            QThread::msleep(1);
        }
    
    }
    default:
        break;
    }
    mSx00->SetOutput(true, false);
    
    //int timeout = para.desktopPara.plc * 0.02 * para.desktopPara.swePara[0].point + 500000;
    GetReadData(para.desktopPara.swePara[0].point, 500000);

}
#endif
void ESSx00Worker::GetReadData(int point, int timeout)
{
    QTime time;
    time.start();
    QVector<double> vecV, vecI;
    mSx00->GetSweepResult(point,vecV, vecI, timeout);
    //emit sigResultReady(time.elapsed(), vecV, vecI);
    qDebug() << vecV << vecI;
}

int ESSx00Worker::SetReadPara(const ScanPara &para)
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
    qDebug() << "1" << QTime::currentTime();
    QTime time;
    double valV, valI;
    int count = 0;
    /* 标识符isRun为false时 停止读取数据 */
    while (!isReadReady)
    {
        QApplication::processEvents();
        time.restart();
        QDateTime startTime = QDateTime::currentDateTime();
        /* 如果本次为读取成功 继续读取直到5s超时 */
        ret = mSx00->ReadOutput(valV, valI, 5000);
        qDebug() << "ret!!!" << ret;
        /* 读取成功 发射数值 */
        if (0 != ret)
        {
            /* 三次都没读到 退出 */
            count++;
            if (count == MAX_READTIMEOUT_COUNT)
            {
                return SetReadError;
            }
            continue;
        }
        qDebug() << "instKey: " << mInstKey;
        emit ResultReady(startTime.addMSecs(time.elapsed()).toMSecsSinceEpoch(), time.elapsed(), valV, valI, mInstKey);
        QThread::usleep(100);
    }
    ret = mSx00->SetOutput(false,false);                                                        /* 关闭输出 */
    if(0 != ret)
    {
        return SetOutputError;
    }
    return SetSuccess;
}

int ESSx00Worker::SetListPara(const ScanPara &para)
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
    if ( !OutputHasRecvOn() )
    {
        return SetReadError;
    }
    /* 获取并发射数据 */
    int point = para.listData.count();
    ret = mSx00->GetSweepResult(point,vecV,vecI, 500000);
    if (0 != ret)
    {
        return SetReadError;
    }
    qDebug() << "ready" << vecV << vecI;
    emit ResultReady(vecV, vecI);
    return SetSuccess;
}

int ESSx00Worker::SetSwePara(const ScanPara &para)
{
    qDebug() << QTime::currentTime();
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
        ret = SendSweScpiList(i,para);
        if (0 != ret)
        {
            break;
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
        qDebug() << QTime::currentTime();
        if ( !OutputHasRecvOn() )
        {
            return SetOutputError;
        }
        emit sigOutputReady(true);
        /* 获取数据并发射 */
        int point = para.pointList.value(i);
        ret = mSx00->GetSweepResult(point, vecV, vecI, 500000);
        if (0 != ret)
        {
            return SetReadError;
        }
        /* 如果是主机并且非独立运行 卡住 */
        if (para.isMasterDevice && !para.isIndependent)
        {
            isOutputReady = false;
        }
        emit ResultReady(vecV, vecI);
    }
    return SetSuccess;
}

int ESSx00Worker::SetApdPara(const ScanPara &para)
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
        while (!isOutputReady)
        {
            QThread::msleep(1);
            QApplication::processEvents();
            if (time.elapsed() > 5000)
            {
                return -99;
            }
        }
        if ( !OutputHasRecvOn() )
        {
            return SetOutputError;
        }
        emit sigOutputReady(true);
        QVector<double> tmpV, tmpI;
        /* 获取数据并发射 */
        int point = para.pointList.value(i);
        ret = mSx00->GetSweepResult(point, tmpV, tmpI, 500000);
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
        /* 如果是主机并且非独立运行 卡住 */
        if (para.isMasterDevice && !para.isIndependent)
        {
            isOutputReady = false;
        }
    }
    emit ResultReady(vecV, vecI);
    return SetSuccess;
}