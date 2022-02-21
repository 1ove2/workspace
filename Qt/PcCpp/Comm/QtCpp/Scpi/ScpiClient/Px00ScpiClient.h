/******************************************************************************
 *
 * 文件名  ： Px00ScpiClient.h
 * 负责人  ： ryl
 * 创建日期： 20210201
 * 版本号  ： v1.0 20210201
 * 文件描述： PSS 对 Sx00产品 SCPI指令的封装
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_QT_PX00_SCPI_CLIENT_H_
#define _PSS_QT_PX00_SCPI_CLIENT_H_

/************************************ 头文件 ***********************************/
#include "ScpiClient.h"


/*********************************** 命名空间 **********************************/
namespace Pss {
class IIo;

namespace Qt {

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/

class Px00ScpiClient : public ScpiClient
{
    public:
        Px00ScpiClient(IIo *iio, const QString &idStr);
        Px00ScpiClient(const QHostAddress& ip, const QString& idStr);
        Px00ScpiClient(const QString& commName, uint32_t commBaud, const QString& idStr);
#ifdef _PSS_GPIB_SUPPORT_
        Px00ScpiClient(uint8_t cardAddr, uint8_t instrumentAddr, const QString& idStr);
#endif
        ~Px00ScpiClient(void);
        /* 扫描类型，与PssBinAppPx00AnlgEnum.h中定义一致 */
        typedef enum _Px00ScpiSweepTypeTag_
        {
            Px00ScpiSweepTypeTBD    = 0,
            Px00ScpiSweepTypeLiner  = 1,                                                                /* 线性扫描 */
            Px00ScpiSweepTypeLog    = 2,                                                                /* 对数扫描 */
            Px00ScpiSweepTypeCustom = 3,                                                                /* 自定义扫描 */
            Px00ScpiSweepTypeFix    = 4,                                                                /* 固定扫描 */
        } Px00ScpiSweepType;

        typedef enum _Px00ScpiTrigTypeTag_
        {
            Px00ScpiTrigTypeSOUR = 0,                                                                   /*源TRIG状态*/
            Px00ScpiTrigTypeMEAS = 1,                                                                   /*测量TRIG状态*/
            Px00ScpiTrigTypeALL  = 2,                                                                   /*源和测量TRIG状态*/
        } Px00ScpiTrigType;
    
        int SendRead(uint8_t chanID, QString& readRstStr, uint32_t timeout = mcWait4RecvTimeout);                   /* 发送read指令 */
        int SendOutputCtrl(uint8_t chanID, bool on) const;                                                          /* 控制输出 */
        int SendVSrc(uint8_t chanID, bool isVSrc) const;                                                            /* 源设置 */
        int SendSrcRange(uint8_t chanID, bool isVSrc, double val) const;                                            /* 设置源量程 */
        int SendSrcVal(uint8_t chanID, bool isVSrc, double val) const;                                              /* 设置源值 */
        int SendLmtRange(uint8_t chanID, bool isVSrc, double val) const;                                            /* 设置限量程 */
        int SendLmtVal(uint8_t chanID, bool isVSrc, double val) const;                                              /* 设置限值 */
        int Send4W(uint8_t chanID, bool is4W) const;                                                                /* 设置4线 */
        int SendF(uint8_t chanID, bool isF) const;                                                                  /* 设置前后面板 */
        int SendScanStartVal(uint8_t chanID, bool isVSrc, double val) const;                                        /* 设置扫描起点值 */
        int SendScanStopVal(uint8_t chanID, bool isVSrc, double val) const;                                         /* 设置扫描终点值 */
        int SendScanStepVal(uint8_t chanID, bool isVSrc, double val) const;                                         /* 设置扫描步进值 */
        int SendScanPointNum(uint8_t chanID, uint32_t points) const;                                                /* 设置扫描点数 */
        int SendScanType(uint8_t chanID, bool isVSrc, Px00ScpiSweepType type) const;                                /* 设置扫描源 */
        int SendScanMaster(uint8_t chanID, bool isMaster) const;                                                    /* 设置主从设备 */
        int SendTrigInput(bool on);                                                                                 /* 设置trig输入开关 */
        int SendListVal(uint8_t chanID, bool isVSrc, const QVector<double> &data) const;                            /* 设置列表扫描数值 */
        int SendAppendListVal(uint8_t chanID, bool isV, const QVector<double> &data);                               /* 追加设置自定扫描参数 */
        int SendLmtAbort(uint8_t chanID, bool on);                                                                  /* 超限停止 */
        int SendNplc(uint8_t chanID, bool isVSens, double nplc) const;                                              /* 设置NPLC */
        int SendGetOutput(uint8_t chanID, bool &on, uint32_t timeout);                                              /* 获取输出状态 */
        int SendGetSrcType(uint8_t chanID, bool& isVSrc, uint32_t timeout);                                         /* 获取源类型 */
        int SendSrcAutoRange(uint8_t chanID, bool isVSrc, bool on);                                                 /* 设置源自动量程开关 */
        int SendLmtAutoRange(uint8_t chanID, bool isVSrc, bool on);                                                 /* 设置限自动量程开关 */
        int SendSrcAutoRangeQuery(uint8_t chanID, bool isVSrc, bool* on, uint32_t timeout = mcWait4RecvTimeout);    /* 源量程自动开关请求 */
        int SendLmtAutoRangeQuery(uint8_t chanID, bool isVSrc, bool* on, uint32_t timeout = mcWait4RecvTimeout);    /* 限量程自动开关请求 */
        int SendSrcRangeQuery(uint8_t chanID, bool isVSrc, char* range, uint32_t timeout = mcWait4RecvTimeout);     /* 源量程值请求 */
        int SendLmtRangeQuery(uint8_t chanID, bool isVSrc, char* range, uint32_t timeout = mcWait4RecvTimeout);     /* 限量程值请求 */
        int SendOutputMode(uint8_t chanID, bool isPulse);                                                           /* 设置源输出形状 */
        int SendOutputModeQuery(uint8_t chanID, bool &isPulse, uint32_t timeout = mcWait4RecvTimeout);              /* 获取源输出形状 */
        int SendPulseDelay(uint8_t chanID, uint32_t delayNs);                                                       /* 设置脉冲延时 */
        int SendPulseDelayQuery(uint8_t chanID, uint32_t &delayNs, uint32_t timeout = mcWait4RecvTimeout);               /* 获取脉冲延时 */
        int SendPulseWidth(uint8_t chanID, uint32_t pulseWidth);                                                    /* 设置脉冲宽度 */
        int SendPulseWidthQuery(uint8_t chanID, uint32_t &pulseWidth, uint32_t timeout = mcWait4RecvTimeout);            /* 获取脉冲宽度 */
        int SendPulsePeriod(uint8_t chanID, uint32_t pulsePeriod);                                                  /* 设置脉冲周期 */
        int SendPulsePeriodQuery(uint8_t chanID, uint32_t &pulsePeriod, uint32_t timeout = mcWait4RecvTimeout);          /* 获取脉冲周期 */
        int SendTraceSet(uint8_t chanID, bool on);                                                                  /* 缓存开关 */
        int SendTrigDelay(uint8_t chanID, uint32_t delayUs);                                                        /* 设置Trig延时 us */
        int SendInitTrigState(uint8_t chanID, Px00ScpiTrigType trigType);                                           /* 设置Trig状态 */
        int SendTrigStateQuery(uint8_t chanID, Px00ScpiTrigType trigType, int &state);                              /* 获取Trig状态 */
        int SendIPConfig(bool isAuto, const QString &ip, const QString &netMask, const QString &gateWay);           /* 设置网络IP */
        int SendIPConfigQuery(bool &isAuto, QString &ip, QString &netMask, QString &gateWay, uint32_t timeout = mcWait4RecvTimeout);     /* 网络IP信息请求 */
        int SendUpdateIPConfig(void);                                                                               /* 更新网络IP */
        int SendGPIBConfig(uint8_t devAddr);                                                                        /* 设置GPIB地址 */
        int SendGPIBConfigQuery(bool &on, uint8_t &devAddr, uint32_t timeout);                                                /* GPIB地址请求 */
        int SendUARTConfig(uint32_t baud);                                                                          /* 设置串口波特率 */
        int SendUARTConfigQuery(bool &on, uint32_t &baud, uint32_t timeout);                                                  /* 串口波特率请求 */
        int SendMeasureQuery(uint8_t chanID, bool isVSrc, double& val, uint32_t timeout);                           /* 进入测量界面 */
        int SendFireCalParameter(uint8_t chanID);                                                                   /* 校准系数烧录 */
        int SendGetAdcValue(uint8_t chanID, bool isV, double& adc, uint32_t timeout = mcWait4RecvTimeout);          /* 获取ADC值 */
        int SendGetDacVaule(uint8_t chanID, bool isV, double& dac, uint32_t timeout = mcWait4RecvTimeout);          /* 获取DAC值 */
        int SendSetCalParameter(uint8_t chanID, int index, double dac0, double dac1, double adc0, double adc1);     /* 设置校准系数 */
        int SendVersionReq(QString &idnRstStr, uint32_t timeout);                                                   /* 获取版本信息 */
        int SendVersionQuery(uint8_t chanID, QString& verStr, uint32_t timeout = mcWait4RecvTimeout);
        int SetPulseCount(uint8_t chanID, uint32_t count);                                                          /* 设置脉冲输出个数  */
        int PulseCountQuery(uint8_t chanID, uint32_t& count, uint32_t timeout = mcWait4RecvTimeout);                /* 脉冲输出个数请求 */
        int SetPulseSamplePoint(uint8_t chanID, uint32_t point);                                                    /* 设置脉冲采样点 */
        int PulseSamplePointQuery(uint8_t chanID, uint32_t &point, uint32_t timeout);                                /* 脉冲采样点请求 */
        int TraceDataQuery(uint8_t chanID, const char *traceName, char *data, uint32_t &dataSize, uint32_t timeout);/* 缓存数据请求 */
    
        int SendFireCalParameter(void);                                                                             /* 校准系数烧录 */
        int SendGetAdcValue(bool isV, double& adc, uint32_t timeout = mcWait4RecvTimeout);                          /* 获取ADC值 */
        int SendGetDacVaule(bool isV, double& dac, uint32_t timeout = mcWait4RecvTimeout);                          /* 获取DAC值 */
        int SendSetCalParameter(int index, double dac0, double dac1, double adc0, double adc1);                     /* 设置校准系数 */
    
    private:
        bool             mDeleteIo;
        const static int mcWait4RecvTimeout = 500;                                                                  /* 等待接收超时ms */
        const static int mcPerListSweepValNumbers = 50;                                                             /* 自定义参数个数最大值 */
        const static int mcMaxSweepPoints = 2000;
        const static QString mcClientIdStr;
};

/*********************************** 工具函数 **********************************/

}
}

#endif // _PSS_QT_PX00_SCPI_CLIENT_H_