/******************************************************************************
 *
 * 文件名  ： ISX00Brd.h
 * 负责人  ： ryl
 * 创建日期： 20200611
 * 版本号  ： v1.0 20200611
 * 文件描述： 封装SX00系列源表子板
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_QT_I_QT_SX00_BRD_H_
#define _PSS_QT_I_QT_SX00_BRD_H_

/************************************ 头文件 ***********************************/
#include <QString>
#include <QObject>
#include "IPssBinAppCommQt.h"
#include "PssBinAppCommMcuAtsha204aStatus.h"
#include "PssBinAppCommQtUpgradeData.h"
#include "PssRstDef.h"

/*********************************** 命名空间 **********************************/
namespace Pss {
namespace Qt {
class PssBinQtProcesser;
/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class IQtBrd : public QObject
{
    Q_OBJECT

    public:
        virtual ~IQtBrd(void);
    
        PssRstT Connect(void);                                                                          /* 连接 */
        bool    IsConnected(void);                                                                      /* 获取子板连接状态 */
        QString GetBrdVer(void) const;                                                                  /* 获取版本 */
        QString GetBrdIdStr(void) const;                                                                /* 获取子板名称 */
        PssRstT GetAtsha204aStatus(PssBinAppCommMcuAtsha204aStatus &status) const;                      /* 获取ATSHA204A状态 */
        PssRstT Upgrade(const QString &filePath, const QString &infoFileName);                          /* 升级子板 */
#ifdef _PSS_QT_PLX00_
        PssRstT Upgrade(const QString &filePath, const QString &infoFileName, const QString &str);                                                            /* 升级子板 */
#endif
        PssRstT Reboot(void);                                                                           /* 重启 */
        PssRstT SendCommAtsha204aDataBrd(const PssBinAppCommQtFireAtsha204a &data);                     /* 烧录命令 */
        PssRstT SendCommReqBrd(PssBinAppCommEnumStatus &status);                                        /* 请求状态(升级使用) */
    protected:
        IQtBrd(const QString& comName, int baudrate, const QString &brdIdStr, QObject *parent = 0);     /* 串口构造 */
        IQtBrd(IIo* io, const QString &brdIdStr, QObject *parent = 0);
        IIo *GetIo(void) const;                                                                         /* 返回 物理IO */

    signals:
        void sigUpdateUpgradeProcess(int rate, const QString &msgStr) const;                            /* 更新升级进度 信号 */
    
    private:
        /* 升级接口 */
        PssRstT Upgrade(const QByteArray &binDataA, const QByteArray &binDataB);                        /* 底层升级接口 */
        PssRstT Upgrade(const QByteArray &binDataA, const QByteArray &binDataB, const uint32_t &address);
        void UpdateUi(int rate, const QString &msgStr);
        PssRstT ShouldWriteA(bool &isA);                                                                /* 需写入A面？ */
        void SplitBinData(QVector<QByteArray> &dataVector, const QByteArray &binData, int maxUpgradeDataSize);/* 升级文件拆包 */
        PssRstT Erase(void);                                                                            /* 擦除 */
        PssRstT Fire(const QVector<QByteArray> &dataVector, int startRate, int endRate);                /* 烧写 */
        PssRstT Fire(const QVector<QByteArray> &dataVector, int startRate, int endRate, int firstPackLen);/* 烧写 */
        PssRstT FireAUpgradeData(const PssBinAppCommQtUpgradeData &upgradeData);                        /* 烧写一帧 */
        PssRstT SetFlag(void);                                                                          /* 设置标记 */
    
    private:
        virtual IPssBinAppCommQt *GetPssBin(void) const = 0;                                            /* 获取处理器 */
        IIo               *mIIo;                                                                        /* 物理IO */
        QString            mVerStr;                                                                     /* 版本信息 */
        QString            mBrdIdStr;                                                                   /* 板标记 */
        static const int32_t mcWaitMcuRebootMs = 800;                                                   /* 等待Mcu重启时间 */
        bool               mDeleteIoFlag;                                                               /* 析构io标志 */
        bool               mConnected;                                                                  /* 子板连接状态 */
};
/*********************************** 工具函数 **********************************/

}
}

#endif // _PSS_QT_I_QT_SX00_BRD_H_