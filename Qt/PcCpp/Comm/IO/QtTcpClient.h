/******************************************************************************
 *
 * 文件名  ： QtTcpClient.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20200119
 * 版本号  ： v1.0 20200119
 * 文件描述： PSS Qt版本TCP 客户类,实现IIo接口
 * 其    他： 无
 * 修改日志： 
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

/************************************ 头文件 ***********************************/
#include "QtIo.h"
#include <QTcpSocket>
#include <QHostAddress>

/*********************************** 命名空间 **********************************/


/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class QtTcpClient : public QtIo
{
    Q_OBJECT

    public:
        QtTcpClient(const QHostAddress &serverIp, uint16_t serverPort, const QString &tcpIdStr, QObject *parent = 0);
        ~QtTcpClient(void);
    
        virtual bool IsOpen(void) const;                                                                /* 判断IO是否打开 */
    
        virtual int Open(void);                                                                         /* 打开 */
        virtual int Close(void);                                                                        /* 关闭 */
    
    protected:
        virtual QIODevice *GetQIODevice(void) const;                                                    /* 获取Qt底层Io */
    
    private:
        QHostAddress mServerIp;                                                                         /* 服务端IP */
        uint16_t     mServerPort;                                                                       /* 服务端监听端口 */
    
        QTcpSocket  *mSocket;                                                                           /* 与服务端连接的套接字 */
        bool         mIsConnectted;                                                                     /* 标记当前是否与服务端连接成功 */
    
        static const int mcConnectWaitMs = 5000;                                                        /* 等待连接就绪的延迟(5s) */
};
/*********************************** 工具函数 **********************************/


#endif // _QT_TCP_CLIENT_H_
