/********************************************************************************
 *
 * 文件名  : PssCLibCirBuf.c
 * 负责人  : PengPeng
 * 创建日期: 2019.05.12
 * 版本号  : v1.1
 * 文件描述: Pss 环形缓冲 实现
 * 其    他: None
 * 修改日志: 2021.05.12 PengPeng     初始版本                   v1.0
 *           2021.07.20 PengPeng     完善设计                   v1.1
 *           2021.09.02 PengPeng     实现字节读写(优化性能)     v1.2
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
/************************************ 头文件 ***********************************/
#include <string.h>

#include "PssCLibCirBuf.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/
static u32 ModAddIndex(u32 index, u32 addNum);

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
void PssCLibCirBufInit(PSS_CLIB_CIR_BUF_T *cBuf)
{
    memset(cBuf->buf, 0x00, _PSS_CLIB_CIR_BUF_SIZE_);

    cBuf->head = 0;
    cBuf->tail = 0;
}

i32 PssCLibCirBufWrite(PSS_CLIB_CIR_BUF_T *cBuf, const u8 *buf, u32 len)
{
    u32 i = 0;
    u32 freeSize = 0;
    u32 index = 0;
    u32 willWriteSize = 0;

    /* step1: 检查能存入的数据 */
    freeSize = PssCLibCirBufGetFreeSize(cBuf);

    /* step2: 求可以写入的大小 */
    if(len < freeSize)
    {
        willWriteSize = len;
    }
    else                                            /* FIXME:出现溢出 */
    {
        willWriteSize = freeSize;
        return PssRstOverFlow;
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

i32 PssCLibCirBufRead(PSS_CLIB_CIR_BUF_T *cBuf, u8 *buf, u32 len)
{
    u32 i = 0;
    u32 buffedSize = 0;
    u32 index = 0;
    u32 willReadSize = 0;

    /* step1: 检查可读取的数据大小 */
    buffedSize = PssCLibCirBufGetValidSize(cBuf);
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
i32 PssCLibCirBufGetFreeSize(const PSS_CLIB_CIR_BUF_T *cBuf)
{
    if(cBuf->tail == cBuf->head)
    {
        return _PSS_CLIB_CIR_BUF_TOTAL_SIZE_;
    }

    return (cBuf->head + _PSS_CLIB_CIR_BUF_TOTAL_SIZE_ - cBuf->tail) % _PSS_CLIB_CIR_BUF_SIZE_;
}

bool PssCLibCirBufIsFull(const PSS_CLIB_CIR_BUF_T *cBuf)
{
    bool isFull = (0 == PssCLibCirBufGetFreeSize(cBuf));
    return isFull;
}

bool PssCLibCirBufIsEmpty(const PSS_CLIB_CIR_BUF_T *cBuf)
{
    bool isEmpty = (0 == PssCLibCirBufGetValidSize(cBuf));
    return isEmpty;
}

i32 PssCLibCirBufClearFromHead(PSS_CLIB_CIR_BUF_T *cBuf, u32 clearSize)
{
    u32 validSize = PssCLibCirBufGetValidSize(cBuf);
    u32 realClearSize = (validSize < clearSize) ? (validSize) : (clearSize);

    /* 更新头指针 等效于 删除 */
    cBuf->head = ModAddIndex(cBuf->head, realClearSize);

    return PssRstSuccess;
}

/* totalSize = size + freeSize */
/* 故 size = totalSize - freeSize */
i32 PssCLibCirBufGetValidSize(const PSS_CLIB_CIR_BUF_T *cBuf)
{
    return (_PSS_CLIB_CIR_BUF_TOTAL_SIZE_ - PssCLibCirBufGetFreeSize(cBuf));
}

u32 ModAddIndex(u32 index, u32 addNum)
{
    return (index + addNum) % _PSS_CLIB_CIR_BUF_SIZE_;
}

PssRstT PssCLibCirBufWriteByte(PSS_CLIB_CIR_BUF_T *cBuf, u8  byte)
{
    /* step1: 检查是否溢出,见PssCLibCirBufGetFreeSize */
    if(cBuf->head == (cBuf->tail + 1))
    {
        return PssRstOverFlow;
    }

    /* step2: 写入 */
    u32 tail = cBuf->tail;
    cBuf->buf[tail] = byte;
    cBuf->tail = (tail + 1) % _PSS_CLIB_CIR_BUF_SIZE_;

    return PssRstSuccess;
}

PssRstT PssCLibCirBufReadByte(PSS_CLIB_CIR_BUF_T *cBuf,  u8 *byte)
{
    /* step1: 检查是否空,见PssCLibCirBufGetFreeSize */
    if(cBuf->head == cBuf->tail)
    {
        return PssRstBufIsNotEnough;
    }

    u32 head = cBuf->head;
    *byte = cBuf->buf[head];
    u32 newHead = (head + 1) % _PSS_CLIB_CIR_BUF_SIZE_;
    cBuf->head = newHead;

    return PssRstSuccess;
}

#ifdef _LINUX_DBG_
#include <stdio.h>
void PssCLibCirBufPrint(const PSS_CLIB_CIR_BUF_T *cBuf)
{
    u32 i = 0;
    u32 index = 0;
    u32 buffedSize = 0;

    buffedSize = PssMiscCirBufGetSize(cBuf);

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

