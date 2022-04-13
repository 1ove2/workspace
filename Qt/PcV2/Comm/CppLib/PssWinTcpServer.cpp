/******************************************************************************
 *
 * 文件名  ： PssWinTcpServer.cpp
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220301
 * 版本号  ： v1.0 20220301
 * 文件描述： Pss Windows平台Tcp服务端 实现
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "PssRstDef.h"
#include "PssCppLog.h"
#include "PssWinTcpServer.h"

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
PssWinTcpServer::PssWinTcpServer(u16 listenPort) :
    PssIIo(),
    mListenPort(listenPort),
    mListenSock(INVALID_SOCKET),
    mIoSock(INVALID_SOCKET)
{
    WORD wVersion;
    WSADATA wsaData;

    wVersion = MAKEWORD(2, 2);
    int rst = WSAStartup(wVersion, &wsaData);
    if(0 != rst)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "WSAStartup err:%d", rst);
        return;
    }

    PssCLibLogIL(PssCppLog::GetCLibLog(), " PssWinTcpServer Init   Done:%u.", mListenPort);
    return;
}

PssWinTcpServer::~PssWinTcpServer(void)
{
    WSACleanup();
    PssCLibLogIL(PssCppLog::GetCLibLog(), "~PssWinTcpServer UnInit Done:%u.", mListenPort);
    return;
}

i32 PssWinTcpServer::Open(Mode mode)
{
    int rst = 0;
    mListenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(INVALID_SOCKET == mListenSock)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "socket err.");
        return PssRstSysCallError;
    }

    if(mode == Mode::NonBlock)
    {
        unsigned long ul = 1; /* 非零则非阻塞 */
        rst = ioctlsocket(mListenSock, FIONBIO, (unsigned long *)&ul);
        if(SOCKET_ERROR == rst)
        {
            PssCLibLogEL(PssCppLog::GetCLibLog(), "ioctlsocket err:%u.", mListenPort);
            return PssRstSocketConnectError;
        }
    }

    /* 绑定 */
    struct sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(mListenPort);
    rst = bind(mListenSock, (const struct sockaddr*)&localAddr, sizeof(localAddr));
    if(rst < 0)
    {
        Close();
        PssCLibLogEL(PssCppLog::GetCLibLog(), "bind mListenSock with bind err:%d", rst);
        exit(0);
    }

    /* 监听 */
    rst = listen(mListenSock, mcBacklog);
    if(rst < 0)
    {
        Close();
        PssCLibLogEL(PssCppLog::GetCLibLog(), "listen mListenSock with listen err:%d", rst);
        exit(0);
    }

    PssCLibLogIL(PssCppLog::GetCLibLog(), "PssWinTcpServer::Open  success:%u,%d,%d.",
                 mListenPort, mListenSock, mIoSock);
    return PssRstSuccess;
}

i32 PssWinTcpServer::Close(void)
{
    PssCLibLogIL(PssCppLog::GetCLibLog(), "PssWinTcpServer::Close success:%u,%d,%d.",
                 mListenPort, mListenSock, mIoSock);
    if(INVALID_SOCKET != mIoSock)
    {
        shutdown(mListenSock, SD_BOTH);
        closesocket(mIoSock);
        mIoSock = INVALID_SOCKET;
    }

    shutdown(mListenSock, SD_BOTH);
    closesocket(mListenSock);
    mListenSock = INVALID_SOCKET;
    return PssRstNotImpl;
}

i32 PssWinTcpServer::Send(const char *buf, i32 size)
{
    int rst = send(mIoSock, buf, size, 0);
    if(rst > 0)
    {
        return rst;
    }
    else
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssWinTcpServer::Send err:%d.", rst);
        return PssRstWriteToIOError;
    }
}

i32 PssWinTcpServer::Recv(char *buf, i32 size)
{
    int rst = recv(mIoSock, buf, size, 0);
    if(rst > 0)
    {
        return rst;
    }
    else if(0 == rst)
    {
        return PssRstReadEOF;
    }
    else
    {
        int err = WSAGetLastError();
        if(WSAEWOULDBLOCK == err)
        {
            return PssRstReadEAGAIN;
        }
        PssCLibLogEL(PssCppLog::GetCLibLog(), "pp PssWinTcpServer::Recv err:%d,%d.", rst, err);
        return PssRstRecvFromIOError;
    }
}

i32 PssWinTcpServer::FlushOut(void)
{
    return PssRstNotImpl;
}

i32 PssWinTcpServer::ClearIn(void)
{
    return PssRstNotImpl;
}

bool PssWinTcpServer::IsRemoteValid(void)
{
    if(INVALID_SOCKET != mIoSock)
    {
        return true;
    }

    /* 等待accept */
    struct sockaddr clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    mIoSock = accept(mListenSock, &clientAddr, (int *)(&clientAddrLen));
    if(INVALID_SOCKET == mIoSock)
    {
        /* PssCLibLogEL(PssCppLog::GetCLibLog(), "listenSock:%d accept err:%d", mListenSock, mIoSock); */
        return false;
    }
    else
    {
        struct sockaddr_in *addr = (struct sockaddr_in*)(&clientAddr);
        PssCLibLogIL(PssCppLog::GetCLibLog(), "Acceptted with        sock:%d(%s,%d)",
                mIoSock, inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
        return true;
    }
}
