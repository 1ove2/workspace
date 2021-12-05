#include "RstDef.h"
#include "IIoTester.h"
#include "ITool.h"

#include <string>
#include <stdlib.h>
#include <QDebug>
#include <QTime>


IIoTester::IIoTester(IIo *iio, int bytesPerMs, bool isSendWithNoise) :
    mExpectRecvChar(0),
    mRecvBufValidSize(0),
    mIIo(iio),
    mBytesPerMs(bytesPerMs)
{
    /* 构造发送数据(0x00 => 0xFF 循环) */
    for(int i = 0; i < mcBufSize; i++)
    {
        mSendBuf[i] = (char)i;
    }
    mExpectRecvChar = (char)0;

    /* 制造错误,检查 检查算法的有效性 */
    if(isSendWithNoise)
    {
        mSendBuf[4096] = 0x55;
    }
}

IIoTester::~IIoTester(void)
{
}

int IIoTester::Start(Mode mode)
{
    int rst = 0;
    switch (mode)
    {
        case ModeSendUntilErr:
            rst = SendUntilErr();
            break;

        case ModeRecvAndCheck:
            rst = RecvAndCheck();
            break;
    
        case ModeEchoClient:
            rst = StartEchoClient();
            break;
    
        case ModeEchoServer:
            rst = StartEchoServer();
            break;
    
        default:
            return RstParameterError;
    }
    
    return rst;
}

int IIoTester::SendUntilErr(void)
{
    int rst = SendLoop(NULL);
    return rst;
}

int IIoTester::SendAllBufData(RecvAndCheckATimeFuncPtr funcPtr)
{
    size_t sendedLen = 0;
    int randLen = 0;
    char *goPtr = mSendBuf;
    while(sendedLen < mcBufSize)
    {
        /* 单次发送 256 以内的随机长度 */
        randLen = rand() % mcPkgMaxSize + 1;
        sendedLen += randLen;
        if(sendedLen > mcBufSize) /* 避免发送越界 */
        {
            /* randLen保存 本次发送的长度, sendedLen记录总共发送的长度 */
            sendedLen -= randLen;
            randLen = mcBufSize - sendedLen;
            sendedLen += randLen;
        }
        int rst = Send(goPtr, randLen);
        if(rst < 0)
        {
            return rst;
        }
        goPtr += randLen;

        /* 收 & 查 */
        if(funcPtr)
        {
            int result = (this->*funcPtr)();
            if(result < 0)
            {
                return result;
            }
        }
    }
    
    return sendedLen;
}

int IIoTester::RecvAndCheckATime(void)
{
    char buf[mcRecvATime] = {0};

    /* 收 */
    int recvBytesNums = Recv(buf, mcRecvATime);
    if(recvBytesNums <= 0)
    {
        return recvBytesNums;
    }
    
    /* 检查 */
    int rst = Check(buf, recvBytesNums);
    if(rst < 0)
    {
        return rst;
    }
    
    return recvBytesNums;
}

int IIoTester::RecvAndCheck(void)
{
    while(1)
    {
        int recvOkBytesNums = RecvAndCheckATime();
        if(0 == recvOkBytesNums)
        {
            continue;
        }
        if(recvOkBytesNums < 0)
        {
            return recvOkBytesNums;
        }

        /* 调试 数据测试进度 */
        ShowProcess(mRecvBufValidSize, mcShowBytesInterval, "K", "Recved");
    }
}

int IIoTester::StartEchoServer(void)
{
    char buf[mcRecvATime] = {0};
    while(1)
    {
        /* 收 */
        int recvBytesNums = Recv(buf, mcRecvATime);
        if(0 == recvBytesNums)
        {
            continue;
        }
        if(recvBytesNums < 0)
        {
            return recvBytesNums;
        }

        /* 检查 */
        int rst = Check(buf, recvBytesNums);
        if(rst < 0)
        {
            return rst;
        }
    
        /* 发 */
        rst = Send(buf, recvBytesNums);
        if(rst < 0)
        {
            return rst;
        }
    
        /* 调试 数据测试进度 */

        ShowProcess(mRecvBufValidSize, mcShowBytesInterval, "K", "Recved");
    }
}

int IIoTester::SendLoop(RecvAndCheckATimeFuncPtr funcPtr)
{
    int bytesNumsATime = 0;
    long long totalBytesNums = 0;
    do {
        totalBytesNums += bytesNumsATime;

        /* 有收则打印收的情况,否则打印发的情况 */
        if(funcPtr)
        {
            ShowProcess(mRecvBufValidSize, mcShowBytesInterval, "K", "Recved");
        }
        else
        {
            ShowProcess(totalBytesNums, mcShowBytesInterval, "K", "Sended");
        }
    
        bytesNumsATime = SendAllBufData(funcPtr);
    } while(bytesNumsATime > 0);


    qDebug() << "Total Send Bytes:" << totalBytesNums << bytesNumsATime;


    return bytesNumsATime;
}


int IIoTester::StartEchoClient(void)
{
    int rst = SendLoop(&IIoTester::RecvAndCheckATime);
    return rst;
}

void IIoTester::ShowProcess(long long totalBytesNums, int showInterval, const char *unitStr, const char *dirStr)
{
    static int lastMsTime = 0;
    static long long lastTotalBytesNums = 0;
    static int lastCount = 0;

    long long nowTotalBytesNums = totalBytesNums;
    int nowCount = nowTotalBytesNums / showInterval;
    int nowMsTime = GetNowMs();
    
    /* 调试 数据测试进度 */
    if(nowCount > lastCount)
    {
        /* 计算速度 */
        double speed = 1000.0 * (nowTotalBytesNums - lastTotalBytesNums) / (nowMsTime - lastMsTime);
    
        qDebug().noquote().nospace() << QString(":%1 %2Bytes(%3 %4Bytes/s) %5.")
            .arg(nowTotalBytesNums / showInterval, 5)
            .arg(QString(unitStr))
            .arg(speed / showInterval, -7)
            .arg(QString(unitStr))
            .arg(QString(dirStr));
    
        lastMsTime = nowMsTime;
        lastTotalBytesNums = nowTotalBytesNums;
        lastCount = nowCount;
    }
}

int IIoTester::Send(const char *buf, int size)
{
    int32_t timeout = 1 + 1.0 * size / mBytesPerMs;
    int writeBytesNums = mIIo->Send(buf, size, timeout);
    if(RstOptTimeoutError == writeBytesNums)
    {
        int rst = mIIo->FlushOut();
        if(RstSuccess != rst)
        {

        qDebug() << "IIo FlushOut Err:" << rst;

        }
    }

    return size;
}

int IIoTester::Recv(char *buf, int size)
{
    int randRecvTimeout = rand() % mcRecvTimeoutMax;
    /* step1: 接收 */
    int recvBytesNums = mIIo->Recv(buf, size, randRecvTimeout);
    if(RstOptTimeoutError == recvBytesNums)
    {
        return 0;
    }
    if(recvBytesNums < 0)
    {

        qDebug() << "IIo Recv Err:" << recvBytesNums;

    }

    return recvBytesNums;
}

int IIoTester::Check(const char *buf, int size)
{
    /* 接收到有效数据 */
    /* step2: 检查 */
    for(int i = 0; i < size; i++)
    {
        char val = (char)buf[i];
        if(mExpectRecvChar != val)
        {

            QString expectRecvCharStr;
            QString recvChar;
            expectRecvCharStr.sprintf("0x%02X", mExpectRecvChar);
            recvChar.sprintf("0x%02X", val);
            qDebug().noquote() << mRecvBufValidSize
                << "Check Failled: expect" << expectRecvCharStr
                << "recv " << recvChar;

            return RstCheckError;
        }
        mExpectRecvChar++;
        mRecvBufValidSize++;
    }

    return RstSuccess;

}
