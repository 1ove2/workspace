/******************************************************************************
 *
 * 文件名  ： PssIDevice.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220112
 * 版本号  ： v1.0 20220112
 * 文件描述： Pss 设备接口(抽象)类 头文件
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_I_DEVICE_H_
#define _PSS_I_DEVICE_H_

/************************************ 头文件 ***********************************/
#include "PssTypeDef.h"

/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class PssIIo;
class PssIDevice
{
public:
    virtual ~PssIDevice(void) = 0;                                                                      /* 析构 */

    /* 通用指令(2400) */
    i32          GetIdn(char *idnBuf, i32 idnBufSize);                                                  /* 获取IDN */
    virtual i32  SetVSrc(u32 ch, bool isVSrc);                                                          /* 设置源 */
    virtual bool IsVSrc(u32 ch) const;                                                                  /* 获取源 */
    virtual i32  SetVRange(u32 ch, f64  rangeVal, bool  isFixed);                                       /* 设置电压量程 */
    virtual i32  GetVRange(u32 ch, f64 *rangeVal, bool *isFixed) const;                                 /* 获取电压量程 */
    virtual i32  SetIRange(u32 ch, f64  rangeVal, bool  isFixed);                                       /* 设置电流量程 */
    virtual i32  GetIRange(u32 ch, f64 *rangeVal, bool *isFixed) const;                                 /* 获取电流量程 */
    virtual i32  SetVVal(u32 ch, f64  val);                                                             /* 设置电压值 */
    virtual i32  GetVVal(u32 ch, f64 *val) const;                                                       /* 获取电压值 */
    virtual i32  SetIVal(u32 ch, f64  val);                                                             /* 设置电流值 */
    virtual i32  GetIVal(u32 ch, f64 *val) const;                                                       /* 获取电流值 */

protected:
    explicit PssIDevice(PssIIo *iio);                                                                   /* 构造 */
    /* 底层通信 */
    i32 FetchData(char *buf, i32 bufSize);                                                              /* 获取数据 */
    i32 RunCmd(const char *cmd, i32 cmdSize) const;                                                     /* 下行单向控制 指令 */
    i32 RunCmdWithResponse(char *response, i32 responseSize, const char *cmd, i32 cmdSize) const;       /* 双向指令 */

private:
    /* 增加复用性代码 */
    i32 SetRange(u32 ch, bool isV, f64 rangeVal, bool isFixed);                                         /* 设置电压(or 电流)量程 */
    i32 GetRange(u32 ch, bool isV, f64 *rangeVal, bool *isFixed) const;                                 /* 获取电压(or 电流)量程 */
    i32 SetVal(u32 ch, bool isV, f64 val);                                                              /* 设置电压(or 电流)值 */
    i32 GetVal(u32 ch, bool isV, f64 *val) const;                                                       /* 获取电压(or 电流)值 */
    i32 SetOutput(u32 ch, bool on) const;                                                               /* 输出 */

    PssIIo *mIIo;                                                                                       /* 通讯口 */
};

#endif // _PSS_I_DEVICE_H_

