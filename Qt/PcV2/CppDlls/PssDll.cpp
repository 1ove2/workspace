/******************************************************************************
 *
 * 文件名  ： PssDll.cpp
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220402
 * 版本号  ： v1.0 20220402
 * 文件描述： Pss DLL实现
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/
#include "PssDll.h"

#include "PssWinTcpClient.h"
#include "PssWinSerialPort.h"

#include "PssIIo.h"
#include "PssIDevice.h"
#include "PssDeviceFactory.h"

#include "PssVersion.h"
#include "PssCppLog.h"

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/

/*********************************** 命名空间 **********************************/

/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/********************************** 变量声明区 *********************************/
static bool        sIsInitted = false;
static PssIIo     *sIIo = NULL;
static PssIDevice *sDev = NULL;
static const u16   scDevPort = 5025;

/********************************** 函数声明区 *********************************/
static void Init(void);
static void UnInit(void);

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
__declspec(dllexport) i32 __stdcall PssDllDevNetOpen(const char *ip)
{
    Init();

    sIIo = new PssWinTcpClient(ip, scDevPort);
    sDev = PssDeviceFactory::CreatePssDevice(sIIo);
    const i32 cIdnBufSize = 256;
    char idnBuf[cIdnBufSize] = {0};
    i32 rst = sDev->GetIdn(idnBuf, cIdnBufSize);
    if(rst < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssIDevice::ScpiIdn->ScpiIdn err:%d.", rst);
        return rst;
    }

    PssCLibLogI(PssCppLog::GetCLibLog(), "PssDllDevNetOpen(%s) IDN:%s.", ip, idnBuf);
    return PssRstSuccess;
}

__declspec(dllexport) i32 __stdcall PssDllDevComOpen(const char *name, PSS_DLL_BUADRATE baudrate)
{
    Init();

    /* step1: 打开串口 */
    i32 baudrateVal = 0;
    switch (baudrate)
    {
        case PSS_DLL_BUADRATE_9600:
        {
            baudrateVal = 9600;
            break;
        }

        case PSS_DLL_BUADRATE_115200:
        {
            baudrateVal = 115200;
            break;
        }

        default:
        {
            PssCLibLogEL(PssCppLog::GetCLibLog(), "PssDllDevComOpen With Err Baudrate:%d.", baudrate);
            baudrateVal = 9600;
            break;
        }
    }
    sIIo = new PssWinSerialPort(name, baudrateVal);

    /* step2: 打开设备 */
    sDev = PssDeviceFactory::CreatePssDevice(sIIo);
    const i32 cIdnBufSize = 256;
    char idnBuf[cIdnBufSize] = {0};
    i32 rst = sDev->GetIdn(idnBuf, cIdnBufSize);
    if(rst < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssIDevice::ScpiIdn->ScpiIdn err:%d.", rst);
        return rst;
    }

    PssCLibLogI(PssCppLog::GetCLibLog(), "PssDllDevComOpen(%s,%d) IDN:%s", name, baudrate, idnBuf);
    return PssRstSuccess;
}

__declspec(dllexport) i32  __stdcall PssDllDevClose(i32 dev)
{
    delete sDev;
    sDev = NULL;
    sIIo->Close();
    delete sIIo;
    sIIo = NULL;

    PssCLibLogI(PssCppLog::GetCLibLog(), "PssDllDevClose:%d.", dev);
    UnInit();
    return PssRstSuccess;
}

__declspec(dllexport) bool __stdcall PssDllIsOpenned(i32 dev)
{
    PSS_UNUSED(dev)
    return sIIo->IsOpenned();
}

__declspec(dllexport) i32  __stdcall PssDllGetIdn(i32 dev, char *buf, i32 bufSize)
{
    PSS_UNUSED(dev)
    i32 rst = sDev->GetIdn(buf, bufSize);
    return rst;
}

__declspec(dllexport) i32  __stdcall PssDllSetVSrc(i32 dev, u32 ch, bool isVSrc)
{
    PSS_UNUSED(dev)
    i32 rst = sDev->SetVSrc(ch, isVSrc);
    return rst;
}

__declspec(dllexport) bool __stdcall PssDllIsVSrc(i32 dev, u32 ch)
{
    PSS_UNUSED(dev)
    bool rst = sDev->IsVSrc(ch);
    return rst;
}

__declspec(dllexport) i32  __stdcall PssDllSetVRange(i32 dev, u32 ch, f64  rangeVal, bool  isFixed)
{
    PSS_UNUSED(dev)
    i32 rst = sDev->SetVRange(ch, rangeVal, isFixed);
    return rst;
}

__declspec(dllexport) i32  __stdcall PssDllGetVRange(i32 dev, u32 ch, f64 *rangeVal, bool *isFixed)
{
    PSS_UNUSED(dev)
    i32 rst = sDev->GetVRange(ch, rangeVal, isFixed);
    return rst;
}

__declspec(dllexport) i32  __stdcall PssDllSetIRange(i32 dev, u32 ch, f64  rangeVal, bool  isFixed)
{
    PSS_UNUSED(dev)
    i32 rst = sDev->SetIRange(ch, rangeVal, isFixed);
    return rst;
}

__declspec(dllexport) i32  __stdcall PssDllGetIRange(i32 dev, u32 ch, f64 *rangeVal, bool *isFixed)
{
    PSS_UNUSED(dev)
    i32 rst = sDev->GetIRange(ch, rangeVal, isFixed);
    return rst;
}

__declspec(dllexport) i32  __stdcall PssDllSetVVal(i32 dev, u32 ch, f64 val)
{
    PSS_UNUSED(dev)
    i32 rst = sDev->SetVVal(ch, val);
    return rst;
}

__declspec(dllexport) i32  __stdcall PssDllGetVVal(i32 dev, u32 ch, f64 *val)
{
    PSS_UNUSED(dev)
    i32 rst = sDev->GetVVal(ch, val);
    return rst;
}

__declspec(dllexport) i32  __stdcall PssDllSetIVal(i32 dev, u32 ch, f64 val)
{
    PSS_UNUSED(dev)
    i32 rst = sDev->SetIVal(ch, val);
    return rst;
}

__declspec(dllexport) i32  __stdcall PssDllGetIVal(i32 dev, u32 ch, f64 *val)
{
    PSS_UNUSED(dev)
    i32 rst = sDev->GetIVal(ch, val);
    return rst;
}

void Init(void)
{
    if(!sIsInitted)
    {
        PssCppLog::SetStdout(true);
        PssCppLog::SetLogFile("PssDll.log");
        PssCLibLogI(PssCppLog::GetCLibLog(), "PssDll Init Done.");
        PssCLibLogI(PssCppLog::GetCLibLog(), "Version:%s", PssGetGitAllStr());
        sIsInitted = true;
    }
}

void UnInit(void)
{
    PssCLibLogI(PssCppLog::GetCLibLog(), "PssDll UnInit Done.");
    sIsInitted = false;
}
