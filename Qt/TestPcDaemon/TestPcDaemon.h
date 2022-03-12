#ifndef TESTPCDAEMON_H
#define TESTPCDAEMON_H

#include <QMainWindow>
#include "TestTcpServer.h"
#include <QSharedMemory>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

typedef struct _tagPx00AnlgStatusInfo
{
    bool    isVSrc;                                                                                    /* 当前是否为电压源 */
    bool    isOutput;                                                                                  /* 当前输出是否打开 */
    bool    is4W;                                                                                      /* 当前是否4线 */
    int 	voltRangeType;                                                								/* 当前电压量程模式 */
    int  	currRangeType;                                                								/* 当前电流量程模式 */
    int     voltRange;                                                                             /* 当前电压量程 */
    int     currRange;                                                                             /* 当前电流量程 */
    double  voltValue;                                                                                   /* 当前用户设置电压值 */
    double  currValue;                                                                                   /* 当前用户设置电流值 */
    double  nplcValue;                                                                                 /* 模拟板NPLC值 */
    bool    trigInputOn;                                                                               /* trigin开关 */
    int     trigInputLine;                                                                             /* trig输入线 */
    bool    traceTrig;                                                                                  /* 缓存开关 */
    bool    isPulseShape;                                                                              /* 脉冲输出模式;true:脉冲;false:直流 */
    int     pulseWidth;                                                                                /* 脉冲宽度 us */
    int     pulsePeriod;                                                                               /* 脉冲周期 us */
    double  pulsePeriodBackup;                                                                         /* 脉冲周期(double) us */
    uint    pulseDelayNs;                                                                                /* 脉冲采样延时 ns */
    uint    pulseSamplePoints;                                                                         /* 脉冲高电平采样点 */
    uint    pulseCount;                                                                               /* 脉冲输出个数 */
    double  readVVal;                                                                                  /* 最后一次读取的电压值 */
    double  readIVal;                                                                                  /* 最后一次读取的电流值 */
    bool    nplcMode;                                                                                  /* 是否NPLC模式 */
    uint8_t pulseSampleMode;                                                                           /* 采样模式,1:高电平;2:全周期 */
    int     pulseLowSamplePoints;                                                                      /* 脉冲低电平采样点 */
    bool    isSweep;                                                                                   /* 是否扫描模式 */
    char    userMsg[128];                                                                              /* 用户信息 */
} Px00AnlgStatusInfo;

namespace Ui {
class TestPcDaemon;
}

class TestPcDaemon : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestPcDaemon(QWidget *parent = 0);
    ~TestPcDaemon();
private:
    Ui::TestPcDaemon *ui;
    TestTcpServer *mServer;
    //QSharedMemory mSharememory;
    QLineEdit *mLEditMasterDevType;/* 设备系列号,SX00 ... */
    QLineEdit *mLEditDevType;      /* 设备型号 S100...*/

    void init(void);
    void initShareMemory(void);
private slots:
    void sltConnect(void);
    void sltDisConnect(void);
    void sltSendData(void);
    void sltClearText(void);
    void sltUpdateShareMemory(void);
    void sltChangeOutputStatus(const bool &isopen);
    void sltChangePulseMode(const bool &isPulse);
//    void sltPulseInfiniteMode(void);
    void sltSwitchOutput(void);
    void sltRemoteScan(void);
    void sltEditCard(void);
    void sltCheckAutoReply(const bool &isCheck);
signals:
};

#endif // TESTPCDAEMON_H
