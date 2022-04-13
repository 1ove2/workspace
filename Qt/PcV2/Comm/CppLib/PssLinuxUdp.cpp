/******************************************************************************
 *
 * 文件名  ： PssLinuxUdp.cpp
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220126
 * 版本号  ： v1.0 20220126
 * 文件描述： Pss Linux平台Udp 实现
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "PssRstDef.h"
#include "PssCppLog.h"
#include "PssLinuxUdp.h"

#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

/*********************************** 命名空间 **********************************/

/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
PssLinuxUdp::PssLinuxUdp(void) :
    PssIIo(),
    mSock(-1)
{
}

PssLinuxUdp::~PssLinuxUdp(void)
{
    Close();
}

i32 PssLinuxUdp::Open(Mode mode)
{
    mSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(mSock < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "create mSock with socket err:%d", mSock);
        return PssRstDevCreateError;
    }

#if 0
    int opt = 1;
    setsockopt(mSock, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
    setsockopt(mSock, SOL_SOCKET, SO_REUSEPORT, (const void *)&opt, sizeof(opt));
#endif

    PssCLibLogIL(PssCppLog::GetCLibLog(), "Udp server socket:%d Open Done.", mSock);

    return PssRstSuccess;
}

i32 PssLinuxUdp::Close(void)
{
    if(mSock > 0)
    {
        close(mSock);
        mSock = -1;
    }
    return PssRstSuccess;
}

bool PssLinuxUdp::IsOpenned(void) const
{
    if(mSock > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

i32 PssLinuxUdp::Send(const char *buf, i32 size)
{
    i32 sendBytesCount = send(mSock, buf, size, 0);
    if(sendBytesCount < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "Send err:%d, %d", sendBytesCount, errno);
        return PssRstWriteToIOError;
    }

    PrintSendLog(buf, sendBytesCount);
    return sendBytesCount;
}

i32 PssLinuxUdp::Recv(char *buf, i32 size)
{
    i32 recvBytesCount = recv(mSock, buf, size, 0);
    if(recvBytesCount < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "Recv err:%d, %d", recvBytesCount, errno);
        return PssRstReadFromIOError;
    }

    PrintRecvLog(buf, recvBytesCount);
    return recvBytesCount;
}

i32 PssLinuxUdp::FlushOut(void)
{
    return PssRstNotImpl;
}

i32 PssLinuxUdp::ClearIn(void)
{
    return PssRstNotImpl;
}

i32 PssLinuxUdp::Bind(const char *ip, u16 port)
{
    struct sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &localAddr.sin_addr);
    localAddr.sin_port = htons(port);

    int rst = bind(mSock, (const struct sockaddr*)&localAddr, sizeof(localAddr));
    if(rst < 0)
    {
        Close();
        PssCLibLogEL(PssCppLog::GetCLibLog(), "bind socket error:%d,%d, addr: %s:%d", rst, errno,
                inet_ntoa(localAddr.sin_addr), ntohs(localAddr.sin_port));
        return PssRstSocketBindError;
    }

    PssCLibLogIL(PssCppLog::GetCLibLog(), "Udp Bind to    %s:%d.", ip, port);
    return PssRstSuccess;
}

i32 PssLinuxUdp::Connect(const char *ip, u16 port)
{
    if(mSock < 0)
    {
        PssCLibLogIL(PssCppLog::GetCLibLog(), "Udp Connect to %s:%d with Error sock:%d.", ip, port, mSock);
        return PssRstSocketConnectError;
    }

    struct sockaddr_in remoteAddr;
    bzero(&remoteAddr, sizeof(remoteAddr));
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(port);
    remoteAddr.sin_addr.s_addr = inet_addr(ip);

    int rst = connect(mSock, (struct sockaddr *)&remoteAddr, sizeof(remoteAddr));
    if(rst < 0)
    {
        PssCLibLogIL(PssCppLog::GetCLibLog(), "Udp Connect to %s:%d with Error:%d,%d.", ip, port, rst, errno);
        return PssRstSocketConnectError;
    }

    PssCLibLogIL(PssCppLog::GetCLibLog(), "Udp Connect to %s:%d.", ip, port);
    return PssRstSuccess;
}

void PssLinuxUdp::PrintSendLog(const char *buf, i32 bufSize)
{
#if 0
    struct sockaddr localAddr;
    int len = sizeof(localAddr);
    int rst = getsockname(mSock, &localAddr, (socklen_t *)(&len));
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

    PssCLibLogIL(PssCppLog::GetCLibLog(), "%s", printBuf1);
#endif

    /* PssCLibLogIBytes(PssCppLog::GetCLibLog(), (const u8 *)buf, bufSize, "send     :"); */
    return;
}

void PssLinuxUdp::PrintRecvLog(const char *buf, i32 bufSize)
{
#if 0
    struct sockaddr localAddr;
    i32 localAddrLen = sizeof(localAddr);
    int rst = getsockname(mSock, &localAddr, (socklen_t *)(&localAddrLen));
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

    PssCLibLogIL(PssCppLog::GetCLibLog(), "%s", printBuf1);
#endif

    /* PssCLibLogIBytes(PssCppLog::GetCLibLog(), (const u8 *)buf, bufSize, "recv     :"); */

#if 0
    PssCLibLogIBytes(PssCppLog::GetCLibLog(), (const u8 *)buf, bufSize, "Mac Bytes:");

    const u8 *ptr = (const u8 *)buf;
    u32 offset    = 0;
    u32 len       = 6;
    PssCLibLogIBytes(PssCppLog::GetCLibLog(), ptr + offset, len, "dst mac  :");

    offset += len;
    len     = 6;
    PssCLibLogIBytes(PssCppLog::GetCLibLog(), ptr + offset, len, "src mac  :");

    offset += len;
    len     = 2;
    PssCLibLogIBytes(PssCppLog::GetCLibLog(), ptr + offset, len, "mac type :");

    offset += len;
    len     = 4;
    PssCLibLogIBytes(PssCppLog::GetCLibLog(), ptr + offset, len, "IP 1_32  :");

    offset += len;
    len     = 4;
    PssCLibLogIBytes(PssCppLog::GetCLibLog(), ptr + offset, len, "IP 2_32  :");

    offset += len;
    len     = 4;
    PssCLibLogIBytes(PssCppLog::GetCLibLog(), ptr + offset, len, "IP 3_32  :");

    offset += len;
    len     = 4;
    PssCLibLogIBytes(PssCppLog::GetCLibLog(), ptr + offset, len, "src ip   :");

    offset += len;
    len     = 4;
    PssCLibLogIBytes(PssCppLog::GetCLibLog(), ptr + offset, len, "dest ip  :");

    offset += len;
    len     = 2;
    PssCLibLogIBytes(PssCppLog::GetCLibLog(), ptr + offset, len, "src port :");

    offset += len;
    len     = 2;
    PssCLibLogIBytes(PssCppLog::GetCLibLog(), ptr + offset, len, "dst port :");

    offset += len;
    len     = 2;
    PssCLibLogIBytes(PssCppLog::GetCLibLog(), ptr + offset, len, "udp len  :");

    offset += len;
    len     = 2;
    PssCLibLogIBytes(PssCppLog::GetCLibLog(), ptr + offset, len, "udp crc  :");

    offset += len;
    len     = bufSize - offset;
    PssCLibLogIBytes(PssCppLog::GetCLibLog(), ptr + offset, len, "Data     :");
    PssCLibLogI(PssCppLog::GetCLibLog(), "-----------------------------------------------");

#endif

    return;
}

