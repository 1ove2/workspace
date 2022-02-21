/******************************************************************************
 *
 * 文件名  ： PLx00ScpiClient.h
 * 负责人  ： ryl
 * 创建日期： 20210201
 * 版本号  ： v1.0 20210201
 * 文件描述： PSS 对 Sx00产品 SCPI指令的封装
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_QT_PLX00_SCPI_CLIENT_H_
#define _PSS_QT_PLX00_SCPI_CLIENT_H_

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

class PLx00ScpiClient : public ScpiClient
{
    public:
        PLx00ScpiClient(IIo *iio, const QString &idStr);
        PLx00ScpiClient(const QHostAddress& ip, const QString& idStr);
        PLx00ScpiClient(const QString& commName, uint32_t commBaud, const QString& idStr);
#ifdef _PSS_GPIB_SUPPORT_
        PLx00ScpiClient(uint8_t cardAddr, uint8_t instrumentAddr, const QString& idStr);
#endif
        ~PLx00ScpiClient(void);

        int SendVersionReq(QString &idnRstStr, uint32_t timeout);                                       /* 获取版本信息 */
        int SendRead(QString& readRstStr, uint32_t timeout = mcWait4RecvTimeout);                       /* 发送read指令 */
        int SendOutputMode(const bool &isPulse, uint32_t timeout = mcWait4RecvTimeout);                 /* 设置源模式 */
        int OutputModeQuery(bool &isPulse, uint32_t timeout = mcWait4RecvTimeout);                      /* 查询源模式 */
        int SendPulseWidth(const uint32_t &pulseWidth, uint32_t timeout = mcWait4RecvTimeout);          /* 设置脉冲宽度 */
        int SendPulsePeriod(const uint32_t &pulsePeriod, uint32_t timeout = mcWait4RecvTimeout);        /* 设置脉冲周期 */
        int PulseWidthQuery(int &pulseWidth, uint32_t timeout = mcWait4RecvTimeout);                    /* 查询脉冲宽度 */
        int PulsePeriodQuery(int &pulsePeriod, uint32_t timeout = mcWait4RecvTimeout);                  /* 查询脉冲周期 */
        int SendScanCurrStartVal(const double &val, uint32_t timeout = mcWait4RecvTimeout);             /* 设置扫描起始电流 */
        int SendScanCurrSteptVal(const double &val, uint32_t timeout = mcWait4RecvTimeout);             /* 设置扫描步进电流 */
        int SendScanCurrStopVal(const double &val, uint32_t timeout = mcWait4RecvTimeout);              /* 设置扫描终止电流 */
        int ScanCurrStartValQuery(double &val, uint32_t timeout = mcWait4RecvTimeout);                  /* 查询扫描起始电流 */
        int ScanCurrStepValQuery(double &val, uint32_t timeout = mcWait4RecvTimeout);                   /* 查询扫描步进电流 */
        int ScanCurrStopValQuery(double &val, uint32_t timeout = mcWait4RecvTimeout);                   /* 查询扫描终止电流 */
        int SendWaveLength(const uint32_t &len, uint32_t timeout = mcWait4RecvTimeout);                 /* 设置波长 */
        int WaveLengthQuery(int &len, uint32_t timeout = mcWait4RecvTimeout);                           /* 查询波长 */
        int SendStartScan(uint32_t timeout = mcWait4RecvTimeout);                                       /* 开始扫描 */
        int ScanStatusQuery(bool &isFree, uint32_t timeout = mcWait4RecvTimeout);                       /* 查询扫描状态 */
        int SendDCScanPoint(const int &point, uint32_t timeout = mcWait4RecvTimeout);                   /* 设置直流扫描点数 */
        int SendSampPoint(const int &sampPoint, uint32_t timeout = mcWait4RecvTimeout);                 /* 设置采样点数 */
        int SendSampDelay(const int &sampDelay, uint32_t timeout = mcWait4RecvTimeout);                 /* 设置采样延时 */
        int SendOutputDelay(uint32_t delayUs, uint32_t timeout = mcWait4RecvTimeout);                   /* 设置源输出延时 */
        int SendTrigDelay(uint32_t delayUs, uint32_t timeout = mcWait4RecvTimeout);                     /* 设置Trig延时 */
        int SendTrigPulseWidth(uint32_t pulseWidth, uint32_t timeout = mcWait4RecvTimeout);             /* 设置Trig脉宽 */
        int SendInitDataCtrl(bool isOn, uint32_t timeout = mcWait4RecvTimeout);                         /* 设置输出原始数据 */
        int SendDutvCtlType(const double &dut, uint32_t timeout = mcWait4RecvTimeout);                  /* 设置最大检测电压 */
        int SendDCCurrent(const double &val, uint32_t timeout = mcWait4RecvTimeout);                    /* 设置直流电流 */
        int SendOverVolProtect(const double &val, uint32_t timeout = mcWait4RecvTimeout);               /* 设置过压保护 */
        int SendMaxPower(const double &val, uint32_t timeout = mcWait4RecvTimeout);                     /* 设置最大测量光功率 */
        int SendOffsetVoltage(const double &val, uint32_t timeout = mcWait4RecvTimeout);                /* 设置光功率偏压 */
        int SendImPower(const float &val, uint32_t timeout = mcWait4RecvTimeout);
        int SendOpticalPort(const int &val, uint32_t timeout = mcWait4RecvTimeout);
    
        /* 校准 */
        int ReqAdc(const bool &isVol, double &adcVal, uint32_t timeout = mcWait4RecvTimeout);           /* 请求值ADC */
        int ReqDac(const bool &isVol, double &dacVal, uint32_t timeout = mcWait4RecvTimeout);           /* 请求值DAC */
        int SendCalFire(uint32_t timeout = mcWait4RecvTimeout);
        int SendCalVal(const int &index,const double &dac0,const double &dac1,
                       const double &adc0,const double &adc1,uint32_t timeout = mcWait4RecvTimeout);
    private:
        bool             mDeleteIo;
        const static int mcWait4RecvTimeout = 500;                                                      /* 等待接收超时ms */
        const static int mcPerListSweepValNumbers = 50;                                                 /* 自定义参数个数最大值 */
        const static int mcMaxSweepPoints = 2000;
        const static QString mcClientIdStr;
};

/*********************************** 工具函数 **********************************/

}
}

#endif // _PSS_QT_PLX00_SCPI_CLIENT_H_