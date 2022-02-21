/******************************************************************************
 *
 * 文件名  ： QtIo.h
 * 创建日期： 20200103
 * 版本号  ： v1.0 20200103
 * 文件描述： Qt IO抽象类
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _IO_H_
#define _IO_H_

/************************************ 头文件 ***********************************/
#include "IIo.h"
#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>

/*********************************** 命名空间 **********************************/
class QIODevice;

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class QtIo : public QObject, public IIo
{
    Q_OBJECT

    public:
        QtIo(const QString &idStr, QObject *parent = 0);                                                /* 构造 */
        virtual ~QtIo(void);                                                                            /* 析构 */
    
        virtual bool IsOpen(void) const = 0;                                                            /* 判断IO是否打开 */
    
        virtual int Send(const char *buf, size_t size, int32_t timeout);                                /* 发送 */
        virtual int Recv(char *buf, size_t size, int32_t timeout);                                      /* 接收 */
        virtual int FlushOut(void);                                                                     /* 刷新发送缓冲 */
        virtual int ClearIn(void);                                                                      /* 清空接收缓冲 */
    
        bool IsReadyRead(void) const;                                                                   /* 有数据 可读 */
        qint64 GetLeftBufSize(void) const;                                                              /* 缓存 待 发送的字节数 */
        QString GetIdStr(void) const;                                                                   /* 获取IO 名 */
    
    protected:
        int FlushOut(int32_t timeout);                                                                  /* 刷新发送缓存 */
        int WaitDataReady(int32_t timeout);                                                             /* 等待数据就绪 */
        int BlockUntilReady(int32_t timeout, bool isSend);                                              /* 等待QIODevice就绪 */
    
        virtual QIODevice *GetQIODevice(void) const = 0;                                                /* 获取Qt底层Io */
        void DbgPrint(const QString &head, const char *buf, size_t size, bool done) const;              /* 调试打印 */
        void DbgPrint(const QString &head, const QByteArray &bytes, bool done) const;                   /* 调试打印 */


    protected slots:
        void BytesWritten(qint64 bytes);                                                                /* 发送 完成 槽 */
        virtual void ReadyRead(void);                                                                           /* 接收 到达 槽 */
    
    private:
        QString mIdStr;                                                                                 /* IO 名 */
    
        /* 用于实现阻塞 */
        qint64 mLeftBufSize;                                                                            /* 缓存 待 发送的字节数 */
        bool   mIsReadyRead;                                                                            /* 有数据 可读 */
};
/*********************************** 工具函数 **********************************/


#endif // _IO_H_
