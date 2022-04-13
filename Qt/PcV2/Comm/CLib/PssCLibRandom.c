/******************************************************************************
 *
 * 文件名  ： PssCLibRandom.c
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20210706
 * 版本号  ： v1.0
 * 文件描述： 随机 相关算法 实现
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include <stdio.h>
#include <stdlib.h>
#include "PssCLibRandom.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
i32 PssCLibGetRandMax(void)
{
    return RAND_MAX;
}

i32 PssCLibRandom(void)
{
    return rand();
}

i32 PssCLibRandomRange(i32 min, i32 max)
{
    i32 range = max - min;
    i32 mod = range + 1;

    /* step1: [0, RandMax] */
    i32 rand = PssCLibRandom();

    /* step2: [0, range] */
    rand = rand % mod;

    /* step3: [min, max] */
    rand = rand + min;

    return rand;
}

bool PssCLibRandomRateTrue(f32 rate)
{
    bool isTrue = FALSE;

    if((0 == rate) || ((1 / rate) > PssCLibGetRandMax()))
    {
        fprintf(stderr,"%s,%d:err para:%g.\n", __FILE__, __LINE__, rate);
    }

    /* [0, 1 / rate] */
    i32 max = (i32)(1 / rate);
    i32 rand = PssCLibRandom() % max;
    if(0 == rand)
    {
        isTrue = TRUE;
    }

    /* fprintf(stdout,"%s,%d:isTrue:%d, max:%d,rand:%d.\n", __FILE__, __LINE__, isTrue, max, rand); */
    return isTrue;
}

