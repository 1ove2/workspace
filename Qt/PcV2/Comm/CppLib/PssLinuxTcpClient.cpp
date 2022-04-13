/******************************************************************************
 *
 * 文件名  ： PssLinuxTcpClient.cpp
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220111
 * 版本号  ： v1.0 20220111
 * 文件描述： Pss Linux平台Tcp客户端 实现
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "PssRstDef.h"
#include "PssLinuxTcpClient.h"

#include <stdio.h>
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
PssLinuxTcpClient::PssLinuxTcpClient(const char *ip, u16 port) :
    PssIIo(),
    mIp(ip),
    mPort(port),
    mClientSock(0),
    mIsConnectted(false)
{
    mClientSock = socket(PF_INET, SOCK_STREAM, 0);
    if(mClientSock < 0)
    {
        fprintf(stderr, "socket err:%d\n", mClientSock);
        return;
    }

    return;
}

PssLinuxTcpClient::~PssLinuxTcpClient(void)
{
    Close();
    return;
}

i32 PssLinuxTcpClient::Open(Mode mode)
{
    PSS_UNUSED(mode)
    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port   = htons(mPort);
    serverAddr.sin_addr.s_addr = inet_addr(mIp);

    int rst = connect(mClientSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if(rst < 0)
    {
        fprintf(stderr, "connect err:%s:%u:%d.\n", mIp, mPort, rst);
        return PssRstSocketConnectError;
    }
    mIsConnectted = true;

    return PssRstSuccess;
}

i32 PssLinuxTcpClient::Close(void)
{
    if(mClientSock > 0)
    {
        close(mClientSock);
        mClientSock = -1;
    }

    return PssRstSuccess;
}

bool PssLinuxTcpClient::IsOpenned(void) const
{
    return mIsConnectted;
}

i32 PssLinuxTcpClient::Send(const char *buf, i32 size)
{
    int rst = send(mClientSock, buf, size, 0);
    if(rst > 0)
    {
        return rst;
    }
    else
    {
        fprintf(stderr, "Send err:%d.\n", rst);
        return PssRstWriteToIOError;
    }
}

i32 PssLinuxTcpClient::Recv(char *buf, i32 size)
{
    int rst = recv(mClientSock, buf, size, 0);
    if(rst > 0)
    {
        return rst;
    }
    else
    {
        fprintf(stderr, "Recv err:%d.\n", rst);
        return PssRstReadFromIOError;
    }
}

i32 PssLinuxTcpClient::FlushOut(void)
{
    return PssRstNotImpl;
}

i32 PssLinuxTcpClient::ClearIn(void)
{
    return PssRstNotImpl;
}

