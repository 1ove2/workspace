/******************************************************************************
 *
 * 文件名  ： PssLinuxUdp.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220125
 * 版本号  ： v1.0 20220125
 * 文件描述： Pss Linux平台Udp 头文件
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_LINUX_UDP_H_
#define _PSS_LINUX_UDP_H_

/************************************ 头文件 ***********************************/
#include "PssIIo.h"

#include <sys/socket.h>
#include <arpa/inet.h>

/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class PssLinuxUdp : public PssIIo
{
public:
    explicit PssLinuxUdp(void);                                                                         /* 构造 */
    ~PssLinuxUdp(void);                                                                                 /* 析构 */

    virtual i32 Open(Mode mode);                                                                        /* 打开 */
    virtual i32 Close(void);                                                                            /* 关闭 */
    virtual bool IsOpenned(void) const;                                                                 /* 是否已打开? */
    virtual i32 Send(const char *buf, i32 size);                                                        /* 发送 */
    virtual i32 Recv(      char *buf, i32 size);                                                        /* 接收 */
    virtual i32 FlushOut(void);                                                                         /* 刷新发送缓冲 */
    virtual i32 ClearIn(void);                                                                          /* 清空接收缓冲 */

    i32 Bind(const char *ip, u16 port);                                                                 /* 绑定 */
    i32 Connect(const char *ip, u16 port);                                                              /* 连接 */

protected:
    void PrintSendLog(const char *buf, i32 bufSize);                                                    /* 发送调试打印 */
    void PrintRecvLog(const char *buf, i32 bufSize);                                                    /* 接收调试打印 */

private:
    void ShowProcess(u64 totalByteCount, bool isSender);                                                /* 字节速率 */

private:
    const char        *mListenIp;                                                                       /* 本机监听口IP */
    u16                mListenPort;                                                                     /* 监听端口 */
    i32                mSock;                                                                           /* 服务端 套接字 */
};
/*********************************** 工具函数 **********************************/

#endif // _PSS_LINUX_UDP_H_

