/******************************************************************************
 *
 * 文件名  ： IIoTester.h
 * 创建日期： 20200630
 * 版本号  ： v1.0 20200630
 * 文件描述： 测试物理链路的可靠性
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _IIO_TESTER_H_
#define _IIO_TESTER_H_

/************************************ 头文件 ***********************************/
#include "IIo.h"

/*********************************** 命名空间 **********************************/
/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 结构定义 **********************************/

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class IIoTester;
typedef int (IIoTester::* RecvAndCheckATimeFuncPtr) (void);

class IIoTester
{
    public:
        typedef enum _ModeTag_
        {
            ModeTBD                 = 0x00,                                                             /* 未定义 */
            ModeSendUntilErr        = 0x01,                                                             /* 发送直到出错 */
            ModeRecvAndCheck        = 0x02,                                                             /* 接收 & 检查 */
            ModeEchoClient          = 0x03,                                                             /* Echo 客户端 */
            ModeEchoServer          = 0x04,                                                             /* Echo 服务端 */
        } Mode;

        IIoTester(IIo *iio, int bytesPerMs, bool isSendWithNoise = false);
        ~IIoTester(void);
    
        int Start(Mode mode);                                                                           /* 启动 */
    
    private:
        /* 单向 发 收 查 */
        int SendUntilErr(void);                                                                         /* 持续发送测试数据 */
        int RecvAndCheck(void);                                                                         /* 接收 & 检查 */
    
        /* Echo 测试 */
        int StartEchoClient(void);                                                                      /* 实现Echo Master功能: 发送 接收 检查 */
        int StartEchoServer(void);                                                                      /* 实现Echo Slave功能 */
    
        int  Send(const char *buf, int size);                                                           /* 发 */
        int  Recv(char *buf, int size);                                                                 /* 收 */
        int  Check(const char *buf, int size);                                                          /* 检查 */
        int  SendLoop(RecvAndCheckATimeFuncPtr funcPtr);                                                /* 循环发送 */
        int  SendAllBufData(RecvAndCheckATimeFuncPtr funcPtr = NULL);                                   /* 发送缓存中的测试数据 */
        int  RecvAndCheckATime(void);                                                                   /* 调用一次 接收 & 检查 */
        void ShowProcess(long long totalBytesNums, int showInterval, const char *unitStr, const char *dirStr);/* 调试输出打印 */


        /* 发送 接收 缓存 */
        static const int mcBufSize = 1024 * 5;                                                          /* 缓存大小 */
        static const int mcPkgMaxSize = 256;                                                            /* 每包(帧)数据最大长度 */
        static const int mcRecvTimeoutMax = 10;                                                         /* 每次Recv的timeout最大值 */
        static const int mcRecvATime = mcPkgMaxSize;                                                    /* 每次接受的缓存大小 */
        static const int mcShowBytesInterval = 1024;                                                    /* 调试打印的Bytes间隔 */
    
        char      mSendBuf[mcBufSize];                                                                  /* 发送缓存 */
        char      mExpectRecvChar;                                                                      /* 期望接收字节 */
        long long mRecvBufValidSize;                                                                    /* 接收到的有效数据 */
        IIo       *mIIo;                                                                                /* 物理IO */
        int       mBytesPerMs;                                                                          /* Bps, 字节数每毫秒 用于流控 */
};

/*********************************** 工具函数 **********************************/

/*********************************** 工具变量 **********************************/


#endif // _IIO_TESTER_H_
