/******************************************************************************
 *
 * 文件名  ： QtCom.h
 * 创建日期： 20190911
 * 版本号  ： v1.0 20190911
 * 文件描述： PSS Qt版本串口类,实现IIo接口
 * 其    他： 无
 * 修改日志： v1.0 20190911 初始版本
 *            v1.0 20191202 适配新版本IIo接口
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _COM_H_
#define _COM_H_

/************************************ 头文件 ***********************************/
#include "QtIo.h"

#include <QTimer>
#include <QObject>
#include <QString>
#include <QIODevice>
#include <QByteArray>
#include <QSerialPort>

/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class QtCom : public QtIo
{
    Q_OBJECT

    public:
        QtCom(const QString& name, int baudrate, const QString& comIdStr, QObject *parent = 0);         /* 构造 */
        ~QtCom(void);                                                                                   /* 析构 */
    
        virtual bool IsOpen(void) const;                                                                /* 判断IO是否打开 */
        virtual int  Open(void);                                                                        /* 打开 */
        virtual int  Close(void);                                                                       /* 关闭 */
    
    protected:
        virtual QIODevice *GetQIODevice(void) const;                                                    /* 获取Qt底层Io */
    
    private:
        QString      mName;                                                                             /* 串口名 */
        int          mBaudrate;                                                                         /* 串口波特率 */
    
        QSerialPort *mQSerialPort;                                                                      /* Qt串口对象 */
};
/*********************************** 工具函数 **********************************/




#endif // _COM_H_
