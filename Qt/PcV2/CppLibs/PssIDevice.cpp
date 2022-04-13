/******************************************************************************
 *
 * 文件名  ： PssIDevice.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220112
 * 版本号  ： v1.0 20220112
 * 文件描述： Pss 设备接口(抽象)类 头文件
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "PssRstDef.h"
#include "PssIDevice.h"
#include "PssCppLog.h"
#include "PssIIo.h"

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
PssIDevice::PssIDevice(PssIIo *iio) :
    mIIo(iio)
{
    if(mIIo->IsOpenned())
    {
        return;
    }

    int rst = mIIo->Open(PssIIo::Block);
    if(PssRstSuccess != rst)
    {
        PssCLibLogE(PssCppLog::GetCLibLog(), "PssCx00 Open failled:%d.\n", rst);
        return;
    }
}

PssIDevice::~PssIDevice(void)
{
    mIIo->Close();
}

/* FIXME: 目前以2400的SCPI为模板实现,需要针对不同设备进行修改 */
i32 PssIDevice::GetIdn(char *idnBuf, i32 idnBufSize)
{
    const char *cmdIdn = "*IDN?\n";
    i32 cmdIdnSize = strlen(cmdIdn);
    i32 rst = RunCmdWithResponse(idnBuf, idnBufSize, cmdIdn, cmdIdnSize);
    if(rst < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssIDevice::RunCmdWithResponse(%s) err:%d.", cmdIdn, rst);
        return rst;
    }

    return rst;
}

i32 PssIDevice::SetVSrc(u32 ch, bool isVSrc)
{
    PSS_UNUSED(ch)
    const i32 cCmdSize = 256;
    char cmd[cCmdSize] = {0};
    const char *cmdHead = ":SOUR:FUNC ";
    i32 cmdHeadSize = strlen(cmdHead);
    strncat(cmd, cmdHead, cmdHeadSize);
    if(isVSrc)
    {
        strncat(cmd, "VOLT\n", 5);
    }
    else
    {
        strncat(cmd, "CURR\n", 5);
    }
    i32 cmdSize = strlen(cmd);
    i32 rst = RunCmd(cmd, cmdSize);
    if(rst < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssIDevice::RunCmd(%s) err:%d.", cmd, rst);
        return rst;
    }

    return rst;
}

bool PssIDevice::IsVSrc(u32 ch) const
{
    PSS_UNUSED(ch)
    const i32 cResponseBufSize = 256;
    char responseBuf[cResponseBufSize] = {0};
    const char *cmd = ":SOUR:FUNC?\n";
    i32 cmdSize = strlen(cmd);
    i32 rst = RunCmdWithResponse(responseBuf, cResponseBufSize, cmd, cmdSize);
    if(rst < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssIDevice::RunCmdWithResponse(%s) err:%d.", cmd, rst);
        return rst;
    }

    PssCLibLogIL(PssCppLog::GetCLibLog(), "%s.", responseBuf);
    if(0 == strncmp("VOLT", responseBuf, 4))
    {
        return true;
    }
    else if (0 == strncmp("CURR", responseBuf, 4))
    {
        return false;
    }
    else
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssIDevice::RunCmdWithResponse(%s => %s) err:%d.", cmd, responseBuf, rst);
        return false;
    }
}

i32 PssIDevice::SetVRange(u32 ch, f64 rangeVal, bool isFixed)
{
    i32 rst = SetRange(ch, true, rangeVal, isFixed);
    return rst;
}

i32 PssIDevice::GetVRange(u32 ch, f64 *rangeVal, bool *isFixed) const
{
    i32 rst = GetRange(ch, true, rangeVal, isFixed);
    return rst;
}

/* 与VRange相关函数对称 */
i32 PssIDevice::SetIRange(u32 ch, f64 rangeVal, bool isFixed)
{
    i32 rst = SetRange(ch, false, rangeVal, isFixed);
    return rst;
}

i32 PssIDevice::GetIRange(u32 ch, f64 *rangeVal, bool *isFixed) const
{
    i32 rst = GetRange(ch, false, rangeVal, isFixed);
    return rst;
}

i32 PssIDevice::SetVVal(u32 ch, f64 val)
{
    i32 rst = SetVal(ch, true, val);
    return rst;
}

i32 PssIDevice::GetVVal(u32 ch, f64 *val) const
{
    i32 rst = GetVal(ch, true, val);
    return rst;
}

i32 PssIDevice::SetIVal(u32 ch, f64 val)
{
    i32 rst = SetVal(ch, false, val);
    return rst;
}

i32 PssIDevice::GetIVal(u32 ch, f64 *val) const
{
    i32 rst = GetVal(ch, false, val);
    return rst;
}

i32 PssIDevice::FetchData(char *buf, i32 bufSize)
{
    i32 rst = mIIo->Recv(buf, bufSize);
    return rst;
}

i32 PssIDevice::RunCmd(const char *cmd, i32 cmdSize) const
{
    i32 rst = mIIo->Send(cmd, cmdSize);
    return rst;
}

i32 PssIDevice::RunCmdWithResponse(char *response, i32 responseSize, const char *cmd, i32 cmdSize) const
{
    i32 rst = mIIo->Send(cmd, cmdSize);
    if(rst < 0)
    {
        PssCLibLogE(PssCppLog::GetCLibLog(), "RunCmdWithResponse Send err:%d\n", rst);
        return rst;
    }
    rst = mIIo->Recv(response, responseSize);
    if(rst < 0)
    {
        PssCLibLogE(PssCppLog::GetCLibLog(), "RunCmdWithResponse Recv err:%d\n", rst);
        return rst;
    }

    /* 移除行尾'\n','\r','\t',' ' */
    i32 bufSize = rst;
    char *ptrTail = response + bufSize - 1;
    while(('\r' == *ptrTail) || ('\n' == *ptrTail) || ('\t' == *ptrTail) || (' ' == *ptrTail))
    {
        *ptrTail = NUL;

        ptrTail--;
        bufSize--;
    }

    /* TODO: 移除行首'\n','\r','\t',' '  */

    return bufSize;
}

i32 PssIDevice::SetRange(u32 ch, bool isV, f64 rangeVal, bool isFixed)
{
    PSS_UNUSED(ch)
    const i32 cCmdSize = 256;
    char cmdBuf[cCmdSize] = {0};
    if(isFixed)
    {
        const char *fmt = ":SOUR:%s:RANG:AUTO\n";
        snprintf(cmdBuf, cCmdSize, fmt, isV ? ("VOLT") : ("CURR"));
    }
    else
    {
        const char *fmt = ":SOUR:%s:RANG %15.12e\n";
        snprintf(cmdBuf, cCmdSize, fmt, isV ? ("VOLT") : ("CURR"), rangeVal);
    }

    i32 rst = RunCmd(cmdBuf, strlen(cmdBuf));
    if(rst < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssIDevice::RunCmd(%s) err:%d.", cmdBuf, rst);
        return rst;
    }
    return rst;
}

i32 PssIDevice::GetRange(u32 ch, bool isV, f64 *rangeVal, bool *isFixed) const
{
    PSS_UNUSED(ch)
    const i32 cResponseBufSize = 256;
    char responseBuf[cResponseBufSize] = {0};
    const char *cmd = NULL;
    if(isV)
    {
        cmd = ":SOUR:VOLT:RANG:AUTO?\n";
    }
    else
    {
        cmd = ":SOUR:CURR:RANG:AUTO?\n";
    }
    i32 cmdSize = strlen(cmd);
    i32 rst = RunCmdWithResponse(responseBuf, cResponseBufSize, cmd, cmdSize);
    if(rst < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssIDevice::RunCmdWithResponse(%s) err:%d.", cmd, rst);
        return rst;
    }

    /* 自动量程判断 */
    if(NULL != isFixed)
    {
        PssCLibLogIL(PssCppLog::GetCLibLog(), "%s.", responseBuf);
        if(0 == strncmp("ON", responseBuf, 4))
        {
            *isFixed = false;
        }
        else if (0 == strncmp("OFF", responseBuf, 4))
        {
            *isFixed = true;
        }
        else
        {
            PssCLibLogEL(PssCppLog::GetCLibLog(), "PssIDevice::RunCmdWithResponse(%s => %s) err:%d.", cmd, responseBuf, rst);
        }
    }

    /* 当前量程 */
    memset(responseBuf, 0, cResponseBufSize);
    if(isV)
    {
        cmd = ":SOUR:VOLT:RANG?\n";
    }
    else
    {
        cmd = ":SOUR:CURR:RANG?\n";
    }
    cmdSize = strlen(cmd);
    rst = RunCmdWithResponse(responseBuf, cResponseBufSize, cmd, cmdSize);
    if(rst < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssIDevice::RunCmdWithResponse(%s => %s) err:%d.", cmd, responseBuf, rst);
        return rst;
    }
    *rangeVal = atof(responseBuf);

    return PssRstSuccess;
}

i32 PssIDevice::SetVal(u32 ch, bool isV, f64 val)
{
    PSS_UNUSED(ch)
    /* step1: 判断当前源类型 */
    bool isVSrc = IsVSrc(ch);

    /* step2: 构造命令 */
    const i32 cCmdSize = 256;
    char cmdBuf[cCmdSize] = {0};
    const char *fmt = NULL;
    if(isVSrc && isV)           /* 电压源状态 设置 电压值 */
    {
        fmt = ":SOUR:VOLT:LEV %15.12e\n";
    }
    else if(isVSrc && (!isV))   /* 电压源状态 设置 电流值 */
    {
        fmt = ":SOUR:CURR:PROT %15.12e\n";
    }
    else if((!isVSrc) && (isV)) /* 电流源状态 设置 电压值 */
    {
        fmt = ":SOUR:VOLT:PROT %15.12e\n";
    }
    else /* (!isVSrc) && (!isV)    电流源状态 设置 电流值 */
    {
        fmt = ":SOUR:CURR:LEV %15.12e\n";
    }
    snprintf(cmdBuf, cCmdSize, fmt, val);

    i32 rst = RunCmd(cmdBuf, strlen(cmdBuf));
    if(rst < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssIDevice::RunCmd(%s) err:%d.", cmdBuf, rst);
        return rst;
    }
    return rst;
}

i32 PssIDevice::GetVal(u32 ch, bool isV, f64 *val) const
{
    /* step1: 开输出 */
    i32 rst = SetOutput(ch, true);
    if(rst < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssIDevice::SetOutput err:%d.", rst);
        return rst;
    }

    /* step2: 发送命令 */
    const i32 cResponseBufSize = 256;
    char responseBuf[cResponseBufSize] = {0};
    const char *cmd = ":READ?\n";
    rst = RunCmdWithResponse(responseBuf, cResponseBufSize, cmd, strlen(cmd));
    if(rst < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssIDevice::RunCmdWithResponse(%s => %s) err:%d.", cmd, responseBuf, rst);
        *val = 0;
        SetOutput(ch, false);
        return rst;
    }

    /* step3: 关输出 */
    rst = SetOutput(ch, false);
    if(rst < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssIDevice::SetOutput err:%d.", rst);
        *val = 0;
        return rst;
    }

    /* step4: 提取需要的(电压 or 电流) 值 */
    /* PssCLibLogEL(PssCppLog::GetCLibLog(), "PssIDevice::GetVal: %s => %s.", cmd, responseBuf); */
    /* 第1个逗号之后 */
    const char *vStrHead = responseBuf;
    /* 第2个逗号之前 */
          char *vStrTail = (char *)strchr(vStrHead, ',');
    *vStrTail = NUL;
    const char *iStrHead = vStrTail + 1;
          char *iStrTail = (char *)strchr(iStrHead, ',');
    *iStrTail = NUL;
    if(isV)
    {
        *val = atof(vStrHead);
        /* PssCLibLogEL(PssCppLog::GetCLibLog(), "%p,%p:%s => %7.4g.", responseBuf, vStrHead, vStrHead, *val); */
    }
    else
    {
        *val = atof(iStrHead);
        /* PssCLibLogEL(PssCppLog::GetCLibLog(), "%p,%p:%s => %7.4g.", responseBuf, iStrHead, iStrHead, *val); */
    }

    return PssRstSuccess;
}

i32 PssIDevice::SetOutput(u32 ch, bool on) const
{
    PSS_UNUSED(ch)
    const char *cmd = NULL;
    if(on)
    {
        cmd = ":OUTP ON\n";
    }
    else
    {
        cmd = ":OUTP OFF\n";
    }
    i32 rst = RunCmd(cmd, strlen(cmd));
    if(rst < 0)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "PssIDevice::RunCmd(%s) err:%d.", cmd, rst);
        return rst;
    }
    return rst;
}
