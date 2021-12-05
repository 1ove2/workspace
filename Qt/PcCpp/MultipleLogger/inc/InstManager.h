#ifndef _PSS_QT_INSTMANAGER_H_
#define _PSS_QT_INSTMANAGER_H_

#include <QMap>
#include <QObject>
#include <QThread>
#include <QDateTime>
#include "ParaDef.h"
#include "CSx00ReaderWorker.h"

/* 设备交互类 */
enum InstType {
    S100 ,                            /* S100 */
    CS400,                            /* CS400 */
};

struct InstPara {
    int key;                          /* 标识 */
    InstType type;                    /* 是否是插卡设备 */
    QMap<int, QVector<int>> channels; /* 卡号-通道号 */
};

namespace Pss {
namespace Qt {

class ScpiWorker;
class InstManager : public QObject
{
    Q_OBJECT
public:
    InstManager(QObject *parent = 0);
    ~InstManager();
    void StartRead();
    void StopRead();
    int GetKey() const;
    QString GetIP(int key) const;
    void RemoveChan(const ChanInfo &info);
public slots:
    void ConnectToLan(const QString &ip);
    void ConnectToRs232(const QString &comName, int baud);
    void ConnectToGpib(const QString &addr);
    void SendScanPara(const QVector<MeasPara> &paraVec);
    void SearchChan(int chanID);
    void SetUpgradePort(const ChanInfo &info, int port);
signals:
    void sigSetUpgradePort(const ChanInfo &info,int port);
    void sigTimeout();         /* 超时信号 */
    void sigSearchChan(int chanID);
    void sigSetOutputCtrl(bool isOn);
    void sigSetScanPara(const MeasPara &para);
    void sigResultReady(QDateTime date, const QVector<DataTag> &dataP);
    void sigIsInstConnect(bool isConnect, const QString &ip, InstPara para);
    void sigGetReadResult();
    void sigSetReadReady(bool isReady);
private slots:
    void ParseData(int key, const DataTag &dataP);
    void IsInstConnect(bool isConnect, const QString &ip);
private:
    QVector<int> GetAvailableChannel(const QString &idn);
    bool mStopRead;
    void InitRecvStatus(bool isRecvReady);          /* 初始化各通道读数状态 */
    bool GetRecvReady();                            /* 判断各通道数据是否收全 */

    QVector<DataTag> mData;                         /* 所有通道一次读取的数据 */
    QDateTime mDateTime;                            /* 发read信号的时间 */
    
    int mKey;
    InstType mInstType;
    InstPara mInstPara;
    QThread *mThread;
    QVector<ChanInfo> mUnUsedInst;                        /* 未使用的设备 */
    QMap<int, InstType> mInstTypeGrp;
    QMap<int, bool> mRecvReadyStatus;                /* 每个通道读数状态 */
    QMap<int, QThread *> mThreadGroup;               /* 线程组 */
    QMap<int, QString> mIPAddrs;
};
}
}

#endif // _PSS_QT_INSTMANAGER_H_