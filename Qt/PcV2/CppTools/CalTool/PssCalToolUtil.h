/********************************************************************************
*
* 文件名  ： PssCalToolUtil.h
* 负责人  ： sls
* 创建日期： 20220317
* 版本号  ： v1.0
* 文件描述： Pss校准工具数据定义文件
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_CALTOOL_UTIL_H_
#define _PSS_CALTOOL_UTIL_H_

/************************************ 头文件 ***********************************/
#include <QString>
#include <QStringList>
#include <QVector>
/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/*********************************** 枚举定义 **********************************/
enum CalItem{
   CAL_VOLT_DAC   = 1,                                                                                  /* 校准V路DAC */
   CAL_CURR_DAC   = 2,                                                                                  /* 校准I路DAC */
   CAL_VOLT_ADC   = 4,                                                                                  /* 校准V路ADC */
   CAL_CURR_ADC   = 3,                                                                                  /* 校准I路ADC */
   CAL_RESISTANCE = 5,                                                                                  /* 校准电阻 */
};


struct PSS_DEVICE_SCPI
{
    QString SET_SOUR;                                                                                   /* 选择源指令 */
    QString SET_SOUR_RANGE;                                                                             /* 设置源量程 */
    QString SET_LIMT_RANGE;                                                                             /* 设置限值量程 */
    QString SET_SOUR_LEV;                                                                               /* 设置源值 */
    QString SET_LIMT_LEV;                                                                               /* 设置限值 */
    QString SET_NPLC;                                                                                   /* 设置NPLC */
    QString SET_OUTP;                                                                                   /* 开输出 */
};

struct PSS_DEVICE_CFG
{
    QVector<double>     PssVRanges;                                                                     /* 电压挡位 */
    QVector<double>     PssVIndex;                                                                      /* 电压Index */
    QVector<double>     PssIRanges;                                                                     /* 电压挡位 */
    QVector<int>        PssIIndex;                                                                      /* 电流Index */
    QVector<double>     PssVLimit;                                                                      /* 电压限值 */
    QVector<double>     PssILimit;                                                                      /* 电流限值 */
    PSS_DEVICE_SCPI     PssDevScpi;                                                                     /* SCPI指令集 */
    QVector<CalItem>    PssCalItem;                                                                     /* 校准项 */
};


struct STD_DEVICE_SCPI
{
    QString SET_SOUR;                                                                                   /* 选择源指令 */
    QString SET_SOUR_RANGE;                                                                             /* 设置源量程 */
    QString SET_LIMT_RANGE;                                                                             /* 设置限值量程 */
    QString SET_SOUR_LEV;                                                                               /* 设置源值 */
    QString SET_LIMT_LEV;                                                                               /* 设置限值 */
    QString SET_NPLC;                                                                                   /* 设置NPLC */
    QString SET_OUTP;                                                                                   /* 开输出 */
};

struct STD_DEVICE_CFG
{
    QStringList         StdVRanges;                                                                     /* 电压挡位 */
    QStringList         StdIRanges;                                                                     /* 电压挡位 */
    STD_DEVICE_SCPI     StdDevScpi;                                                                     /* SCPI指令集 */
};
/************************************ 类定义 ***********************************/


#endif // _PSS_CALTOOL_UTIL_H_
