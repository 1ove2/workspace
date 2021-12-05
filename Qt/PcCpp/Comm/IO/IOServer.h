/******************************************************************************
 *
 * 文件名  ： IOServer.h
 * 负责人  ： ryl
 * 创建日期： 20200610
 * 版本号  ： v1.1 20200610
 * 文件描述： SX00 IOServer线程头文件
 * 其    他： 无
 * 修改日志：
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _IO_SERVER_H_
#define _IO_SERVER_H_

/************************************ 头文件 ***********************************/
#include <QObject>
#include <QThread>
#include <QQueue>
#include <QTimer>
#include "QtTcpServer.h"
/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/
                                                                                            /* 注释定位 */

/*********************************** 结构定义 **********************************/

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class DaemonNetIOServer;
class IOServer : public QThread
{
    Q_OBJECT
    public:
        IOServer(const uint serverPort, const uint clientPort, const QString& serverName, QObject* parent = 0);
        virtual ~IOServer(void);
        bool StartServer(void);
        void StopServer(void);
        void RecvFromClient(QByteArray& data);                                              /* 接收客户端数据 */

    public slots:
        void slotStopThrad(void);                                                           /* 启动线程 */
        void slotStartThread(void);                                                         /* 停止线程 */
        void slotSend2Client(const QByteArray &data);                                       /* 发送数据给客户端：缓存至本地 */
    
    private slots:
        void slotLoopParse(void);                                                           /* 循环解析槽函数 */
    signals:
        void sigRecvFromClient(const QByteArray& data);                                     /* 接收到电源服务进程数据信号 */
        void sigThreadAbort(void);                                                          /* 线程异常退出信号 */
    
    private:
        void Parse(void);                                                                   /* 解析数据 */
        int Send2Client(const QByteArray &data);                                            /* 发送数据给客户端：直接发送 */
        uint                 mServerPort;                                                   /* 服务端端口号 */
        uint                 mClientPort;                                                   /* 电源服务进程客户端端口号 */
        QString              mServerName;                                                   /* 服务端名称 */
        QtTcpServer          *mServerSocket;                                                /* 服务端scoket */
        const static int     mcRecvWaitMs = 0;                                              /* 接收等待超时Ms */
        const static int     mcSendWaitMs = 100;                                            /* 发送等待Ms */
        QQueue<QString>      mCmdQue;                                                       /* 电源进程接收指令缓存队列 */
        QByteArray           mRecvBuf;                                                      /* 接收客户端缓存 */
        QByteArray           mSendBuf;                                                      /* 发送缓存 */
        QTimer*              mLoopParseTimer;                                               /* 循环解析定时器 */
        volatile bool        mLoopParseStoped;                                              /* 循环解析停止标志 */
        const static int     mcLoopParseTimeoutMs = 1;                                      /* 循环解析超时时间(ms)*/

};

/*********************************** 工具函数 **********************************/

/*********************************** 工具变量 **********************************/

#endif // _IO_SERVER_H_
