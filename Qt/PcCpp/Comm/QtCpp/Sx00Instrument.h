/******************************************************************************
 *
 * 文件名  ： Sx00Instrument.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20200422
 * 版本号  ： v1.0 20200422
 * 文件描述： Sx00整机代理类
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _QT_SX00_INSTRUMENT_H_
#define _QT_SX00_INSTRUMENT_H_

/************************************ 头文件 ***********************************/
#include "QtIInstrument.h"
#include "Sx00ScpiClient.h"
/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class Sx00Instrument : public QtIInstrument
{
    Q_OBJECT

    public:
        Sx00Instrument(const QHostAddress &ip, const QString& instName = "Sx00", bool hasSrc = true, QObject *parent = 0);
        Sx00Instrument(const QString comName, uint32_t comBaud, const QString& instName = "Sx00", bool hasSrc = true, QObject *parent = 0);
        //Sx00Instrument(const QString strIP, const QString& instName = "Sx00", bool hasSrc = true, QObject *parent = 0);
#ifdef _PSS_GPIB_SUPPORT_
        Sx00Instrument(uint8_t cardAddr, uint8_t instrumentAddr, const QString& instName = "Sx00", bool hasSrc = true, QObject *parent = 0);
#endif
        ~Sx00Instrument(void);

        static QString GetIdName(void);                                                                /* 获取Sx00的设备型号标识 */
    
        /* SCPI 接口 */
        int Set4W(bool is4W);                                                                           /* 设置24线 */
        int SetF(bool isF);                                                                             /* 设置前后面板 */
        int SetVSrc(bool isVSrc);                                                                       /* 设置源 */
        int SetLmtRange(bool isVSrc, double val);                                                       /* 设置限量程 */
        int SetLmtVal(bool isVSrc, double val);                                                         /* 设置限值 */
        int SetSrcRange(bool isVSrc, double val);                                                       /* 设置源量程 */
        int SetSrcVal(bool isVSrc, double val);                                                         /* 设置源值 */
        int SetNPLC(bool isV, double nplc);                                                             /* 设置NPLC */
        int GetOutputStatus(bool &on, uint32_t timeout = mcWait4RecvTimeout);                           /* 获取输出状态 */
        int GetSrcType(bool& isVSrc, uint32_t timeout = mcWait4RecvTimeout);                            /* 获取源类型 */
        int MeasureQuery(bool isVSrc, double& val, uint32_t timeout = mcWait4RecvTimeout);              /* 进入测量界面  */
        int AppendCustomSweepData(bool isV, const QVector<double>& data);                               /* 追加自定义扫描参数 */
        int SetSrcAutoRange(bool isVSrc, bool on);                                                      /* 设置源自动量程开关 */
        int SetLmtAutoRange(bool isVSrc, bool on);                                                      /* 设置限自动量程开关 */
        int SrcAutoRangeQuery(bool isVSrc, bool* on, uint32_t timeout = mcWait4RecvTimeout);            /* 源量程自动开关请求 */
        int LmtAutoRangeQuery(bool isVSrc, bool* on, uint32_t timeout = mcWait4RecvTimeout);            /* 限量程自动开关请求 */
        int SrcRangeQuery(bool isVSrc, char* range, uint32_t timeout = mcWait4RecvTimeout);             /* 源量程值请求 */
        int LmtRangeQuery(bool isVSrc, char* range, uint32_t timeout = mcWait4RecvTimeout);             /* 限量程值请求 */
        int TraceSet(bool on);                                                                          /* 设置设备缓存开关 */
        PssRstT SetResolution(bool isV, int digits);                                                    /* 设置仪器分辨率 */
        int ReadOutput(double& voltVal, double& currVal, uint32_t timeout = mcWait4RecvTimeout);        /* 读取输出值 */
        int SetOutput(bool on, bool isRead = true);                                                     /* 输出控制 开关 */
        int SetScanStartVal(bool isVSrc, double val);                                                   /* 设置扫描起点值 */
        int SetScanStopVal(bool isVSrc, double val);                                                    /* 设置扫描终点值 */
        int SetScanPointNum(int val);                                                                   /* 设置扫描点数 */                                                                 /* 设置扫描源 */
        int SetMasterMode(bool isMaster);                                                               /* 设置主从模式 */
        int SetTrigInput(bool on);                                                                      /* 设置trig输入开关 */
        int SetCustomScanData(bool isV, const QVector<double> &data);                                   /* 设置自定义扫描数据 */
        int SetLmtAbort(bool on);                                                                       /* 超限停止开关 */
        int SetScanType(bool isV, Sx00ScpiClient::Sx00ScpiSweepType type);                                                       /* 设置扫描类型 */
        int SetTrigLine(int inputLine);                                                                 /* 设置trig输入线 */
        int SetNetUpgradePort(uint32_t port);                                                           /* 设置设备升级端口 */
    
        /* 仪表 功能接口 */
        virtual int GetMeasureVal(PssInstrumentMeasureType type, double& val, uint32_t timeout = mcWait4RecvTimeout);/* 获取测量值 */
        QString ReqVal(uint32_t timeout = mcWait4RecvTimeout);                                /* 请求值 */
        int GetSweepResult(int &points, QVector<double>& vResList, QVector<double>& iResList, uint32_t timeout = mcWait4RecvTimeout * 10);/* 获取扫描结果 串口1000点需要至少20s接收  */
    
        /* Sx00独有 伪SCPI接口 */
        QString GetVersion(uint32_t timeout = mcWait4RecvTimeout);                                      /* 获取设备版本信息 */
        int GetVoltAdcVal(double& vAdc, uint32_t timeout = mcWait4RecvTimeout);                         /* 获取电压ADC值 */
        int GetCurrAdcVal(double &iAdc, uint32_t timeout = mcWait4RecvTimeout);                         /* 获取电流ADC值 */
        int GetVoltDacVal(double& vDac, uint32_t timeout = mcWait4RecvTimeout);                         /* 获取电压DAC值 */
        int GetCurrDacVal(double &iDac, uint32_t timeout = mcWait4RecvTimeout);                         /* 获取电流DAC值 */
        int GetAnlgState(QString &retStr, uint32_t timeout = mcWait4RecvTimeout);
        int SetCalParameter(int index, double dac0, double dac1, double adc0, double adc1);             /* 设置校准参数 */
        int FireCalParameter(void);                                                                     /* 烧录校准系数 */
    
    protected:
        ScpiClient *GetScpiClient(void) const override;                                                 /* 获取Scpi客户端 */
        Sx00ScpiClient *mScpiClient;                                                                    /* SCPI客户端 */
    
    private:
        virtual void   InitSourceInfo(void);                                                            /* 初始化设备源信息 */
        virtual int UpdateCfgToInstrument(void) override;                                               /* 配置信息 设置 到仪表 */
    
        static const QString  mcName;                                                                   /* IDN中的设备标识 */
        const static int mcWait4RecvTimeout = 2000;                                                      /* 复位需要的ms延迟 */

};
/*********************************** 工具函数 **********************************/



#endif // _QT_SX00_INSTRUMENT_H_
