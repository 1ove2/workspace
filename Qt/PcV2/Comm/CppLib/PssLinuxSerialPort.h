/******************************************************************************
 *
 * 文件名  ： PssLinuxSerialPort.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220111
 * 版本号  ： v1.0 20220111
 * 文件描述： Pss Linux平台串口 头文件
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_LINUX_SERIAL_PORT_H_
#define _PSS_LINUX_SERIAL_PORT_H_

/************************************ 头文件 ***********************************/
#include "PssTypeDef.h"
#include "PssIIo.h"

/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class PssLinuxSerialPort: public PssIIo
{
public:
    explicit PssLinuxSerialPort(void);                                                                  /* 构造 */
    ~PssLinuxSerialPort(void);                                                                          /* 析构 */

    virtual int Open(Mode mode);                                                                        /* 打开 */
    virtual int Close(void);                                                                            /* 关闭 */
    virtual bool IsOpenned(void) const;                                                                 /* 是否已打开? */
    virtual int Send(const char *buf, i32 size);                                                        /* 发送 */
    virtual int Recv(      char *buf, i32 size);                                                        /* 接收 */
    virtual int FlushOut(void);                                                                         /* 刷新发送缓冲 */
    virtual int ClearIn(void);                                                                          /* 清空接收缓冲 */

protected:

private:
};

#endif // _PSS_LINUX_SERIAL_PORT_H_

