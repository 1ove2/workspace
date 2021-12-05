/************************************ 头文件 ***********************************/
#include "Bytes.h"

#include "CLibEndian.h"
#include <string.h>

Bytes::Bytes(void) :
    IBytesAble(),
    mValidLen(0)
{
    /* TODO:提升效率,去掉memset */
    memset(mData, 0x00, mcMemLen);
}

Bytes::~Bytes(void)
{
    Clear();
}

void Bytes::Clear(uint32_t clearBytesNums)
{
    if(0 == clearBytesNums)
    {
        return;
    }

    /* 清理索引 */
    if(mValidLen <= clearBytesNums)
    {
        mValidLen = 0;
        return;
    }
    else
    {
        mValidLen -= clearBytesNums;
    }
    
    /* 移动数据 */
    uint8_t *dest = mData;
    uint8_t *src = mData + clearBytesNums;
    memmove(dest, src, mValidLen);
}

uint32_t Bytes::ComputeCrc32(void)
{
    uint32_t crc32 = 0;
    Bytes bytesFilled;
    if(!FillWithZeroTo4BytesLen(bytesFilled, *this))
    {
        return 0;
    }
    CLibCrc32((const uint32_t *)bytesFilled.ConstData(), bytesFilled.GetValidLen() / 4, &crc32);

    return crc32;
}

bool Bytes::Append(const Bytes &bytes, int appendLen)
{
    int len = bytes.GetValidLen();
    if((-1 != appendLen) && (appendLen < len))
    {
        len = appendLen;
    }

    const uint8_t *ptr = bytes.ConstData();
    
    /* 原样复制,不转回字节序 */
    return Append(ptr, len);
}

bool Bytes::Append(const uint8_t *buf, size_t size, bool isHton)
{
    /* step1: 检查空间是否足够 */
    if(!IsSpaceEnough(size))
    {
        return false;
    }

    /* step2: 搬移数据,并转换为网络字节序 */
    if(isHton)
    {
        Hton(&mData[mValidLen], buf, size);
    }
    else
    {
        memmove(&mData[mValidLen], buf, size);
    }
    
    mValidLen += size;
    
    return true;
}

bool Bytes::Append(bool boolVal)
{
    return Append((const uint8_t *)(&boolVal), 1);
}

bool Bytes::Append(uint8_t byte)
{
    size_t size = sizeof(byte);
    return Append((const uint8_t *)(&byte), size);
}

bool Bytes::Append(uint16_t byte2)
{
    size_t size = sizeof(byte2);
    return Append((const uint8_t *)(&byte2), size, true);
}

bool Bytes::Append(uint32_t byte4)
{
    size_t size = sizeof(byte4);
    return Append((const uint8_t *)(&byte4), size, true);
}

bool Bytes::Append(int intData)
{
    size_t size = sizeof(intData);
    return Append((const uint8_t *)(&intData), size, true);
}

bool Bytes::Append(f64 f64Data)
{
    size_t size = sizeof(f64Data);
    return Append((const uint8_t *)(&f64Data), size, true);
}

bool Bytes::Append(f32 f32Data)
{
    size_t size = sizeof(f32Data);
    return Append((const uint8_t *)(&f32Data), size, true);
}

bool Bytes::ToBool(void) const
{
    bool val = 0;
    Ntoh((uint8_t *)(&val), ConstData(), 1);
    return val;
}

uint8_t Bytes::ToU8(void) const
{
    uint8_t val = 0;
    Ntoh((uint8_t *)(&val), ConstData(), sizeof(val));
    return val;
}

uint16_t Bytes::ToU16(void) const
{
    uint16_t val = 0;
    Ntoh((uint8_t *)(&val), ConstData(), sizeof(val));
    return val;
}

uint32_t Bytes::ToU32(void) const
{
    uint32_t val = 0;
    Ntoh((uint8_t *)(&val), ConstData(), sizeof(val));
    return val;
}

int32_t Bytes::ToI32(void) const
{
    int32_t val = 0;
    Ntoh((uint8_t *)(&val), ConstData(), sizeof(val));
    return val;
}

f64 Bytes::ToF64(void) const
{
    f64 val = 0;
    Ntoh((uint8_t *)(&val), ConstData(), sizeof(val));
    return val;
}

f32 Bytes::ToF32(void) const
{
    f32 val = 0;
    Ntoh((uint8_t *)(&val), ConstData(), sizeof(val));
    return val;
}

bool Bytes::ToUtf8(char *buf, size_t bufSize) const
{
    /* bufSize 不足 返回false */
    bool flag = false;

    size_t size = GetValidLen();
    size_t memmoveSize = 0;
    if(bufSize < size)              /* Bytes的长度大于bufSize,则尽力而为 */
    {
        memmoveSize = bufSize;
        flag = false;
    }
    else
    {
        memmoveSize = size;
        flag = true;
    }


    memmove(buf, ConstData(), memmoveSize);
    return flag;
}

int Bytes::ToBytes(Bytes &bytes) const
{
    bytes.Clear();
    const uint8_t *buf = this->ConstData();
    size_t size = this->GetValidLen();
    bytes.Append(buf, size);
    return size;
}

int Bytes::FromBytes(const Bytes &bytes)
{
    Clear();
    Append(bytes);
    return bytes.GetValidLen();
}

const uint8_t *Bytes::ConstData(void) const
{
    return mData;
}

size_t Bytes::GetValidLen(void) const
{
    return mValidLen;
}

size_t Bytes::GetFreeLen(void) const
{
    return mcMemLen - GetValidLen();
}

bool Bytes::IsSpaceEnough(size_t size) const
{
    size_t newValidLen = mValidLen + size;
    if(newValidLen <= mcMemLen)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Bytes::Hton(uint8_t *dest, const uint8_t *src, size_t size)
{
    CLibHton(dest, src, size);
}

void Bytes::Ntoh(uint8_t *dest, const uint8_t *src, size_t size)
{
    CLibNtoh(dest, src, size);
}

bool Bytes::IsEmpty() const
{
    return GetValidLen() <= 0;
}

bool Bytes::IsEqByteInIndex(uint32_t index, uint8_t val) const
{
    /* 避免越界 */
    size_t needValidLen = index + 1;
    if(GetValidLen() < needValidLen)
    {
        return false;
    }

    const uint8_t *ptrChar = ConstData() + index;
    if(val == *ptrChar)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int32_t Bytes::IndexOf(uint8_t val)
{
    uint32_t end = GetValidLen();
    for(uint32_t i = 0; i < end; i++)
    {
        if(IsEqByteInIndex(i, val))
        {
            return i;
        }
    }

    return -1;
}

bool Bytes::ParseData(uint8_t *dest, size_t offset, size_t size) const
{
    const uint8_t *src = ConstData() + offset;
    Ntoh(dest, src, size);

    return true;
}
QString Bytes::ToQString(void) const
{
    if(0 == mValidLen)
    {
        return QString("Empty.\n");
    }

    QString debugOutStr;
    for(size_t i = 0; i < mValidLen; i++)
    {
        QString aByteStr;
        aByteStr.sprintf("%02X ", mData[i]);
        debugOutStr.append(aByteStr);
    }
    
    return debugOutStr;
}

void Utf82Bytes(Bytes &bytes, const char *buf, size_t bufSize)
{
    /* 溢出 */
    if(!bytes.IsSpaceEnough(bufSize))
    {
#ifdef _PSS_QT_
        qCritical() << __FILE__ << __LINE__ << "err.";
#endif
        while(1);
    }

    bytes.Clear();
    bytes.Append((const uint8_t *)(buf), bufSize);
}

size_t Bytes2Utf8(char *buf, size_t bufSize, const Bytes &bytes)
{
    bool done = bytes.ToUtf8(buf, bufSize);
    if(done)
    {
        return 0;
    }

    return bytes.GetValidLen();
}


bool FillWithZeroTo4BytesLen(Bytes &bytesFilled, const Bytes &bytes)
{
    /* 计算需要的补0个数 并 补零 */
    int leftBytes = bytes.GetValidLen() % 4;
    int needZeroLen = 0;
    if((0 < leftBytes) && (leftBytes < 4)) /* 1 2 3 */
    {
        needZeroLen = 4 - leftBytes;
    }

    if(!bytesFilled.IsSpaceEnough(needZeroLen))
    {
        return false;
    }
    
    bytesFilled.Append(bytes);
    for(int i = 0; i < needZeroLen; i++)
    {
        bytesFilled.Append(static_cast<uint8_t>(0));
    }
    return true;
}

bool FillWithZeroTo3BytesLen(Bytes &bytesFilled, const Bytes &bytes)
{
    /* 计算需要的补0个数 并 补零 */
    int leftBytes = bytes.GetValidLen() % 3;
    int needZeroLen = 0;
    if((0 < leftBytes) && (leftBytes < 3)) /* 1 2 */
    {
        needZeroLen = 3 - leftBytes;
    }

    if(!bytesFilled.IsSpaceEnough(needZeroLen))
    {
        return false;
    }
    
    bytesFilled.Append(bytes);
    for(int i = 0; i < needZeroLen; i++)
    {
        bytesFilled.Append(static_cast<uint8_t>(0));
    }
    return true;
}
