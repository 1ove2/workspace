/************************************************************************************************
 *
 * 文件名  : PssCLibLog.h
 * 负责人  : ZhanWei
 * 创建日期: 2021.07.14
 * 版本号  : v1.2
 * 文件描述: 日志模块 实现
 * 其    他: None
 * 修改日志: 2021.07.14 ZhanWei      初始版本       v1.0
 *           2021.07.19 PengPeng     完善设计       v1.1
 *           2021.07.22 PengPeng     移到Comm目录   v1.2
 *
 *************************************************************************************************/

/*-------------------------------------- 预处理区 -----------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _PSS_CLIB_LOG_H_
#define _PSS_CLIB_LOG_H_

/************************************** 头文件区 *************************************************/
#include "PssRstDef.h"
#include "PssTypeDef.h"

/************************************** 宏定义区 *************************************************/
#define _PSS_CLIB_LOG_LAST_BUF_SIZE_                    (128)

/* 错误级别日志 */
#define PssCLibLogE(log, ...)                           PssCLibLogPrint(PSS_CLIB_LOG_LEVEL_ERROR,   log, 0, __FILE__, __LINE__, ##__VA_ARGS__)
#define PssCLibLogEL(log, ...)                          PssCLibLogPrint(PSS_CLIB_LOG_LEVEL_ERROR,   log, 1,  __FILE__, __LINE__, ##__VA_ARGS__)

/* 警告级别日志 */
#define PssCLibLogW(log, ...)                           PssCLibLogPrint(PSS_CLIB_LOG_LEVEL_WARNING, log, 0, __FILE__, __LINE__, ##__VA_ARGS__)
#define PssCLibLogWL(log, ...)                          PssCLibLogPrint(PSS_CLIB_LOG_LEVEL_WARNING, log, 1, __FILE__, __LINE__, ##__VA_ARGS__)

/* 提示级别日志 */
#define PssCLibLogI(log, ...)                           PssCLibLogPrint(PSS_CLIB_LOG_LEVEL_INFO,    log, 0,__FILE__, __LINE__, ##__VA_ARGS__)
#define PssCLibLogIL(log, ...)                          PssCLibLogPrint(PSS_CLIB_LOG_LEVEL_INFO,    log, 1, __FILE__, __LINE__, ##__VA_ARGS__)

/* 调试级别日志 */
#define PssCLibLogD(log, ...)                           PssCLibLogPrint(PSS_CLIB_LOG_LEVEL_DBG,     log, 0,__FILE__, __LINE__, ##__VA_ARGS__)
#define PssCLibLogDL(log, ...)                          PssCLibLogPrint(PSS_CLIB_LOG_LEVEL_DBG,     log, 1, __FILE__, __LINE__, ##__VA_ARGS__)

/* 日志 带Bytes输出 */
#define PssCLibLogEBytes(log, buf, size, ...)           PssCLibLogPrintBytes(PSS_CLIB_LOG_LEVEL_ERROR,   log, buf, size, ##__VA_ARGS__)
#define PssCLibLogWBytes(log, buf, size, ...)           PssCLibLogPrintBytes(PSS_CLIB_LOG_LEVEL_WARNING, log, buf, size, ##__VA_ARGS__)
#define PssCLibLogIBytes(log, buf, size, ...)           PssCLibLogPrintBytes(PSS_CLIB_LOG_LEVEL_INFO,    log, buf, size, ##__VA_ARGS__)
#define PssCLibLogDBytes(log, buf, size, ...)           PssCLibLogPrintBytes(PSS_CLIB_LOG_LEVEL_DBG,     log, buf, size, ##__VA_ARGS__)

/************************************** 类型定义 ***********************************************/
typedef enum _PSS_CLIB_LOG_LEVEL_E_TAG_ {
    PSS_CLIB_LOG_LEVEL_DBG,                                                      /* 调试 */
    PSS_CLIB_LOG_LEVEL_INFO,                                                     /* 提示 */
    PSS_CLIB_LOG_LEVEL_WARNING,                                                  /* 警告 */
    PSS_CLIB_LOG_LEVEL_ERROR,                                                    /* 错误 */

    PSS_CLIB_LOG_LEVEL_CLOSED_ALL,                                               /* 关闭所有日志 */
    PSS_CLIB_LOG_LEVEL_MAX_NUM                                                   /* 日志级别计数 */
} PSS_CLIB_LOG_LEVEL_E;                                                          /* 日志等级 */

typedef struct _PSS_CLIB_LOG_TAG_ {
    void *handle;                                                               /* 物理层句柄 */
    i32  (*Output)(void *handle, const u8 *buf, u32 size);                      /* 输出 */
    u64  (*GetMs)(void);                                                        /* 获取Mcu时间(ms) */

    PSS_CLIB_LOG_LEVEL_E  level;                                                /* 过滤级别 */

    /* 调试信息过多,导致溢出 */
    u8   lastBuf[_PSS_CLIB_LOG_LAST_BUF_SIZE_];                                 /* 溢出前的最后输出 */
    bool isOutputOverFlow;                                                      /* 是否发生溢出 */
} PSS_CLIB_LOG_T;

/************************************* 函数声明区 **********************************************/
PssRstT PssCLibLogInit(PSS_CLIB_LOG_T *log);
PssRstT PssCLibLogUnInit(PSS_CLIB_LOG_T *log);
PssRstT PssCLibLogUpdate(PSS_CLIB_LOG_T *log);
PssRstT PssCLibLogSetLevel(PSS_CLIB_LOG_T *log, PSS_CLIB_LOG_LEVEL_E level);
/* mode：0：普通打印；1：文件名，行数打印 */
i32     PssCLibLogPrint(PSS_CLIB_LOG_LEVEL_E level, PSS_CLIB_LOG_T *log, int mode, const char *file, int line, const char *fmt, ...);
i32     PssCLibLogPrintBytes(PSS_CLIB_LOG_LEVEL_E level, PSS_CLIB_LOG_T *log, const u8 *bytesBuf, u32 bytesSize, const char *fmt, ...);

#endif /* _PSS_MCU_MDLS_LOG_H_ */

#ifdef __cplusplus
}
#endif
