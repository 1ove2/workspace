/******************************************************************************
 *
 * 文件名  ： PssStdDevice.cpp
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220407
 * 版本号  ： v1.0 20220407
 * 文件描述： 普赛斯 友商标准仪表类 实现
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "PssRstDef.h"
#include "PssCppLog.h"
#include "PssStdDevice.h"

/*********************************** 命名空间 **********************************/

/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
PssStdDevice::PssStdDevice(PssIIo *iio) :
    PssIDevice(iio)
{
}

PssStdDevice::~PssStdDevice(void)
{
}
