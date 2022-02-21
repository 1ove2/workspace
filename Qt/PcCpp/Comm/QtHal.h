/******************************************************************************
 *
 * 文件名  ： QtHal.h
 * 创建日期： 20200110
 * 版本号  ： v1.0 20200110
 * 文件描述： Qt 适配和共用函数,借用Android的名字(HAL 硬件抽象层)
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _QT_HAL_H_
#define _QT_HAL_H_

/************************************ 头文件 ***********************************/
#include "Version.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include "RstDef.h"
#ifndef _QT_NO_GUI_
#include <QLabel>
#include <QMessageBox>
#endif

#ifdef Q_OS_LINUX
#include <execinfo.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#endif

/*********************************** 命名空间 **********************************/
class Bytes;

namespace Qt {
class QtIo;
/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 结构定义 **********************************/

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/

/*********************************** 工具函数 **********************************/
/* */
bool IsDisplayBootLogo(void);
bool CheckSystemCallResult(int result);

/* 版本相关 */
QString GetQtVerStr(void);
bool    SplitVerStr(QString &verStr, QString &bldTimeStr, const QString &str, const QString &spliter = QString(_SPLITER_));
bool    IdnStr2VerStr(QString& qtVer, QString &fntVer, QString& anlgVer, QString& pwrVer, const QString& idnStr);
#ifndef _PSS_QT_NO_GUI_
void    ShowVer2Label(QLabel *labelVer, QLabel *labelBldTime, const QString &ver, const QString &spliter = QString(_SPLITER_));
void    ShowQtVerToUi(QLabel *labelVer, QLabel *labelBldTime);
#endif

/* 程序配置文件 */
bool    ReadQtAppCfg(QByteArray &data);
bool    WriteQtAppCfg(const QByteArray &data);

/* /etc/network/interfaces文件 */
bool    ReadInterfacesFile(QByteArray &data);
bool    WriteInterfaceFile(const QByteArray &data);
/* 判断IP合法性 */
bool    IsIpValid(const QString &ipStr);

/* 升级相关 */
/* QT程序升级，info为校验文件，对应进程文件名必须为文件绝对路径，文件名为空表示不升级对应程序 */
bool    QtAppUpgrade(const QString &infoFileName, const QString &daemonImageFileName, const QString &pwrImageFileName, const QString &uiImageFileName, const QString &scriptUpgradeFile);
bool    CheckAndGetMcuImageFilePath(QByteArray &imageA, QByteArray &imageB, const QString &imageFileName, const QString &infoFileName);
QString GetInfoSha256(const QString &infoFileName, const QString &fileName);

QString GetQtAppRootPath(void);
QString GetUsbImagePath(void);
QString GetLocalQtInfoFileName(void);
QString GetOnlineImagePath(void);
QString GetLocalQtDaemonAppImageName(void);                                                     /* 获取本地升级数据中Daemon进程文件名 */
QString GetLocalQtPwrAppImageName(void);                                                        /* 获取本地升级数据中Pwr进程文件名 */
QString GetLocalQtUIAppImageName(void);                                                         /* 获取本地升级数据中UI进程文件名 */

QString GetOnlineQtInfoFileName(void);                                                           /* 获取本地升级数据中Info文件名 */
QString GetOnlineQtDaemonAppImageName(void);                                                     /* 获取本地升级数据中Daemon进程文件名 */
QString GetOnlineQtPwrAppImageName(void);                                                        /* 获取本地升级数据中Pwr进程文件名 */
QString GetOnlineQtUIAppImageName(void);                                                         /* 获取本地升级数据中UI进程文件名 */

QString GetQtAppCfgPath(void);
QString GetQtDaemonAppExeFileName(void);
QString GetQtPwrAppExeFileName(void);
QString GetQtUIAppExeFileName(void);
QString JoinPath(const QString &path, const QString &name);
QString GetFileNameFromPath(const QString &path);
/* 获取仪表服务端网址 */
QString GetPssYBNetServer(void);
/* 获取在线升级镜像目录 */
QString GetServerOnlineImagesPath(void);

/* 在线升级校验码验证：解析校验码并解析出文件下载地址 */
bool CheckOnlineCode(const QString& code, QString& result);

/* 在线升级校验码生成： 根据文件下载地址生成升级校验码 */
const QString BuildOnlineCode(const QString& srcCode);

/* 标准值 与 单位值 互转 */
QStringList GetAllVUnit(void);
QStringList GetAllIUnit(void);
double UnitStr2Factor(const QString &unitStr);

/* 带单位值转为标准值: 2mV --> 0.02 */
double UnitVal2StdVal(const QString& strVal);

double  ValWithUnitToStdVal(double valWithUnit, const QString &unitStr);
double  StdValToValWithUnit(double stdVal, const QString &unitStr);

/* QString与Bytes互转 */
size_t QString2Bytes(Bytes &bytes, const QString &str);
void   Bytes2QString(QString &str, const Bytes &bytes);

/* 密码相关 */
QString QString2Passwd(const QString &passwdStr);
bool IsPasswdValid(const QString &passwdStr, const QString &passwd);

/* 特殊用途 */
QStringList GetAvailableComNames(void);
void LinearFitting(const QVector<double> &x, const QVector<double> &y, double &c1, double &c0, double &sqSum);

/* 保存值到文件中 */
void SaveValToFile(const QVector<double> &xVals, const QVector<double> &yVals, QString suffix = "");
void Save4ValToFile(const QVector<double> &x1Vals, const QVector<double> &x2Vals
                    , const QVector<double> &x3Vals, const QVector<double> &x4Vals, QString suffix = "");

/* 求极值 */
double GetMax(const QVector<double> &vals);
double GetMin(const QVector<double> &vals);

/* 清空QT中IO接收垃圾缓存数据 */
void ClearIOTrashData(QtIo *io);

/* 认证 */
/* 获取机器使用中的mac地址 */
QString GetActiveMacStr(void);
bool IsValidPc(void);

/* 判断浮点数是否相等 */
bool IsDoubleEqual(double val1, double val2, unsigned char precision = 15);

/* 指针转为字符串 */
QString Ptr2QString(void *ptr);

/* 当前时间 */
QString GetNowDateTimeStr(bool useBracket = true, bool hasMs = true, bool isFileNameFmt = false);


/* 日志初始化，日志文件名为空表示不写日志文件，至输出至控制台，日志最大大小默认10MB */
bool InitLog(QString logFileName = QString(""), quint32 logSizeKB = 10240);

/* 日志关闭并设置权限 */
void ClearLog(void);

/* utf8 与 QString互转 */
void Utf82QString(QString &str, const char *buf, size_t bufSize);
size_t QString2Utf8(char *buf, size_t bufSize, const QString &str);

/* QString与Bytes互转 */
size_t QString2Bytes(Bytes &bytes, const QString &str);
void   Bytes2QString(QString &str, const Bytes &bytes);

/* 同步至网络时间 */
bool Sync2NetTime(void);
bool SetSystemTime(const QDateTime& time);
bool CheckRTC(void);
void SetRTC(bool on);

/* 警告信息 */
#ifdef _PSS_QT_NO_GUI_
void ShowMsgBoxInfo(const QString &str, bool def = false);
#else
void ShowMsgBoxInfo(const QString &str, bool isOkButtonShow = true, const QMessageBox::Icon &icon = QMessageBox::Critical);
#endif

/* 错误代码转换 */
QString ErrorCode2String(RstT errCode);

/*********************************** 工具变量 **********************************/

#ifdef Q_OS_LINUX
/* dump初始化 */
void DumpBackTraceInit(void);

void ThreadDelay(uint delayUs);

#endif

}



#endif // _QT_HAL_H_
