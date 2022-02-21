/******************************************************************************
 *
 * 文件名  ： IIOClient.h
 * 负责人  ： ryl
 * 创建日期： 20200720
 * 版本号  ： v1.1 20200720
 * 文件描述： SX00 IOClient线程头文件
 * 其    他： 无
 * 修改日志：
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _IO_CLIENT_H_
#define _IO_CLIENT_H_

/************************************ 头文件 ***********************************/
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include "QtTcpClient.h"
/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/
                                                    /* 注释定位 */

/*********************************** 结构定义 **********************************/

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/

class IOClient : public QThread
{
    Q_OBJECT

    public:
        IOClient(const QHostAddress &serverIp, uint16_t serverPort, const QString& clientName, int localPort = -1, QObject* parent = 0);
        ~IOClient(void);
        int Send2Server(const QByteArray& data);                                                        /* 直接向服务端发送消息 */
        int RecvFromServer(QByteArray &data);                                                           /* 读取IO数据，由信号驱动 */
        bool ConnectToServer(void);                                                                     /* 连接服务进程 */
        bool DisconnectFromServer(void);                                                                /* 断开与服务进程连接 */
        bool IsConnected(void);
    
    public slots:
        void slotStartThread(void);                                                                     /* 线程启动 */
        void slotStopThread(void);                                                                      /* 线程停止 */
        void slotSend2Server(const QByteArray& data);                                                   /* 发送数据给服务端：本地缓存模式 */
    signals:
        void sigStartSuccess(void);                                                                     /* 启动成功 */
        void sigThreadAbort(void);                                                                      /* 启动失败 */
        void sigRecvDataFromServer(const QByteArray& data);                                             /* 接收到进程数据信号 */
    
    private slots:
        void slotLoopParse(void);
    
    private:
        void Parse(void);
        QtTcpClient*        mTcpClient;                                                                 /* TCP客户端，用于进程通信 */
        QString             mClientName;                                                                /* 客户端别名 */
        bool                mIsConnected;                                                               /* 与服务端连接上标志 */
        QHostAddress        mServerIP;                                                                  /* 进程服务端IP */
        int                 mServerPort;                                                                /* 进程服务端端口号 */
        int                 mClientPort;                                                                /* 客户端端口号 */
        volatile bool       mLoopParseStoped;                                                           /* 循环接收停止标志 */
        const static int    mcLoopParseTimeoutMs = 5;                                                   /* 事件循环周期毫秒 */
        QTimer*             mLoopEventTimer;                                                            /* 事件循环定时器 */
        static const int    mcRecvTimeoutMs = 0;                                                        /* 接收超时(ms) */
        static const int    mcSendTimeoutMs = 100;                                                      /* 发送超时 */
        QByteArray          mRecvBuf;                                                                   /* 接收缓存 */
        QByteArray          mSendBuf;                                                                   /* 发送缓存 */
};

/*********************************** 工具函数 **********************************/

/*********************************** 工具变量 **********************************/

#endif // _IO_CLIENT_H_
