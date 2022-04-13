/********************************************************************************
*
* 文件名  ： PssTypeDef.h
* 负责人  ： 彭鹏(516190948@qq.com)
* 创建日期： 20190815
* 版本号  ： v1.0
* 文件描述： 通用类型定义文件
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_TYPE_DEF_H_
#define _PSS_TYPE_DEF_H_

/*
 * 修复使用MSVC++编译无法编译通过的问题
 * MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
 * MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
 * MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
 * MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
 * MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
 * MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio 2003)
 * MSVC++ 7.0  _MSC_VER == 1300
 * MSVC++ 6.0  _MSC_VER == 1200
 * MSVC++ 5.0  _MSC_VER == 1100
 */
#ifdef _MSC_VER //== 1800 (Visual Studio 2013)

#define __func__    __FUNCTION__

#if _MSC_VER > 1600
/* MSVC版本编译时，强制中文为UTF-8处理 */
#pragma execution_character_set("utf-8")
#endif

#endif

/************************************ 头文件 ***********************************/
#include <stddef.h>
#include <stdint.h>

/************************************ 宏定义 ***********************************/
/* bool 宏 */
#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (1)
#endif

/* 空字符 */
#ifndef NUL
#define NUL ('\0')
#endif

#ifndef NULL
#define NULL ((void*) 0)
#endif

#ifndef __WEAK
#define __WEAK                                 __attribute__((weak))
#endif

/*********************************** 类型定义 **********************************/
typedef uint8_t             u8;
typedef uint16_t            u16;
typedef uint32_t            u32;
typedef uint64_t            u64;
typedef int8_t              i8;
typedef int16_t             i16;
typedef int32_t             i32;
typedef int64_t             i64;
typedef float               f32;
typedef double              f64;

#ifdef __cplusplus
extern "C"
{
    /* 回调函数 */
    typedef void(*FuncPtr)  (void *argv);

    /* 序列的生成函数 y = func(x) */
    typedef double(*X2YFuncPtr) (double x);
}
#else
/* 回调函数 */
typedef void(*FuncPtr)  (void *argv);
/* 序列的生成函数 y = func(x) */
typedef double(*X2YFuncPtr) (double x);
/* 避免Qt编译报错 */
typedef unsigned char       bool;
#endif

/* 避免未使用参数警告 */
#define PSS_UNUSED(x) (void)x;

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/

#endif // _PSS_TYPE_DEF_H_
