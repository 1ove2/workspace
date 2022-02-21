/******************************************************************************
 *
 * 文件名  ： GPIBIo.h
 * 创建日期： 20200604
 * 版本号  ： v1.0 20200604
 * 文件描述： GPIB 接口
 * 其    他： 基于VISA库封装，适用于安捷伦82357B USB-GPIB接口卡
 *		调用该头文件中接口必须依赖agvisa.lib/agvisa.dll
 *
 * 修改日志： 20200604 初版
 *
  *
  *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _GPIB_IO_H_
#define _GPIB_IO_H_


/************************************ 头文件 ***********************************/
#include "TypeDef.h"
#include "IIo.h"
#ifdef _GPIB_SUPPORT_
#include "visa.h"
#endif
#include <QString>

/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/
                                                    /* 注释定位 */
/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class GPIBIo : public IIo
{
    public:
        GPIBIo(uint8_t instrumentAddr, const QString& name, uint8_t GPIBCardAddr = 0);
    virtual ~GPIBIo(void);

    virtual int Open(void);                                                                     /* 打开 */
    virtual int Close(void);                                                                    /* 关闭 */
    virtual int Send(const char *buf, size_t size, int32_t timeout);                            /* 发送 */
    virtual int Recv(char *buf, size_t size, int32_t timeout);                                  /* 接收 */
    virtual int FlushOut(void);                                                                 /* 刷新发送缓冲 */
    virtual int ClearIn(void);                                                                  /* 清空接收缓冲 */
            bool IsOpen(void);
            bool SetOptTimeout(uint32_t timeout);                                               /* 设置超时 */
    protected:
    
    private:
        bool InitGPIB(uint8_t CardAddr, uint8_t instrAddr, uint32_t timeout = 2000);            /* 初始化GPIB */
#ifdef _PSS_GPIB_SUPPORT_
        ViSession           mDefaultGPIBRM;                                                     /* 默认VISA会话 */
        ViSession           mGPIBHandle;                                                        /* 当前打开的GPIB句柄 */
        ViStatus            mRetStatus;                                                         /* 上次操作返回值 */
#endif
        bool                mIsOpen;                                                            /* GPIB是否打开标志 */
        uint8_t             mGPIBCardAddr;                                                      /* GPIB卡地址 */
        uint8_t             mInstrumentAddr;                                                    /* GPIB目标设备地址 */
        QString             mGPIBIdStr;                                                         /* GPIB标识名 */
        uint32_t            mTimeout;                                                           /* GPIB操作超时时间 */
};

/*********************************** 工具函数 **********************************/



#endif // _GPIB_IO_H_
