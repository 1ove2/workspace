/******************************************************************************
 *
 * 文件名  ： PssCLibTimer.c
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20210714 
 * 版本号  ： v1.0
 * 文件描述： 定时器 模块 实现
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include <stdio.h>
#include <stdlib.h>
#include "PssCLibTimer.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/
static _PSS_CLIB_TIMER_T_ sTimers[_PSS_CLIB_TIMER_MAX_NUM_H_];

/********************************** 函数实现区 *********************************/
u32 PssCLibTimerInit(void)
{
    return _PSS_CLIB_TIMER_MAX_NUM_H_;
}

u32 PssCLibTimerGetAId(void)
{
    return 0;
}

void PssCLibTimerSetCallBack(u32 id, FuncPtr callBack, void *argv)
{
}

void PssCLibTimerSetTick(u32 id, u32 tick)
{
}

void PssCLibTimerStart(u32 id)
{
}

void PssCLibTimerStop(u32 id)
{
}

void PssCLibTimerRestart(u32 id)
{
}

u32 PssCLibTimerGetLeftTick(u32 id)
{
    return sTimers[id].tickCounter;
}

void PssCLibTimerUpdate(void)
{
}

