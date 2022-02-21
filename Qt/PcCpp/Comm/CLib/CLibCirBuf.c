/******************************************************************************
 *
 * 文件名  ： CLibCirBuf.c
 * 创建日期： 20190512
 * 版本号  ： v1.0
 * 文件描述： 环形缓冲接口
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
/************************************ 头文件 ***********************************/
#include "CLibCirBuf.h"
#include "RstDef.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/
static i32 ModAddIndex(i32 index, i32 addNum);

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
void CLibCirBufInit(_MISC_CIR_BUF_T *cBuf)
{
    i32 i = 0;

    for(i = 0; i < _CLIB_CIR_BUF_SIZE_; i++)
    {
        cBuf->buf[i] = 0x00;
    }
    
    cBuf->head = 0;
    cBuf->tail = 0;
}

i32 CLibCirBufWrite(_MISC_CIR_BUF_T *cBuf, const u8 *buf, i32 len)
{
    i32 i = 0;
    i32 freeSize = 0;
    i32 index = 0;
    i32 willWriteSize = 0;

    /* step1: 检查能存入的数据 */
    freeSize = CLibCirBufGetFreeSize(cBuf);
    
    /* step2: 求可以写入的大小 */
    if(len < freeSize)
    {
        willWriteSize = len;
    }
    else                                            /* FIXME:出现溢出 */
    {
        willWriteSize = freeSize;
        return RstOverFlow;
    }
    
    /* step3: 写入 */
    for(i = 0; i < willWriteSize; i++)
    {
        index = ModAddIndex(cBuf->tail, i);
        cBuf->buf[index] = buf[i];
    }
    
    cBuf->tail = ModAddIndex(cBuf->tail, willWriteSize);
    
    /* step4: 返回实际写入大小 */
    return willWriteSize;
}

i32 CLibCirBufRead(_MISC_CIR_BUF_T *cBuf, u8 *buf, i32 len)
{
    i32 i = 0;
    i32 buffedSize = 0;
    i32 index = 0;
    i32 willReadSize = 0;

    /* step1: 检查可读取的数据大小 */
    buffedSize = CLibCirBufGetValidSize(cBuf);
    if(0 == buffedSize)
    {
        return 0;
    }
    
    /* step2: 求可以读取的数据大小 */
    if(len < buffedSize)
    {
        willReadSize = len;
    }
    else
    {
        willReadSize = buffedSize;
    }
    
    /* step3: 读取 */
    for(i = 0; i < willReadSize; i++)
    {
        index = ModAddIndex(cBuf->head, i);
        buf[i] = cBuf->buf[index];
    }
    
    /* step4: 返回实际读取的大小 */
    cBuf->head = ModAddIndex(cBuf->head, willReadSize);
    
    return willReadSize;
}

/* 留1Byte标记,方便区分空和满 */
/* head == tail     空 */
/* head == tail + 1 满 */
i32 CLibCirBufGetFreeSize(const _MISC_CIR_BUF_T *cBuf)
{
    if(cBuf->tail == cBuf->head)
    {
        return _CLIB_CIR_BUF_TOTAL_SIZE_;
    }

    return (cBuf->head + _CLIB_CIR_BUF_TOTAL_SIZE_ - cBuf->tail) % _CLIB_CIR_BUF_SIZE_;
}

bool CLibCirBufIsFull(const _MISC_CIR_BUF_T *cBuf)
{
    bool isFull = (0 == CLibCirBufGetFreeSize(cBuf));
    return isFull;
}

bool CLibCirBufIsEmpty(const _MISC_CIR_BUF_T *cBuf)
{
    bool isEmpty = (0 == CLibCirBufGetValidSize(cBuf));
    return isEmpty;
}

void CLibCirBufClearFromHead(_MISC_CIR_BUF_T *cBuf, i32 clearSize)
{
    i32 validSize = CLibCirBufGetValidSize(cBuf);
    i32 realClearSize = (validSize < clearSize) ? (validSize) : (clearSize);

    /* 更新头指针 等效于 删除 */
    cBuf->head = ModAddIndex(cBuf->head, realClearSize);
}

/* totalSize = size + freeSize */
/* 故 size = totalSize - freeSize */
i32 CLibCirBufGetValidSize(const _MISC_CIR_BUF_T *cBuf)
{
    return (_CLIB_CIR_BUF_TOTAL_SIZE_ - CLibCirBufGetFreeSize(cBuf));
}

i32 ModAddIndex(i32 index, i32 addNum)
{
    return (index + addNum) % _CLIB_CIR_BUF_SIZE_;
}

#ifdef _CYGWIN_DBG_
#include <stdio.h>
void CLibCirBufPrint(const _MISC_CIR_BUF_T *cBuf)
{
    i32 i = 0;
    i32 index = 0;
    i32 buffedSize = 0;

    buffedSize = MiscCirBufGetSize(cBuf);
    
    printf("bufed%4d,", buffedSize);
    printf("free %4d:", GetFreeSize(cBuf));
    
    for(i = 0; i < buffedSize; i++)
    {
        index =  ModAddIndex(cBuf->head, i);
        printf("%02x,", cBuf->buf[index]);
    }
    printf("\n");
}
#endif
