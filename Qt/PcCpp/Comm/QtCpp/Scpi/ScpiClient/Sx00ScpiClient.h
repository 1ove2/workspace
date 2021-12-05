/******************************************************************************
 *
 * 文件名  ： Sx00ScpiClient.h
 * 创建日期： 20210201
 * 版本号  ： v1.0 20210201
 * 文件描述： 对 Sx00产品 SCPI指令的封装
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _SX00_SCPI_CLIENT_H_
#define _SX00_SCPI_CLIENT_H_

/************************************ 头文件 ***********************************/
#include "ScpiClient.h"


/*********************************** 命名空间 **********************************/

class IIo;

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/

class Sx00ScpiClient : public ScpiClient
{
    public:
        Sx00ScpiClient(IIo *iio, const QString &idStr);
        Sx00ScpiClient(const QHostAddress& ip, const QString& idStr);
        Sx00ScpiClient(const QString& commName, uint32_t commBaud, const QString& idStr);
#ifdef _PSS_GPIB_SUPPORT_
        Sx00ScpiClient(uint8_t cardAddr, uint8_t instrumentAddr, const QString& idStr);
#endif
        ~Sx00ScpiClient(void);
        /* 扫描类型，与PssBinAppSx00AnlgEnum.h中定义一致 */
        typedef enum _Sx00ScpiSweepTypeTag_
        {
            Sx00ScpiSweepTypeTBD    = 0,
            Sx00ScpiSweepTypeLiner  = 1,                                                                /* 线性扫描 */
            Sx00ScpiSweepTypeLog    = 2,                                                                /* 对数扫描 */
            Sx00ScpiSweepTypeCustom = 3,                                                                /* 自定义扫描 */
            Sx00ScpiSweepTypeFix    = 4,                                                                /* 固定扫描 */
        } Sx00ScpiSweepType;

        int SendRead(QString& readRstStr, uint32_t timeout = mcWait4RecvTimeout);                       /* 发送read指令 */
        int SendOutputCtrl(bool on) const;                                                              /* 控制输出 */
        int SendVSrc(bool isVSrc) const;                                                                /* 源设置 */
        int SendSrcRange(bool isVSrc, double val) const;                                                /* 设置源量程 */
        int SendSrcVal(bool isVSrc, double val) const;                                                  /* 设置源值 */
        int SendLmtRange(bool isVSrc, double val) const;                                                /* 设置限量程 */
        int SendLmtVal(bool isVSrc, double val) const;                                                  /* 设置限值 */
        int SendNplc(bool isVSens, double nplc) const;                                                  /* 设置NPLC */
        int Send4W(bool is4W) const;                                                                    /* 设置4线 */
        int SendF(bool isF) const;                                                                      /* 设置前后面板 */
        int SendScanStartVal(bool isVSrc, double val) const;                                            /* 设置扫描起点值 */
        int SendScanStopVal(bool isVSrc, double val) const;                                             /* 设置扫描终点值 */
        int SendScanPointNum(uint32_t val) const;                                                       /* 设置扫描点数 */
        int SendScanType(bool isVSrc, Sx00ScpiSweepType type) const;                                    /* 设置扫描源 */
        int SendScanMaster(bool isMaster) const;                                                        /* 设置主从设备 */
        int SendSensFunc(bool isVSrc) const;                                                            /* 限设置 */
        int SendListVal(bool isVSrc, const QVector<double> &data) const;                                /* 设置列表扫描数值 */
        int SendListNum(uint32_t num) const;                                                            /* 设置列表扫描点数 */
        int SendResolution(bool isV, int digits) const;                                                 /* 设置分辨率 */
        int SendConfigure(bool isV, double lmtVal);                                                     /* 配置命令(34470A专用) */
        int SendFireCalParameter(void);                                                                 /* 校准系数烧录 */
        int SendGetAdcValue(bool isV, double& adc, uint32_t timeout = mcWait4RecvTimeout);              /* 获取ADC值 */
        int SendGetDacVaule(bool isV, double& dac, uint32_t timeout = mcWait4RecvTimeout);              /* 获取DAC值 */
        int SendSetCalParameter(int index, double dac0, double dac1, double adc0, double adc1);         /* 设置校准系数 */
        int SendGetOutput(bool &on, uint32_t timeout = mcWait4RecvTimeout);                             /* 获取输出状态 */
        int SendLmtAbort(bool on);                                                                      /* 超限停止 */
        int SendGetSrcType(bool& isVSrc, uint32_t timeout = mcWait4RecvTimeout);                        /* 获取源类型 */
        int SendMeasureQuery(bool isVSrc, double& val, uint32_t timeout = mcWait4RecvTimeout);          /* 进入测量界面 */
        int SendAppendListVal(bool isV, const QVector<double> &data);                                   /* 追加设置自定扫描参数 */
        int SendSrcAutoRange(bool isVSrc, bool on);                                                     /* 设置源自动量程开关 */
        int SendLmtAutoRange(bool isVSrc, bool on);                                                     /* 设置限自动量程开关 */
        int SendSrcAutoRangeQuery(bool isVSrc, bool* on, uint32_t timeout = mcWait4RecvTimeout);        /* 源量程自动开关请求 */
        int SendLmtAutoRangeQuery(bool isVSrc, bool* on, uint32_t timeout = mcWait4RecvTimeout);        /* 限量程自动开关请求 */
        int SendSrcRangeQuery(bool isVSrc, char* range, uint32_t timeout = mcWait4RecvTimeout);         /* 源量程值请求 */
        int SendLmtRangeQuery(bool isVSrc, char* range, uint32_t timeout = mcWait4RecvTimeout);         /* 限量程值请求 */
        int SendTraceSet(bool on);                                                                      /* 缓存开关 */
        int SendVersionReq(QString &idnRstStr, uint32_t timeout);                                       /* 获取版本信息 */
        int SendNetUpgradePort(uint32_t port);                                                          /* 设置设备升级端口 */
        int SendAnlgStateReq(QString &retStr, uint32_t timeout);                                        /* 获取模拟板状态 */
    
    private:
        bool             mDeleteIo;
        const static int mcWait4RecvTimeout = 500;                                                      /* 等待接收超时ms */
        const static int mcPerListSweepValNumbers = 50;                                                 /* 自定义参数个数最大值 */
        const static int mcMaxSweepPoints = 2000;
        const static QString mcClientIdStr;
};

/*********************************** 工具函数 **********************************/



#endif // _SX00_SCPI_CLIENT_H_
