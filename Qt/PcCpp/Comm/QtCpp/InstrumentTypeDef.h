/******************************************************************************
 *
 * 文件名  ： InstrumentTypeDef.h
 * 负责人  ： 彭鹏
 * 创建日期： 20200507
 * 版本号  ： v1.0 20200507
 * 文件描述： SX00 设备Dll 与 QtInstrument类交互类型
 * 其    他： 无
 * 修改日志： v1.0 20200507 初始版本
 *
  *
  *******************************************************************************/
 #ifndef _PSS_QT_INSTRUMENT_TYPE_DEF_H_
 #define _PSS_QT_INSTRUMENT_TYPE_DEF_H_
 #include <stdint.h>

/************************************ 头文件 ***********************************/

/*---------------------------------- 预处理区 ---------------------------------*/


/*********************************** 注释定位 **********************************/
                                                                                        /* 注释定位 */
/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/

/*********************************** 工具函数 **********************************/
typedef int32_t PssDevT;

typedef struct _PssInstrumentSrcInfoTag_
{
    bool   isVSrc;                                                                      /* 电压源标志 false表示电流源*/
    bool   isMeasureV;                                                                  /* 测量电压 */
    double srcVal;                                                                      /* 源值(V) */
    double lmtVal;                                                                      /* 限值(A) */
    double nplc;                                                                        /* nplc值 */
} PssInstrumentCfgInfo;                                                                 /* 仪表 配置信息 */

typedef enum _PssMeasureTypeTag_
{
    PssMeasureVolt      = 0,                                                            /* 测量电压 */
    PssMeasureCurr      = 1,                                                            /* 测量电流 */
    PssMeasureElec      = 2                                                             /* 测量电阻 */
} PssInstrumentMeasureType;                                                             /* 仪表 测量类型 */

#endif // _PSS_QT_INSTRUMENT_TYPE_DEF_H_