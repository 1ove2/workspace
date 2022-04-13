/******************************************************************************
 *
 * 文件名  ： PssWinSerialPort.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220110
 * 版本号  ： v1.0 20220110
 * 文件描述： Pss Windows平台串口 头文件
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_WIN_SERIAL_PORT_H_
#define _PSS_WIN_SERIAL_PORT_H_

/************************************ 头文件 ***********************************/
#include "PssIIo.h"


#include <windows.h>

/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class PssWinSerialPort: public PssIIo
{
public:
    explicit PssWinSerialPort(const char *name, i32 baudrate);                                          /* 构造 */
    ~PssWinSerialPort(void);                                                                            /* 析构 */

    virtual int Open(Mode mode);                                                                        /* 打开 */ 
    virtual int Close(void);                                                                            /* 关闭 */
    virtual bool IsOpenned(void) const;                                                                 /* 是否已经打开 */
    virtual int Send(const char *buf, i32 size);                                                        /* 发送 */
    virtual int Recv(char *buf, i32 size);                                                              /* 接收 */
    virtual int FlushOut(void);                                                                         /* 刷新发送缓冲 */
    virtual int ClearIn(void);                                                                          /* 清空接收缓冲 */

protected:

private:
    HANDLE      mCom;                                                                                   /* 串口句柄 */
    DCB         mDcb;                                                                                   /* 串口参数 */
    char        mName[32];                                                                              /* 串口名字 */
    i32         mBaudrate;                                                                              /* 串口波特率 */
};

#endif // _PSS_WIN_SERIAL_PORT_H_

