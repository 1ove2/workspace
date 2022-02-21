/******************************************************************************
 *
 * 文件名  ： InstWorker.h
 * 创建日期： 20201201
 * 版本号  ： v1.0 20201201
 * 文件描述： 扫描线程类
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _INST_WORKER_H_
#define _INST_WORKER_H_

/************************************ 头文件 ***********************************/
#include <QObject>
#include "ScanPara.h"
#include "ErrorDef.h"

/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class InstWorker : public QObject
{
    Q_OBJECT
public:
    InstWorker(int key, const QString &ip, QObject *parent=0);
    InstWorker(int key, const QString &comName, int comBaud, QObject *parent=0);
    InstWorker(int key, int cardAddr, int devAddr, QObject *parent=0);
    virtual ~InstWorker();
public slots:
    virtual void Init()=0;
    virtual void SetScanPara(const ScanPara &para)=0;
    virtual void SetScanPara(int key, const ScanPara &para)=0;
    virtual void SetScanPara(int key, const QVector<ScanPara> &paras)=0; /* 兼容插卡式 */
    virtual void Connect()=0;
    virtual void SetNetUpgrade(int port)=0;
    //virtual void SetInstPara(const InstParaTag &para)=0;

    void SetOutputReady(bool isReady);
    void SetReadReady(bool isReady);
signals:
    void sigResultReady(qint64 secSinceEpoch, int time, const QVector<double> &vecV, const QVector<double> &vecI,int chanID = 0);
    void sigFailed(int key, int def);
    void sigOutputReady(bool isReady);
    void sigIsConnectSucceed(bool isConnect);                                                              /* 发射连接状态 */
    void sigInstModel(bool isConnect, QString idn = "");
    void ResultReady(const QVector<double> &vecV, const QVector<double> &vecI, int ChanID=0);                         /* 发射扫描结果给窗口 */
    void ResultReady(qint64 mSecSinceEpoch,int time, double valV, double vecI, int chanID=0);                /* 发射READ结果给窗口 */
protected:
    QString mIp;

    QString mComName;
    int mBaud;
    
    int mCardAddr;
    int mDevAddr;
    
    int mInstKey;
    
    ConnectType mType;                                                                                  /* 设备连接类型 */
    bool isOutputReady;
    bool isReadReady;
    
    const int MAX_READTIMEOUT_COUNT = 3;
};
/*********************************** 工具函数 **********************************/

}
}

#endif // _INST_WORKER_H_
