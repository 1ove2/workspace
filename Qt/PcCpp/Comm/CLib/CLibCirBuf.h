/******************************************************************************
 *
 * 文件名  ： CLibCirBuf.h
 * 创建日期： 20190512
 * 版本号  ： v1.0
 * 文件描述： 环形缓冲接口
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _CLIB_CIR_BUF_H_
#define _CLIB_CIR_BUF_H_

/************************************ 头文件 ***********************************/
#include "TypeDef.h"

/************************************ 宏定义 ***********************************/
#define _CLIB_CIR_BUF_SIZE_                   (1024)
/* 留1Byte标记,方便区分空和满 */
#define _CLIB_CIR_BUF_TOTAL_SIZE_             (_CLIB_CIR_BUF_SIZE_ - 1)

/*********************************** 类型定义 **********************************/
typedef struct _MISC_CIR_BUF_TAG_
{
    u8 buf[_CLIB_CIR_BUF_SIZE_];            /* 缓冲 */

    i32 head;                                   /* 有效数据起点 */
    i32 tail;                                   /* 有效数据终点 */
} _MISC_CIR_BUF_T;                           /* 环形缓冲 类型 */


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
void CLibCirBufInit(_MISC_CIR_BUF_T *cBuf);
i32 CLibCirBufWrite(_MISC_CIR_BUF_T *cBuf, const u8 *buf, i32 len);
i32 CLibCirBufRead(_MISC_CIR_BUF_T *cBuf, u8 *buf, i32 len);

i32 CLibCirBufGetValidSize(const _MISC_CIR_BUF_T *cBuf);
i32 CLibCirBufGetFreeSize(const _MISC_CIR_BUF_T *cBuf);

bool CLibCirBufIsFull(const _MISC_CIR_BUF_T *cBuf);
bool CLibCirBufIsEmpty(const _MISC_CIR_BUF_T *cBuf);

void CLibCirBufClearFromHead(_MISC_CIR_BUF_T *cBuf, i32 clearSize);

#ifdef _CYGWIN_DBG_
void CLibCirBufPrint(const _MISC_CIR_BUF_T *cBuf);
#endif

#endif // #ifndef _CLIB_CIR_BUF_H_

#ifdef __cplusplus
}
#endif
