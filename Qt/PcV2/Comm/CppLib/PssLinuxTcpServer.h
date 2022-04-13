/******************************************************************************
 *
 * 文件名  ： PssLinuxTcpServer.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220125
 * 版本号  ： v1.0 20220125
 * 文件描述： Pss Linux平台Tcp服务端 头文件
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_LINUX_TCP_SERVER_H_
#define _PSS_LINUX_TCP_SERVER_H_

/************************************ 头文件 ***********************************/
#include "PssIIo.h"

/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class PssLinuxTcpServer : public PssIIo
{
public:
    explicit PssLinuxTcpServer(u16 listenPort);                                                         /* 构造 */
    ~PssLinuxTcpServer(void);                                                                           /* 析构 */

    virtual i32 Open(Mode mode);                                                                        /* 打开 */
    virtual i32 Close(void);                                                                            /* 关闭 */
    virtual bool IsOpenned(void) const;                                                                 /* 是否已打开? */
    virtual i32 Send(const char *buf, i32 size);                                                        /* 发送 */
    virtual i32 Recv(      char *buf, i32 size);                                                        /* 接收 */
    virtual i32 FlushOut(void);                                                                         /* 刷新发送缓冲 */
    virtual i32 ClearIn(void);                                                                          /* 清空接收缓冲 */

private:
    u16                mListenPort;                                                                     /* 监听端口 */
    i32                mListenSock;                                                                     /* 监听套接字 */
    i32                mIoSock;                                                                         /* 读写套接字 */

    const static i32 mcBacklog = 1;                                                                     /* listen队列 长度 */
};
/*********************************** 工具函数 **********************************/

#endif // _PSS_LINUX_TCP_SERVER_H_

