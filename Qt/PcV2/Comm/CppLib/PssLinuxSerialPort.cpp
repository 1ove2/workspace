/******************************************************************************
 *
 * 文件名  ： PssLinuxSerialPort.cpp
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220111
 * 版本号  ： v1.0 20220111
 * 文件描述： Pss Linux平台串口 实现
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "PssRstDef.h"
#include "PssLinuxSerialPort.h"

/*********************************** 命名空间 **********************************/

/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
PssLinuxSerialPort::PssLinuxSerialPort(void)
{
}

PssLinuxSerialPort::~PssLinuxSerialPort(void)
{
}

int PssLinuxSerialPort::Open(Mode mode)
{
    PSS_UNUSED(mode)
    return PssRstNotImpl;
}

int PssLinuxSerialPort::Close(void)
{
    return PssRstNotImpl;
}

bool PssLinuxSerialPort::IsOpenned(void) const
{
    return false;
}

int PssLinuxSerialPort::Send(const char *buf, i32 size)
{
    PSS_UNUSED(buf)
    PSS_UNUSED(size)

    return PssRstNotImpl;
}

int PssLinuxSerialPort::Recv(char *buf, i32 size)
{
    PSS_UNUSED(buf)
    PSS_UNUSED(size)

    return PssRstNotImpl;
}

int PssLinuxSerialPort::FlushOut(void)
{
    return PssRstNotImpl;
}

int PssLinuxSerialPort::ClearIn(void)
{
    return PssRstNotImpl;
}

