/******************************************************************************
 *
 * 文件名  ： StandardInstrument.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20200422
 * 版本号  ： v1.0 20200422
 * 文件描述： Sx00整机代理类
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_QT_STANDARD_INSTRUMENT_H_
#define _PSS_QT_STANDARD_INSTRUMENT_H_

/************************************ 头文件 ***********************************/
#include "QtIInstrument.h"
#include "StandardScpiClient.h"
/*********************************** 命名空间 **********************************/
namespace Pss {
namespace Qt {

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class StandardInstrument : public QtIInstrument
{
    Q_OBJECT

    public:
        StandardInstrument(const QHostAddress &ip, const QString& instName = "2450", bool hasSrc = true, QObject *parent = 0);
        StandardInstrument(const QString comName, uint32_t comBaud, const QString& instName = "2450", bool hasSrc = true, QObject *parent = 0);
#ifdef _PSS_GPIB_SUPPORT_
        StandardInstrument(uint8_t cardAddr, uint8_t instrumentAddr, const QString& instName = "2450", bool hasSrc = true, QObject *parent = 0);
#endif
        ~StandardInstrument(void);

        /* 仪表 功能接口 */
        virtual int GetMeasureVal(PssInstrumentMeasureType type, double& val, uint32_t timeout = mcWait4RecvTimeout);/* 获取测量值 */
        int SetOutput(bool on, bool isRead = true);                                             /* 输出控制 开关 */
    
        void SetSourSameSens(const bool &isSrc);
    private:
        virtual ScpiClient *GetScpiClient(void) const override;                                                 /* 获取Scpi客户端 */
        StandardScpiClient *mScpiClient;                                                                    /* SCPI客户端 */
    
        virtual void   InitSourceInfo(void);                                                            /* 初始化设备源信息 */
        virtual int UpdateCfgToInstrument(void) override;                                               /* 配置信息 设置 到仪表 */
    
        const static int mcWait4RecvTimeout = 1000;                                                      /* 复位需要的ms延迟 */
        bool    mIsSourSameSens;

};
/*********************************** 工具函数 **********************************/

}
}

#endif // _PSS_QT_STANDARD_INSTRUMENT_H_