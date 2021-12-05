#ifndef PLX00WORKER_H
#define PLX00WORKER_H

#include "InstWorker.h"

typedef struct _UIArgSet_{
#if 1
    float startVal;                                                                                     /*起点*/
    float stepVal;                                                                                      /*步进*/
    float stopVal;                                                                                      /*终点*/
    int   pulseWidth;                                                                                   /*脉冲宽度*/
    int   pulsePeriod;                                                                                  /*脉冲周期*/
    int   waveLen;                                                                                      /*波长*/
    double maxPower;                                                                                    /*最大光功率*/
    double powerOffsetVol;                                                                              /*光功率偏置电压*/
    double Dcurrent;                                                                                    /*直流电流*/
    bool isPulseMode;                                                                                   /*是否脉冲模式*/
    bool isScanOutput;                                                                                  /*是否扫描输出*/
    quint8 probe;                                                                                       /*探头*/
    quint32 pulseDcScanPoint;                                                                           /* 直流扫描点 */
    double overVolProtect;
    int  maxCheckVoltage;                                                                               /* 最大检测电压 */
    int     sampDelay;
    int     sampPoint;
    bool    isInitData;
    bool    isCustomArgument;
    int     trigInDelay;
    int     trigOutDelay;
    int     trigOutWidth;
    bool    trigOn;
#endif
#if 1
    quint8 stoichiometricPointIVWIndex;                                                                 /*功率电压电流计量点下标*/
    double stoichiometricPointIVWVal;                                                                   /*功率电压电流计量点数值*/

    quint8 stoichiometricPointBacklightIndex;                                                           /*背光计量点下标*/
    double stoichiometricPointBacklightVal;                                                             /*背光计量点数值*/
    
    quint8 seCalIndex;                                                                                  /*SE计算公式选择下标*/
    double seCalStartVal;                                                                               /*SE计算公式数值*/
    double seCalEndVal;                                                                                 /*SE计算公式数值*/
    
    quint8 rsCalIndex;                                                                                  /*RS计算公式下标*/
    double rsCalStartVal;                                                                               /*RS计算公式数值*/
    double rsCalEndVal;                                                                                 /*RS计算公式数值*/

//    quint8 thresholdValTestFrontRearIndex;                                                              /*阈值测试前光/背光下标*/
    quint8 thresholdValTestDiffMatchIndex;                                                              /*阈值测试二次微分/线性拟合下标*/
    quint8 thresholdValTestIWIndex;                                                                     /*阈值测试电流功率下标*/
    double thresholdValTestIWStartVal;                                                                  /*阈值测试电流功率区间起点值*/
    double thresholdValTestIWEndVal;                                                                    /*阈值测试电流功率区间终点值*/

//    quint8 kinkFrontRearIndex;                                                                          /*Kink测试前光/背光下标*/
    quint8 kinkDiffMatchIndex;                                                                          /*Kink测试一阶微分/标准差/多阶拟合下标*/
    quint8 kinkIWIndex;                                                                                 /*Kink测试电流功率下标*/
    double kinkIWStartVal;                                                                              /*Kink测试区间起点值*/
    double kinkIWEndVal;                                                                                /*Kink测试区间终点值*/
//    double kinkIWVal;                                                                                   /*Kink测试值，仅选择标准差时有效*/

    double calFactorPo;                                                                                 /*Po校准系数*/
    double calFactorOverVolProtect;                                                                     /*过压保护*/
    
    double txResultIth;                                                                                 /*Ith测试结果*/
    double txResultPo;                                                                                  /*Po测试结果*/
    double txResultRs;                                                                                  /*Rs测试结果*/
    double txResultIo;                                                                                  /*Io测试结果*/
    double txResultSe;                                                                                  /*Se测试结果*/
    double txResultVf;                                                                                  /*Vf测试结果*/
    double txResultEp;                                                                                  /*Ep测试结果*/
    double txResultIKink;                                                                               /*Ikink测试结果*/
    double txResultKink;                                                                                /*Kink测试结果*/
#endif
    void init(){
#if 1
        startVal = 1.0;
        stepVal = 1.0;
        stopVal = 100.0;
        pulseWidth = 1;
        pulsePeriod = 100;
        waveLen = 850;
        maxPower = 10.0;
        isPulseMode = true;
        isScanOutput = true;
        probe = 1;
        pulseDcScanPoint = 100;
        overVolProtect = 50.0;
        powerOffsetVol = 6.0;
        maxCheckVoltage = 10;
        sampDelay = 36;
        sampPoint = 2;
        isInitData = true;
        isCustomArgument = false;
        trigInDelay = 0;
        trigOutDelay = 0;
        trigOutWidth = 1;
        trigOn = false;
#endif
#if 1
        stoichiometricPointIVWIndex = 0;
        stoichiometricPointIVWVal = 10;

        //        stoichiometricPointBacklightIndex = 0;
        //        stoichiometricPointBacklightVal = 0.0;
    
        seCalIndex = 0;
        seCalStartVal = 10.0;
        seCalEndVal = 100.0;
    
        rsCalIndex = 0;
        rsCalStartVal = 10.0;
        rsCalEndVal = 100.0;

//        thresholdValTestFrontRearIndex = 0;
        thresholdValTestDiffMatchIndex = 1;
        thresholdValTestIWIndex = 0;
        thresholdValTestIWStartVal = 10.0;
        thresholdValTestIWEndVal = 100.0;

//        kinkFrontRearIndex = 0;
        kinkDiffMatchIndex = 0;
        kinkIWIndex = 0;
        kinkIWStartVal = 10.0;
        kinkIWEndVal = 100.0;
//        kinkIWVal = 0.0;

        calFactorPo = 1.0;
        calFactorOverVolProtect = 50.0;
    
        txResultIth = 0.0;
        txResultPo = 0.0;
        txResultRs = 0.0;
        txResultIo = 0.0;
        txResultSe = 0.0;
        txResultVf = 0.0;
        txResultEp = 0.0;
        txResultIKink = 0.0;
        txResultKink = 0.0;
#endif
    }
}UiArgSet;

class PLx00Instrument;
class Plx00Worker : public InstWorker
{
    Q_OBJECT
public:
    Plx00Worker(int key, const QString &ip, QObject *parent = 0);
    Plx00Worker(int key, const QString &comName, int baud, QObject *parent = 0);
    Plx00Worker(int key, int cardAddr, int devAddr, QObject *parent = 0);
    ~Plx00Worker();
public slots:
    void SetScanPara(int key, const ScanPara &para);
    void Init() override;
    void SetScanPara(const ScanPara &para) override;
    void Connect() override;
    void SetNetUpgrade(int port) override;
    void SetScanPara(int key, const QVector<ScanPara> &paras);
    void SetLivPara(const UiArgSet &para);
    void DCQuery();
signals:
    void sigResultReady(const QVector<double> &vecI, const QVector<double> &vecV, const QVector<double> &vecPower, const QVector<double> &vecBack);
private:
    PLx00Instrument *mPlx00;
};



#endif // PX00WORKER_H
