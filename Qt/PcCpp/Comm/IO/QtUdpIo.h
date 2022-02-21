/******************************************************************************
 *
 * 文件名  ： QtUdpClient.h
 * 负责人  ： ryl
 * 创建日期： 20200119
 * 版本号  ： v1.0 20200119
 * 文件描述： PSS Qt版本UDP 客户类,实现IIo接口
 * 其    他： 无
 * 修改日志：
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _QT_UDP_IO_H_
#define _QT_UDP_IO_H_

/************************************ 头文件 ***********************************/
#include "QtIo.h"
#include <QUdpSocket>
#include <QHostAddress>

/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/

class QtUdpIo : public QtIo
{
    Q_OBJECT
    public:

        QtUdpIo(const QHostAddress &serverIp, uint16_t serverPort, const QHostAddress & localIp, uint16_t localPort, const QString &udpIdStr,  QObject *parent = 0);
        ~QtUdpIo(void);
    
        virtual bool IsOpen(void) const;                                                                /* 判断IO是否打开 */
    
        virtual int Open(void);                                                                         /* 打开 */
        virtual int Close(void);                                                                        /* 关闭 */
        //virtual int Send(const char *buf, size_t size, int32_t timeout);                                /* 发送 */
        //virtual int Recv(char *buf, size_t size, int32_t timeout);                                      /* 接收 */
    
    protected:
        virtual QIODevice *GetQIODevice(void) const;                                                    /* 获取Qt底层Io */
    
    private:
        QHostAddress mServerIp;                                                                         /* 服务端IP */
        uint16_t     mServerPort;                                                                       /* 服务端监听端口 */
        QHostAddress mLocalIp;                                                                          /* 本地IP */
        uint16_t     mLocalPort;                                                                        /* 本地端口 */
        QUdpSocket  *mSocket;                                                                           /* 与服务端连接的套接字 */
        bool         mIsConnectted;                                                                     /* 标记当前是否与服务端连接成功 */
    
        static const int mcConnectWaitMs = 5000;                                                        /* 等待连接就绪的延迟(5s) */

};
/*********************************** 工具函数 **********************************/

#endif // _QT_UDP_IO_H_
