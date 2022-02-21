#include "QPssLinuxSerialPort.h"

#include "ITool.h"
#include "PssRstDef.h"

#ifdef Q_OS_LINUX
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include<sys/time.h>
#include <sys/select.h>
#endif

using namespace Pss::Qt;

QPssLinuxSerialPort::QPssLinuxSerialPort(const QString &commName, int commBaud, const QString &idStr):
    mCommFd(-1),
    mCommName(commName),
    mCommBaud(commBaud),
    mCommIdStr(idStr)
{

}

QPssLinuxSerialPort::~QPssLinuxSerialPort(void)
{
    close();
}

bool QPssLinuxSerialPort::open(int mode)
{
#ifdef Q_OS_LINUX
    //QIODevice::open((OpenMode)mode);
    char name[128] = {0};
    int flags = 0;
    if(mode == QIODevice::ReadOnly)
    {
        qDebug() << "ReadOnly";
        flags |= O_RDONLY;
    }
    else if(mode == QIODevice::WriteOnly)
    {
        qDebug() << "WriteOnly";
        flags |= O_WRONLY;
    }
    else if(mode == QIODevice::ReadWrite)
    {
        qDebug() << "ReadWrite";
        flags |= O_RDWR;
    }

    if(mode == QIODevice::Append)
    {
        qDebug() << "Append";
        flags |= O_APPEND;
    }
    else if(mode == QIODevice::Truncate)
    {
        qDebug() << "Truncate";
        flags |= O_TRUNC;
    }
    
    snprintf(name, sizeof(name), "%s", mCommName.toStdString().c_str());
    mCommFd = ::open(name, flags| O_NOCTTY | O_NONBLOCK);
    if(mCommFd < 0)
    {
        qCritical() << "Open Comm " << name << "failed: " << strerror(errno);
        return false;
    }
    
    if(CommConfig(mCommFd))
    {
        close();
        qCritical() <<"Config comm: " << mCommIdStr << " failed !";
        return false;
    }
    qDebug() << "open comm: " << name << " success, file fd = " << mCommFd;
#else
    PSS_UNUSED(mode);
#endif
    return true;
}

void QPssLinuxSerialPort::close(void)
{
#ifdef Q_OS_LINUX
    if(mCommFd < 0)
    {
        return ;
    }
    ::close(mCommFd);
    mCommFd = -1;
#endif
}

bool QPssLinuxSerialPort::isOpen(void) const
{
    if(mCommFd > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

qint64 QPssLinuxSerialPort::readData(char *data, qint64 maxlen, int timeout)
{
#ifdef Q_OS_LINUX
    if(timeout > 0)
    {
        //struct  timeval    tv;
        //struct  timezone   tz;
        //gettimeofday(&tv,&tz);

        struct timeval tmout;
        tmout.tv_sec = 0;
        tmout.tv_usec = 0;
        if(timeout > 0)
        {
            tmout.tv_usec += timeout * 1000;
        }
        //int select(int maxfdp,fd_set *readfds,fd_set *writefds,fd_set *errorfds,struct timeval *timeout);
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(mCommFd, &readset);
        int ret = select(mCommFd + 1, &readset, NULL, NULL, &tmout);
        if(ret < 0)
        {
            qCritical() << "read failed: " << strerror(errno);
            return PssRstWriteToIOError;
        }
        else if(0 == ret)
        {
            //qCritical() << "read select timeout !";
            return PssRstOptTimeoutError;
        }
    }
    int readBytes = ::read(mCommFd, data, maxlen);
#if 0
    if(readBytes < 0)
    {
        qCritical() << "comm name:" << mCommName << ", read fd = "  << mCommFd << ", error return = " << readBytes;
    }
#endif
    return readBytes;
#else
    PSS_UNUSED(timeout);
    PSS_UNUSED(data);
    PSS_UNUSED(maxlen);
    return 0;
#endif
}

qint64 QPssLinuxSerialPort::writeData(const char *data, qint64 len, int timeout)
{
#ifdef Q_OS_LINUX
    if(timeout > 0)
    {
        //struct  timeval    tv;
        //struct  timezone   tz;
        //gettimeofday(&tv,&tz);

        struct timeval tmout;
        tmout.tv_sec = 0;
        tmout.tv_usec = 0;
    
        tmout.tv_usec += timeout * 1000;
        //int select(int maxfdp,fd_set *readfds,fd_set *writefds,fd_set *errorfds,struct timeval *timeout);
        fd_set writeset;
        FD_ZERO(&writeset);
        FD_SET(mCommFd, &writeset);
        int ret = select(mCommFd + 1, NULL, &writeset, NULL, &tmout);
        if(ret < 0)
        {
            qCritical() << "write failed: " << strerror(errno);
            return PssRstWriteToIOError;
        }
        else if(0 == ret)
        {
            return PssRstOptTimeoutError;
        }
    
        if(FD_ISSET(mCommFd, &writeset))
        {
            int writeBytes = ::write(mCommFd, data, len);
            if(writeBytes < 0)
            {
                //flush(mCommFd);
                qCritical() << "write to fd: " << mCommFd << " : " << writeBytes << " :  " << strerror(errno);
            }
    
            return writeBytes;
        }
        else
        {
            qCritical() << "write timeout !";
            return PssRstOptTimeoutError;
        }
    }
    else
    {
        int writeBytes = ::write(mCommFd, data, len);
        if(writeBytes < 0)
        {
            //flush(mCommFd);
            qCritical() << "write to fd: " << mCommFd << " : " << writeBytes << " :  " << strerror(errno);
        }
    
        return writeBytes;
    }

#else
    PSS_UNUSED(timeout);
    PSS_UNUSED(data);
    PSS_UNUSED(len);
    return 0;
#endif
}

#if 0
bool QPssLinuxSerialPort::waitForReadyRead(int msecs)
{
    PSS_UNUSED(msecs);
    return true;
}

bool QPssLinuxSerialPort::waitForBytesWritten(int msecs)
{
    PSS_UNUSED(msecs);
    return true;
}

qint64 QPssLinuxSerialPort::bytesAvailable(void) const
{
    return 0;
}

qint64 QPssLinuxSerialPort::bytesToWrite(void) const
{
    return 0;
}
#endif

int QPssLinuxSerialPort::CommConfig(int commFd)
{
#ifdef Q_OS_LINUX
    struct termios tty;
    struct termios tty_old;
    memset (&tty, 0, sizeof(tty));

    /* 获取串口默认配置项 */
    if ( 0 != tcgetattr (commFd, &tty ))
    {
        qCritical() << "Error: " << errno << " from tcgetattr: " << strerror(errno);
        return PssRstUartConfigError;
    }
    
    /* 备份配置项 */
    tty_old = tty;
    
    /* 默认115200 */
    /* 设置波特率 */
    if(115200 == mCommBaud)
    {
        cfsetospeed (&tty, (speed_t)B115200);
        cfsetispeed (&tty, (speed_t)B115200);
    }
    else if(9600 == mCommBaud)
    {
        cfsetospeed (&tty, (speed_t)B9600);
        cfsetispeed (&tty, (speed_t)B9600);
    }
    
    /* 设置串口属性 */
    tty.c_cflag     &=  ~PARENB;            // Make 8n1
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;
    
    tty.c_cflag     &=  ~CRTSCTS;           // 无流控
    tty.c_cc[VMIN]   =  0;                  // 接收缓存阻塞至少0字节
    tty.c_cc[VTIME]  =  0;                  // 接收超时等待0.5ms
    tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
    
    /* 设置终端属性 */
    cfmakeraw(&tty);
    
    /* 刷新串口设置 */
    tcflush(commFd, TCIFLUSH );
    if ( 0 != tcsetattr ( commFd, TCSANOW, &tty ) )
    {
        tcsetattr ( commFd, TCSANOW, &tty_old);
        qCritical() << "Error: " << errno << " from tcsetattr: " << strerror(errno);
        return PssRstUartConfigError;
    }

#else
    PSS_UNUSED(commFd);
#endif
    return PssRstSuccess;
}