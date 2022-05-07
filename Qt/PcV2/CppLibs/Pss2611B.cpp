﻿/******************************************************************************
 *
 * 文件名  ： Pss2611B.cpp
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220406
 * 版本号  ： v1.0 20220406
 * 文件描述： Pss 2611B设备类 实现
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "PssRstDef.h"
#include "PssCppLog.h"
#include "Pss2611B.h"

/*********************************** 命名空间 **********************************/

/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
Pss2611B::Pss2611B(PssIIo *iio) :
    PssStdDevice(iio)
{
}

Pss2611B::~Pss2611B(void)
{
}