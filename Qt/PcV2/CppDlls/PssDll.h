/********************************************************************************
*
* 文件名  ： PssDll.h
* 负责人  ： 彭鹏(516190948@qq.com)
* 创建日期： 20220402
* 版本号  ： v1.0
* 文件描述： Pss公司DLL头文件
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_DLL_H_
#define _PSS_DLL_H_

/************************************ 头文件 ***********************************/
#include "PssRstDef.h"
#include "PssTypeDef.h"

/************************************ 宏定义 ***********************************/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/*********************************** 类型定义 **********************************/
typedef enum _PSS_DLL_COM_BUADRATE_TAG_
{
    PSS_DLL_BUADRATE_9600                       = 0x00000000,
    PSS_DLL_BUADRATE_115200                     = 0x00000001,

    PSS_DLL_BUADRATE_ERR                        = 0xFFFFFFFF,
} PSS_DLL_BUADRATE;

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
/* IO接口 */
/* TODO: 函数接口注释使用doxygen格式 */
__declspec(dllexport) i32 __stdcall PssDllDevNetOpen(const char *ip);
__declspec(dllexport) i32 __stdcall PssDllDevComOpen(const char *name, PSS_DLL_BUADRATE baudrate);
/* TODO: 实现GPIB接口 */
__declspec(dllexport) i32  __stdcall PssDllDevClose(i32 dev);
__declspec(dllexport) bool __stdcall PssDllIsOpenned(i32 dev);

/* 基本接口 */
__declspec(dllexport) i32  __stdcall PssDllGetIdn(i32 dev, char *buf, i32 bufSize);
__declspec(dllexport) i32  __stdcall PssDllSetVSrc(i32 dev, u32 ch, bool isVSrc);
__declspec(dllexport) bool __stdcall PssDllIsVSrc(i32 dev, u32 ch);
__declspec(dllexport) i32  __stdcall PssDllSetVRange(i32 dev, u32 ch, f64  rangeVal, bool  isFixed);
__declspec(dllexport) i32  __stdcall PssDllGetVRange(i32 dev, u32 ch, f64 *rangeVal, bool *isFixed);
__declspec(dllexport) i32  __stdcall PssDllSetIRange(i32 dev, u32 ch, f64  rangeVal, bool  isFixed);
__declspec(dllexport) i32  __stdcall PssDllGetIRange(i32 dev, u32 ch, f64 *rangeVal, bool *isFixed);
__declspec(dllexport) i32  __stdcall PssDllSetVVal(i32 dev, u32 ch, f64  val);
__declspec(dllexport) i32  __stdcall PssDllGetVVal(i32 dev, u32 ch, f64 *val);
__declspec(dllexport) i32  __stdcall PssDllSetIVal(i32 dev, u32 ch, f64  val);
__declspec(dllexport) i32  __stdcall PssDllGetIVal(i32 dev, u32 ch, f64 *val);

/* 扩展接口 */
/* 扫描等 */


#endif // _PSS_DLL_H_

