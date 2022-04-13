/******************************************************************************
 *
 * 文件名  ： PssWinTcpClient.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220110
 * 版本号  ： v1.0 20220110
 * 文件描述： Pss Windows平台Tcp客户端 头文件
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_WIN_TCP_CLIENT_H_
#define _PSS_WIN_TCP_CLIENT_H_

/************************************ 头文件 ***********************************/
#include "PssIIo.h"

#include <WinSock2.h>

/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class PssWinTcpClient : public PssIIo
{
public:
    explicit PssWinTcpClient(const char *ip, u16 port);                                                 /* 构造 */
    ~PssWinTcpClient(void);                                                                             /* 析构 */

    virtual i32 Open(Mode mode);                                                                        /* 打开 */
    virtual i32 Close(void);                                                                            /* 关闭 */
    virtual bool IsOpenned(void) const;                                                                 /* 是否已打开? */
    virtual i32 Send(const char *buf, i32 size);                                                        /* 发送 */
    virtual i32 Recv(      char *buf, i32 size);                                                        /* 接收 */
    virtual i32 FlushOut(void);                                                                         /* 刷新发送缓冲 */
    virtual i32 ClearIn(void);                                                                          /* 清空接收缓冲 */

protected:

private:
    const char  mIp[16];                                                                                /* IP地址(16"000.000.000.000";) */
    u16         mPort;                                                                                  /* IP端口 */
    SOCKET      mClientSock;                                                                            /* Socket */
    bool        mIsConnectted;                                                                          /* 连接标记 */
};

#endif // _PSS_WIN_TCP_CLIENT_H_
