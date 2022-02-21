#include "GPIBIo.h"
#include "ITool.h"
#include "RstDef.h"


#include <QDebug>



GPIBIo::GPIBIo(uint8_t instrumentAddr, const QString &name, uint8_t GPIBCardAddr) :
#ifdef _GPIB_SUPPORT_
    mDefaultGPIBRM(-1),
    mGPIBHandle(-1),
    mRetStatus(0),
#endif
    mIsOpen(false),
    mGPIBCardAddr(GPIBCardAddr),
    mInstrumentAddr(instrumentAddr),
    mGPIBIdStr(name),
    mTimeout(2000)
{
    Open();
}

GPIBIo::~GPIBIo(void)
{
    Close();
}


bool GPIBIo::InitGPIB(uint8_t CardAddr, uint8_t instrAddr, uint32_t timeout)
{
#ifdef _GPIB_SUPPORT_
    ViStatus status;
    mDefaultGPIBRM = 0;
    /*
     * First we must call viOpenDefaultRM to get the resource manager
     * handle.  We will store this handle in defaultRM.
     */
    status = viOpenDefaultRM (&mDefaultGPIBRM);
    if (status < VI_SUCCESS)
    {
       return false;
    }

    /*
     * Now we will open a VISA session to a device at Primary Address 2.
     * You can use any address for your instrument. In this example we are
     * using GPIB Primary Address 2.
     *
     * We must use the handle from viOpenDefaultRM and we must
     * also use a string that indicates which instrument to open.  This
     * is called the instrument descriptor.  The format for this string
     * can be found in the NI-VISA User Manual.
     * After opening a session to the device, we will get a handle to
     * the instrument which we will use in later VISA functions.
     * The two parameters in this function which are left blank are
     * reserved for future functionality.  These two parameters are
     * given the value VI_NULL.
     *
     * This example will also work for serial or VXI instruments by changing
     * the instrument descriptor from GPIB0::2::INSTR to ASRL1::INSTR or
     * VXI0::2::INSTR depending on the necessary descriptor for your
     * instrument.
     */
    char conStr[128] = {0};
    sprintf(conStr,  "GPIB%d::%d::INSTR", CardAddr, instrAddr);
    status = viOpen (mDefaultGPIBRM, conStr, VI_NULL, VI_NULL, &mGPIBHandle);
    if (status < VI_SUCCESS)
    {
        mRetStatus = status;
        return false;
    }
    
    /*
     * Set timeout value to <timeout> milliseconds .
     */
    status = viSetAttribute (mGPIBHandle, VI_ATTR_TMO_VALUE, timeout);
    
    mRetStatus = status;
#else
    Q_UNUSED(CardAddr);
    Q_UNUSED(instrAddr);
    Q_UNUSED(timeout);
#endif
    return true;
}

int GPIBIo::Open(void)
{
#ifdef _GPIB_SUPPORT_
    if(!InitGPIB(mGPIBCardAddr, mInstrumentAddr, mTimeout))
    {
        mIsOpen = false;
        qDebug() << "GPIB Not Connected !";
        return RstGPIBOpenError;
    }
    else
    {
        mIsOpen = true;
        qDebug() << "GPIB Connected !";
    }
#endif
    return RstSuccess;
}

int GPIBIo::Close(void)
{
#ifdef _GPIB_SUPPORT_
    if(mIsOpen)
    {
        viClose(mGPIBHandle);
        mGPIBHandle = -1;
        viClose(mDefaultGPIBRM);
        mDefaultGPIBRM = -1;
    }
#endif
    mIsOpen = false;
    return RstSuccess;
}

int GPIBIo::Send(const char *buf, size_t size, int32_t timeout)
{
#ifdef _GPIB_SUPPORT_
    if(!mIsOpen)
    {
        qDebug() << "GPIB not Open !";
        return RstGPIBNotOpen;
    }
    if(NULL == buf)
    {
        return RstParameterError;
    }

    ViUInt32 writeCount;
    
    mRetStatus = viWrite (mGPIBHandle, (ViBuf)buf, (ViUInt32)size, &writeCount);
    if (mRetStatus < VI_SUCCESS)
    {
        return RstGPIBWriteError;
    }
    return writeCount;
#else
    Q_UNUSED(buf);
    Q_UNUSED(size);
    Q_UNUSED(timeout);
    return 0;
#endif

}

int GPIBIo::Recv(char *buf, size_t size, int32_t timeout)
{
#ifdef _GPIB_SUPPORT_
    if(!mIsOpen)
    {
        qDebug() << "GPIB not Open !";
        return RstGPIBNotOpen;
    }

    if(NULL == buf)
    {
        return RstParameterError;
    }


    ViUInt32 retCount;


    mRetStatus = viRead (mGPIBHandle, (ViPBuf)buf, size, &retCount);
    if (mRetStatus < VI_SUCCESS)
    {
        return RstGPIBReadError;
    }
    
    return retCount;
#else
    Q_UNUSED(buf);
    Q_UNUSED(size);
    Q_UNUSED(timeout);
    return 0;
#endif

}

int GPIBIo::FlushOut()
{
    /* 暂未实现功能 */
    return RstSuccess;
}

int GPIBIo::ClearIn()
{
    /* 暂未实现功能 */
    return RstSuccess;
}

bool GPIBIo::IsOpen()
{
    return mIsOpen;
}

bool GPIBIo::SetOptTimeout(uint32_t timeout)
{
#ifdef _GPIB_SUPPORT_
    if(mIsOpen)
    {
        mRetStatus = viSetAttribute (mGPIBHandle, VI_ATTR_TMO_VALUE, timeout);
        if(mRetStatus != VI_SUCCESS)
        {
            return false;
        }
    }
#else
    Q_UNUSED(timeout);
#endif
    return true;
}
