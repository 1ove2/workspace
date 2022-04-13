/********************************************************************************
 *
 * 文件名  : PssCLibCirBuf.h
 * 负责人  : PengPeng
 * 创建日期: 2019.05.12
 * 版本号  : v1.1
 * 文件描述: Pss 环形缓冲 接口
 * 其    他: None
 * 修改日志: 2021.05.12 PengPeng     初始版本       v1.0
 *           2021.07.20 PengPeng     完善设计       v1.1
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _PSS_CLIB_CIR_BUF_H_
#define _PSS_CLIB_CIR_BUF_H_

/************************************ 头文件 ***********************************/
#include "PssRstDef.h"
#include "PssTypeDef.h"

/************************************ 宏定义 ***********************************/
#ifdef _PSS_LINUX_
#define _PSS_CLIB_CIR_BUF_SIZE_                   (50 * 1024 * 1024)
#elif _PSS_MCU_
#define _PSS_CLIB_CIR_BUF_SIZE_                   (1024)
#else
#define _PSS_CLIB_CIR_BUF_SIZE_                   (1)
#error "please choose one platform"
#endif

/* 留1Byte标记,方便区分空和满 */
#define _PSS_CLIB_CIR_BUF_TOTAL_SIZE_             (_PSS_CLIB_CIR_BUF_SIZE_ - 1)

/*********************************** 类型定义 **********************************/
typedef struct _PSS_CLIB_CIR_BUF_TAG_
{
    u8 buf[_PSS_CLIB_CIR_BUF_SIZE_];            /* 缓冲 */

    u32 head;                                   /* 有效数据起点 */
    u32 tail;                                   /* 有效数据终点 */
} PSS_CLIB_CIR_BUF_T;                           /* 环形缓冲 类型 */


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
void PssCLibCirBufInit(PSS_CLIB_CIR_BUF_T *cBuf);
i32 PssCLibCirBufWrite(PSS_CLIB_CIR_BUF_T *cBuf, const u8 *buf, u32 len);
i32 PssCLibCirBufRead(PSS_CLIB_CIR_BUF_T *cBuf, u8 *buf, u32 len);

i32 PssCLibCirBufGetValidSize(const PSS_CLIB_CIR_BUF_T *cBuf);
i32 PssCLibCirBufGetFreeSize(const PSS_CLIB_CIR_BUF_T *cBuf);

bool PssCLibCirBufIsFull(const PSS_CLIB_CIR_BUF_T *cBuf);
bool PssCLibCirBufIsEmpty(const PSS_CLIB_CIR_BUF_T *cBuf);

i32  PssCLibCirBufClearFromHead(PSS_CLIB_CIR_BUF_T *cBuf, u32 clearSize);

/* 优化性能 */
PssRstT PssCLibCirBufWriteByte(PSS_CLIB_CIR_BUF_T *cBuf, u8  byte);
PssRstT PssCLibCirBufReadByte(PSS_CLIB_CIR_BUF_T *cBuf,  u8 *byte);

#ifdef _LINUX_DBG_
void PssCLibCirBufPrint(const PSS_CLIB_CIR_BUF_T *cBuf);
#endif

#endif // #ifndef _PSS_CLIB_CIR_BUF_H_

#ifdef __cplusplus
}
#endif

