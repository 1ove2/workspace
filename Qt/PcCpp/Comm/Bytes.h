/******************************************************************************
 *
 * 文件名  ： Bytes.h
 * 创建日期： 20190826
 * 版本号  ： v1.0 20190826
 * 文件描述： PSS 字节流 数据结构 接口
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _BYTES_H_
#define _BYTES_H_

/************************************ 头文件 ***********************************/
#include "ITool.h"
#include "RstDef.h"
#include "CLibCrc32.h"


/*********************************** 命名空间 **********************************/
class Bytes;
/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
/* 帧数据类 接口 */
class IBytesAble : public IQStringAble
{
    public:
        IBytesAble(void){;}
        virtual ~IBytesAble(void){;}

        /* 帧参数 */
        virtual int  ToBytes(Bytes &bytes) const = 0;                                                   /* 转换为字节流 */
        virtual int  FromBytes(const Bytes &bytes) = 0;                                                 /* 从字节流填充 */
    protected:
};

class Bytes : public IBytesAble
{
    public:
        Bytes(void);                                                                                    /* 构造 */
        virtual ~Bytes(void);                                                                           /* 析构 */

        /* 字节序  */
        static void Hton(uint8_t *dest, const uint8_t *src, size_t size);                               /* 主机转网络序 */
        static void Ntoh(uint8_t *dest, const uint8_t *src, size_t size);                               /* 网络转主机序 */
    
        bool IsEmpty(void) const;                                                                       /* 空? */
        bool IsSpaceEnough(size_t size) const;                                                          /* 当前mem空间是否足够Append操作 */
        const uint8_t *ConstData(void) const;                                                           /* 获取数据指针 */
        size_t GetValidLen(void) const;                                                                 /* 获取有效数据字节个数 */
        size_t GetFreeLen(void) const;                                                                  /* 获取空闲空间 */
        void Clear(uint32_t clearBytesNums = mcMemLen);                                                 /* 清空 */
        uint32_t ComputeCrc32(void);                                                                    /* 计算Crc32 */
        bool IsEqByteInIndex(uint32_t index, uint8_t val) const;                                        /* 指定位置是指定字节 */
        int32_t IndexOf(uint8_t val);                                                                   /* 从左向右查找第一个val的索引 */
        bool ParseData(uint8_t *dest, size_t offset, size_t size) const;                                /* 解析出数据 */
    
        /*
         * Append时,主机序转网络序
         * ToXXXX时,网络序转主机序
         *
         * FIXME:重载的函数都是重复,利用模板消除重复?
         *  */       
        bool     ToBool(void) const;
        uint8_t  ToU8(void)   const;
        uint16_t ToU16(void)  const;
        uint32_t ToU32(void)  const;
        int32_t  ToI32(void)  const;
        f64      ToF64(void)  const;
        f32      ToF32(void)  const;
        bool     ToUtf8(char *buf, size_t bufSize) const;
    
        bool     Append(bool     boolVal);
        bool     Append(uint8_t  byte);
        bool     Append(uint16_t byte2);
        bool     Append(uint32_t byte4);
        bool     Append(int32_t  intData);
        bool     Append(f64      f64Data);
        bool     Append(f32      f32Data);
    
        /* 不转换字节序  */
        bool Append(const Bytes &bytes, int appendLen = -1);                                            /* 追加 Bytes */
        bool Append(const uint8_t *buf, size_t size, bool isHton = false);                              /* 填充数据,字节流默认不使用网络字节序 */


        virtual int  ToBytes(Bytes &bytes) const;                                                       /* 转换为字节流 */
        virtual int  FromBytes(const Bytes &bytes);                                                     /* 从字节流填充 */

        virtual QString ToQString(void) const;                                                          /* 转为字符串 */
    protected:

    private:
        /* 使用静态内存分配 避免动态分配产生复杂逻辑 */
        static const size_t mcMemLen = 40*1024;                                                         /* 数据mem大小,必须大于最长帧 */
        uint8_t mData[mcMemLen];                                                                        /* 数据 */
        size_t  mValidLen;                                                                              /* 有效数据长度 */
};

/*********************************** 工具函数 **********************************/
/* utf8 与 Bytes互转 */
void Utf82Bytes(Bytes &bytes, const char *buf, size_t bufSize);
size_t Bytes2Utf8(char *buf, size_t bufSize, const Bytes &bytes);

bool FillWithZeroTo4BytesLen(Bytes &bytesFilled, const Bytes &bytes);

bool FillWithZeroTo3BytesLen(Bytes &bytesFilled, const Bytes &bytes);

#endif // _BYTES_H_
