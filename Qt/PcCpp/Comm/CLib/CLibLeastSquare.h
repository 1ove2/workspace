/******************************************************************************
 *
 * 文件名  ： CLibLeastSquare.h
 * 创建日期： 20190705 
 * 版本号  ： v1.0
 * 文件描述： 最小二乘法 接口
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _CLIB_LEAST_SQUARE_H_
#define _CLIB_LEAST_SQUARE_H_

/************************************ 头文件 ***********************************/
#include "TypeDef.h"

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
/*******************************************************************************
 *
 * 函数名  : CLibLeastSquare
 * 创建日期: 2019.07.05
 * 函数功能: 最小二乘法
 * 输入参数: x - 自变量 向量
 *           y - 测量值 向量
 *           size - 向量大小
 * 输出参数: a - 估计斜率
 *           b - 估计截距
 *           sqSum - 误差(平方和) sum((y - yi)^2)
 * 返回值:   无
 * 调用关系: 无
 * 其 它:    无
 *
  ******************************************************************************/
 void CLibLeastSquare(const f64 *x, const f64 *y, size_t size, f64 *a, f64 *b, f64 *sqSum);

#endif // #ifndef _CLIB_LEAST_SQUARE_H_

#ifdef __cplusplus
}
#endif
