#ifndef _PSS_QT_SX00_READER_WORKER_H_
#define _PSS_QT_SX00_READER_WORKER_H_

#include "ScpiWorker.h"

namespace Pss {
namespace Qt {

class Sx00Instrument;
class Sx00ReaderWorker : public ScpiWorker
{
public:
    Sx00ReaderWorker(int key, const QString &ip, QObject *parent = 0);
    Sx00ReaderWorker(int key, const QString &comName, int comBaud, QObject *parent = 0);
    Sx00ReaderWorker(int key, int cardAddr, int devAddr, QObject *parent = 0);
    ~Sx00ReaderWorker();

public slots:
    void Init();                                                                       /* 连接 */
    void Connect(int chanID);
    void SetOutCtrl(bool isOn);                                                        /* 开关输出 */
    void SetReadPara(const MeasPara &para);                                            /* 设置参数 */
    void GetReadResult();                                                              /* 获取结果 */
    void SetUpgradePort(const ChanInfo &info,int port);
private:
    bool mEnable;                                                                      /* 当前设备是否使能 */
    Sx00Instrument *mSx00;
};
}
}

#endif // _PSS_QT_SX00_READER_WORKER_H_