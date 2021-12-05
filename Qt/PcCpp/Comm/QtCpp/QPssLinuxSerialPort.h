/******************************************************************************
 *
 * 文件名  ： QPssLinuxSerialPort.h
 * 负责人  ： ryl
 * 创建日期： 20200806
 * 版本号  ： v1.0 20200806
 * 文件描述： PSS Linux版本下qt串口自定义实现头文件
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_QT_LINUX_SERIALPORT_H_
#define _PSS_QT_LINUX_SERIALPORT_H_

/************************************ 头文件 ***********************************/
#include <QIODevice>


/*********************************** 命名空间 **********************************/
namespace Pss {
namespace Qt {

/*********************************** 注释定位 **********************************/
                                                                                                            /* 注释定位 */

/*********************************** 宏定义 ************************************/

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/

class QPssLinuxSerialPort
{
    public:
        QPssLinuxSerialPort(const QString &commName, int commBaud, const QString &idStr);
        virtual ~QPssLinuxSerialPort(void);

        virtual bool open(int mode);                                                                   /* 打开串口 */
        virtual void close(void);                                                                           /* 关闭串口 */
        virtual bool isOpen(void) const;

#if 0
    protected:
#endif
        virtual qint64 readData(char *data, qint64 maxlen, int timeout = -1);
        virtual qint64 writeData(const char *data, qint64 len, int timeout = -1);
        //virtual bool waitForReadyRead(int msecs);
        //virtual bool waitForBytesWritten(int msecs);
        //virtual qint64 bytesAvailable(void) const;
        //virtual qint64 bytesToWrite(void) const;


    private:
        int CommConfig(int commFd);                                                                         /* 串口配置 */
        int         mCommFd;                                                                                /* 已打开串口文件描述符 */
        QString     mCommName;                                                                              /* 串口名 */
        int         mCommBaud;                                                                              /* 串口波特率 */
        QString     mCommIdStr;                                                                             /* 串口标识符  */

};

/*********************************** 工具函数 **********************************/

}
}

#endif // _PSS_QT_LINUX_SERIALPORT_H_