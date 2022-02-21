#ifndef _I_IO_H_
#define _I_IO_H_

#include "TypeDef.h"
#include "RstDef.h"

class IIo
{
    public:
        virtual ~IIo(void);

        virtual int Open(void) = 0;                                               /* 打开 */
        virtual int Close(void) = 0;                                              /* 关闭 */
        virtual int Send(const char *buf, size_t size, int32_t timeout) = 0;      /* 发送 */
        virtual int Recv(char *buf, size_t size, int32_t timeout) = 0;            /* 接收 */
        virtual int FlushOut(void) = 0;                                           /* 刷新发送缓冲 */
        virtual int ClearIn(void) = 0;                                            /* 清空接收缓冲 */

    protected:
        IIo(void);                                                                /* 构造函数,抽象类,不可实例化 */

    private:

};




#endif // _I_IO_H_
