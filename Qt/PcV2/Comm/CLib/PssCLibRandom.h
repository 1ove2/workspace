﻿/******************************************************************************
 *
 * 文件名  ： PssCLibRandom.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20210706 
 * 版本号  ： v1.0
 * 文件描述： 随机 相关算法 接口
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _PSS_CLIB_RANDOM_H_
#define _PSS_CLIB_RANDOM_H_

/************************************ 头文件 ***********************************/
#include "PssTypeDef.h"

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
/* 获取随机值上限 */
i32  PssCLibGetRandMax(void);
/* 获取随机值,范围[0, PssCLibGetRandMax(void)] */
i32  PssCLibRandom(void);
/* 获取随机值,范围[min, max] */
i32  PssCLibRandomRange(i32 min, i32 max);

/* 以一定概率出现 True */
bool PssCLibRandomRateTrue(f32 rate);

#endif // #ifndef _PSS_CLIB_RANDOM_H_

#ifdef __cplusplus
}
#endif
