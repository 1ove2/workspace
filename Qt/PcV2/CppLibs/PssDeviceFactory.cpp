/******************************************************************************
 *
 * 文件名  ： PssDeviceFactory.cpp
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220112
 * 版本号  ： v1.0 20220112
 * 文件描述： Pss 设备简单工厂类 实现
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "PssRstDef.h"
#include "PssIIo.h"
#include "PssIDevice.h"
#include "PssCppLog.h"
#include "PssDeviceFactory.h"

#include "Pss2400.h"
#include "Pss2611B.h"

#include "PssCx00.h"
#include "PssSx00.h"
#include "PssPx00.h"

#include <string.h>

/*********************************** 命名空间 **********************************/

/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static const char *GetDeviceTypeStr(PssIIo *iio);

/********************************** 变量实现区 *********************************/
PssDeviceFactory::~PssDeviceFactory(void)
{
}

PssIDevice *PssDeviceFactory::CreatePssDevice(PssIIo *iio)
{
    PssIDevice *device = nullptr;
    const char *typeStr = GetDeviceTypeStr(iio);
    if(nullptr == typeStr)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "typeStr is Null.\n");
        return nullptr;
    }

    if((0 == strncmp("MODEL 2400", typeStr, strlen(typeStr))))
    {
        device = new Pss2400(iio);
    }
    else if((0 == strncmp("MODEL 2611", typeStr, strlen(typeStr))))
    {
        PssCLibLogIL(PssCppLog::GetCLibLog(), "2611B Emulate 2400.\n");
        device = new Pss2611B(iio);
    }
    else if((0 == strncmp("1010C", typeStr, strlen(typeStr)))
     ||(0 == strncmp("1003C", typeStr, strlen(typeStr)))) /* 仅实现插卡式 */
    {
        device = new PssCx00(iio);
    }
    else if((0 == strncmp("S100", typeStr, strlen(typeStr)))
         || (0 == strncmp("S200", typeStr, strlen(typeStr)))
         || (0 == strncmp("S300", typeStr, strlen(typeStr))))
    {
        device = new PssSx00(iio);
    }
    else if((0 == strncmp("1004C", typeStr, strlen(typeStr))))
    {
        /* FIXME:调试使用,实际中1004C是错误设备 */
        PssCLibLogIL(PssCppLog::GetCLibLog(), "Err device type:%s.\n", typeStr);
        device = new PssCx00(iio);
    }
    else if ((0 == strncmp("P300", typeStr, strlen(typeStr))))
    {
        PssCLibLogIL(PssCppLog::GetCLibLog(), "Create P300 device.\n");
        device = new PssPx00(iio);
    }
    else
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "%s Not Impl.\n", typeStr);
    }

    PssCLibLogIL(PssCppLog::GetCLibLog(), "%s Created:%p.\n", typeStr, device);
    return device;
}

PssDeviceFactory::PssDeviceFactory(void)
{
}

const char *GetDeviceTypeStr(PssIIo *iio)
{
    const char *idnCmdStr = "*IDN?\n";
    i32 idnCmdStrSize = strlen(idnCmdStr);
    const int cRecvBufSize = 1024;
    static char sRecvBuf[cRecvBufSize] = {0};                    /* 避免返回栈内存 */
    i32 rst = 0;

    if(!iio->IsOpenned())
    {
        rst = iio->Open(PssIIo::Block);
        if(PssRstSuccess != rst)
        {
            PssCLibLogEL(PssCppLog::GetCLibLog(), "Open err:%d.\n", rst);
            return nullptr;
        }
    }

    rst = iio->Send(idnCmdStr, idnCmdStrSize);
    if(idnCmdStrSize != rst)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "Send err:%d.\n", rst);
        return nullptr;
    }

    rst = iio->Recv(sRecvBuf, cRecvBufSize);
    if(rst <= 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "Recv err:%d.\n", rst);
        return nullptr;
    }

    /* 第1个逗号之后 */
    const char *typeStrHead = strchr(sRecvBuf, ',') + 1;
    /* 第2个逗号之前 */
    char *typeStrTail = (char *)strchr(typeStrHead, ',');
    *typeStrTail = NUL;

    return typeStrHead;
}
