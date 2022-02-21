#ifndef _PSS_QT_CSX00_READER_WORKER_H_
#define _PSS_QT_CSX00_READER_WORKER_H_

#include "ScpiWorker.h"
#include <QMap>

namespace Pss {
namespace Qt {

class CSx00Instrument;
class CSx00ReaderWorker : public ScpiWorker
{
public:
    /*
     * chan: CSx00的卡和卡上通道数， QMap中key为卡号, value为该卡中的通道数
     * key:  设备标识
     */
    CSx00ReaderWorker(const QMap<int,QVector<int>> &chan,int key, const QString &ip, QObject *parent = 0);
    CSx00ReaderWorker(const QMap<int,QVector<int>> &chan,int key, const QString &comName, int comBaud, QObject *parent = 0);
    CSx00ReaderWorker(const QMap<int,QVector<int>> &chan,int key, int cardAddr, int devAddr, QObject *parent = 0);
    ~CSx00ReaderWorker();
public slots:
    void Init();                                                                       /* 初始化连接 */
    void Connect(int chanID);                                                          /* 发送IDN指令,设备上home键闪灯 */
    void SetOutCtrl(bool isOn);                                                        /* 开关输出 */
    void SetReadPara(const MeasPara &para);                                            /* 设置基本参数 */
    void GetReadResult();                                                              /* READ?读数 */
    void SetUpgradePort(const ChanInfo &info,int port);
private:
    int mTime;                                                                         /* 读数耗时 */
    QMap<int, QVector<int>> mUsedChan;                                                 /* 使能的卡和通道 */
    void ParseData(const QString &str);                                                /* 解析数据 */
    CSx00Instrument *mCSx00;
};

}
}

#endif // _PSS_QT_CSX00_READER_WORKER_H_