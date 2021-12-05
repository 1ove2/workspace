#ifndef _CX00_SCPI_CLIENT_H_
#define _CX00_SCPI_CLIENT_H_

#include "ScpiClient.h"


class CSx00ScpiClient : public ScpiClient
{
    Q_OBJECT

    public:
        CSx00ScpiClient(IIo *iio, const QString &idStr);
        CSx00ScpiClient(const QHostAddress& ip, const QString& idStr);
        CSx00ScpiClient(const QString& commName, uint32_t commBaud, const QString& idStr);
#ifdef _PSS_GPIB_SUPPORT_
        CSx00ScpiClient(uint8_t cardAddr, uint8_t instrumentAddr, const QString& idStr);
#endif
        ~CSx00ScpiClient(void);

        /* 扫描类型，与PssBinAppCSx00AnlgEnum.h中定义一致 */
        typedef enum _CSx00ScpiSweepTypeTag_
        {
            CSx00ScpiSweepTypeTBD    = 0,
            CSx00ScpiSweepTypeLiner  = 1,                                                                           /* 线性扫描 */
            CSx00ScpiSweepTypeLog    = 2,                                                                           /* 对数扫描 */
            CSx00ScpiSweepTypeCustom = 3,                                                                           /* 自定义扫描 */
            CSx00ScpiSweepTypeFix    = 4,                                                                           /* 固定扫描 */
        } CSx00ScpiSweepType;
    
        /* trig触发类型，与PssBinAppCSx00AnlgEnum.h中定义一致 */
        typedef enum _CSx00ScpiTrigEdgeTypeTag_
        {
            PssCSx00TrigEdgeTypeNone        = 0,                                                /* 无 */
            PssCSx00TrigEdgeTypeFall        = 1,                                                /* 下降沿 */
            PssCSx00TrigEdgeTypeRise        = 2,                                                /* 上升沿 */
            PssCSx00TrigEdgeTypeEither      = 3,                                                /* 边沿 */
    
        } CSx00ScpiTrigEdgeType;
        /* 发送 */
        int SetChanID(uint8_t chanID, uint8_t childChanID);
        int SendRead(uint8_t chanID, QString& readRstStr, uint32_t timeout = mcWait4RecvTimeout);                   /* 发送read指令 */
        int SendReadAll(QString& readRstStr, uint32_t timeout = mcWait4RecvTimeout);
        int SendOutputCtrl(uint8_t chanID, bool on) const;                                                          /* 控制输出 */
        int SendVSrc(uint8_t chanID, bool isVSrc) const;                                                            /* 源设置 */
        int SendSrcRange(uint8_t chanID, bool isVSrc, double val) const;                                            /* 设置源量程 */
        int SendSrcVal(uint8_t chanID, bool isVSrc, double val) const;                                              /* 设置源值 */
        int SendLmtRange(uint8_t chanID, bool isVSrc, double val) const;                                            /* 设置限量程 */
        int SendLmtVal(uint8_t chanID, bool isVSrc, double val) const;                                              /* 设置限值 */
        int Send4W(uint8_t chanID, bool is4W) const;                                                                /* 设置4线 */
        int SendScanStartVal(uint8_t chanID, bool isVSrc, double val) const;                                        /* 设置扫描起点值 */
        int SendScanStopVal(uint8_t chanID, bool isVSrc, double val) const;                                         /* 设置扫描终点值 */
        int SendScanPointNum(uint8_t chanID, uint32_t points) const;                                                /* 设置扫描点数 */
        int SendScanType(uint8_t chanID, bool isVSrc, CSx00ScpiSweepType type) const;                               /* 设置扫描源 */
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
        int SendSrcAutoRangeQuery(uint8_t chanID, bool isVSrc, bool* on, uint32_t timeout);                         /* 源量程自动开关请求 */
        int SendLmtAutoRangeQuery(uint8_t chanID, bool isVSrc, bool* on, uint32_t timeout);                         /* 限量程自动开关请求 */
        int SendSrcRangeQuery(uint8_t chanID, bool isVSrc, char* range, uint32_t timeout);                          /* 源量程值请求 */
        int SendLmtRangeQuery(uint8_t chanID, bool isVSrc, char* range, uint32_t timeout);                          /* 限量程值请求 */
        int SendTraceSet(uint8_t chanID, bool on);                                                                  /* 缓存开关 */
        int SendTrigDelay(uint8_t chanID, uint32_t delayUs);                                                        /* 设置Trig延时 us */
        int SendOutputDelay(uint8_t chanID, uint32_t delayUs);                                                      /* 设置输出延时 us */
        int SendIPConfig(bool isAuto, const QString &ip, const QString &netMask, const QString &gateWay);           /* 设置网络IP */
        int SendIPConfigQuery(bool &isAuto, QString &ip, QString &netMask, QString &gateWay, uint32_t timeout);     /* 网络IP信息请求 */
        int SendUpdateIPConfig(void);                                                                               /* 更新网络IP */
        int SendGPIBConfig(uint8_t devAddr);                                                                        /* 设置GPIB地址 */
        int SendGPIBConfigQuery(uint8_t &devAddr, uint32_t timeout);                                                /* GPIB地址请求 */
        int SendUARTConfig(uint32_t baud);                                                                          /* 设置串口波特率 */
        int SendUARTConfigQuery(uint32_t &baud, uint32_t timeout);                                                  /* 串口波特率请求 */
        int SendStartOutputEvent(uint8_t chanID, int inputLine, int outputLine, CSx00ScpiTrigEdgeType trigType);    /* 设置开始输出事件 */
        int SendFinishOutputEvent(uint8_t chanID, int inputLine, int outputLine, CSx00ScpiTrigEdgeType trigType);   /* 设置完成输出事件 */
        int SendStartSamplingEvent(uint8_t chanID, int inputLine, int outputLine, CSx00ScpiTrigEdgeType trigType);  /* 设置开始采样事件 */
        int SendFinishSamplingEvent(uint8_t chanID, int inputLine, int outputLine, CSx00ScpiTrigEdgeType trigType); /* 设置完成采样事件 */
        int SendStartSweEvent(uint8_t chanID, int inputLine, int outputLine, CSx00ScpiTrigEdgeType trigType);       /* 设置开始扫描事件 */
        int SendClearEvent(uint8_t chanID);                                                                         /* 清除事件配置 */
        int SendTrigCount(uint8_t chanID, int count);                                                               /* 设置trig数量 */
        int SendMeasureQuery(uint8_t chanID, bool isVSrc, double& val, uint32_t timeout);                           /* 进入测量界面 */
        int SendFireCalParameter(uint8_t chanID);                                                                   /* 校准系数烧录 */
        int SendGetAdcValue(uint8_t chanID, bool isV, double& adc, uint32_t timeout = mcWait4RecvTimeout);          /* 获取ADC值 */
        int SendGetDacVaule(uint8_t chanID, bool isV, double& dac, uint32_t timeout = mcWait4RecvTimeout);          /* 获取DAC值 */
        int SendSetCalParameter(uint8_t chanID, int index, double dac0, double dac1, double adc0, double adc1);     /* 设置校准系数 */
        int SendVersionReq(QString &idnRstStr, uint32_t timeout);                                                   /* 获取版本信息 */
        int SendVersionQuery(uint8_t chanID, QString& verStr, uint32_t timeout);
    private:
        bool             mDeleteIo;
        const static int mcMaxSweepPoints = 2000;
        const static int mcPerListSweepValNumbers = 50;
        const static QString mcClientIdStr;
        const static int mcWait4RecvTimeout = 1000;                                                 /* 默认接收等待超时时间 */
        const static  QString mcName;
};



#endif // _CX00_SCPI_CLIENT_H_
