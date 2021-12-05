#include "InstManager.h"
#include "ScpiWorker.h"
#include "CSx00Instrument.h"
#include "Sx00ReaderWorker.h"
#include "PssUtil.h"
#include <QHostAddress>
#include <QApplication>

using namespace Pss::Qt;
InstManager::InstManager(QObject *parent)
    : QObject(parent),
      mStopRead(false),
      mKey(0),
      mThread(nullptr)
{
    qRegisterMetaType<ChanInfo>("ChanInfo");
    qRegisterMetaType<MeasPara>("MeasPara");
    qRegisterMetaType<DataTag>("DataTag");
}

InstManager::~InstManager()
{
    for (int i = 0; i < mThreadGroup.count(); ++i)
    {
        mThreadGroup.values().value(i)->quit();
    }
}

void InstManager::StartRead()
{
    mStopRead = false;
    mData.clear();
    InitRecvStatus(false);
    /* 记录发送read的时间 */
    mDateTime = QDateTime::currentDateTime();
    emit sigSetOutputCtrl(true);
    QTime espTime;
    espTime.start();
    while(!mStopRead)
    {
        if (espTime.elapsed() > 5000)
        {
            mStopRead = true;
            emit sigTimeout();
            return;
        }
        QCoreApplication::processEvents();
        bool isReady = GetRecvReady();  /* 判断每个通道是否都读到数 */
        if (!isReady)
        {
            continue;
        }
        emit sigResultReady(mDateTime, mData);
        espTime.start();
        mDateTime = QDateTime::currentDateTime();
        InitRecvStatus(false);
        mData.clear();
        emit sigGetReadResult();
    }
}

void InstManager::StopRead()
{
    emit sigSetReadReady(true);
    mStopRead = true;
    emit sigSetOutputCtrl(false);
}

int InstManager::GetKey() const
{
    return mKey;
}

QString InstManager::GetIP(int key) const
{
    return mIPAddrs[key];
}

void InstManager::RemoveChan(const ChanInfo &info)
{
    int key = info.key;
    mThreadGroup[key]->quit();
    mThreadGroup.remove(key);
    mRecvReadyStatus.remove(key);
    mIPAddrs.remove(key);
}

void InstManager::ConnectToLan(const QString &ip)
{
    CSx00Instrument *inst = new CSx00Instrument(QHostAddress(ip));
    if (!inst->Connect())
    {
        /* -1代表连接失败 */
        InstPara para;
        emit sigIsInstConnect(false, ip, para);
        delete inst;
        inst = nullptr;
        return;
    }
    QString idn = inst->GetIdn();

    QString model = idn.split(",").value(1).trimmed();
    model.trimmed();
    qDebug() << model;
    ScpiWorker *worker = nullptr;
    if (model.startsWith("1003C"))
    {
        mInstType = CS400;
        QVector<int> availiableChannel = GetAvailableChannel(idn.split(",").value(3));
        QMap<int, QVector<int>> channels;
        for (int i = 0; i < availiableChannel.count(); i++)
        {
            QString ver;
            int ret = inst->VersionQuery(availiableChannel.value(i),ver);
            if (0 != ret)
            {
                return;
            }
            QVector<int> chan = {1, 2, 3, 4};
            channels.insert(availiableChannel.value(i), chan);
            qDebug() << "idn:" << "ver:" << idn << ver;
            idn.append(QString("%1,").arg(ver.split(",").value(0)));
    
        }
        mInstPara.channels = channels;
        delete inst;
        inst = nullptr;
        worker = new CSx00ReaderWorker(channels,mKey,ip);
    }
    else if (model.startsWith("S"))
    {
        mInstType = S100;
        delete inst;
        inst = nullptr;
        worker = new Sx00ReaderWorker(mKey,ip);
    }
    
    mThread = new QThread;
    /* 线程优先级 */
    //mThread->setPriority(QThread::LowestPriority);
    worker->moveToThread(mThread);
    connect(mThread, &QThread::started, worker, &ScpiWorker::Init);
    connect(mThread, &QThread::finished, mThread, &QThread::deleteLater);
    connect(mThread, &QThread::finished, worker, &ScpiWorker::deleteLater);
    connect(this, &InstManager::sigSetScanPara, worker, &ScpiWorker::SetReadPara, ::Qt::BlockingQueuedConnection);
    connect(this, &InstManager::sigSearchChan, worker, &ScpiWorker::Connect);
    connect(this, &InstManager::sigSetReadReady, worker, &ScpiWorker::SetReadReady);
    connect(this, &InstManager::sigSetOutputCtrl, worker, &ScpiWorker::SetOutCtrl,::Qt::BlockingQueuedConnection);
    connect(worker, &ScpiWorker::sigIsConnectSucceed, this, &InstManager::IsInstConnect);
    connect(this, &InstManager::sigGetReadResult, worker, &ScpiWorker::GetReadResult);
    connect(worker, &ScpiWorker::sigResultReady, this, &InstManager::ParseData);
    connect(this, &InstManager::sigSetUpgradePort, worker, &ScpiWorker::SetUpgradePort);
    mThread->start();
}

void InstManager::ConnectToRs232(const QString &comName, int baud)
{
    Q_UNUSED(comName);
    Q_UNUSED(baud);
}

void InstManager::ConnectToGpib(const QString &addr)
{
    Q_UNUSED(addr);
}

void InstManager::SendScanPara(const QVector<MeasPara> &paraVec)
{
    mUnUsedInst.clear();
    int cnt = paraVec.count();
    for (int i = 0; i < cnt; ++i)
    {
        if (!paraVec.value(i).isEnable)
        {
            ChanInfo info;
            info.key = paraVec.value(i).instKey;
            info.card = paraVec.value(i).cardID;
            info.chan = paraVec.value(i).chanID;
            mUnUsedInst.append(info);
           //mUnUsedInst.append(paraVec.value(i).instKey);
        }
        emit sigSetScanPara(paraVec.value(i));
    }
}

void InstManager::SearchChan(int chanID)
{
    emit sigSearchChan(chanID);
}

void InstManager::SetUpgradePort(const ChanInfo &info,int port)
{
    emit sigSetUpgradePort(info,port);
}

void InstManager::ParseData(int key, const DataTag &dataP)
{
    mData.append(dataP);
    mRecvReadyStatus[key] = true;
}

void InstManager::IsInstConnect(bool isConnect, const QString &ip)
{
    if (isConnect)
    {
        mInstPara.key = mKey;
        mInstPara.type = mInstType;
    }
    emit sigIsInstConnect(isConnect, ip, mInstPara);
    if (!isConnect)
    {
        mThread->quit();
        return;
    }
    mThreadGroup.insert(mKey, mThread);
    mInstTypeGrp.insert(mKey, mInstType);
    mRecvReadyStatus.insert(mKey, false);
    mIPAddrs.insert(mKey, ip);
    mKey++;
}

QVector<int> InstManager::GetAvailableChannel(const QString &idn)
{
    QVector<int> availableChannel;
    /* Qt-Channel-Pwr */
    QString channel = idn.split("-").value(1);
    QStringList channelList = channel.split("/");
    if (!channelList.isEmpty())
    {
        foreach (QString channelStr, channelList)
        {
            if (channelStr.isEmpty())
            {
                continue;
            }
            availableChannel << channelStr.toInt();
        }
    }
    return availableChannel;
}

void InstManager::InitRecvStatus(bool isRecvReady)
{
    int cnt = mRecvReadyStatus.values().count();
    QList<int> keys = mRecvReadyStatus.keys();
    for (int i = 0; i < cnt; ++i)
    {
        mRecvReadyStatus[keys.value(i)] = isRecvReady;
    }
    for (int i = 0; i < mUnUsedInst.count(); ++i)
    {
        if (mInstTypeGrp[mUnUsedInst.value(i).key] == CS400)
        {
            mRecvReadyStatus[mUnUsedInst.value(i).key] = false;
            continue;
        }
        mRecvReadyStatus[mUnUsedInst.value(i).key] = true;
    }
}

bool InstManager::GetRecvReady()
{
    for (int i = 0; i < mRecvReadyStatus.values().count(); ++i)
    {
        /* 有一个通道没收到数 返回false */
        if (!mRecvReadyStatus.values().value(i))
        {
            return false;
        }
    }
    return true;
}