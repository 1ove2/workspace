/******************************************************************************
 *
 * 文件名  ： QtIInstrument.h
 * 创建日期： 20200326
 * 版本号  ： v1.0 20200326
 * 文件描述： 友商 仪表接口类
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _QT_I_INSTRUMENT_H_
#define _QT_I_INSTRUMENT_H_

/************************************ 头文件 ***********************************/
#include "InstrumentTypeDef.h"
#include "RstDef.h"
#include <QObject>
#include <QString>
#include <QHostAddress>

/*********************************** 命名空间 **********************************/
class IIo;

class ScpiClient;

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class QtIInstrument : public QObject
{
    Q_OBJECT
    public:

        typedef enum _tagIOType
        {
            InstrIOTypeTBD      = 0,
            InstrIOTypeComm     = 1,
            InstrIOTypeNet      = 2,
            InstrIOTypeGPIB     = 3,
        } InstrIOType;
        QtIInstrument(InstrIOType ioType, const QString &name, bool hasSrc, QObject *parent = 0);
        ~QtIInstrument(void);
        static int GetScpiTcpServerPort(void);                                                          /* 获取Scpi服务端 端口号 */
    
        bool    Connect(uint32_t timeout = mcWait4RecvTimeout);                                         /* 连接：等待连接超时时间ms */
        int    Reset(void);                                                                            /* 复位 */
        bool    IsConnected(void) const;                                                                /* 获取设备连接状态 */
        int     ClearScpiError(void);                                                                   /* 清空SCPI错误代码缓存 */
        int     GetScpiError(int& errCode, uint32_t timeout = mcWait4RecvTimeout);                      /* 获取最早一次SCPI操作错误代码，0表示没有错误 */
    
        QString GetIdn(void) const;                                                                     /* 获取IDN */
        QString GetName(void) const;                                                                    /* 获取名字 */
        bool    HasSrc(void) const;                                                                     /* 有源仪表？ */
    
        int SetSrcInfo(const PssInstrumentCfgInfo *info);                                               /* 源信息设置 */
        int GetSrcInfo(PssInstrumentCfgInfo *info) const;                                               /* 源信息获取 */
    
    protected:
        virtual ScpiClient *GetScpiClient(void) const = 0;                                              /* 获取Scpi客户端 */
        virtual int UpdateCfgToInstrument(void) = 0;                                                    /* 配置信息 设置 到仪表 */
    
        /* 仪表配置 */
        bool        IsVSrc(void) const;                                                                 /* 仪表源类型 */
        double      GetSrcVal(void) const;                                                              /* 源值 */
        double      GetLmtVal(void) const;                                                              /* 限值 */
        double      GetNplc(void) const;                                                                /* NPLC */
        PssInstrumentCfgInfo mCfgInfo;                                                                  /* 仪表配置 */
    
    private:
        virtual void   InitSourceInfo(void);                                                            /* 初始化设备源信息 */
    
        QString     mName;                                                                              /* 仪表名 */
        QString     mIdnStr;                                                                            /* IDN返回 信息 */
        bool        mConnected;                                                                         /* 已连接成功标志 */
        bool        mHasSrc;                                                                            /* 有源仪表? */
        const static int mcScpiTcpServerPort = 5025;                                                    /* SCPI TCP端口号 */
        const static int mcWait4RecvTimeout = 1000;                                                     /* 等待接收超时ms */
        InstrIOType mIOType;                                                                            /* 设备IO类型 */

};

/*********************************** 工具函数 **********************************/




#endif // _QT_I_INSTRUMENT_H_
