#ifndef _PSS_QT_SCPIWORKER_H_
#define _PSS_QT_SCPIWORKER_H_

#include <QObject>
#include "ParaDef.h"
#include <QVector>
enum ConnectType {
    LAN,
    RS232,
    GPIB
};

enum ErrorCode {
    SendError = -1,
    RecvError = -2,
};

struct RecvDataPara {
    int key;
    int card;
    int channel;
    double volt;
    double curr;
};

struct DataTag {
    int time;
    QVector<RecvDataPara> para;
};

namespace Pss {
namespace Qt {

class ScpiWorker : public QObject
{
    Q_OBJECT
public:
    ScpiWorker(int key, const QString &ip, QObject *parent = 0);
    ScpiWorker(int key, const QString &comName, int comBaud, QObject *parent = 0);
    ScpiWorker(int key, int cardAddr, int devAddr, QObject *parent = 0);
    ~ScpiWorker();
public slots:
    virtual void Init()=0;                                                                       /* 连接 */
    virtual void Connect(int chanID)=0;
    virtual void SetOutCtrl(bool isOn)=0;                                                        /* 开关输出 */
    virtual void SetReadPara(const MeasPara &para)=0;                                /* 设置参数 */
    virtual void GetReadResult()=0;                                                              /* 读数 */
    virtual void SetUpgradePort(const ChanInfo &info,int port) = 0;
    void SetReadReady(bool isReady);
signals:
    void sigIsConnectSucceed(bool isConnect, const QString &IP = "");
    void ResultReady(int time, double valV, double vecI, int chanID=0);                /* 发射READ结果给窗口 */
    void sigResultReady(int key, const DataTag &dataP);
    void sigError(int errCode);
protected:
    int mInstKey;
    QString mIp;
    QString mComName;
    int mBaud;
    int mCardAddr;
    int mDevAddr;

    ConnectType mType;
    bool isReadReady;
    
    const static int MaxReadTimeout = 5000;
};
}
}

#endif // _PSS_QT_SCPIWORKER_H_