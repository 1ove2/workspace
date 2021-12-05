/******************************************************************************
 *
 * 文件名  ： ScpiClient.h
 * 创建日期： 20200326
 * 版本号  ： v1.0 20200326
 * 文件描述： PSS 对SCPI指令的封装
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _SCPI_CLIENT_H_
#define _SCPI_CLIENT_H_

/************************************ 头文件 ***********************************/
#include "IIo.h"
#include "ITool.h"
#include <QTimer>
#include <QObject>
#include <QHostAddress>

/*********************************** 命名空间 **********************************/

class IIo;


/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class ScpiClient : public QObject
{
    Q_OBJECT

    public:
        ~ScpiClient(void);
    
        /* *RST */
        int SendReset(void) const;                                                                      /* 发送复位 */
        /* *IDN? */
        int SendIDNReq(QString &idnRstStr, uint32_t timeout = mcWait4RecvTimeout);                      /* 请求IDN */
    
        int SendTrigDir(int dir) const;                                                                 /* 设置trig线方向 */
        int SendTrigInput(bool on);                                                                     /* 设置trig输入开关 */
        int SendClearScpiError(void);                                                                   /* 清除SCPI错误代码缓存 */
        int SendGetScpiError(int& errCode, uint32_t timeout);                                           /* 获取最早一次SCPI操作错误代码 */
        void ClearIoRecv(void);                                                                         /* 清除Io接收缓存 */
        QString GetReadValStr(uint32_t timeout = mcWait4RecvTimeout, bool check = true);                /* 接收数据，check结束符返回 */
        int SendRawScpi(const QString& scpi, uint32_t timeout = mcWait4RecvTimeout);                    /* 发送自定义指令 */
    
    protected:
        ScpiClient(IIo *iio, const QString &idStr);
        int Send(const QString &str, uint32_t delayMs = 10) const;                                       /* 以QString格式发送 */
        int Recv(QString &str, uint32_t delayMs = mcWait4RecvTimeout, bool check = true) const;         /* 以QString格式接收 */
    
        const QString ToScpiName(bool isV) const;                                                       /* 获取SCPI参数名 */
    
        IIo     *mIIo;                                                                                  /* 底层IO */
        QString  mIdStr;                                                                                /* 用于调试 */
    
    private:
        const static int        mcWait4RecvTimeout = 1000;                                              /* 接收等待超时ms */
};

/*********************************** 工具函数 **********************************/



#endif //_SCPI_CLIENT_H_
