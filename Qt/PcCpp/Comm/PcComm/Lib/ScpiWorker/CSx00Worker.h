#ifndef _CSX00_WORKER_H_
#define _CSX00_WORKER_H_

#include "InstWorker.h"
#include "CSx00Instrument.h"

typedef struct _EventTag_ {
    int startOutputIn;
    int startOutputOut;
    CSx00ScpiClient::CSx00ScpiTrigEdgeType startOutputEdge;

    int finishOutputIn;
    int finishOutputOut;
    CSx00ScpiClient::CSx00ScpiTrigEdgeType finishOutputEdge;
    
    int startSamplingIn;
    int startSamplingOut;
    CSx00ScpiClient::CSx00ScpiTrigEdgeType startSamplingEdge;
    
    int finishSamplingIn;
    int finishSamplingOut;
    CSx00ScpiClient::CSx00ScpiTrigEdgeType finishSamplingEdge;
    
    int startSweIn;
    int startSweOut;
    CSx00ScpiClient::CSx00ScpiTrigEdgeType startSweEdge;
}EventT;

struct TrigPara {
    int chanID;
    uint32_t trigDelay;
    uint32_t outputDelay;
    int trigCount;
    EventT eventTag;
};

/* CSX00worker类 */
class CSx00Worker : public InstWorker
{
    Q_OBJECT
public:
    CSx00Worker(int key, const QString &ip, QObject *parent = 0);
    CSx00Worker(int key, const QString &comName, int baud, QObject *parent = 0);
    CSx00Worker(int key, int card, int dev, QObject *parent = 0);
    ~CSx00Worker();
public slots:
    void SetScanPara(int key, const ScanPara &para);
    void SetScanPara(const ScanPara &para);
    void SetNetUpgrade(int port);
    void Init() override;
    void SetScanPara(int key, const QVector<ScanPara> &paras);
    void Connect();
    void SetIPConfig(bool isAuto, const QStringList &ipList);                    /* 配置设备IP信息 */
    void SetUARTConfig(int baud);                                                /* 配置设备通信方式为串口并修改波特率 */
    void SetGPIBConfig(int devAddr);                                             /* 配置设备通信方式为GPIB并修改地址 */
    void SetEvent(int chanID,EventT eventTag);                                   /* 配置事件 */
    void ClearEvent(int chanID);                                                 /* 清除事件配置 */
    void SetTrigEvent(const TrigPara &para);
    void SetMosPara(const QVector<ScanPara> &paraVec);
    void SetLivPara(const QVector<ScanPara> &paraVec);
    void SetOutPutCtrl(int chanID, bool isOn);
    void SetChannelReady(int chanID, bool isReady);
private:
    int SetReadPara(const ScanPara &para);
    int SetListPara(const ScanPara &para);
    int SetSwePara(const ScanPara &para);
    int SendCommScpiList(const ScanPara &para);
    int SendSweScpiList(int i,const ScanPara &para);
    int SendListScpiList(const ScanPara &para);
    int SendReadScpiList(const ScanPara &para);
    int SendSweCommScpiList(int i, const ScanPara &para,bool isSwe=true);
    int SendRst(const ScanPara &para);

    bool OutputHasRecvOn(const ScanPara &para);
    
    CSx00Instrument *mCSx00;
    bool isChan1Ready;
    bool isChan2Ready;
    bool isChan3Ready;
    
    const static int mcRecvONDelayMs = 5000;
};


#endif // CSX00WORKER_H
