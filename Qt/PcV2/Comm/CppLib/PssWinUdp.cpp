/******************************************************************************
 *
 * 文件名  ： PssWinUdp.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220128
 * 版本号  ： v1.0 20220128
 * 文件描述： Pss Windows平台Udp 实现
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "PssRstDef.h"
#include "PssCppLog.h"
#include "PssWinUdp.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*********************************** 命名空间 **********************************/

/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
PssWinUdp::PssWinUdp(u16 listenPort) :
    PssIIo(),
    mListenPort(listenPort),
    mSock(-1),
    mIsRemoteAddrValid(false)
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

    mSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(INVALID_SOCKET == mSock)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "socket err.");
        return;
    }

    PssCLibLogIL(PssCppLog::GetCLibLog(), " PssWinUdp Done:%d.", mListenPort);
    return;
}

PssWinUdp::~PssWinUdp(void)
{
    WSACleanup();
    PssCLibLogIL(PssCppLog::GetCLibLog(), "~PssWinTcpClient Done:%u,%d.", mListenPort, mSock);
    return;
}

i32 PssWinUdp::Open(Mode mode)
{
    PSS_UNUSED(mode)

    mSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(INVALID_SOCKET == mSock)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "create mSock with socket err.");
        exit(0);
    }

    struct sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(mListenPort);
    int rst = bind(mSock, (const struct sockaddr*)&localAddr, sizeof(localAddr));
    if(rst < 0)
    {
        Close();
        PssCLibLogEL(PssCppLog::GetCLibLog(), "bind mSock with bind err:%d,%s", rst, strerror(errno));
        exit(0);
    }
    PssCLibLogIL(PssCppLog::GetCLibLog(), "Udp server socket:%d(%s:%d) Open Done.",
            mSock, inet_ntoa(localAddr.sin_addr), ntohs(localAddr.sin_port));

    return PssRstSuccess;
}

i32 PssWinUdp::Close(void)
{
    return PssRstNotImpl;
}

i32 PssWinUdp::Send(const char *buf, i32 size)
{
    if(!mIsRemoteAddrValid)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssLinuxUdp::Send with out client addr.");
        exit(0);
    }

    i32 sendBytesCount = sendto(mSock, buf, size, 0, (struct sockaddr *)&mRemoteAddr, sizeof(mRemoteAddr));
    if(sendBytesCount < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "sendto err:%d", sendBytesCount);
        exit(0);
    }

    PrintSendLog(buf, sendBytesCount);
    return sendBytesCount;
}

i32 PssWinUdp::Recv(char *buf, i32 size)
{
    struct sockaddr_in remoteAddr;
    i32 len = sizeof(remoteAddr);
    i32 recvBytesCount = recvfrom(mSock, buf, size, 0, (struct sockaddr *)&remoteAddr, &len);
    if(recvBytesCount < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "recvfrom err:%d", recvBytesCount);
        exit(0);
    }
    SetRemoteAddr(&remoteAddr);

    PrintRecvLog(buf, recvBytesCount);
    return recvBytesCount;
}

i32 PssWinUdp::FlushOut(void)
{
    return PssRstNotImpl;
}

i32 PssWinUdp::ClearIn(void)
{
    return PssRstNotImpl;
}

void PssWinUdp::SetRemoteAddr(const struct sockaddr_in *remoteAddr)
{
    i32 len = sizeof(struct sockaddr_in);
    memset(&mRemoteAddr, 0, len);
    memmove(&mRemoteAddr, remoteAddr, len);

    mIsRemoteAddrValid = true;
}

void PssWinUdp::PrintSendLog(const char *buf, i32 bufSize)
{
    struct sockaddr localAddr;
    i32 len = sizeof(localAddr);
    int rst = getsockname(mSock, &localAddr, &len);
    if(0 != rst)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "getsockname err:%d", rst);
        return;
    }
    struct sockaddr_in *local  = (struct sockaddr_in *)(&localAddr);
    struct sockaddr_in *remote = (struct sockaddr_in *)(&mRemoteAddr);

    /* inet_ntoa 不可重入,故不可一个PssCLibLogIL输出 */
    char printBuf1[256] = {0};
    char printBuf2[256] = {0};
    char printBuf3[256] = {0};
    snprintf(printBuf1, 256, "send   to: %s:%d => ", inet_ntoa(local->sin_addr), ntohs(local->sin_port));
    snprintf(printBuf2, 256, "%s:%d ", inet_ntoa(remote->sin_addr), ntohs(remote->sin_port));
    snprintf(printBuf3, 256, "%d:%s", bufSize, buf);
    strncat(printBuf1, printBuf2, 256);
    strncat(printBuf1, printBuf3, 256);

    PssCLibLogIL(PssCppLog::GetCLibLog(), "%s", printBuf1);
    return;
}

void PssWinUdp::PrintRecvLog(const char *buf, i32 bufSize)
{
    struct sockaddr localAddr;
    i32 localAddrLen = sizeof(localAddr);
    int rst = getsockname(mSock, &localAddr, &localAddrLen);
    if(0 != rst)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "getsockname err:%d", rst);
        return;
    }
    struct sockaddr_in *remote = (struct sockaddr_in *)(&mRemoteAddr);
    struct sockaddr_in *local  = (struct sockaddr_in *)(&localAddr);

    /* inet_ntoa 不可重入,故不可一个PssCLibLogIL输出 */
    char printBuf1[256] = {0};
    char printBuf2[256] = {0};
    char printBuf3[256] = {0};
    snprintf(printBuf1, 256, "recv from: %s:%d => ", inet_ntoa(remote->sin_addr), ntohs(remote->sin_port));
    snprintf(printBuf2, 256, "%s:%d ", inet_ntoa(local->sin_addr), ntohs(local->sin_port));
    snprintf(printBuf3, 256, "%d:%s", bufSize, buf);
    strncat(printBuf1, printBuf2, 256);
    strncat(printBuf1, printBuf3, 256);

    PssCLibLogIL(PssCppLog::GetCLibLog(), "%s", printBuf1);

    return;
}
