/******************************************************************************
 *
 * 文件名  ： QtCom.cpp
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20190911
 * 版本号  ： v1.0 20190911
 * 文件描述： PSS Qt版本串口类,实现IIo接口
 * 其    他： 无
 * 修改日志： v1.0 20190911 初始版本
 *            v1.0 20191202 适配新版本IIo接口
 *          v2.0  20200813  整理QtIo继承体系
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "QtCom.h"
#include "RstDef.h"
#include "ITool.h"

#include <QDebug>

/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
QtCom::QtCom(const QString& name, int baudrate, const QString& comIdStr, QObject *parent) :
    QtIo(comIdStr, parent),
    mName(name),
    mBaudrate(baudrate),
    mQSerialPort(nullptr)
{
    Open();
}

QtCom::~QtCom(void)
{
    Close();
}

int QtCom::Open(void)
{
    /* 初始化串口 */
    mQSerialPort = new QSerialPort();

    /* 打开串口 */
    mQSerialPort->setPortName(mName);
    bool success = mQSerialPort->open(QIODevice::ReadWrite);
    if(!success)
    {
        qCritical() << "Open Comm " << GetIdStr() << "(" << mName << ") failed :" << mQSerialPort->errorString();
        return RstDevCreateError;
    }
    
    mQSerialPort->setBaudRate(mBaudrate);
    mQSerialPort->setDataBits(QSerialPort::Data8);
    mQSerialPort->setFlowControl(QSerialPort::NoFlowControl);
    mQSerialPort->setParity(QSerialPort::NoParity);
    mQSerialPort->setStopBits(QSerialPort::OneStop);
    /* 清理buf,避免脏读 */
    mQSerialPort->clear();
    
    /* 使用信号槽 支撑 阻塞操作 */
    connect(mQSerialPort, &QSerialPort::bytesWritten, this, &QtCom::BytesWritten);
    connect(mQSerialPort, &QSerialPort::readyRead,    this, &QtCom::ReadyRead);
    
    qDebug().noquote() << "open " << mName.toUtf8().data() << "with baudrate:" << mBaudrate << "for" << GetIdStr() << success;
    
    return RstSuccess;
}

int QtCom::Close(void)
{
    if(NULL == mQSerialPort)
    {
        return RstSuccess;
    }

    if(mQSerialPort->isOpen())
    {
        mQSerialPort->close();
        qDebug().noquote() << "close" << mName.toUtf8().data() << "of" << GetIdStr();
    }
    
    disconnect(mQSerialPort, &QSerialPort::bytesWritten, this, &QtCom::BytesWritten);
    disconnect(mQSerialPort, &QSerialPort::readyRead,    this, &QtCom::ReadyRead);
    delete mQSerialPort;
    mQSerialPort = nullptr;
    
    return RstSuccess;
}

bool QtCom::IsOpen(void) const
{
    bool isOpen = mQSerialPort->isOpen();
    return isOpen;
}

QIODevice *QtCom::GetQIODevice(void) const
{
    return mQSerialPort;
}
