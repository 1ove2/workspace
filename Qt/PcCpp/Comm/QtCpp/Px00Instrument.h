#ifndef _PSS_QT_PX00_INSTRUMENT_H_
#define _PSS_QT_PX00_INSTRUMENT_H_

#include "QtIInstrument.h"
#include "Px00ScpiClient.h"

namespace Pss {
namespace Qt {

class Px00Instrument : public QtIInstrument
{
    Q_OBJECT
    public:
        Px00Instrument(const QHostAddress &ip, QObject *parent = 0);
        Px00Instrument(const QString comName, uint32_t comBaud, QObject *parent = 0);
#ifdef _PSS_GPIB_SUPPORT_
        Px00Instrument(uint8_t cardAddr, uint8_t instrumentAddr, QObject *parent = 0);
#endif
        ~Px00Instrument(void);

        /* Px00 SCPI接口 */
        void SetChannelID(uint8_t chanID);
        int Set4W(uint8_t chanID, bool is4W);                                                           /* 设置24线 */
        int SetF(uint8_t chanID, bool isF);                                                             /* 设置前后面板 */
        int SetVSrc(uint8_t chanID, bool isVSrc);                                                       /* 设置源 */
        int SetLmtRange(uint8_t chanID, bool isVSrc, double val);                                       /* 设置限量程 */
        int SetLmtVal(uint8_t chanID, bool isVSrc, double val);                                         /* 设置限值 */
        int SetSrcRange(uint8_t chanID, bool isVSrc, double val);                                       /* 设置源量程 */
        int SetSrcVal(uint8_t chanID, bool isVSrc, double val);                                         /* 设置源值 */
        int SetNPLC(uint8_t chanID, bool isV, double nplc);                                             /* 设置NPLC */
        int GetOutputStatus(uint8_t chanID, bool &on, uint32_t timeout = mcWait4RecvTimeout);           /* 获取输出状态 */
        int GetSrcType(uint8_t chanID, bool& isVSrc, uint32_t timeout = mcWait4RecvTimeout);            /* 获取源类型 */
        int AppendCustomSweepData(uint8_t chanID, bool isV, QVector<double>& data);                     /* 追加自定义扫描参数 */
        int SetSrcAutoRange(uint8_t chanID, bool isVSrc, bool on);                                      /* 设置源自动量程开关 */
        int SetLmtAutoRange(uint8_t chanID, bool isVSrc, bool on);                                      /* 设置限自动量程开关 */
        int SrcAutoRangeQuery(uint8_t chanID, bool isVSrc, bool* on, uint32_t timeout = mcWait4RecvTimeout);/* 源量程自动开关请求 */
        int LmtAutoRangeQuery(uint8_t chanID, bool isVSrc, bool* on, uint32_t timeout = mcWait4RecvTimeout);/* 限量程自动开关请求 */
        int SrcRangeQuery(uint8_t chanID, bool isVSrc, char* range, uint32_t timeout = mcWait4RecvTimeout); /* 源量程值请求 */
        int LmtRangeQuery(uint8_t chanID, bool isVSrc, char* range, uint32_t timeout = mcWait4RecvTimeout); /* 限量程值请求 */
        int TraceSet(uint8_t chanID, bool on);                                                          /* 设置缓存 */
        int SetScanStartVal(uint8_t chanID, bool isVSrc, double val);                                   /* 设置扫描起点值 */
        int SetScanStopVal(uint8_t chanID, bool isVSrc, double val);                                    /* 设置扫描终点值 */
        int SetScanStepVal(uint8_t chanID, bool isVSrc, double val);                                    /* 设置扫描步进值 */
        int SetScanPointNum(uint8_t chanID, uint32_t val);                                              /* 设置扫描点数 */
        int SetScanType(uint8_t chanID, bool isV, Px00ScpiClient::Px00ScpiSweepType isSwe);             /* 设置扫描类型 */
        int SetMasterMode(uint8_t chanID, bool isMaster);                                               /* 设置主从模式 */
        int SetTrigInput(bool on);                                                                      /* 设置trig输入开关 */
        int SetCustomScanData(uint8_t chanID, bool isV, const QVector<double> &data);                   /* 设置自定义扫描数据 */
        int SetLmtAbort(uint8_t chanID, bool on);                                                       /* 超限停止开关 */
        int SetTrigDelay(uint8_t chanID, uint32_t delayUs);                                             /* 设置Trig延时 us */
        int SetOutputMode(uint8_t chanID, bool isPulse);                                                /* 设置源输出形状 */
        int OutputModeQuery(uint8_t chanID, bool &isPulse, uint32_t timeout);                        /* 获取源输出形状 */
        int SetPulseDelay(uint8_t chanID, uint32_t delayNs);                                            /* 设置脉冲延时 */
        int PulseDelayQuery(uint8_t chanID, uint32_t &delayUs, uint32_t timeout);                         /* 获取脉冲延时 */
        int SetPulseWidth(uint8_t chanID, uint32_t pulseWidth);                                         /* 设置脉冲宽度 */
        int PulseWidthQuery(uint8_t chanID, uint32_t &pulseWidth, uint32_t timeout);                      /* 获取脉冲宽度 */
        int SetPulsePeriod(uint8_t chanID, uint32_t pulsePeriod);                                       /* 设置脉冲周期 */
        int PulsePeriodQuery(uint8_t chanID, uint32_t &pulsePeriod, uint32_t timeout);                    /* 获取脉冲周期 */
        int SetInitTrigState(uint8_t chanID, Px00ScpiClient::Px00ScpiTrigType trigType);                /* 设置Trig状态 */
        int SetTrigStateQuery(uint8_t chanID, Px00ScpiClient::Px00ScpiTrigType trigType, int &state);   /* 获取Trig状态 */
        int SetIPConfig(bool isAuto, const QString &ip, const QString &netMask, const QString &gateWay);/* 设置网络IP */
        int IPConfigQuery(bool &isAuto, QString &ip, QString &netMask, QString &gateWay, uint32_t timeout = mcWait4RecvTimeout);/* 网络IP信息请求 */
        int SetUpdateIPConfig(void);                                                                    /* 更新网络IP */
        int SetGPIBConfig(uint8_t devAddr);                                                             /* 设置GPIB地址 */
        int GPIBConfigQuery(bool &on, uint8_t& devAddr, uint32_t timeout = mcWait4RecvTimeout);                   /* GPIB地址请求 */
        int SetUARTConfig(uint32_t baud);                                                               /* 设置串口波特率 */
        int UARTConfigQuery(bool &on, uint32_t& baud, uint32_t timeout = mcWait4RecvTimeout);                     /* 串口波特率请求 */
        int MeasureQuery(uint8_t chanID, bool isVSrc, double& val, uint32_t timeout = mcWait4RecvTimeout);/* 进入测量界面  */
        int SetOutput(uint8_t chanID, bool on);
        int ReadOutput(uint8_t chanID, double& vVal, double& iVal, uint32_t timeout = mcWait4RecvTimeout);
        int SetTrigLine(int inputLine);                                                                 /* 设置trig输入线 */
        int VersionQuery(uint8_t chanID, QString& ver, uint32_t timeout = mcWait4RecvTimeout);          /* 获取模拟子板版本信息 */
        int SetPulseCount(uint8_t chanID, uint32_t count);                                              /* 设置脉冲输出个数 */
        int PulseCountQuery(uint8_t chanID, uint32_t& count, uint32_t timeout = mcWait4RecvTimeout);    /* 脉冲输出个数请求 */
        int SetPulseSamplePoint(uint8_t chanID, uint32_t point);                                        /* 设置脉冲采样点 */
        int PulseSamplePointQuery(uint8_t chanID, uint32_t &point, uint32_t timeout);                    /* 脉冲采样点请求 */
        int TraceDataQuery(uint8_t chanID, const char* traceName, char* data, uint32_t& dataSize, uint32_t timeout);/* 缓存数据请求 */
    
        /* 功能接口 */
        QString ReqVal(uint32_t timeout = mcWait4RecvTimeout);                                          /* 请求值 */
        int GetMeasureVal(uint8_t chanID, PssInstrumentMeasureType type, double& val, uint32_t timeout = mcWait4RecvTimeout) const; /* 获取测量值 */
        int GetSweepResult(uint8_t chanID, uint32_t& points, QVector<double>& vResList, QVector<double>& iResList, uint32_t timeout = mcWait4RecvTimeout * 10);/* 获取扫描结果 */
    
        /* Px00独有 伪SCPI接口 */
        QString GetVersion(uint32_t timeout = mcWait4RecvTimeout);                                      /* 获取设备版本信息 */
        int GetVoltAdcVal(uint8_t chanID, double& vAdc, uint32_t timeout = mcWait4RecvTimeout);         /* 获取电压ADC值 */
        int GetCurrAdcVal(uint8_t chanID, double &iAdc, uint32_t timeout = mcWait4RecvTimeout);         /* 获取电流ADC值 */
        int GetVoltDacVal(uint8_t chanID, double& vDac, uint32_t timeout = mcWait4RecvTimeout);         /* 获取电压DAC值 */
        int GetCurrDacVal(uint8_t chanID, double &iDac, uint32_t timeout = mcWait4RecvTimeout);         /* 获取电流DAC值 */
        int SetCalParameter(uint8_t chanID, int index, double dac0, double dac1, double adc0, double adc1);/* 设置校准参数 */
        int FireCalParameter(uint8_t chanID);                                                           /* 烧录校准系数 */
    
    private:
        virtual ScpiClient *GetScpiClient(void) const override;                                         /* 获取Scpi客户端 */
        virtual int UpdateCfgToInstrument(void) override;
    
        Px00ScpiClient *mScpiClient;
    
        const static int mcScpiTcpServerPort = 5025;
        static const QString mcName;
        const static int mcWait4RecvTimeout = 1000;                                                 /* 默认接收等待超时时间 */
        uint8_t mChanID;
};

}
}

#endif // _PSS_QT_PX00_INSTRUMENT_H_