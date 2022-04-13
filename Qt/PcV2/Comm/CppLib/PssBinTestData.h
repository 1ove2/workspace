/******************************************************************************
 *
 * 文件名  ： PssBinTestData.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220301
 * 版本号  ： v1.0 20220301
 * 文件描述： PssBin 协议测试数据 头文件
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_BIN_TEST_DATA_H_
#define _PSS_BIN_TEST_DATA_H_

/************************************ 头文件 ***********************************/
#include "PssTypeDef.h"

#if _PSS_QT_
#include <QString>
#endif

#include <stdio.h>
#include <vector>

/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class PssBinTestData
{
public:
    explicit PssBinTestData(u8 destAddr, const std::vector<char> &appData);                             /* 发送构造 */
    explicit PssBinTestData(const char *rxBuf, i32 rxBufSize);                                          /* 接收构造 */
    ~PssBinTestData(void);                                                                              /* 析构 */

    const char *AllData(void) const;                                                                    /* 帧字节流指针 */
    i32  AllSize(void) const;                                                                           /* 帧字节流大小 */
    const char *AppData(void) const;                                                                    /* 应用层字节流指针 */
    i32  AppSize(void) const;                                                                           /* 应用层字节流大小 */
    u32  GetId(void) const;                                                                             /* 获取ID */
    bool IsCheckSumPassed(void) const;                                                                  /* 检查校验和 */
    void Print(FILE *f = stdout);                                                                       /* 调试打印 */
    bool IsValid(void) const;                                                                           /* 合法帧? */

#if _PSS_QT_
    QString ToQString(void) const;                                                                      /* 转换为QString,用于调试 */
#endif

protected:

private:

private:
    /* Head(3) + Type(1) + destAddr(1) + srcAddr(1) + Len(3) + App(x) + Tail(4)  */
    u8         mType;                                                                                   /* 帧类型 */
    u8         mDestAddr;                                                                               /* 目的地址 */
    u8         mSrcAddr;                                                                                /* 源地址 */
    u32        mFrmLen;                                                                                 /* 帧长度 */
    std::vector<char> mData;                                                                            /* 帧字节流 */

    u32        mFilledLen;                                                                              /* 3Bytes对齐 填充长度 */

    /* 下面两个字段位于 mAppData中 */
    u32        mId;                                                                                     /* 帧id */
    u32        mCheckSum;                                                                               /* 校验合 */

    bool       mIsValid;                                                                                /* 合法帧 */
};

bool PssBinTestDataCmpl(const PssBinTestData *sendData, const PssBinTestData *recvData);                /* 比较PssBinV3数据 */
void PrintBytes(FILE *f, const char *buf, i32 bufSize);                                                 /* 打印字节流 */

#endif // _PSS_BIN_TEST_DATA_H_

