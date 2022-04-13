/******************************************************************************
 *
 * 文件名  ： PssWinUdp.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220128
 * 版本号  ： v1.0 20220128
 * 文件描述： Pss Windows平台Udp 头文件
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_WIN_UDP_H_
#define _PSS_WIN_UDP_H_

/************************************ 头文件 ***********************************/
#include "PssIIo.h"

#include <WinSock2.h>

/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class PssWinUdp : public PssIIo
{
public:
    explicit PssWinUdp(u16 listenPort);                                                                 /* 构造 */
    ~PssWinUdp(void);                                                                                   /* 析构 */

    virtual i32 Open(Mode mode);                                                                        /* 打开 */
    virtual i32 Close(void);                                                                            /* 关闭 */
    virtual i32 Send(const char *buf, i32 size);                                                        /* 发送 */
    virtual i32 Recv(      char *buf, i32 size);                                                        /* 接收 */
    virtual i32 FlushOut(void);                                                                         /* 刷新发送缓冲 */
    virtual i32 ClearIn(void);                                                                          /* 清空接收缓冲 */

    void SetRemoteAddr(const struct sockaddr_in *remoteAddr);                                           /* 设置远端地址 */

private:
    void PrintSendLog(const char *buf, i32 bufSize);                                                    /* 发送调试打印 */
    void PrintRecvLog(const char *buf, i32 bufSize);                                                    /* 接收调试打印 */
private:
    u16                mListenPort;                                                                     /* 监听端口 */
    SOCKET             mSock;                                                                           /* 服务端 套接字 */

    bool               mIsRemoteAddrValid;                                                              /* 远端地址有效? */
    struct sockaddr_in mRemoteAddr;                                                                     /* 远端地址 */
};
/*********************************** 工具函数 **********************************/

#endif // _PSS_WIN_UDP_H_

