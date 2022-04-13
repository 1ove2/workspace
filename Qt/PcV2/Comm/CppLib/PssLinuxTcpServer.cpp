/******************************************************************************
 *
 * 文件名  ： PssLinuxTcpServer.cpp
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220125
 * 版本号  ： v1.0 20220125
 * 文件描述： Pss Linux平台Tcp服务端 实现
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "PssRstDef.h"
#include "PssCppLog.h"
#include "PssLinuxTcpServer.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
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
PssLinuxTcpServer::PssLinuxTcpServer(u16 listenPort) :
    PssIIo(),
    mListenPort(listenPort),
    mListenSock(-1),
    mIoSock(-1)
{
}

PssLinuxTcpServer::~PssLinuxTcpServer(void)
{
    Close();
}

i32 PssLinuxTcpServer::Open(Mode mode)
{
    /* step1: 被动打开 */
    mListenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(mListenSock < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "create mListenSock with socket err:%d", mListenSock);
        return PssRstSysCallError;
    }

    /* 设置阻塞模式 */
    if(NonBlock == mode)
    {
        Close();
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssLinuxTcpServer NonBlock Is not Impl.");
        return PssRstNotImpl;
    }

    /* step2: 绑定 */
    struct sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(mListenPort);

    int opt = 1;
    setsockopt(mListenSock, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
    setsockopt(mListenSock, SOL_SOCKET, SO_REUSEPORT, (const void *)&opt, sizeof(opt));
    int rst = bind(mListenSock, (const struct sockaddr*)&localAddr, sizeof(localAddr));
    if(rst < 0)
    {
        Close();
        PssCLibLogEL(PssCppLog::GetCLibLog(), "bind mListenSock with bind err:%d", rst);
        return PssRstSysCallError;
    }

    /* step3: 监听 */
    rst = listen(mListenSock, mcBacklog);
    if(rst < 0)
    {
        Close();
        PssCLibLogEL(PssCppLog::GetCLibLog(), "listen mListenSock with listen err:%d", rst);
        return PssRstSysCallError;
    }

    /* step4: 接收客户端连接(阻塞) */
    /* step4.1: 输出监听地址 */
    PssCLibLogIL(PssCppLog::GetCLibLog(), "Wait Accept by server sock:%d(%s:%d)",
            mListenSock, inet_ntoa(localAddr.sin_addr), ntohs(localAddr.sin_port));

    /* step4.2: 等待accept */
    struct sockaddr clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    mIoSock = accept(mListenSock, &clientAddr, (socklen_t *)(&clientAddrLen));
    if(mIoSock > 0)
    {
        struct sockaddr_in *addr = (struct sockaddr_in*)(&clientAddr);
        PssCLibLogIL(PssCppLog::GetCLibLog(), "Acceptted with        sock:%d(%s,%d)",
                mIoSock, inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
    }
    else
    {
        Close();
        PssCLibLogEL(PssCppLog::GetCLibLog(), "listenSock:%d accept err:%d", mListenSock, mIoSock);
        return PssRstSysCallError;
    }

    return PssRstSuccess;
}

i32 PssLinuxTcpServer::Close(void)
{
    if(mIoSock > 0)
    {
        close(mIoSock);
        mIoSock = -1;
    }

    if(mListenSock > 0)
    {
        close(mListenSock);
        mListenSock = -1;
    }

    return PssRstSuccess;
}

bool PssLinuxTcpServer::IsOpenned(void) const
{
    if(mIoSock > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

i32 PssLinuxTcpServer::Send(const char *buf, i32 size)
{
    if(mIoSock < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssLinuxTcpServer::Send err:%d.", mIoSock);
        return PssRstWriteToIOError;
    }

    int rst = send(mIoSock, buf, size, MSG_NOSIGNAL);
    if(rst > 0)
    {
        return rst;
    }
    else if(EPIPE == errno)
    {
        PssCLibLogIL(PssCppLog::GetCLibLog(), "remote socket:%d closed(when send).", mIoSock);
        return PssRstWriteToIOError;
    }
    else
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssLinuxTcpServer::Send err:%d.", rst);
        return PssRstWriteToIOError;
    }
}

i32 PssLinuxTcpServer::Recv(char *buf, i32 size)
{
    if(mIoSock < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssLinuxTcpServer::Recv err:%d.", mIoSock);
        return PssRstRecvFromIOError;
    }

    int rst = recv(mIoSock, buf, size, 0);
    if(0 == rst)
    {
        PssCLibLogIL(PssCppLog::GetCLibLog(), "remote socket:%d closed(when recv).", mIoSock);
    }
    else if(rst < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssLinuxTcpServer::read err:%d.", rst);
        exit(0);
    }

    /* 收到有效数据 */
    return rst;

}

i32 PssLinuxTcpServer::FlushOut(void)
{
    return PssRstNotImpl;
}

i32 PssLinuxTcpServer::ClearIn(void)
{
    return PssRstNotImpl;
}

