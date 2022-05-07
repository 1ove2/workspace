/******************************************************************************
 *
 * 文件名  ： PssWinSerialPort.cpp
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220110
 * 版本号  ： v1.0 20220110
 * 文件描述： Pss Windows平台串口 实现
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include <stdio.h>

#include "PssRstDef.h"
#include "PssCppLog.h"

#include "PssWinSerialPort.h"

/*********************************** 命名空间 **********************************/

/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
PssWinSerialPort::PssWinSerialPort(const char *name, i32 baudrate) :
    PssIIo(),
    mCom(INVALID_HANDLE_VALUE),
    mDcb(),
    mName(),
    mBaudrate(baudrate)
{
    i32 size = strlen(name);
    if(size > 4)
    {
        snprintf(mName,7,"\\\\.\\");
    }
    strncat(mName, name, size);
}

PssWinSerialPort::~PssWinSerialPort(void)
{
}

int PssWinSerialPort::Open(Mode mode)
{
    /* step1: 打开串口 */
    mCom = CreateFileA(
                mName,
                GENERIC_READ | GENERIC_WRITE,//支持读写
                0,//独占方式，串口不支持共享
                NULL,//安全属性指针，默认值为NULL
                OPEN_EXISTING,//打开现有的串口文件
                0,//0：同步方式，FILE_FLAG_OVERLAPPED：异步方式
                NULL);//用于复制文件句柄，默认值为NULL，对串口而言该参数必须置为NULL
    if(INVALID_HANDLE_VALUE == mCom)
    {
        DWORD err = GetLastError();
        PssCLibLogEL(PssCppLog::GetCLibLog(), "CreateFileA:%s err:%d,%d.", mName, mCom, err);
        return PssRstDevCreateError;
    }

    /* step2: 设置串口波特率 */
    GetCommState(mCom, &mDcb);
    mDcb.BaudRate = mBaudrate;
    mDcb.Parity = NOPARITY;
    mDcb.ByteSize = 8;
    mDcb.StopBits = ONESTOPBIT;
    SetCommState(mCom, &mDcb);

#if 1
    /* step3: 设置缓冲 */
    SetupComm(mCom, 1024, 1024); /* 读写各1k */

    /* step4: 设置阻塞 */
    //超时处理,单位：毫秒
    //总超时＝时间系数×读或写的字符数＋时间常量
    COMMTIMEOUTS timeout;
    if(Mode::Block == mode)
    {
        timeout.ReadIntervalTimeout         = 0;
        timeout.ReadTotalTimeoutMultiplier  = 0;
        timeout.ReadTotalTimeoutConstant    = 50;          /* 避免读持续阻塞,故设置50ms延迟 */

        timeout.WriteTotalTimeoutConstant   = 0;
        timeout.WriteTotalTimeoutMultiplier = 0;
        PssCLibLogIL(PssCppLog::GetCLibLog(), "PssWinSerialPort::Open Block.");
    }
    else
    {
        timeout.ReadIntervalTimeout         = 1;
        timeout.ReadTotalTimeoutMultiplier  = 1;
        timeout.ReadTotalTimeoutConstant    = 100;

        timeout.WriteTotalTimeoutConstant   = 0;
        timeout.WriteTotalTimeoutMultiplier = 0;

        PssCLibLogIL(PssCppLog::GetCLibLog(), "PssWinSerialPort::Open NonBlock.");
    }
    SetCommTimeouts(mCom, &timeout);

    /* Step5: 清空缓冲 */
    PurgeComm(mCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
#endif
    PssCLibLogIL(PssCppLog::GetCLibLog(), "PssWinSerialPort::Open Done:%s,%d.", mName, mBaudrate);
    return PssRstSuccess;
}

int PssWinSerialPort::Close(void)
{
    if(IsOpenned())
    {
        FlushFileBuffers(mCom);
        CloseHandle(mCom);
        mCom = INVALID_HANDLE_VALUE;
        PssCLibLogIL(PssCppLog::GetCLibLog(), "PssWinSerialPort::Close Done:%s,%d.", mName, mBaudrate);
    }
    return PssRstSuccess;
}

bool PssWinSerialPort::IsOpenned(void) const
{
    if(INVALID_HANDLE_VALUE == mCom)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int PssWinSerialPort::Send(const char *buf, i32 size)
{
    PurgeComm(mCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

    DWORD bufLen = size;
    bool writeStatus = WriteFile(mCom, buf, size, &bufLen, NULL);
    if(!writeStatus)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "WriteFile err:%d != %d.", size, bufLen);
        return PssRstWriteToIOError;
    }
    return bufLen;
}

int PssWinSerialPort::Recv(char *buf, i32 size)
{
    DWORD len = 0;
    bool readStatus = ReadFile(mCom, buf, size, &len, NULL);
    if(!readStatus)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "ReadFile err:%d != %d.", size, len);
        return PssRstReadFromIOError;
    }

    return len;
}

int PssWinSerialPort::FlushOut(void)
{
    return PssRstNotImpl;
}

int PssWinSerialPort::ClearIn(void)
{
    return PssRstNotImpl;
}
