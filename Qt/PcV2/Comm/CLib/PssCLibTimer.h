/******************************************************************************
 *
 * 文件名  ： PssCLibTimer.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20210714 
 * 版本号  ： v1.0
 * 文件描述： 定时器 模块 接口
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _PSS_CLIB_TIMER_H_
#define _PSS_CLIB_TIMER_H_

/************************************ 头文件 ***********************************/
#include "PssTypeDef.h"

/************************************ 宏定义 ***********************************/
#define _PSS_CLIB_TIMER_MAX_NUM_H_                      (16)

/*********************************** 类型定义 **********************************/
typedef struct _PSS_CLIB_TIMER_TAG_
{
    bool    en;
    u32     tickCounter;
    u32     tickInit;

    FuncPtr callBack;
    void    *argv;
} _PSS_CLIB_TIMER_T_;

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
/* 初始化 定时器模块
 * 返回值为可用定时器上限
 * 即定时器可用id范围为:[0, id)
 * */
u32  PssCLibTimerInit(void);

/*
 * 返回值最小可用定时器的ID
 * */
u32  PssCLibTimerGetAId(void);

/* 设置超时回调函数 */
void PssCLibTimerSetCallBack(u32 id, FuncPtr callBack, void *argv);
/* 设置超时值 */
void PssCLibTimerSetTick(u32 id, u32 tick);
/* 启动 */
void PssCLibTimerStart(u32 id);
/* 停止 */
void PssCLibTimerStop(u32 id);
/* 重置 */
void PssCLibTimerRestart(u32 id);
/* 获取剩余Tick */
u32  PssCLibTimerGetLeftTick(u32 id);

/* 需调用方tick为粒度调用 */
void PssCLibTimerUpdate(void);

#endif // #ifndef _PSS_CLIB_TIMER_H_

#ifdef __cplusplus
}
#endif

