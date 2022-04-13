/******************************************************************************
 *
 * 文件名  ： PssWinTcpClient.cpp
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220110
 * 版本号  ： v1.0 20220110
 * 文件描述： Pss Windows平台Tcp客户端 实现
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "PssRstDef.h"
#include "PssCppLog.h"
#include "PssWinTcpClient.h"

#include <stdio.h>

/* pro文件中写入链接依赖 */
/* #pragma comment(lib, "ws2_32.lib") */

/*********************************** 命名空间 **********************************/

/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
PssWinTcpClient::PssWinTcpClient(const char *ip, u16 port) :
    mIp(),
    mPort(port),
    mClientSock(INVALID_SOCKET),
    mIsConnectted(false)
{
    memmove((char *)mIp, ip, strlen(ip));

    WORD wVersion;
    WSADATA wsaData;

    wVersion = MAKEWORD(2, 2);
    int rst = WSAStartup(wVersion, &wsaData);
    if(0 != rst)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "WSAStartup err:%d", rst);
        return;
    }

    PssCLibLogIL(PssCppLog::GetCLibLog(), " PssWinTcpClient Done:%s:%u.", mIp, mPort);
    return;
}

PssWinTcpClient::~PssWinTcpClient(void)
{
    Close();
    WSACleanup();
    PssCLibLogIL(PssCppLog::GetCLibLog(), "~PssWinTcpClient Done:%s:%u.", mIp, mPort);
    return;
}

i32 PssWinTcpClient::Open(Mode mode)
{
    int rst = 0;
    unsigned long ul = 0;

    mClientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(INVALID_SOCKET == mClientSock)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "socket err.");
        return PssRstSysCallError;
    }

    if(mode == Mode::NonBlock)
    {
        /* 设置超时 */
        ul = 1; /* 非零则非阻塞 */
        rst = ioctlsocket(mClientSock, FIONBIO, (unsigned long *)&ul);
        if(SOCKET_ERROR == rst)
        {
            PssCLibLogEL(PssCppLog::GetCLibLog(), "ioctlsocket err:%s:%u.", mIp, mPort);
            return PssRstSysCallError;
        }
    }

    /* 连接 */
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(struct sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port   = htons(mPort);
    serverAddr.sin_addr.s_addr = inet_addr(mIp);
    rst = connect(mClientSock, (SOCKADDR *)&serverAddr, sizeof(struct sockaddr_in));
    if(mode == Mode::Block)
    {
        if(-1 == rst)
        {
            PssCLibLogIL(PssCppLog::GetCLibLog(), "connect rst:%d.", rst);
            return PssRstSocketConnectError;
        }
    }

    /* select模型,即设置超时 */
    if(mode == Mode::NonBlock)
    {
        fd_set r;
        FD_ZERO(&r);
        FD_SET(mClientSock, &r);
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 15 * 1000; /* 超时 15ms */
        rst = select(0, 0, &r, 0, &timeout);
        if(rst <= 0)
        {
            PssCLibLogEL(PssCppLog::GetCLibLog(), "select err:%s:%u,%d.", mIp, mPort, rst);
            return PssRstSocketConnectError;
        }

#if 0
        /* 设置回到阻塞 */
        ul = 0;
        rst = ioctlsocket(mClientSock, FIONBIO, (unsigned long *)&ul);
        if(SOCKET_ERROR == rst)
        {
            PssCLibLogEL(PssCppLog::GetCLibLog(), "ioctlsocket err:%s:%u.", mIp, mPort);
            return PssRstSysCallError;
        }
#endif
    }

#if 0
    /* 发送超时 10ms */
    i32 to = 10;
    rst = setsockopt(mClientSock, SOL_SOCKET, SO_SNDTIMEO, (char *)&to, sizeof(to));
    if(SOCKET_ERROR == rst)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "ioctlsocket err:%s:%u.", mIp, mPort);
        return PssRstSysCallError;
    }

    /* 接受超时 100ms */
    to = 10;
    rst = setsockopt(mClientSock, SOL_SOCKET, SO_RCVTIMEO, (char *)&to, sizeof(to));
    if(SOCKET_ERROR == rst)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "ioctlsocket err:%s:%u.", mIp, mPort);
        return PssRstSysCallError;
    }
#endif
    mIsConnectted = true;
    PssCLibLogIL(PssCppLog::GetCLibLog(), "Open   success:%s:%u,%d.", mIp, mPort, mClientSock);

    return PssRstSuccess;
}

i32 PssWinTcpClient::Close(void)
{
    if(INVALID_SOCKET != mClientSock)
    {
        PssCLibLogIL(PssCppLog::GetCLibLog(), "PssWinTcpClient::Close success:%s:%u,%d.", mIp, mPort, mClientSock);
        shutdown(mClientSock, SD_BOTH);
        closesocket(mClientSock);
        mClientSock = INVALID_SOCKET;
    }
    return PssRstSuccess;
}

bool PssWinTcpClient::IsOpenned(void) const
{
    return mIsConnectted;
}

i32 PssWinTcpClient::Send(const char *buf, i32 size)
{
    int rst = send(mClientSock, buf, size, 0);
    if(rst > 0)
    {
        return rst;
    }
    else
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssWinTcpClient::Send err:%d.", rst);
        return PssRstWriteToIOError;
    }
}

i32 PssWinTcpClient::Recv(char *buf, i32 size)
{
    int rst = recv(mClientSock, buf, size, 0);
    if(rst > 0)
    {
        return rst;
    }
    else
    {
        int err = WSAGetLastError();
        if(WSAEWOULDBLOCK == err) /* 无有效数据 */
        {
            return 0;
        }
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssWinTcpClient::Recv err:%d,%d,%d.", rst, err, mClientSock);
        return PssRstReadFromIOError;
    }
}

i32 PssWinTcpClient::FlushOut(void)
{
    return PssRstNotImpl;
}

i32 PssWinTcpClient::ClearIn(void)
{
    return PssRstNotImpl;
}

