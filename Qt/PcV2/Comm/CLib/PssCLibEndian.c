/******************************************************************************
 *
 * 文件名  ： PssCLibEndian.c
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20191205 
 * 版本号  ： v1.0
 * 文件描述： Pss 大小端转换算法
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "PssCLibEndian.h"
#include <string.h>

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
/* 检测大小端 */
static bool IsBigEndian(void)
{
    u16 tempInt = 0x0102;
    u8  *ptr    = (u8 *)(&tempInt);

    if(0x01 == *ptr)    /* 高位在前 大端 */
    {
        return TRUE;
    }
    else                /* 低位在前 小端*/
    {
        return FALSE;
    }
}

static void Swap(void *dest, const void *src, u32 size)
{
    uint8_t *ptrDest         = (uint8_t *)dest;
    const uint8_t *ptrSrc    = (const uint8_t *)src;

    u32 i = 0;
    for(i = 0; i < size; i++)
    {
        ptrDest[i] = ptrSrc[size - i - 1];
    }
}

/* 网络序 是 大端 */
void PssCLibHton(void *dest, const void *src, u32 size)
{
    PssCLibHtob(dest, src, size);
}

void PssCLibNtoh(void *dest, const void *src, u32 size)
{
    PssCLibBtoh(dest, src, size);
}

void PssCLibHtob(void *dest, const void *src, u32 size)
{
    /* 单字节无大小端 */
    if((size <= 1) || (IsBigEndian()))
    {
        memmove(dest, src, size);
    }
    else
    {
        Swap(dest, src, size);
    }
}

void PssCLibBtoh(void *dest, const void *src, u32 size)
{
    /* 单字节无大小端 */
    if((size <= 1) || (IsBigEndian()))
    {
        memmove(dest, src, size);
    }
    else
    {
        Swap(dest, src, size);
    }
}

void PssCLibHtol(void *dest, const void *src, u32 size)
{
    /* 单字节无大小端 */
    if((size <= 1) || (!IsBigEndian()))
    {
        memmove(dest, src, size);
    }
    else
    {
        Swap(dest, src, size);
    }
}

void PssCLibLtoh(void *dest, const void *src, u32 size)
{
    /* 单字节无大小端 */
    if((size <= 1) || (!IsBigEndian()))
    {
        memmove(dest, src, size);
    }
    else
    {
        Swap(dest, src, size);
    }
}

