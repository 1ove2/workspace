/******************************************************************************
 *
 * 文件名  ： QtTcp.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20200103
 * 版本号  ： v1.0 20200103
 * 文件描述： PSS Qt版本TCP类,实现IIo接口
 * 其    他： 无
 * 修改日志： 
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _QT_TCP_SERVER_H_
#define _QT_TCP_SERVER_H_

/************************************ 头文件 ***********************************/
#include "QtIo.h"
#include <QTcpSocket>
#include <QTcpServer>
#include <QMutex>
/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class QtTcpServer : public QtIo
{
    Q_OBJECT

    public:
        QtTcpServer(uint16_t listenPort, const QString& tcpIdStr, QObject *parent = 0);
        ~QtTcpServer(void);
    
        virtual bool IsOpen(void) const;                                                                /* 判断IO是否打开 */
    
        virtual int  Open(void);                                                                        /* 打开 */
        virtual int  Close(void);                                                                       /* 关闭 */
    
    protected:
        virtual QIODevice *GetQIODevice(void) const;                                                    /* 获取Qt底层Io */
    
    private slots:
        void slotAcceptClientSocket(void);                                                              /* 接受客户端请求连接的套接字 */
    
    private:
        void ClearSocket4Client(void);                                                                  /* 清理 与客户端通信的套接字 */
        uint16_t     mListenPort;                                                                       /* 监听端口 */
        QTcpServer  *mTcpServer;                                                                        /* TCP服务器对象,用于监听 */
        QTcpSocket  *mSocket4Client;                                                                    /* 与客户端通信的套接字 */
};
/*********************************** 工具函数 **********************************/


#endif // _QT_TCP_SERVER_H_
