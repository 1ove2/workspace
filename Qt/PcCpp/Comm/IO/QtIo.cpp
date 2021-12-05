/******************************************************************************
 *
 * 文件名  ： QtIo.cpp
 * 创建日期： 20200103
 * 版本号  ： v1.0 20200103
 * 文件描述：  Qt IO 通用 接口
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/
 /*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "QtIo.h"
#include "ITool.h"
#include "Bytes.h"
#include "RstDef.h"

#include <QTime>
#include <QDebug>
#include <QThread>
#include <QObject>
#include <QDateTime>
#include <QIODevice>
#include <QCoreApplication>


/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
QtIo::QtIo(const QString &idStr, QObject *parent) :
    QObject(parent),
    mIdStr(idStr),
    mLeftBufSize(0),
    mIsReadyRead(false)
{
}

QtIo::~QtIo(void)
{
}

QString QtIo::GetIdStr(void) const
{
    return mIdStr;
}

int QtIo::Send(const char *buf, size_t size, int32_t timeout)
{

    if(0 == size)
    {
        return 0;
    }
    if(NULL == buf)
    {
        qCritical() << __FILE__ << __LINE__ << "send buf is null !";
        return RstParameterError;
    }
    
    int rst = FlushOut(timeout);
    if(RstSuccess != rst)
    {
        qCritical() << "write timeout !";
        /* 超时过多,不打印 */
        if(RstOptTimeoutError != rst)
        {
            qCritical() << __FILE__ << __LINE__ << " flush out failed" << timeout << size << rst;
        }
        return rst;
    }
    
    QIODevice *qIODevice = GetQIODevice();
    if(nullptr == qIODevice)
    {
        qCritical() << __FILE__ << __LINE__ << "IO is null !";
        return RstIOEmptyError;
    }
    if(!qIODevice->isWritable())
    {
        return RstIOBrokenErr;
    }
    qint64 writeBytesNums = qIODevice->write(buf, size);
    if(size != writeBytesNums)
    {
        qCritical() << __FILE__ << __LINE__ << " write failed: " << qIODevice->errorString() << " , to be wirte: " << size << ", return value: " << writeBytesNums;
        return RstWriteToIOError;
    }
    mLeftBufSize += size;
    //DbgPrint(mIdStr + " Send", buf, size, true);
    FlushOut();
    return writeBytesNums;
}

int QtIo::Recv(char *buf, size_t size, int32_t timeout)
{
    if(0 == size)
    {
        return 0;
    }
    if(NULL == buf)
    {
        qCritical() << __FILE__ << __LINE__ << "recv buf is null !";
        return RstParameterError;
    }

    /*
     * 此处修改为用户不想阻塞等待接收时，直接读取缓存，有数则返回，没有则返回0
     * 不用等待信号槽返回导致接收效率较低，因为串口数据缓存可能达到一定字节数才会
     * 发射信号，此机制与用户想法不一致。
     */
    if(0 == timeout)
    {
        mIsReadyRead = true;
    }
    /* step1: 等待数据就绪 */
    int rst = WaitDataReady(timeout);
    if(RstSuccess != rst)
    {
        /* 超时过多,不打印 */
        if(RstOptTimeoutError != rst)
        {
            qCritical() << __FILE__ << __LINE__ << " Wait Data Ready failed" << timeout << rst;
        }
        return rst;
    }


    QIODevice *qIODevice = GetQIODevice();
    if(nullptr == qIODevice)
    {
        //qCritical() << __FILE__ << __LINE__ << "IO is null !";
        return RstIOEmptyError;
    }
    if(!qIODevice->isReadable())
    {
        return RstIOBrokenErr;
    }
    
    /* step2: 读取数据 */
    QByteArray data = qIODevice->read(size);
    if(data.isEmpty())
    {
        //qCritical() << __FILE__ << __LINE__ << " read failed: " << qIODevice->errorString() << size;
        return RstReadFromIOError;
    }
    
    const char *dataBuf = nullptr;
    dataBuf = data.constData();
    rst = data.size();
    memcpy(buf, dataBuf, rst);
    DbgPrint(mIdStr + " Recv", buf, rst, true);
    return rst;
}

int QtIo::FlushOut(int32_t timeout)
{
    int rst = BlockUntilReady(timeout, true);
    return rst;
}

int QtIo::WaitDataReady(int32_t timeout)
{
    int rst = BlockUntilReady(timeout, false);
    return rst;
}

int QtIo::BlockUntilReady(int32_t timeout, bool isSend)
{
    bool isReady = false;
    QTime timer = QTime::currentTime();
    int timeElapsed = 0;
    timer.start();
    while(1)
    {
        QCoreApplication::processEvents();
        if(isSend)
        {
            isReady = (0 == mLeftBufSize);
        }
        else
        {
            isReady = mIsReadyRead;
        }

        if(isReady) /* 底层就绪 */
        {
            break;
        }
    
        /* 超时 */
        timeElapsed = timer.elapsed();
        if(timeElapsed > timeout)
        {
#if 0
            qCritical() << __FILE__ << __LINE__ << "timeout:"
                     << "timeElapsed:" << timeElapsed
                     << "timeout:" << timeout
                     << "mIsReadyRead:" << mIsReadyRead
                     << "mLeftBufSize:" << mLeftBufSize;
#endif

            return RstOptTimeoutError;
        }
#if 0
        qCritical().noquote() << "timeElapsed:" << timeElapsed
                           << "timeout:" << timeout
                           << "mIsReadyRead:" << mIsReadyRead
                           << "mLeftBufSize:" << mLeftBufSize;
#endif

    }
#if 0
    qCritical() << "FlushWithTimeout Done:" << "timeElapsed:" << timeElapsed
             << "timeout:" << timeout
             << "mIsReadyRead:" << mIsReadyRead
             << "mLeftBufSize:" << mLeftBufSize
             << "\n";
#endif

    if(isSend)
    {
        mLeftBufSize = 0;
    }
    else
    {
        mIsReadyRead = false;
    }
    
    return RstSuccess;
}

int QtIo::FlushOut(void)
{
    int rst = RstSuccess;

    do {
        rst = FlushOut(1);
    } while(RstSuccess != rst);
    
    return RstSuccess;
}

int QtIo::ClearIn(void)
{
    QIODevice *qIODevice = GetQIODevice();
    if(nullptr == qIODevice)
    {
        qCritical() << __FILE__ << __LINE__ << "IO is null !";
        return RstParameterError;
    }

    if(qIODevice->bytesAvailable()) /* 有数据,读取并丢弃 */
    {
        qIODevice->readAll();
    }
    
    return RstSuccess;
}

void QtIo::BytesWritten(qint64 bytes)
{
    //qDebug() << "BytesWritten signal: " << bytes;
    if(mLeftBufSize != bytes)
    {
        qCritical() << __FILE__ << __LINE__ << "BytesWritten mLeftBufSize != bytes." << mLeftBufSize << bytes;
    }
    mLeftBufSize -= bytes;
    if(mLeftBufSize < 0)
    {
        qCritical() << __FILE__ << __LINE__ << "BytesWritten Set mLeftBufSize < 0.";
    }
}

void QtIo::ReadyRead(void)
{
    //qDebug() << "ReadyRead signal !" ;

    mIsReadyRead = true;
}

void QtIo::DbgPrint(const QString &head, const char *buf, size_t size, bool done) const
{
#if 1

#else
    Bytes bytes;
    bytes.Clear();
    bytes.Append((const uint8_t *)buf, size);

    /* 美化调试打印 */
    QString doneStr;
    if(done)
    {
        doneStr = QString("true :");
    }
    else
    {
        doneStr = QString("false:");
    }
    
    QString timeStr = GetNowDateTimeStr();
    QString dbgStr = QString("%1 [%2] %3 %4").arg(head).arg(timeStr).arg(doneStr).arg(bytes.ToQString());
    
    qDebug () << dbgStr;
#endif
}

void QtIo::DbgPrint(const QString &head, const QByteArray &bytes, bool done) const
{
    DbgPrint(head, bytes.constData(), bytes.size(), done);
}
