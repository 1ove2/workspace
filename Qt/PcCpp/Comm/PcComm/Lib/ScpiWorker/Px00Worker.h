#ifndef ESPX00WORKER_H
#define ESPX00WORKER_H

#include "ESInstWorker.h"


namespace Pss {
namespace Qt {
class Px00Instrument;
class ESPx00Worker : public ESInstWorker
{
    Q_OBJECT
public:
    ESPx00Worker(int key, const QString &ip, QObject *parent = 0);
    ESPx00Worker(int key, const QString &comName, int baud, QObject *parent = 0);
    ESPx00Worker(int key, int cardAddr, int devAddr, QObject *parent = 0);
    ~ESPx00Worker();
public slots:
    void Init() override;
    void SetScanPara(const ScanPara &para) override;
    void SetScanPara(int key, const ScanPara &para);
    void Connect() override;
    void SetNetUpgrade(int port) override;
    void SetScanPara(int key, const QVector<ScanPara> &paras);
private:
    int SendRst(int chanID, bool isV);
    bool OutputHasRecvOn(int chanID);
    int SendCommScpiList(const ScanPara &para);
    int SendSweCommScpiList(int i,  const ScanPara &para, bool isSwe = true);
    int SendSweScpiList(int i, const ScanPara &para);
    int SetSwePara(const ScanPara &para);
    int SetApdPara(const ScanPara &para);
    int SetListPara(const ScanPara &para);
    int SetWavePara(const ScanPara &para);
    int SendListScpiList(const ScanPara &para);
    int SetReadPara(const ScanPara &para);
    int SendReadScpiList(const ScanPara &para);
    Px00Instrument *mPx00;
};
}
}


#endif // ESPX00WORKER_H