/******************************************************************************
 *
 * 文件名  ： StandardScpiClient.h
 * 负责人  ： ryl
 * 创建日期： 20210201
 * 版本号  ： v1.0 20210201
 * 文件描述： PSS 对 Sx00产品 SCPI指令的封装
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_QT_STANDARD_SCPI_CLIENT_H_
#define _PSS_QT_STANDARD_SCPI_CLIENT_H_

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

class StandardScpiClient : public ScpiClient
{
    public:
        StandardScpiClient(IIo *iio, const QString &idStr);
        StandardScpiClient(const QHostAddress& ip, const QString& idStr);
        StandardScpiClient(const QString& commName, uint32_t commBaud, const QString& idStr);
#ifdef _PSS_GPIB_SUPPORT_
        StandardScpiClient(uint8_t cardAddr, uint8_t instrumentAddr, const QString& idStr);
#endif
        ~StandardScpiClient(void);

        int SendRead(QString& readRstStr, uint32_t timeout = mcWait4RecvTimeout);                       /* 发送read指令 */
        int SendOutputCtrl(bool on) const;                                                              /* 控制输出 */
    
        int SendVSrc(bool isVSrc) const;                                                                /* 源设置 */
        int SendSrcRange(bool isVSrc, double val) const;                                                /* 设置源量程 */
        int SendSrcVal(bool isVSrc, double val) const;                                                  /* 设置源值 */
        int SendLmtRange(bool isVSrc, double val) const;                                                /* 设置限量程 */
        int SendLmtVal(bool isVSrc, double val) const;                                                  /* 设置限值 */
        int SendNplc(bool isVSens, double nplc) const;                                                  /* 设置NPLC */
        int SendSensFunc(bool isVSrc) const;                                                            /* 限设置 */
    private:
        const QString ToScpiSrcName(bool isVSrc) const;                                                 /* 获取SCPI源名 */
        const QString ToScpiLmtName(bool isVSrc) const;                                                 /* 获取SCPI限名 */
    
        bool             mDeleteIo;
        const static int mcWait4RecvTimeout = 500;                                                      /* 等待接收超时ms */
        const static int mcPerListSweepValNumbers = 50;                                                 /* 自定义参数个数最大值 */
        const static int mcMaxSweepPoints = 2000;
        const static QString mcClientIdStr;

};

/*********************************** 工具函数 **********************************/

}
}

#endif // _PSS_QT_STANDARD_SCPI_CLIENT_H_