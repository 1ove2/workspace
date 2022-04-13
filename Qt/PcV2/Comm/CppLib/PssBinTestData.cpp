/******************************************************************************
 *
 * 文件名  ： PssBinTestData.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220301
 * 版本号  ： v1.0 20220301
 * 文件描述： PssBin 协议测试数据 实现
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "PssCppLog.h"
#include "PssCLibEndian.h"
#include "PssBinTestData.h"

#include <stdlib.h>
#include <string.h>

/*********************************** 命名空间 **********************************/

/*----------------------------------- 声明区 ----------------------------------*/
//#define _PRINT_BUILD_FROM_APP_DATA_
//#define _PRINT_BUILD_FROM_BYTES_

#define _BYTES_MAX_SIZE_                                (512)
#define _BYTES_MAX_STR_SIZE_                            (_BYTES_MAX_SIZE_ * 3)

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/********************************** 变量声明区 *********************************/
const i32 scMinFrmLen = 21;
/* Head(3) + Type(1) + destAddr(1) + srcAddr(1) + Len(3) + App(x) + Tail(3)  */
const i32 scDataOffset = 13;

const u8 scHead[3] = {0x55, 0xAA, 0x11};
const u8 scType    = 0x01;
/* 目标地址 */
const u8 scSrcAddr = 0x00;                                                                              /* 控制板I.MX6地址 */
const u8 scTail[3] = {0xAA, 0x55, 0xEE};

static u32 sId = 0;
/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
PssBinTestData::PssBinTestData(u8 destAddr, const std::vector<char> &appData) :
    mType(scType),
    mDestAddr(destAddr),
    mSrcAddr(scSrcAddr),
    mFrmLen(0),
    mData(),
    mFilledLen(0),
    mId(sId++),
    mCheckSum(0),
    mIsValid(true)
{
    /* step1: 求帧长 */
    /* step1.1: 帧有效数据长度 */
    u32 validDataLen = 0;
    validDataLen += sizeof(scHead);
    validDataLen += sizeof(scType);
    validDataLen += sizeof(mDestAddr);
    validDataLen += sizeof(scSrcAddr);
    validDataLen += 3;                    /* 帧长域 */
    validDataLen += sizeof(mId);
    validDataLen += appData.size();
    validDataLen += sizeof(mCheckSum);
    validDataLen += sizeof(scTail);
    /* step1.2: 计算填充长度 */
    u32 mod3Left = validDataLen % 3;
    if(0 != mod3Left)
    {
        mFilledLen = 3 - mod3Left;
    }
    else
    {
        mFilledLen = 0;
    }
    /* step1.3: 计算帧长 */
    mFrmLen = validDataLen + mFilledLen;
    mFrmLen /= 3;

#ifdef _PRINT_BUILD_FROM_APP_DATA_
    fprintf(stdout, "validDataLen:%d\n", validDataLen);
    fprintf(stdout, "mod3Left:    %d\n", mod3Left);
    fprintf(stdout, "mFilledLen:  %d\n", mFilledLen);
    fprintf(stdout, "mFrmLen:     %d\n", mFrmLen);
    fprintf(stdout, "appSize:     %d\n", appData.size());
    fprintf(stdout, "build:\n");
#endif

    /* step2: 构造字节流 */
    /* step2.1: 帧头 */
    u8 bytesBuf[4] = {0};
    mData.clear();
    mData.push_back(scHead[0]);
    mData.push_back(scHead[1]);
    mData.push_back(scHead[2]);
    mData.push_back(scType);
    mData.push_back(mDestAddr);
    mData.push_back(scSrcAddr);
    memset(bytesBuf, 0, 4);
    PssCLibHton(bytesBuf, &mFrmLen, 3);
    mData.push_back(bytesBuf[0]);
    mData.push_back(bytesBuf[1]);
    mData.push_back(bytesBuf[2]);

#ifdef _PRINT_BUILD_FROM_APP_DATA_
    fprintf(stdout, "to frmLen:");
    PrintBytes(stdout, mData.data(), mData.size());
    fprintf(stdout, "\n");
#endif

    /* step2.2: 应用层数据: mId(4) + appData + 填充0 + checksum(4) */
    memset(bytesBuf, 0, 4);
    PssCLibHton(bytesBuf, &mId, sizeof(mId));
    mData.push_back(bytesBuf[0]);
    mData.push_back(bytesBuf[1]);
    mData.push_back(bytesBuf[2]);
    mData.push_back(bytesBuf[3]);

#ifdef _PRINT_BUILD_FROM_APP_DATA_
    fprintf(stdout, "to id    :");
    PrintBytes(stdout, mData.data(), mData.size());
    fprintf(stdout, "\n");
#endif

    mData.insert(mData.end(), appData.begin(), appData.end());

#ifdef _PRINT_BUILD_FROM_APP_DATA_
    fprintf(stdout, "to data  :");
    PrintBytes(stdout, mData.data(), mData.size());
    fprintf(stdout, "\n");
#endif

    /* 填充 */
    for(u32 i = 0; i < mFilledLen; i++)
    {
        mData.push_back(0);
    }

#ifdef _PRINT_BUILD_FROM_APP_DATA_
    fprintf(stdout, "to fill  :");
    PrintBytes(stdout, mData.data(), mData.size());
    fprintf(stdout, "\n");
#endif

    /* 校验(校验和) */
    int size = mData.size();
    for(int i = 0; i < size; i++)
    {
        mCheckSum += (u8)mData[i];
    }
    mCheckSum += (u8)scTail[0];
    mCheckSum += (u8)scTail[1];
    mCheckSum += (u8)scTail[2];
    mCheckSum = ~mCheckSum;

    memset(bytesBuf, 0, 4);
    PssCLibHton(bytesBuf, &mCheckSum, sizeof(mCheckSum));
    mData.push_back(bytesBuf[0]);
    mData.push_back(bytesBuf[1]);
    mData.push_back(bytesBuf[2]);
    mData.push_back(bytesBuf[3]);

#ifdef _PRINT_BUILD_FROM_APP_DATA_
    fprintf(stdout, "to cksum :");
    PrintBytes(stdout, mData.data(), mData.size());
    fprintf(stdout, "\n");
    PssCLibLogI(PssCppLog::GetCLibLog(), "send % 4d, mCheckSum:%08x => %02x,%02x,%02x,%02x", mId, mCheckSum, bytesBuf[0], bytesBuf[1], bytesBuf[2], bytesBuf[3]);
#endif

    /* step2.3: 帧尾 */
    mData.push_back(scTail[0]);
    mData.push_back(scTail[1]);
    mData.push_back(scTail[2]);

#ifdef _PRINT_BUILD_FROM_APP_DATA_
    fprintf(stdout, "to tail  :");
    PrintBytes(stdout, mData.data(), mData.size());
    fprintf(stdout, "\n");
#endif

#ifdef _PRINT_BUILD_FROM_APP_DATA_
    fprintf(stdout, "build:\n");
    Print();
#endif
}


PssBinTestData::PssBinTestData(const char *rxBuf, i32 rxBufSize) :
    mType(0),
    mDestAddr(0),
    mSrcAddr(0),
    mFrmLen(0),
    mData(),
    mFilledLen(0),
    mId(0),
    mCheckSum(0),
    mIsValid(false)
{
    u8 bytesBuf[4] = {0};

    /* 不足最小帧长 */
    if(rxBufSize < scMinFrmLen)
    {
        PssCLibLogI(PssCppLog::GetCLibLog(), "rxBuf not enough(len field):%d < %d.", rxBufSize, scMinFrmLen);
        return;
    }

    /* step1: All Data */
    i32 i = 0;
    while(i < rxBufSize)
    {
        mData.push_back(rxBuf[i]);
        i++;
    }

#ifdef _PRINT_BUILD_FROM_BYTES_
    fprintf(stdout, "recv Bytes:");
    PrintBytes(stdout, AllData(), AllSize());
    fprintf(stdout, "\n");
#endif

    /* Head(3) + Type(1) + destAddr(1) + srcAddr(1) + Len(3) + App(x) + Tail(3)  */
    /* step2: Head */
    bytesBuf[0] = rxBuf[0];
    bytesBuf[1] = rxBuf[1];
    bytesBuf[2] = rxBuf[2];
    if(!((scHead[0] == bytesBuf[0]) && (scHead[1] == bytesBuf[1]) && (scHead[2] == bytesBuf[2])))
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "Error Frame Head.");
        return;
    }

#ifdef _PRINT_BUILD_FROM_BYTES_
    fprintf(stdout, "head Done\n");
#endif

    /* 不检查 Type,destAddr,srcAddr */
    mType     = rxBuf[3];
    mDestAddr = rxBuf[4];
    mSrcAddr  = rxBuf[5];
    if(mType != scType)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "Error Type.");
        return;
    }
    if(mDestAddr != scSrcAddr)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "Error Dest Addr.");
        return;
    }

#ifdef _PRINT_BUILD_FROM_BYTES_
    fprintf(stdout, "mType:%02x,mDestAddr:%02x,mSrcAddr:%02x\n", mType, mDestAddr, mSrcAddr);
#endif

    /* step3: Len(6,7,8 Bytes)*/
    bytesBuf[0] = 0;
    bytesBuf[1] = rxBuf[6];
    bytesBuf[2] = rxBuf[7];
    bytesBuf[3] = rxBuf[8];
    PssCLibNtoh(&mFrmLen, bytesBuf, 4);
    i32 frmBytesCount = mFrmLen * 3;
    if(frmBytesCount != rxBufSize)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "Frame Len Error:%d,%d", frmBytesCount, rxBufSize);
        return;
    }

#ifdef _PRINT_BUILD_FROM_BYTES_
    fprintf(stdout, "mFrmLen:%06x,%d\n", mFrmLen, mFrmLen);
#endif

    /* step4: id */
    bytesBuf[0] = rxBuf[9];
    bytesBuf[1] = rxBuf[10];
    bytesBuf[2] = rxBuf[11];
    bytesBuf[3] = rxBuf[12];
    PssCLibNtoh(&mId, bytesBuf, 4);

#ifdef _PRINT_BUILD_FROM_BYTES_
    fprintf(stdout, "mId:%08x,%d\n", mId, mId);
#endif

    /* 跳过APP段 */
    /* step5: checksum */
    i32 checkSumOffset = frmBytesCount - 3 - 4;
    bytesBuf[0] = rxBuf[checkSumOffset++];
    bytesBuf[1] = rxBuf[checkSumOffset++];
    bytesBuf[2] = rxBuf[checkSumOffset++];
    bytesBuf[3] = rxBuf[checkSumOffset++];
    PssCLibNtoh(&mCheckSum, bytesBuf, 4);

#ifdef _PRINT_BUILD_FROM_BYTES_
    fprintf(stdout, "cksum:%08x => %02x,%02x,%02x,%02x\n", mCheckSum, bytesBuf[0], bytesBuf[1], bytesBuf[2], bytesBuf[3]);
#endif

    /* step6: Tail */
    i32 tailOffset = frmBytesCount - 3;
    bytesBuf[0] = rxBuf[tailOffset++];
    bytesBuf[1] = rxBuf[tailOffset++];
    bytesBuf[2] = rxBuf[tailOffset];
    if(!((scTail[0] == bytesBuf[0]) && (scTail[1] == bytesBuf[1]) && (scTail[2] == bytesBuf[2])))
    {
        PssCLibLogI(PssCppLog::GetCLibLog(), "Error Frame Tail.");
        return;
    }

#ifdef _PRINT_BUILD_FROM_BYTES_
    //fprintf(stdout, "parse:\n");
    //Print();
#endif
    mIsValid = true;
}

PssBinTestData::~PssBinTestData(void)
{
}

const char *PssBinTestData::AllData(void) const
{
    return mData.data();
}

i32 PssBinTestData::AllSize(void) const
{
    return mData.size();
}

const char *PssBinTestData::AppData(void) const
{
    return AllData() + scDataOffset;
}

i32 PssBinTestData::AppSize(void) const
{
    i32 len = mFrmLen * 3 - 20;
    if(len < 0)
    {
        return 0;
    }

    return len;
}

void PssBinTestData::Print(FILE *f)
{
    fprintf(f, "=================================================PssBinDataV3(% 4d)==========================================\n", AllSize());
    PrintBytes(f, AllData(), AllSize());
    fprintf(f, "\n");
    fprintf(f, "type      :0x%02x.\n", mType);
    fprintf(f, "dAddr     :0x%02x.\n", mDestAddr);
    fprintf(f, "sAddr     :0x%02x.\n", mSrcAddr);
    fprintf(f, "len       :0x  %06x(%d), %dBytes.\n", mFrmLen, mFrmLen, mFrmLen * 3);
    fprintf(f, "id        :0x%08x(%d).\n", mId, mId);
    fprintf(f, "data(% 4d):", AppSize());
    PrintBytes(f, AppData(), AppSize());
    fprintf(f, "\n");
    /* fprintf(f, "fillLen   :%d\n", mFilledLen); */
    fprintf(f, "cksum     :0x%08x.\n", mCheckSum);
    fprintf(f, "\n");
}

bool PssBinTestData::IsValid(void) const
{
    return mIsValid;
}

u32  PssBinTestData::GetId(void) const
{
    return mId;
}

bool PssBinTestData::IsCheckSumPassed(void) const
{
    /* 校验 */
    u32 checkSum = 0;
    const char *ptr = AllData();
    i32 size = AllSize() - 7;     /* 去掉checkSum和tail */
    for(i32 i = 0; i < size; i++)
    {
        checkSum += ptr[i];
    }
    checkSum += scTail[0];
    checkSum += scTail[1];
    checkSum += scTail[2];

    u32 result = mCheckSum + checkSum;
    if(0xFFFFFFFF != result)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "checkSum(0x%08x), mCheckSum(0x%08x), result:0x%08x", mCheckSum, checkSum, result);
        return false;
    }
    return true;
}

#if _PSS_QT_
QString PssBinTestData::ToQString(void) const
{
    QString str;
    int size = mData.size();
    for(int i = 0; i < size; i++)
    {
        QString charHexStr;
        charHexStr.sprintf("%02X ", (u8)mData[i]);
        str += charHexStr;
    }

    return str;
}
#endif

bool PssBinTestDataCmpl(const PssBinTestData *sendData, const PssBinTestData *recvData)
{
    const char *sendPtr = sendData->AllData();
    i32 sendCount = sendData->AllSize();
    const char *recvPtr = recvData->AllData();
    i32 recvCount = recvData->AllSize();

    if(sendCount != recvCount)
    {
        PssCLibLogEL(PssCppLog::GetCLibLog(), "sendCount(%d) != recvCount(%d)", sendCount, recvCount);
        return false;
    }

    i32 i = 0;
    for(i = 0; i < sendCount; i++)
    {
        char sendChar = sendPtr[i];
        char recvChar = recvPtr[i];
        if(sendChar != recvChar)
        {
            PssCLibLogEL(PssCppLog::GetCLibLog(), "%d:%02x(sendChar) != %02x(recvChar)", i, sendChar, recvChar);
            return false;
        }
    }

    return true;
}

void PrintBytes(FILE *f, const char *buf, i32 bufSize)
{
    char charHexbuf[4] = {0};
    char bufHexStrBuf[_BYTES_MAX_STR_SIZE_] = {0};
    u32 i = 0;
    u32 iMax = bufSize;
    if(_BYTES_MAX_SIZE_ < bufSize) /* 截断 避免溢出 */
    {
        iMax = _BYTES_MAX_SIZE_;
    }
    for(i = 0; i < iMax; i++)
    {
        /* 此处到行尾 用于调试 的代码 */
        memset(charHexbuf, 0, 4);
        sprintf(charHexbuf, "%02x,", 0x000000FF & buf[i]);
        strncat(bufHexStrBuf, charHexbuf, 3);
    }

    fprintf(f, bufHexStrBuf, strlen(bufHexStrBuf));
}

