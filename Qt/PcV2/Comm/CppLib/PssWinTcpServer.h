/******************************************************************************
 *
 * 文件名  ： PssWinTcpServer.cpp
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220301
 * 版本号  ： v1.0 20220301
 * 文件描述： Pss Windows平台Tcp服务端 头文件
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_WIN_TCP_SERVER_H_
#define _PSS_WIN_TCP_SERVER_H_

/************************************ 头文件 ***********************************/
#include "PssIIo.h"

#include <WinSock2.h>

/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class PssWinTcpServer : public PssIIo
{
public:
    explicit PssWinTcpServer(u16 listenPort);                                                           /* 构造 */
    ~PssWinTcpServer(void);                                                                             /* 析构 */

    virtual i32 Open(Mode mode);                                                                        /* 打开 */
    virtual i32 Close(void);                                                                            /* 关闭 */
    virtual i32 Send(const char *buf, i32 size);                                                        /* 发送 */
    virtual i32 Recv(      char *buf, i32 size);                                                        /* 接收 */
    virtual i32 FlushOut(void);                                                                         /* 刷新发送缓冲 */
    virtual i32 ClearIn(void);                                                                          /* 清空接收缓冲 */

    bool IsRemoteValid(void);                                                                           /* 远端就绪 */

protected:

private:

private:
    u16    mListenPort;                                                                                 /* 监听端口 */
    SOCKET mListenSock;                                                                                 /* 监听套接字 */
    SOCKET mIoSock;                                                                                     /* 通信端口 */

    const static i32 mcBacklog = 1;                                                                     /* listen队列 长度 */
};

#endif // _PSS_WIN_TCP_SERVER_H_
