/******************************************************************************
 *
 * 文件名  ： PssCppLog.cpp
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220112
 * 版本号  ： v1.0 20220112
 * 文件描述： Pss Cpp的Log模块 实现
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "PssCppLog.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

/*********************************** 命名空间 **********************************/

/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static i32 Output(void *handle, const u8 *buf, u32 size);
static u64 GetMs(void);

/********************************** 变量实现区 *********************************/
static bool            sIsStdoutEnable;                                                                  /* 使能标准输出 */
static time_t          sStartTime;                                                                       /* 启动时间 */
static PSS_CLIB_LOG_T  sCLibLog;                                                                         /* CLib中的Log模块 */
static FILE           *sLogFile;                                                                         /* log文件 */
PssCppLog *PssCppLog::mSingleton = new PssCppLog();                                                      /* 单例 */

/********************************** 函数实现区 *********************************/
PssCppLog::PssCppLog(void) :
    mLog(&sCLibLog)
{
    sLogFile        = nullptr;
    sIsStdoutEnable = true;

    sCLibLog.GetMs  = GetMs;
    sCLibLog.Output = Output;

    PssCLibLogInit(&sCLibLog);
    PssCLibLogSetLevel(&sCLibLog, PSS_CLIB_LOG_LEVEL_DBG);

    time(&sStartTime);
}

PssCppLog::~PssCppLog(void)
{
    if(sLogFile)
    {
        fclose(sLogFile);
    }
    PssCLibLogUnInit(&sCLibLog);
}

PSS_CLIB_LOG_T *PssCppLog::GetCLibLog(void)
{
    return mSingleton->mLog;
}

void PssCppLog::SetStdout(bool enable)
{
    sIsStdoutEnable = enable;
}

void PssCppLog::SetLogFile(const char *logFilePath)
{
    if(sLogFile)
    {
        fclose(sLogFile);
    }

    sLogFile = fopen(logFilePath, "w");
    if(!sLogFile)
    {
        fprintf(stderr, "fopen %s err.\n", logFilePath);
        exit(0);
    }

    sCLibLog.handle = sLogFile;
}

i32 Output(void *handle, const u8 *buf, u32 size)
{
    if(nullptr != handle)
    {
        fprintf((FILE *)handle, (const char *)(buf), size);
        fflush((FILE *)handle);
    }

    if(sIsStdoutEnable)
    {
        fprintf(stdout, (const char *)(buf), size);
        fflush(stdout);
    }
    return PssRstSuccess;
}

u64 GetMs(void)
{
    time_t now;
    time(&now);

    f64 diffSeconds = difftime(now, sStartTime);
    u64 ms = diffSeconds * 1000;
    return ms;
}

