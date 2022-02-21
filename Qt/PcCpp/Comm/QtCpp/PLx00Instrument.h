/******************************************************************************
 *
 * 文件名  ： PLx00Instrument.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20200422
 * 版本号  ： v1.0 20200422
 * 文件描述： PLx00整机代理类
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_QT_PLX00_INSTRUMENT_H_
#define _PSS_QT_PLX00_INSTRUMENT_H_

/************************************ 头文件 ***********************************/
#include "QtIInstrument.h"
#include "PLx00ScpiClient.h"
/*********************************** 命名空间 **********************************/
namespace Pss {
namespace Qt {

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class PLx00Instrument : public QtIInstrument
{
    Q_OBJECT

    public:
        PLx00Instrument(const QHostAddress &ip, const QString& instName = "PLx00", bool hasSrc = false, QObject *parent = 0);
        PLx00Instrument(const QString comName, uint32_t comBaud, const QString& instName = "PLx00", bool hasSrc = false, QObject *parent = 0);
    #ifdef _PSS_GPIB_SUPPORT_
        PLx00Instrument(uint8_t cardAddr, uint8_t instrumentAddr, const QString& instName = "PLx00", bool hasSrc = false, QObject *parent = 0);
    #endif
        ~PLx00Instrument(void);
    
        static QString GetIdnName(void);                                                                                                /* 获取Sx00的IDN标识 */
    
        virtual int GetMeasureVal(PssInstrumentMeasureType type, double& val, uint32_t timeout = mcWait4RecvTimeout);                   /* 获取测量值 */
        QString GetVersion(uint32_t timeout = mcWait4RecvTimeout);                                                                      /* 获取设备版本信息 */
    
        /******* 新增 ******/
        int SetOutputMode(const bool &isPulse, uint32_t timeout = mcWait4RecvTimeout);                                                  /* 设置输出模式 */
        int SetPulsePara(const uint32_t &pulseWidth, const uint32_t &pulsePeriod, uint32_t timeout = mcWait4RecvTimeout);               /* 设置脉冲宽度和周期 */
        int GetPulsePara(int &pulseWidth, int &pulsePeriod, uint32_t timeout = mcWait4RecvTimeout);                                     /* 获取脉冲宽度和周期 */
        int SetScanCurrVal(const double &startVal, const double &stepVal, const double &stopVal, uint32_t timeout = mcWait4RecvTimeout);/* 设置电流扫描参数 */
        int GetScanCurrVal(double &startVal, double &stepVal, double &stopVal, uint32_t timeout = mcWait4RecvTimeout);                  /* 获取电流扫描参数 */
        int SetWaveLength(const uint32_t &len, uint32_t timeout = mcWait4RecvTimeout);                                                  /* 设置光功率波长 */
        int GetWaveLength(int &len, uint32_t timeout = mcWait4RecvTimeout);                                                             /* 获取光功率波长 */
        int SetDCCurrent(const double &val, uint32_t timeout = mcWait4RecvTimeout);                                                     /* 设置直流电流 */
        int GetTestDC(double &iVal,double &vVal,double &pVal,double &imVal, uint32_t timeout = mcWait4RecvTimeout);                     /* 获取直流测试数据 */
        int SetStartScan(uint32_t timeout = mcWait4RecvTimeout);                                                                        /* 启动扫描 */
        int SetMaxPower(const double &val, uint32_t timeout = mcWait4RecvTimeout);                                                      /* 设置最大光功率 */
        int SetOffsetVol(const double &val, uint32_t timeout = mcWait4RecvTimeout);                                                     /* 设置光功率偏压 */
        int SetDutvCtlType(const double &dut, uint32_t timeout = mcWait4RecvTimeout);                                                   /* 设置最大检测电压 */
        int SetOverVolProtect(const double &val, uint32_t timeout = mcWait4RecvTimeout);                                                /* 设置过压保护值 */
        int SetDCScanPoint(const uint32_t &point, uint32_t timeout = mcWait4RecvTimeout);                                               /* 设置直流扫描点数 */
        int SetSampPoint(const int &sampPoint, uint32_t timeout = mcWait4RecvTimeout);                                                  /* 设置采样点数 */
        int SetSampDelay(const int &sampDelay, uint32_t timeout = mcWait4RecvTimeout);                                                  /* 设置采样延时 */
        int SetInitDataCtrl(const bool &isOn, uint32_t timeout = mcWait4RecvTimeout);                                                   /* 设置是否输出原始数据 */
        int SetOutputDelay(const uint32_t &delayUs, uint32_t timeout = mcWait4RecvTimeout);                                             /* 设置输出延时 */
        int SetTrigDelay(const uint32_t &delayUs, uint32_t timeout = mcWait4RecvTimeout);                                               /* 设置trig延时 */
        int SetTrigPulseWidth(const uint32_t &pulseWidth, uint32_t timeout = mcWait4RecvTimeout);                                       /* 设置trig脉宽 */
        int GetSweepStatus(bool &isFree, uint32_t timeout = mcWait4RecvTimeout);                                                        /* 获取扫描状态 */
        int GetSweepResult(QVector<double> &vecI, QVector<double> &vecV,
                           QVector<double> &vecPower, QVector<double> &vecBackI, uint32_t timeout = mcWait4RecvTimeout * 100);          /* 获取扫描数据 */
    
        int SetImPower(const float &val);
    
        int SetOpticalPort(const int &port);
        /* 校准 */
        int ReqAdc(const bool &isVol, double &adcVal);                                                                                  /* 请求值 */
        int ReqDac(const bool &isVol, double &dacVal);                                                                                  /* 请求值 */
        int SetCalFire(void);
        int SetCalVal(const int &index,const double &dac0,const double &dac1,const double &adc0,const double &adc1);
    private:
        virtual ScpiClient *GetScpiClient(void) const override;                                                                         /* 获取Scpi客户端 */
        PLx00ScpiClient *mScpiClient;                                                                                                   /* SCPI客户端 */
    
        virtual void  InitSourceInfo(void);                                                                                             /* 初始化设备源信息 */
        virtual int UpdateCfgToInstrument(void) override;
    
        static const QString  mcName;                                                                                                   /* IDN中的设备标识 */
        const static int mcWait4RecvTimeout = 5000;                                                                                     /* 复位需要的ms延迟 */
};
/*********************************** 工具函数 **********************************/

}
}

#endif // _PSS_QT_PLX00_INSTRUMENT_H_