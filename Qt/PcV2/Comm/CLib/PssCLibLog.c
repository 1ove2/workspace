/************************************************************************************************
 *
 * 文件名  : PssCLibLog.c
 * 负责人  : ZhanWei
 * 创建日期: 2021.07.14
 * 版本号  : v1.2
 * 文件描述: 日志模块 实现
 * 其    他: None
 * 修改日志: 2021.07.14 ZhanWei      初始版本       v1.0
 *           2021.07.19 PengPeng     完善设计       v1.1
 *           2021.07.22 PengPeng     移到Comm目录   v1.2
 *
 *************************************************************************************************/
/*-------------------------------------- 预处理区 -----------------------------------------------*/

/************************************** 头文件区 ************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifdef _PSS_LINUX_
#include <pthread.h>
static pthread_mutex_t sMutex = PTHREAD_MUTEX_INITIALIZER;
#endif

#include "PssCLibLog.h"


/* 最多128字节 */
#define _LOG_BYTES_MAX_SIZE_                                (512)
#define _LOG_BYTES_MAX_STR_SIZE_                            (_LOG_BYTES_MAX_SIZE_ * 3)

/************************************** 变量声明区 **********************************************/

/************************************** 函数声明区 **********************************************/
static i32  PrintFmt(PSS_CLIB_LOG_T *dbg, PSS_CLIB_LOG_LEVEL_E level);
static void Print(PSS_CLIB_LOG_T *log, const u8 *buf, u32 size);

/************************************** 函数实现区 **********************************************/
PssRstT PssCLibLogInit(PSS_CLIB_LOG_T *log)
{
    memset(log->lastBuf, 0, _PSS_CLIB_LOG_LAST_BUF_SIZE_);
    log->isOutputOverFlow = FALSE;

    /* 默认 开启调试模式 */
    PssCLibLogSetLevel(log, PSS_CLIB_LOG_LEVEL_DBG);
    return PssRstSuccess;
}

PssRstT PssCLibLogUnInit(PSS_CLIB_LOG_T *log)
{
    PSS_UNUSED(log);
    return PssRstNotImpl;
}

PssRstT PssCLibLogUpdate(PSS_CLIB_LOG_T *log)
{
    if(log->isOutputOverFlow)
    {
        log->Output(log->handle, log->lastBuf, strlen((const char *)log->lastBuf));
        log->isOutputOverFlow = FALSE;
    }

#if 0 /* 测试us级别延迟 */
    PssRstT PssMcuBrdAnlgDelayUs(u32 us);
    PssMcuBrdAnlgDelayUs(250);
#endif
#if 0 /* 测试Log输出溢出 */
    u32 i = 0;
    for(i = 0; i < 256; i++)
    {
        const char *str = "0123456789abcdefghijklmnopqrstuvwxyz\r\n";
        Print(log, (const u8 *)str, strlen(str));
    }
#endif

    return PssRstSuccess;
}

PssRstT PssCLibLogSetLevel(PSS_CLIB_LOG_T *log, PSS_CLIB_LOG_LEVEL_E level)
{
    log->level = level;
    return PssRstSuccess;
}

i32 PssCLibLogPrint(PSS_CLIB_LOG_LEVEL_E level, PSS_CLIB_LOG_T *log, int mode, const char* file, int line, const char *fmt, ...)
{
    if(level < log->level)
    {
        return PssRstSuccess;
    }

#ifdef _PSS_LINUX_
    int rst = pthread_mutex_lock(&sMutex);
    if(0 != rst)
    {
        const char *msg = "pthread_mutex_lock err.";
        i32  msgSize = strlen(msg);
        rst = log->Output(log->handle, (const u8 *)msg, msgSize);
    }
#endif

    if(mode)
    {
        char tmp[128] = {0};
        snprintf(tmp, sizeof(tmp), "[%s,%d]", file, line);
        Print(log, (const u8*)tmp, strlen(tmp));
    }

    PrintFmt(log, level);

    char buf[256] = {0};
    u32 size = 0;
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    size = strlen(buf);
    Print(log, (const u8 *)buf, size);

    /* 添加回车换行 */
    Print(log, (const u8 *)"\r\n", 2);

#ifdef _PSS_LINUX_
    rst = pthread_mutex_unlock(&sMutex);
    if(0 != rst)
    {
        const char *msg = "pthread_mutex_unlock err.";
        i32  msgSize = strlen(msg);
        rst = log->Output(log->handle, (const u8 *)msg, msgSize);
    }
#endif

    return PssRstSuccess;
}

i32 PssCLibLogPrintBytes(PSS_CLIB_LOG_LEVEL_E level, PSS_CLIB_LOG_T *log, const u8 *bytesBuf, u32 bytesSize, const char *fmt, ...)
{
    if(level < log->level)
    {
        return PssRstSuccess;
    }

#ifdef _PSS_LINUX_
    int rst = pthread_mutex_lock(&sMutex);
    if(0 != rst)
    {
        const char *msg = "pthread_mutex_lock err.";
        i32  msgSize = strlen(msg);
        rst = log->Output(log->handle, (const u8 *)msg, msgSize);
    }
#endif

    PrintFmt(log, level);

    char buf[256] = {0};
    u32 size = 0;
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    size = strlen(buf);
    Print(log, (const u8 *)buf, size);

    /* Bytes */
    char charHexbuf[4] = {0};
    char bufHexStrBuf[_LOG_BYTES_MAX_STR_SIZE_] = {0};
    u32 i = 0;
    u32 iMax = bytesSize;
    if(_LOG_BYTES_MAX_SIZE_ < bytesSize) /* 截断 避免溢出 */
    {
        iMax = _LOG_BYTES_MAX_SIZE_;
    }
    for(i = 0; i < iMax; i++)
    {
        /* 此处到行尾 用于调试 的代码 */
        memset(charHexbuf, 0, 4);
        sprintf(charHexbuf, "%02x,", bytesBuf[i]);
        strncat(bufHexStrBuf, charHexbuf, 3);
    }
    Print(log, (const u8 *)bufHexStrBuf, strlen(bufHexStrBuf));

    if(_LOG_BYTES_MAX_SIZE_ < bytesSize) /* 截断 避免溢出 */
    {
        Print(log, (const u8 *)"...", 3);
    }
    Print(log, (const u8 *)"\r\n", 2);

#ifdef _PSS_LINUX_
    rst = pthread_mutex_unlock(&sMutex);
    if(0 != rst)
    {
        const char *msg = "pthread_mutex_unlock err.";
        i32  msgSize = strlen(msg);
        rst = log->Output(log->handle, (const u8 *)msg, msgSize);
    }
#endif

    return PssRstSuccess;
}

i32 PrintFmt(PSS_CLIB_LOG_T *log, PSS_CLIB_LOG_LEVEL_E level)
{
    if(level < log->level)
    {
        return PssRstSuccess;
    }

    char timeBuf[16] = {0};
    u32 size = 0;

    /* 输出时间 */
    if(log->GetMs)
    {
        u64 ms = log->GetMs();
        sprintf(timeBuf, "[%07lu]:", (long unsigned int)ms);
        size = strlen(timeBuf);
        Print(log, (const u8 *)timeBuf, size);
    }
    return PssRstSuccess;
}

void Print(PSS_CLIB_LOG_T *log, const u8 *buf, u32 size)
{
    if(NULL == log)
    {
        return ;
    }

    if(NULL == buf)
    {
        return ;
    }
    if(log->isOutputOverFlow)
    {
        return;
    }

    /* FIXME:修复此处缓存溢出 导致的死循环 */
    i32 rst = PssRstSuccess;
    rst = log->Output(log->handle, buf, size);
    if(rst < 0)
    {
        const char *fmtStr = "Log OverFlow:%s";
        u32 printSize = size;
        u32 freeSize = _PSS_CLIB_LOG_LAST_BUF_SIZE_ - strlen(fmtStr) - 1;
        if(printSize > freeSize)
        {
            printSize = freeSize;
        }
        snprintf((char *)log->lastBuf, printSize, fmtStr, buf);
        log->isOutputOverFlow = TRUE;
    }
}

