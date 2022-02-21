/******************************************************************************
 *
 * 文件名  ： PssQtHal.cpp
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20190911
 * 版本号  ： v1.0 20190911
 * 文件描述： PSS Qt 适配和共用函数
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "QtHal.h"

#include "ITool.h"
#include "Bytes.h"
#include "CLibCrc32.h"
#include "CLibSha256.h"
#include "CLibLeastSquare.h"
#include "QtIo.h"
#include <QDir>
#include <QFile>
#include <QTime>
#include <QMutex>
#include <QDate>
#include <QDebug>
#include <QRegExp>
#include <QDateTime>
#include <QSerialPortInfo>
#include <QNetworkInterface>
#include <QThread>
#include <QProcess>
#include <QCoreApplication>
#ifndef _PSS_QT_NO_GUI_
#include <QApplication>
#endif


/*----------------------------------- 声明区 ----------------------------------*/
#define _BUILD_ONLINE_CODE_OFFSET       (4)                                                             /* 生成在线升级校验码偏移长度 */
#define _BUILD_ONLINE_CODE_MAX_LENGTH   (32)                                                            /* 生成在线升级校验码最大长度 */

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/********************************** 变量声明区 *********************************/
/* 日志文件句柄 */
static QFile sLogFile;
/* 日志文件名*/
static QString sCurrentLogFileName;
/* 日志最大空间 */
static int  sLogFileSize = 0;
/* 日志锁 */
static QMutex sLogLock;
/* 堆栈锁 */
static QMutex sBackTraceLock;

static bool sRtcOnFlag = true;

/********************************** 函数声明区 *********************************/
static QString GetInterfacesFilePath(void);
static bool ReadFile(QByteArray &data, const QString &fileName);
static bool WriteFile(const QByteArray &data, const QString &fileName);
static void MakeUnitStr2FactorHash(void);
static bool RunSystemCmd(const QString &cmd);
static QString GetCsvFilePath(void);

/* 打开日志文件 */
static bool QDbgOpenLogFile(const QString& fileName, QString& errMsg);
/* 关闭日志文件 */
static bool QDbgCloseLogFile(QString &errMsg);
/* 日志输出至控制台 */
static void QDbgMessage2Console(QtMsgType type, const QMessageLogContext &context, const QString &msg);
/* 返回调试头字符串 */
static QString GetDbgHeadStr(QtMsgType type, const QMessageLogContext &context);


/* QtV2升级 */
static QString GetQtAppNeedUpgradeName(const QString& fileName);
static bool IsQtAppAValid(const QString& fileName);
static bool SetQtImageFlag(const QString& fileName);
static bool CheckInitScript(const QString& fileName);
static bool UpgradeInitScript(const QString &fileName = QString("/etc/init.d/myzr"));

/* 升级公用 */
static bool CheckAndGetImage(QByteArray &image, const QString &infoFileName, const QString &fileName);
static QString ComputeSha256(const QByteArray &data);
static void SplitSha256FromStrLine(QString &fileName, QString &sha256, const QString &lineStr);

/* 认证 */
/* 获取机器mac地址列表 */
static QStringList GetMacStrList(void);

/* 获取当前线程ID */
static QString GetThreadIdStr(void);

#ifdef Q_OS_LINUX
/* 信号捕捉 */
static void DumpBackTrace(int signo);
#endif
/********************************** 变量实现区 *********************************/
static QMap<QString, double> sUnitStr2FactorHash;

static QString sValidMacStrList[] =
{
    QString("D8:CB:8A:94:65:4E"),       /* 研发实验室 */
    QString("6C:4B:90:8B:F6:C5"),       /* pp办公电脑 */
    QString("00:E0:4C:36:01:18"),       /* pp笔记本 有线 */
    QString("38:00:25:E6:4E:CA"),       /* pp笔记本 无线 */
    QString("2C:56:DC:4A:1A:6D"),       /* pp家    台式*/
    QString("E8:6A:64:B0:49:49"),       /* xcy办公电脑 */
    QString("F8:75:A4:35:80:7A"),
};


/********************************** 函数实现区 *********************************/
QString GetQtVerStr(void)
{
    QString verStr;
    verStr.append(_GIT_REPO_HASH_);
    verStr.append(_SPLITER_);
    verStr.append(_BLD_TIME_);
    return verStr;
}

bool SplitVerStr(QString &verStr, QString &bldTimeStr, const QString &str, const QString &spliter)
{
    QStringList strList;
    strList = str.split(spliter);

    if(strList.size() < 2)
    {
        return false;
    }
    
    verStr = strList[0];
    bldTimeStr = strList[1];
    return true;
}

bool ReadQtAppCfg(QByteArray &data)
{
    QString qtAppCfgFilePath = GetQtAppCfgPath();
    return ReadFile(data, qtAppCfgFilePath);
}

bool WriteQtAppCfg(const QByteArray &data)
{
    QString qtAppCfgFilePath = GetQtAppCfgPath();
    return WriteFile(data, qtAppCfgFilePath);
}

bool ReadInterfacesFile(QByteArray &data)
{
    QString interfacesFilePath = GetInterfacesFilePath();
    return ReadFile(data, interfacesFilePath);
}

bool WriteInterfaceFile(const QByteArray &data)
{
    QString interfacesFilePath = GetInterfacesFilePath();
    bool ret = WriteFile(data, interfacesFilePath);
    return ret;
}

bool IsIpValid(const QString &ipStr)
{
    QRegExp validIp("\\d+\\.\\d+\\.\\d+\\.\\d+");

    if(validIp.exactMatch(ipStr))
    {
        return true;
    }
    
    return false;
}

bool QtAppUpgrade(const QString& infoFileName, const QString& daemonImageFileName,
                           const QString& pwrImageFileName, const QString& uiImageFileName,
                           const QString& scriptUpgradeFile)
{
    /* step1: 校验 并读取 */
    QByteArray daemonImageData, pwrImageData, uiImageData;

    /* 守护进程校验 */
    if((!daemonImageFileName.isEmpty()) && (!CheckAndGetImage(daemonImageData, infoFileName, daemonImageFileName)))
    {
        qCritical() << "check daemon upgrade file failed !";
        return false;
    }
    
    /* 电源进程校验 */
    if((!pwrImageFileName.isEmpty()) && (!CheckAndGetImage(pwrImageData, infoFileName, pwrImageFileName)))
    {
        qCritical() << "check pwr upgrade file failed !";
        return false;
    }
    
    /* ui进程校验 */
    if((!uiImageFileName.isEmpty()) && (!CheckAndGetImage(uiImageData, infoFileName, uiImageFileName)))
    {
        qCritical() << "check ui upgrade file failed !";
        return false;
    }
    
    /* step3: 写文件 */
    QString daemonNeedUpgradeName = GetQtAppNeedUpgradeName(daemonImageFileName);
    QString pwrNeedUpgradeName = GetQtAppNeedUpgradeName(pwrImageFileName);
    QString uiNeedUpgradeName = GetQtAppNeedUpgradeName(uiImageFileName);
    qDebug() << "daemonNeedUpgradeName = " << daemonNeedUpgradeName;
    qDebug() << "pwrNeedUpgradeName = " << pwrNeedUpgradeName;
    qDebug() << "uiNeedUpgradeName = " << uiNeedUpgradeName;
    
    /* 写入守护进程文件 */
    if((!daemonImageFileName.isEmpty()) && (!WriteFile(daemonImageData, daemonNeedUpgradeName)))
    {
        QString msg("Qt升级daemon镜像写入失败");
        ShowMsgBoxInfo(msg);
        qCritical() << msg;
        return false;
    }
    
    /* 写入电源进程文件 */
    if((!pwrImageFileName.isEmpty()) && (!WriteFile(pwrImageData, pwrNeedUpgradeName)))
    {
        QString msg("Qt升级pwr镜像写入失败");
        ShowMsgBoxInfo(msg);
        qCritical() << msg;
        return false;
    }
    
    /* 写入UI进程文件 */
    if((!uiImageFileName.isEmpty()) && (!WriteFile(uiImageData, uiNeedUpgradeName)))
    {
        QString msg("Qt升级ui镜像写入失败");
        ShowMsgBoxInfo(msg);
        qCritical() << msg;
        return false;
    }
    
    /* step4: 置标记 */
    if(!daemonImageFileName.isEmpty())
    {
        QString daemonExe =  GetQtAppRootPath() + GetFileNameFromPath(daemonImageFileName);
        if(!SetQtImageFlag(daemonExe))
        {
            QString msg("Qt升级daemon镜像置标记失败");
            qCritical() << msg;
            ShowMsgBoxInfo(msg);
            return false;
        }
    }
    
    if(!pwrImageFileName.isEmpty())
    {
        QString pwrExe =  GetQtAppRootPath() + GetFileNameFromPath(pwrImageFileName);
        if(!SetQtImageFlag(pwrExe))
        {
            QString msg("Qt升级pwr镜像置标记失败");
            ShowMsgBoxInfo(msg);
            qCritical() << msg;
            return false;
        }
    }
    
    if(!uiImageFileName.isEmpty())
    {
        QString uiExe =  GetQtAppRootPath() + GetFileNameFromPath(uiImageFileName);
        if(!SetQtImageFlag(uiExe))
        {
            QString msg("Qt升级镜像置标记失败");
            ShowMsgBoxInfo(msg);
            qCritical() << msg;
            return false;
        }
    }
    if(scriptUpgradeFile.isEmpty())
    {
        return UpgradeInitScript();
    }
    
    return UpgradeInitScript(scriptUpgradeFile);

}

bool CheckAndGetMcuImageFilePath(QByteArray &imageA, QByteArray &imageB, const QString &imageFileName, const QString& infoFileName)
{
    qDebug() << "check mcu image: " << imageFileName << ", infoFile: " << infoFileName;
    /* step1: A面 */
    QString mcuImageNameA = imageFileName + ".bin.a";
    if(!CheckAndGetImage(imageA, infoFileName, mcuImageNameA))
    {
        return false;
    }

    /* step2: b面 */
    QString mcuImageNameB = imageFileName + ".bin.b";
    if(!CheckAndGetImage(imageB, infoFileName, mcuImageNameB))
    {
        return false;
    }
    
    return true;
}

void MakeUnitStr2FactorHash(void)
{
    /* 仅执行一次,优化效率 */
    if(0 == sUnitStr2FactorHash.size())
    {
        sUnitStr2FactorHash["nV"] = 1e-9;
        sUnitStr2FactorHash["uV"] = 1e-6;
        sUnitStr2FactorHash["mV"] = 1e-3;
        sUnitStr2FactorHash[" V"] = 1e+0;
        sUnitStr2FactorHash["nA"] = 1e-9;
        sUnitStr2FactorHash["uA"] = 1e-6;
        sUnitStr2FactorHash["mA"] = 1e-3;
        sUnitStr2FactorHash[" A"] = 1e+0;
    }
}

QStringList GetAllVUnit(void)
{
    MakeUnitStr2FactorHash();

    QStringList list = sUnitStr2FactorHash.keys();
    return list.filter("V");
}

QStringList GetAllIUnit(void)
{
    MakeUnitStr2FactorHash();

    QStringList list = sUnitStr2FactorHash.keys();
    return list.filter("A");
}

double UnitStr2Factor(const QString &unitStr)
{
    QString unit(unitStr);
    /* 避免A/V单位前面没有空格解析错误 */
    if(unit.length() == 1)
    {
        unit.insert(0, " ");
    }
    MakeUnitStr2FactorHash();

    return sUnitStr2FactorHash[unit];
}

double ValWithUnitToStdVal(double valWithUnit, const QString &unitStr)
{
    double factor = UnitStr2Factor(unitStr);
    double stdVal = valWithUnit * factor;
    return stdVal;
}

double StdValToValWithUnit(double stdVal, const QString &unitStr)
{
    double factor = UnitStr2Factor(unitStr);
    double valWithUnit = stdVal / factor;
    return valWithUnit;
}

QString JoinPath(const QString &path, const QString &name)
{
    QDir dir(path);
    QString fullPath = dir.absoluteFilePath(name);
    return fullPath;
}

QString GetQtAppRootPath(void)
{
#ifdef Q_OS_LINUX
    QString qtAppPath = QString("/app/");
#else
    QString qtAppPath = QString("..\\PcDemo\\PcApp\\");
#endif
    return qtAppPath;
}

QString GetUsbImagePath(void)
{
#ifdef Q_OS_LINUX
    QString usbImagePath = QString("/mnt/usb/PssImages/");
#else
    QString usbImagePath = QString("..\\PcDemo\\PssImages\\");
#endif
    return usbImagePath;
}

QString GetLocalQtInfoFileName(void)
{
    QString usbImagePath = GetUsbImagePath();
    QString infoFilePath = JoinPath(usbImagePath, "info");
    return infoFilePath;
}


QString GetLocalQtDaemonAppImageName(void)
{
    QString usbImagePath = GetUsbImagePath();
    QString qtAppExeFileName = GetQtDaemonAppExeFileName();
    QString qtAppImagePath = JoinPath(usbImagePath, qtAppExeFileName);
    return qtAppImagePath;
}

QString GetLocalQtPwrAppImageName(void)
{
    QString usbImagePath = GetUsbImagePath();
    QString qtAppExeFileName = GetQtPwrAppExeFileName();
    QString qtAppImagePath = JoinPath(usbImagePath, qtAppExeFileName);
    return qtAppImagePath;
}

QString GetLocalQtUIAppImageName(void)
{
    QString usbImagePath = GetUsbImagePath();
    QString qtAppExeFileName = GetQtUIAppExeFileName();
    QString qtAppImagePath = JoinPath(usbImagePath, qtAppExeFileName);
    return qtAppImagePath;
}

QString GetOnlineQtInfoFileName(void)
{
    QString onlineImagePath = GetOnlineImagePath();
    QString infoFilePath = JoinPath(onlineImagePath, "info");
    return infoFilePath;
}

QString GetOnlineQtDaemonAppImageName(void)
{
    QString onlineImagePath = GetOnlineImagePath();
    QString qtAppExeFileName = GetQtDaemonAppExeFileName();
    QString qtAppImagePath = JoinPath(onlineImagePath, qtAppExeFileName);
    return qtAppImagePath;
}

QString GetOnlineQtPwrAppImageName(void)
{
    QString onlineImagePath = GetOnlineImagePath();
    QString qtAppExeFileName = GetQtPwrAppExeFileName();
    QString qtAppImagePath = JoinPath(onlineImagePath, qtAppExeFileName);
    return qtAppImagePath;
}

QString GetOnlineQtUIAppImageName(void)
{
    QString onlineImagePath = GetOnlineImagePath();
    QString qtAppExeFileName = GetQtUIAppExeFileName();
    QString qtAppImagePath = JoinPath(onlineImagePath, qtAppExeFileName);
    return qtAppImagePath;
}


QString GetCsvFilePath(void)
{
#ifdef Q_OS_LINUX
    QString csvFilePath = QString("/mnt/usb/");
#else
#ifndef _PSS_QT_NO_GUI_
    QString csvFilePath = QApplication::applicationDirPath();
#else
    QString csvFilePath("./");
#endif
#endif
     return csvFilePath;
}

QString GetQtAppCfgPath(void)
{
    QString qtAppPath = GetQtAppRootPath();
#ifdef Q_OS_LINUX /* ARM_Linux 实际运行 */
    QString qtAppCfgFilePath = JoinPath(qtAppPath, "cfg.json");
#else             /* X86_Win   调试 */
    QString qtAppCfgFilePath = JoinPath(qtAppPath, "Pc.json");
#endif
    return qtAppCfgFilePath;
}

QString GetQtDaemonAppExeFileName(void)
{
    return QString("DaemonProcess");
}

QString GetQtPwrAppExeFileName(void)
{
    return QString("PwrProcess");
}

QString GetQtUIAppExeFileName(void)
{
    return QString("UIProcess");
}

QString GetInterfacesFilePath(void)
{
#ifdef Q_OS_LINUX /* ARM_Linux 实际运行 */
    QString interfacesFilePath = QString("/etc/network/interfaces");
#else             /* X86_Win   调试 */
    QString qtAppPath = GetQtAppRootPath();
    QString interfacesFileName = QString("interfaces.txt");
    QString interfacesFilePath = JoinPath(qtAppPath, interfacesFileName);
#endif

    return interfacesFilePath;
}

bool ReadFile(QByteArray &data, const QString &fileName)
{
    QString name(fileName);
    name = name.simplified();
    QFile file(name);
    if(!file.open(QIODevice::ReadOnly))
    {
        qCritical() << "open file " << name << " failed: " << file.errorString();
        return false;
    }
    data = file.readAll();
    /* qDebug() << fileName << "readded."; */
    file.close();

    return true;
}

bool WriteFile(const QByteArray &data, const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        qCritical() << "open file: " << fileName << " failed: " << file.errorString();
        return false;
    }
    qint64 writeBytesNum = file.write(data);
    if(writeBytesNum != data.size())
    {
        qCritical() << "write file: " << fileName << " failed: " << file.errorString();
        file.close();
        return false;
    }
    file.flush();
    file.close();
#ifdef Q_OS_LINUX
   system("sync") ;
#endif
    return true;
}

bool CheckAndGetImage(QByteArray &image, const QString& infoFileName, const QString &fileName)
{
    /* step1: 读取 */
    QByteArray imageReadded;
#if 1
    if(!ReadFile(imageReadded, fileName))
    {
        QString msg = QString("%1 读取失败").arg(fileName);
        qCritical() << msg;
        ShowMsgBoxInfo(msg);
        return false;
    }

    /* step2: 校验 */
    QString sha256Readded = GetInfoSha256(infoFileName, fileName);
    QString sha256Computed = ComputeSha256(imageReadded);
    if(sha256Readded != sha256Computed)
    {
        QString msg = QString("%1 校验失败").arg(fileName);
        qCritical() << msg;
        ShowMsgBoxInfo(msg);
        return false;
    }
#endif
    /* step3: 返回 */
    image.clear();
    image.append(imageReadded);
    return true;
}

bool IsQtAppAValid(const QString &fileName)
{
#ifdef Q_OS_LINUX /* ARM_Linux 实际运行 */
    QFileInfo appInfo(fileName);
    QString target = appInfo.symLinkTarget();
    QFileInfo targetInfo(target);
    QString abSuffix = targetInfo.suffix();
    qDebug() << fileName << " => " << abSuffix;

    if("a" == abSuffix)
    {
        return true;
    }
    else if("b" == abSuffix)
    {
        return false;
    }
    else
    {
        QString msg("Invalid upgrade file: ");
        msg += fileName;
        qCritical() << msg;
        msg += QString(", please reboot the device to abort !");
        ShowMsgBoxInfo(msg);
        while(1);
    }
#else             /* X86_Win   调试 */
    QString qtAppPath = fileName;
    QString abFile = JoinPath(qtAppPath, "a");
    QFileInfo fileInfo(abFile);
    qDebug() << "debug upgrade file: " << abFile;
    if(fileInfo.isFile())
    {
        return true;
    }
    else
    {
        return false;
    }
#endif
}

QString GetQtAppNeedUpgradeName(const QString &fileName)
{
    QString qtAppExeRootPath = GetQtAppRootPath();
    QFileInfo fileInfo(fileName);

    QString qtAppNeedUpgradeName = qtAppExeRootPath + "/" + fileInfo.fileName();
    /*
     * ab写入逻辑
     * 当前使用a,写入b,
     * 当前使用b,写入a */
    if(IsQtAppAValid(qtAppNeedUpgradeName))
    {
        qtAppNeedUpgradeName.append(".b");
    }
    else
    {
        qtAppNeedUpgradeName.append(".a");
    }
    
    return qtAppNeedUpgradeName;
}

bool SetQtImageFlag(const QString &fileName)
{
#ifdef Q_OS_LINUX /* ARM_Linux 实际运行 */
    QString qtAppExeFilePath = fileName;
    QString src = GetQtAppNeedUpgradeName(fileName);

    /* step1: 删除原来的符号链接 */
    
    QString cmd = QString("rm -rf %1").arg(qtAppExeFilePath);
    RunSystemCmd(cmd);
    qDebug() << "remove cmd = " << cmd;
    /* step2: 创建新符号链接 */
    QString dest = qtAppExeFilePath;
    cmd = QString("ln -s %1 %2").arg(src).arg(dest);
    RunSystemCmd(cmd);
    qDebug() << "create cmd = " << cmd;
    
    /* step3: 同步文件系统 */
    cmd = QString("sync");
    RunSystemCmd(cmd);
    RunSystemCmd(cmd);
    RunSystemCmd(cmd);
#else             /* X86_Win   调试 */
    QString dest = GetQtAppNeedUpgradeName(fileName);
    QString qtAppPath = GetQtAppRootPath();
    QString addFileName;
    QString delFileName;
    QFileInfo fileInfo(dest);
    if("a" == fileInfo.suffix())
    {
        addFileName = JoinPath(qtAppPath, "a");
        delFileName = JoinPath(qtAppPath, "b");
    }
    else
    {
        addFileName = JoinPath(qtAppPath, "b");
        delFileName = JoinPath(qtAppPath, "a");
    }

    QByteArray ba;
    ba.clear();
    WriteFile(ba, addFileName);
    
    /* ryl:此处可考虑不删除，用来应急回退 */
    QDir dir(qtAppPath);
    dir.remove(delFileName);
    
    /* 消除X86 PC编译告警 */
    RunSystemCmd("echo");
#endif
    return true;
}

bool CheckInitScript(const QString &fileName)
{
    QString scriptFile = QDir::cleanPath(fileName);
    QFile file(scriptFile);
    qDebug() << "script file name: " << scriptFile;
    return file.exists();
}

bool UpgradeInitScript(const QString& fileName)
{
#if 0
    QString scriptFile = QDir::cleanPath(fileName);
    QString cmd(QString("cp %1 /etc/init.d/").arg(scriptFile));
    qDebug() << "upgrade script cmd: " << cmd;
    QProcess proc;
    proc.execute(cmd);
#else

    QByteArray scriptData;
    scriptData.append("#!/bin/sh\n \
        \n\
        HOSTNAME=\n\
        VERSION=\n\
        ROOTFS=\n\
        CFG_TIME=\n\
        \n\
        IPADDR=`ifconfig eth0 | grep \"inet addr\"`\n\
        IPINET=${IPADDR%Bcast*}\n\
        IPADDR=${IPINET#*addr:}\n\
        \n\
        HOST_BIN=/bin/hostname\n\
        VER_FILE=/etc/version\n\
        RTFS_FILE=/etc/rootfs\n\
        CFG_FILE=/etc/config\n\
        INIT_FILE=/etc/myzr.d/myzr_init\n\
        \n\
        myzr_setenv()\n\
        {\n\
            if [ -f ${HOST_BIN} ]; then\n\
                HOSTNAME=`${HOST_BIN}`\n\
            fi\n\
            \n\
            if [ -f ${VER_FILE} ]; then\n\
                VERSION=`cat ${VER_FILE}`\n\
            fi\n\
            \n\
            if [ -f ${RTFS_FILE} ]; then\n\
                ROOTFS=`cat ${RTFS_FILE}`\n\
            fi\n\
            \n\
            if [ -f ${CFG_FILE} ]; then\n\
                CFG_TIME=`cat ${CFG_FILE}`\n\
            fi\n\
        }\n\
        \n\
        myzr_welcome()\n\
        {\n\
            echo \"\"\n\
            echo \"*** Welcome to i.MX6 *************************************** \"\n\
            echo \"    Rootfs File: ${ROOTFS} \"\n\
            echo \"    Build Time:  ${VERSION} \"\n\
            echo \"    Config Time: ${CFG_TIME} \"\n\
            echo \"    IP Address:  ${IPADDR} \"\n\
            echo \"\"\n\
            echo \"    SITE: www.myzr.com.cn \"\n\
            echo \"    WIKI: wiki.myzr.com.cn \"\n\
            echo \"    BBS:  bbs.myzr.com.cn \"\n\
            echo \"************************************************************* \"\n\
        }\n\
        \n\
        myzr_board_config()\n\
        {\n\
            case ${HOSTNAME} in\n\
                *)\n\
                    ;;\n\
            esac\n\
        }\n\
        \n\
        if [ -f ${INIT_FILE} ]; then\n\
            source ${INIT_FILE}\n\
        else\n\
            myzr_setenv\n\
            myzr_welcome\n\
            myzr_board_config\n\
        fi\n\
        \n\
        # 首次上电处理app目录\n\
        \n\
        if [ -f /app/DaemonProcess.a  ]; then\n\
            echo \"app have done.\"\n\
        else\n\
            #step1: 修改root密码\n\
            echo -e \"pss123\\npss123\" | passwd root\n\
            \n\
            #step2: 实现Qt主程序AB面\n\
            cp -rf /app/DaemonProcess /app/DaemonProcess.a\n\
            cp -rf /app/DaemonProcess /app/DaemonProcess.b\n\
            rm -rf /app/DaemonProcess\n\
            ln -s /app/DaemonProcess.a /app/DaemonProcess\n\
            \n\
            cp -rf /app/UIProcess /app/UIProcess.a\n\
            cp -rf /app/UIProcess /app/UIProcess.b\n\
            rm -rf /app/UIProcess\n\
            ln -s /app/UIProcess.a /app/UIProcess\n\
            \n\
            cp -rf /app/PwrProcess /app/PwrProcess.a\n\
            cp -rf /app/PwrProcess /app/PwrProcess.b\n\
            rm -rf /app/PwrProcess\n\
            ln -s /app/PwrProcess.a /app/PwrProcess\n\
            \n\
            sync\n\
        fi\n\
        \n\
        # 配置Can\n\
        ip link set can0 up type can bitrate 500000\n\
        echo 4096 > /sys/class/net/can0/tx_queue_len\n\
        \n\
        # 启动主程序\n\
        \n\
        /app/UIProcess -platform eglfs -plugin evdevtouch:/dev/input/event0 &\n\
        /app/DaemonProcess -platform eglfs -plugin evdevtouch:/dev/input/event0 &\n\
        sleep 1s\n\
        /app/PwrProcess -platform eglfs -plugin evdevtouch:/dev/input/event0 &\n\
        sleep 1s\n\0"
        );
    QString qtVer = QString("\n#Qt版本号\n\\#QtVer: %1\n").arg(GetQtVerStr());
    scriptData.append(qtVer);
    QString backupFile(fileName);
    backupFile += ".bak";
    QString cmd = QString("cp %1 %2").arg(fileName).arg(backupFile);
    QFile file(fileName);
    QProcess proc;
    int ret = proc.execute(cmd);
    qDebug() << "exec cmd: " << cmd << " return : " << ret;
    if(!file.open(QIODevice::ReadWrite))
    {
        qCritical() << "open start script file: " << fileName << " failed: " << file.errorString();
        file.close();
        QString cmd = QString("mv %1 %2").arg(backupFile).arg(fileName);
        ret = proc.execute(cmd);
        qDebug() << "exec cmd: " << cmd << " return : " << ret;
        return false;
    }
    
    ret = file.write(scriptData);
    if(ret != scriptData.size())
    {
        qCritical() << "write start script failed: " << file.errorString();
        file.close();
        QString cmd = QString("mv %1 %2").arg(backupFile).arg(fileName);
        ret = proc.execute(cmd);
        qDebug() << "exec cmd: " << cmd << " return : " << ret;
        return false;
    }
    file.flush();
    file.close();
    cmd = QString("chmod +x %1").arg(fileName);
    ret = proc.execute(cmd);
    qDebug() << "exec cmd: " << cmd << " return : " << ret;
    system("sync");
    system("sync");
    QFile::remove(backupFile);
#endif
    return true;
}

QString GetInfoSha256(const QString& infoFileName, const QString &fileName)
{
    QString onlyFileName = GetFileNameFromPath(fileName);
    /* step1: 读取info文件 */
    QByteArray infoData;
    if(!ReadFile(infoData, infoFileName))
    {
#ifndef _PSS_QT_NO_GUI_
        ShowMsgBoxInfo("info文件读取失败");
#endif
        qCritical() << "info文件读取失败 !";
        return "";
    }

    /* step2: 查找fileName指示的sha256 */
    QString str = QString(infoData);
    QStringList strList = str.split("\n");
    for(int i = 0; i < strList.size(); i++)
    {
        QString imageName;
        QString sha256;
        SplitSha256FromStrLine(imageName, sha256, strList[i]);
        if(imageName == onlyFileName)
        {
            return sha256;
        }
    }
    
    QString msg = QString("%1 info中未找到对应的SHA256").arg(fileName);
    qCritical() << msg;
#ifndef _PSS_QT_NO_GUI_
    ShowMsgBoxInfo(msg);
#endif
    return "";
}

void SplitSha256FromStrLine(QString &fileName, QString &sha256, const QString &lineStr)
{
    QStringList strList = lineStr.split(" *");
    if(strList.size() < 2)
    {
        qDebug() << "null file for 256 info !";
        return ;
    }
    sha256 = strList[0];
    fileName = strList[1];
    return;
}

QString ComputeSha256(const QByteArray &data)
{
    u8 hash[32] = {0};
    char hashStr[65] = {0};
    const u8 *dataPtr = (const u8*)(data.constData());
    i32 dataSize = data.count();
    PssCLibSha256(dataPtr, dataSize, hash);

    int i = 0;
    for(i = 0; i < 32; i++)
    {
        char *pos = hashStr + (i * 2);
        sprintf(pos, "%02x", hash[i]);
    }
    
    return QString(hashStr);
}

QString GetFileNameFromPath(const QString &path)
{
    QFileInfo fileInfo = QFileInfo(path);
    return fileInfo.fileName();
}

bool RunSystemCmd(const QString &cmd)
{
    const size_t cBufSize = 1024;
    char buf[cBufSize] = {0};
    QString2Utf8(buf, cBufSize, cmd);
    int rst = system(buf);

    if(0 != rst)
    {
        qCritical() << cmd << "return err:" <<rst;
        return false;
    }
    
    return true;
}

QStringList GetAvailableComNames(void)
{
    QStringList allComNames;
    allComNames.clear();
    foreach (const QSerialPortInfo &serInfo, QSerialPortInfo::availablePorts())
    {
        allComNames.append(serInfo.portName());
    }

    return allComNames;
}

void LinearFitting(const QVector<double> &x, const QVector<double> &y, double &c1, double &c0, double &sqSum)
{
    PssCLibLeastSquare(x.constData(), y.constData(), x.size(), &c1, &c0, &sqSum);
}


size_t QString2Bytes(Bytes &bytes, const QString &str)
{
    QByteArray ba = str.toUtf8();

    size_t size = ba.size();
    bytes.Append((const uint8_t *)ba.constData(), size);
    return size;
}

void Bytes2QString(QString &str, const Bytes &bytes)
{
    str.append((const char *)bytes.ConstData());
}

bool CheckRTC(void)
{
    return sRtcOnFlag;
}

void SetRTC(bool on)
{
    sRtcOnFlag = on;
}

bool Sync2NetTime(void)
{
    QString cmd("ntpdate -u 202.112.29.82");
    int ret = 0;
    ret = system(cmd.toUtf8().constData());
    if(!CheckSystemCallResult(ret))
    {
        qCritical() << "ntpdate -u 202.112.29.82 failed !";
        return false;
    }
#if 1
    /* 网络时间比北京时间慢8小时 */
    QTime time = QTime::currentTime();
    time.setHMS(time.hour() + 8, time.minute(), time.second(), time.msec());
    QDateTime date = QDateTime::currentDateTime();
    date.setTime(time);
    cmd = QString("date -s \"%1\"").arg(date.toString("yyyy-MM-dd hh:mm:ss"));
    qDebug() << "set system time: " << cmd;
    ret = system(cmd.toUtf8().constData());
    if(!CheckSystemCallResult(ret))
    {
        qCritical() << "date -s failed !";
        return false;
    }
#endif
    if(CheckRTC())
    {
        /* 同步系统时间和RTC时间 */
        cmd = QString("hwclock --systohc");
        ret = system(cmd.toUtf8().constData());
        if(!CheckSystemCallResult(ret))
        {
            qCritical() << "hwclock --systohc failed !";
            return false;
        }
    }
    return true;
}


void SaveValToFile(const QVector<double> &xVals, const QVector<double> &yVals, QString suffix)
{
    /* 构造文件名 */
    QString csvFilePath = GetCsvFilePath();
    QString fileName = GetNowDateTimeStr(false, true, true);
    if(0 != suffix.size())
    {
        fileName = fileName + "_" + suffix;
    }
    fileName += ".csv";
    QString fullFileName = JoinPath(csvFilePath, fileName);

    /* 构造数据 */
    QByteArray ba;
    int size = xVals.size();
    for(int i = 0;i < size; i++)
    {
        QString dataLineStr = QString("%1,%2\n").arg(xVals[i]).arg(yVals[i]);
        ba.append(dataLineStr.toUtf8());
    }


    /* 写入数据 */
    if(WriteFile(ba, fullFileName))
    {
        qDebug() << QString("%1 is writed.").arg(fullFileName);
    }
}

double GetMax(const QVector<double> &vals)
{
    if(vals.isEmpty())
    {
        ShowMsgBoxInfo("empty QVector haven't max value.");
        return 0;
    }

    auto max = std::max_element(std::begin(vals), std::end(vals));
    return *max;
}

double GetMin(const QVector<double> &vals)
{
    if(vals.isEmpty())
    {
        ShowMsgBoxInfo("empty QVector haven't min value.");
        return 0;
    }

    auto min = std::min_element(std::begin(vals), std::end(vals));
    return *min;
}


double UnitVal2StdVal(const QString &strVal)
{
    double val = 0;
    double fact = 1.0;
    do{
        if(strVal.contains("V"))
        {
            break;
        }
        else if(strVal.contains("A"))
        {
            break;
        }

        return val;
    
    }while(0);
    
    int i = 0;
    for(i = strVal.length() - 1;i >= 0;i--)
    {
        if(strVal[i].isDigit())
        {
            break;
        }
    }
    
    fact = UnitStr2Factor(strVal.right(strVal.length() - i - 1));
    val = strVal.left(i + 1).toDouble() * fact;
    
    return val;
}


void ClearIOTrashData(QtIo *io)
{
    int cnt = 5;
    while(--cnt)
    {
#ifndef _PSS_QT_NO_GUI_
        QCoreApplication::processEvents();
#endif
        if(NULL == io)
        {
            break ;
        }

        char buf[1024] = {0};
        int ret = io->Recv(buf, sizeof(buf), 50);
        if(ret > 0)
        {
            //qDebug() << "Recv Trash Data From IO: " <<  buf;
        }
    }
}

#ifdef Q_OS_LINUX
static void DumpBackTrace(int signo)
{
    QMutexLocker locker(&sBackTraceLock);
    signal(signo, DumpBackTrace);
    if(SIGPIPE == signo)
    {
        qCritical() << "sigpipe signals crashed !";
        return ;
    }
    const int bufSize = 1024;
    void *buffer[bufSize] = {0};
    size_t size;
    char **strings = NULL;
    size_t i = 0;

    size = backtrace(buffer, bufSize);
#ifdef _PSS_QT_
    qCritical() << " ** signo: " << signo << " Obtained " << size << " stack frames.nm **";
#else
    fprintf(stderr, " ** signo: %d Obtained %zd stack frames.nm **\n", signo, size);
#endif

    strings = backtrace_symbols(buffer, size);
    if (strings == NULL)
    {

#ifdef _PSS_QT_
        qCritical() << strerror(errno);
#else
        perror("backtrace_symbols.");
#endif
        exit(-1);
    }

    for (i = 0; i < size; i++)
    {
#ifdef _PSS_QT_
        qCritical() << strings[i];
#else
        fprintf(stderr, "%s\n", strings[i]);
#endif

    }
    free(strings);
    strings = NULL;
    exit(-1);
}

void DumpBackTraceInit(void)
{
    signal(SIGSEGV, DumpBackTrace);
    signal(SIGINT, DumpBackTrace);
    signal(SIGILL, DumpBackTrace);
    signal(SIGPIPE, DumpBackTrace);
}

void ThreadDelay(uint delayUs)
{
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    struct timeval tm;
    tm.tv_sec = delayUs / 1000000;
    tm.tv_usec = delayUs % 1000000;
    select(1, &rfds, NULL, NULL, &tm);
}
#endif


bool IdnStr2VerStr(QString& qtVer, QString &fntVer, QString& anlgVer, QString& pwrVer, const QString& idnStr)
{
    QStringList list;
    list = idnStr.split(",");
    if(list.size() < 5)
    {
        qCritical() << "Invalid Ind str: " << idnStr;
        return false;
    }

    anlgVer = list.at(1);
    qtVer = list.at(2);
    fntVer = list.at(3);
    pwrVer = list.at(4);
    return true;
}

#ifndef _PSS_QT_NO_GUI_
void ShowVer2Label(QLabel *labelVer, QLabel *labelBldTime, const QString &ver, const QString &spliter)
{
    QString verStr;
    QString bldTimeStr;
    /* 版本相关 */
    bool done = SplitVerStr(verStr, bldTimeStr, ver, spliter);
    if(!done)
    {
        QString msg = QString("%1:err version string.").arg(ver);
        ShowMsgBoxInfo(msg);
        qCritical() << msg;
        return ;
    }
    labelVer->setText(verStr);
    labelBldTime->setText(bldTimeStr);
}

void ShowQtVerToUi(QLabel *labelVer, QLabel *labelBldTime)
{
    /* 版本相关 */
    QString qtVerStr = GetQtVerStr();
    ShowVer2Label(labelVer, labelBldTime, qtVerStr);
}

#endif

/* 获取机器mac地址列表 */
QStringList GetMacStrList(void)
{
    QStringList macList;
    QString strMac;
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    for (int i = 0; i < ifaces.count(); i++)
    {
        QNetworkInterface iface = ifaces.at(i);
        /* 过滤掉本地回环地址、没有开启的地址 */
        if((iface.flags().testFlag(QNetworkInterface::IsUp))                                /* 网口必须开启 */
                && (!iface.flags().testFlag(QNetworkInterface::IsLoopBack))                 /* 非回环 */
                && (!iface.humanReadableName().contains("VMware", ::Qt::CaseInsensitive))) /* 非虚拟地址 */
        {
            strMac = iface.hardwareAddress();
            macList.append(strMac);
        }
    }

    return macList;
}

bool IsValidPc(void)
{
    QString currencMacStr = GetActiveMacStr();
    //qDebug() << currencMacStr;
    int validMacStrSize = sizeof(sValidMacStrList) / sizeof(sValidMacStrList[0]);
    for(int i = 0; i < validMacStrSize; i++)
    {
        int rst = currencMacStr.compare(sValidMacStrList[i]);
        if(0 == rst)
        {
            return true;
        }
    }

    return false;
}

/* 获取机器使用中的mac地址 */
QString GetActiveMacStr(void)
{
    QStringList macList = GetMacStrList();
    int size = macList.count();
    if(0 == size)
    {
        ShowMsgBoxInfo(QStringLiteral("请使能已认证网口."));
        return "";
    }
    if(1 != size)
    {
        ShowMsgBoxInfo(QStringLiteral("请关闭未使用的网口."));
        return "";
    }

    QString currencMacStr = macList[0];
    return currencMacStr;
}

/* 明文 转为 密文 */
/* FIXME: 实现密码加盐 */
QString QString2Passwd(const QString &passwdStr)
{
    /* 密文 = SHA256(明文) */
    QByteArray ba = passwdStr.toUtf8();
    QString passwd = ComputeSha256(ba);

    return passwd;
}

/* 密码合法性判定 */
bool IsPasswdValid(const QString &passwdStr, const QString &passwd)
{
    QString computePasswd = QString2Passwd(passwdStr);
    if(0 == computePasswd.compare(passwd))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* 判断浮点数是否相等 */
bool IsDoubleEqual(double val1, double val2, unsigned char precision)
{
    if((val1 >= (val2 - pow(10.0, -1.0 * precision))) && (val1 <= (val2 + pow(10.0, -1.0 * precision))))
    {
        return true;
    }
    return false;
}

static QString GetThreadIdStr(void)
{
    QThread *ptrThread = QThread::currentThread();
    QString str = Ptr2QString(ptrThread);

    return str;
}

QString Ptr2QString(void *ptr)
{
    QString str = QString("0x%1")
            .arg(QString::number((unsigned long long)ptr, 16));

    return str;
}


QString GetNowDateTimeStr(bool useBracket, bool hasMs, bool isFileNameFmt)
{
    QString dateSpliter = "-";
    QString timeSpliter = ":";
    QString dateTimeSpliter = " ";
    if(isFileNameFmt)
    {
        dateSpliter = "_";
        timeSpliter = "_";
        dateTimeSpliter = "_";
    }

    QDateTime now = QDateTime::currentDateTime();
    QDate date = now.date();
    QTime time = now.time();
    QString dateStr = QString("%1%2%3%4%5%6")
        .arg(date.year(), 4, 10, QChar('0'))
        .arg(dateSpliter)
        .arg(date.month(), 2, 10, QChar('0'))
        .arg(dateSpliter)
        .arg(date.day(), 2, 10, QChar('0'))
        .arg(dateTimeSpliter);
    QString timeStr = QString("%1%2%3%4%5")
        .arg(time.hour(), 2, 10, QChar('0'))
        .arg(timeSpliter)
        .arg(time.minute(), 2, 10, QChar('0'))
        .arg(timeSpliter)
        .arg(time.second(), 2, 10, QChar('0'));
    
    if(hasMs)
    {
        QString msStr = QString("%1%2")
            .arg(timeSpliter)
            .arg(time.msec(), 3, 10, QChar('0'));
        timeStr.append(msStr);
    }
    
    QString dateTimeStr;
    if(useBracket)
    {
        dateTimeStr.append("[");
    }
    dateTimeStr.append(dateStr);
    dateTimeStr.append(timeStr);
    if(useBracket)
    {
        dateTimeStr.append("]");
    }
    
    return dateTimeStr;
}


static bool QDbgOpenLogFile(const QString& fileName, QString& errMsg)
{
    QMutexLocker locker(&sLogLock);
    if(sLogFile.isOpen())
    {
        return true;
    }
    sLogFile.setFileName(fileName);
    if(sLogFile.exists())
    {
        if(!sLogFile.setPermissions(QFileDevice::WriteOther | QFileDevice::ReadOther))
        {
            qInstallMessageHandler(0);
            errMsg = sLogFile.errorString();
            qCritical() << "set log file permission: " << errMsg;
            return false;
        }
    }
    if(!sLogFile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text))
    {
        qInstallMessageHandler(0);
        errMsg = sLogFile.errorString();
        qCritical() << "open log file : " << errMsg;
        return false;
    }

    sCurrentLogFileName = fileName;
    return true;
}

static bool QDbgCloseLogFile(QString &errMsg)
{
    QMutexLocker locker(&sLogLock);
    if(sLogFile.isOpen())
    {
        if(!sLogFile.setPermissions(QFileDevice::ReadOwner))
        {
            errMsg = sLogFile.errorString();
            sLogFile.close();
            qInstallMessageHandler(0);
            qCritical() << "set log file permission failed: " << errMsg;
            return false;
        }
        sLogFile.close();
    }
    return true;
}

static QString GetDbgHeadStr(QtMsgType type, const QMessageLogContext &context)
{
    QString text;
    switch (type)
    {
        case QtDebugMsg:
            text = QString("Debug:");
            break;
        case QtInfoMsg:
            text = QString("Info: ");
            break;
        case QtWarningMsg:
            text = QString("Warn: ");
            break;
        case QtCriticalMsg:
            text = QString("Crit: ");
            break;
        case QtFatalMsg:
            text = QString("Fatal:");
            break;
        default:
            text = QString("Debug:");
    }

    /* 获取文件名，不带路径 */
    QString fileName = QString(context.file);
    fileName = QDir::fromNativeSeparators(fileName);
    fileName = fileName.mid(fileName.lastIndexOf("/") + 1);
    
    /* 获取函数名，不带命名空间 */
    QString function = QString(context.function);
    int pos = function.lastIndexOf("::");
    if(pos >= 0)
    {
        function = function.mid(function.lastIndexOf("::") + 2);
    }
    
    function = function.left(function.indexOf("(") + 1);
    
    QString context_info = QString("File:(%1->%2) Line:(%3)").arg(fileName).arg(function).arg(context.line); // F文件信息, L行数
    QString current_date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString message = QString("\n[%1 Time:%2 %3 ]").arg(text).arg(current_date).arg(context_info);
    return message;
}

static void QDbgMessage2Console(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QMutexLocker locker(&sLogLock);
    QString text;
    text = GetDbgHeadStr(type, context);
    text += QString(" : %1").arg(msg);
    fprintf(stderr, "%s", text.toStdString().c_str());
#if 1
    fflush(stderr);
    if(QtDebugMsg == type)
    {
        return ;
    }
    if(sLogFile.isOpen())
    {
        sLogFile.write(text.toUtf8());
        if(sLogFile.size() >= sLogFileSize)
        {
            /* 删除之前的Log文件，始终保证同时最多存在两个Log文件 */
#ifdef Q_OS_LINUX
            QString cmd = QString("rm -rf %1_*").arg(sCurrentLogFileName);
            system(cmd.toUtf8().constData());
#endif
            sLogFile.close();
            /* 重命名必须在关闭文件之后 */
            QString newName = sLogFile.fileName() + QString("_") + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
            sLogFile.rename(newName);
            sLogFile.setPermissions(QFileDevice::ReadOwner);
            sLogFile.setFileName(sCurrentLogFileName);
            sLogFile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
        }
    }
#endif
}

bool InitLog(QString logFileName, uint32_t logSizeKB)
{

    qInstallMessageHandler(QDbgMessage2Console);
    
    sLogFileSize = logSizeKB * 1024;
    sCurrentLogFileName = logFileName;
    if(!logFileName.isEmpty())
    {
#ifdef Q_OS_LINUX
        QFileInfo fInfo(logFileName);
        QString path = fInfo.absolutePath();
        QString cmd = QString("mkdir %1").arg(path);
        system(cmd.toUtf8().constData());
#endif
        QString errMsg;
        if(!QDbgOpenLogFile(logFileName, errMsg))
        {
            sCurrentLogFileName = QString("");
            qInstallMessageHandler(0);
            qCritical() << "Log file(" << logFileName << ") open failed :" << errMsg;
            return false;
        }
    }
    return true;
}

void ClearLog(void)
{
    QString errMsg;
    QDbgCloseLogFile(errMsg);
    //qInstallMessageHandler(0);
}


void Utf82QString(QString &str, const char *buf, size_t bufSize)
{
    QByteArray ba = QByteArray(buf, bufSize);
    str = QString(ba);
}

size_t QString2Utf8(char *buf, size_t bufSize, const QString &str)
{
    QByteArray ba = str.toUtf8();
    size_t size = ba.size();
    if(bufSize < size)
    {
        size = bufSize;
        qCritical() << QString("%1,%2:over flow.").arg(__FILE__).arg(__LINE__);
    }

    memset(buf, 0x00, bufSize);
    memmove(buf, ba.constData(), size);
    return size;
}

#ifdef _PSS_QT_NO_GUI_
void ShowMsgBoxInfo(const QString &str, bool def)
{
    PSS_UNUSED(def)
    qDebug() << str;
}
#else
void ShowMsgBoxInfo(const QString &str, bool isOkButtonShow, const QMessageBox::Icon &icon)
{
    QMessageBox msgBox;
    msgBox.setIcon(icon);
    msgBox.setText(str);
    if(!isOkButtonShow)
    {
        msgBox.addButton(QMessageBox::Ok);
        msgBox.removeButton(msgBox.button(QMessageBox::Ok));
    }
    msgBox.exec();
    return;
}
#endif

QString GetOnlineImagePath(void)
{
#ifdef Q_OS_LINUX
    QString onlineImagePath = QString("/app/tmp/PssImages/");
#else
    QString onlineImagePath = QString("..\\PcDemo\\PssImages\\");
#endif
    return onlineImagePath;
}

QString GetPssYBNetServer(void)
{
    return QString("www.whpssins.com");
}


QString GetServerOnlineImagesPath(void)
{
    return QString("/upload/file/");
}


void Save4ValToFile(const QVector<double> &x1Vals, const QVector<double> &x2Vals, const QVector<double> &x3Vals, const QVector<double> &x4Vals, QString suffix)
{
    /* 构造文件名 */
    QString csvFilePath = GetCsvFilePath();
    QString fileName = GetNowDateTimeStr(false, true, true);
    if(0 != suffix.size())
    {
        fileName = fileName + "_" + suffix;
    }
    fileName += ".csv";
    QString fullFileName = JoinPath(csvFilePath, fileName);

    /* 构造数据 */
    QByteArray ba;
    int size = x1Vals.size();
    for(int i = 0;i < size; i++)
    {
        QString dataLineStr = QString("%1,%2,%3,%4\n").arg(x1Vals[i]).arg(x2Vals[i]).arg(x3Vals[i]).arg(x4Vals[i]);
        ba.append(dataLineStr.toUtf8());
    }


    /* 写入数据 */
    if(WriteFile(ba, fullFileName))
    {
        qDebug() << QString("%1 is writed.").arg(fullFileName);
    }
}


bool CheckOnlineCode(const QString &code, QString &result)
{
    int pos = 0;
    result.clear();
    qDebug() << "code = " << code;
    pos = code.indexOf(".");
    if(pos < 0)
    {
        return false;
    }
    QString src = code.left(pos);

    int srcLen = code.mid(pos + 1).toUInt();
    qDebug() << "srcLen = " << srcLen;
    uint32_t crc32 = src.mid(srcLen).toUInt();
    qDebug() << "crc32 = " << crc32;
    
    src = src.left(srcLen);
    qDebug() << "src2 = " << src;
    
    if(src.length()%2)
    {
        return false;
    }
    
    uint32_t newCrc32 = 0;
    QVector<uint32_t> intVec;
    for(int i = 0; i < src.length();i++)
    {
        intVec.append(src.mid(i, 1).toUInt());
    }
    
    PssCLibCrc32((const uint32_t*)intVec.constData(), src.length(), &newCrc32);
    if(newCrc32 != crc32)
    {
        return false;
    }
#if 0
    QString str;
    for(int i = 0, j = src.length() -1 ;(i < src.length() / 2 && j >= 0);i++,j--)
    {
        if(src[i] != src[j])
        {
            return false;
        }
        str.append(src[i*2+1]);
    }
#endif

    result = src;
    qDebug() << "result = " << result;
    return true;
}


const QString BuildOnlineCode(const QString &srcCode)
{
    QString src(srcCode);
    qDebug() << "src = " << srcCode;
#if 0
    for(int i = 0, j = srcCode.length() - 1; (i < srcCode.length() && j >= 0);i++, j--)
    {
        src.insert(i * 2, srcCode[j]);
    }
    qDebug() << "dest src = " << src;
#endif
    uint32_t crc32 = 0, srcLen = (uint32_t)src.length();

    QVector<uint32_t> intVec;
    for(int i = 0; i < src.length();i++)
    {
        intVec.append(src.mid(i, 1).toUInt());
    }
    
    PssCLibCrc32((const uint32_t*)intVec.constData(), src.length(), &crc32);
    qDebug() << "crc32 = " << crc32 << ", srcLen = " << srcLen;
    src.append(QString::number(crc32));
    src.append(".");
    src.append(QString::number(srcLen));
    qDebug() << "dest src2 = " << src;
    return src;
}

bool IsDisplayBootLogo(void)
{
    /* kernel boot logo */
    //QString logoCfgStr("CONFIG_LOGO=y");
    QString cmd("zcat /proc/config.gz | grep 'CONFIG_LOGO=y'");
    int ret = system(cmd.toUtf8().constData());
    return CheckSystemCallResult(ret);
}

bool CheckSystemCallResult(int result)
{
#ifdef Q_OS_LINUX
    if((-1 != result) && WIFEXITED(result) && (!WEXITSTATUS(result)))
    {
        return true;
    }
    qCritical() << "system call return false !";
    return false;
#else
    Q_UNUSED(result);
    return true;
#endif
}


QString ErrorCode2String(PssRstT errCode)
{
    QString err = "Unknow error code !";
    switch(errCode)
    {
    case PssRstSuccess:
    {
        err = QString("No Error !");
        break;
    }
    case PssRstParameterError:
    {
        err = QString("Parameter Error !");
        break;
    }
    case PssRstDevCreateError:
    {
        err = QString("Device Create Error !");
        break;
    }
    case PssRstDeviceNotInit:
    {
        err = QString("Device not init !");
        break;
    }
    case PssRstScpiClientNotFound:
    {
        err = QString("Client not found !");
        break;
    }
    case PssRstNotImpl:
    {
        err = QString("Not implement !");
        break;
    }case PssRstOutputFail:
    {
        err = QString("Output Error !");
        break;
    }
    case PssRstDeviceNotConnect:
    {
        err = QString("Device not connected !");
        break;
    }
    case PssRstSetSrcValError:
    {
        err = QString("Set source Error !");
        break;
    }
    case PssRstSetSrcRangeError:
    {
        err = QString("Set source range Error !");
        break;
    }case PssRstSetLmtValError:
    {
        err = QString("Set limit Error !");
        break;
    }
    case PssRstSetLmtRangeError:
    {
        err = QString("Set limit range Error !");
        break;
    }
    case PssRstSetSrcError:
    {
        err = QString("Set source Error !");
        break;
    }
    case PssRstSetNPLCError:
    {
        err = QString("Set NPLC Error !");
        break;
    }
    case PssRstSetMeasureSrcError:
    {
        err = QString("Set measure Error !");
        break;
    }
    case PssRstReadError:
    {
        err = QString("Read Error !");
        break;
    }
    case PssRstAlreadyConnect:
    {
        err = QString("Already connected !");
        break;
    }
    case PssRstRecvFromIOError:
    {
        err = QString("Read Error !");
        break;
    }
    case PssRstIOEmptyError:
    {
        err = QString("IO null Error !");
        break;
    }
    case PssRstOptTimeoutError:
    {
        err = QString("Operation timeout !");
        break;
    }
    case PssRstWriteToIOError:
    {
        err = QString("Write Error !");
        break;
    }
    case PssRstReadFromIOError:
    {
        err = QString("Read Error !");
        break;
    }
    case PssRstMallocFailed:
    {
        err = QString("Alloc Error !");
        break;
    }
    case PssRstSetSensFuncError:
    {
        err = QString("Set limit Error !");
        break;
    }
    case PssRstBufIsNotEnough:
    {
        err = QString("Buffer not enough !");
        break;
    }
    case PssRstResponseArrived:
    {
        err = QString("Reaponse arrived !");
        break;
    }
    case PssRstResponseMismatch:
    {
        err = QString("Response not match Error !");
        break;
    }
    case PssRstSet24WError:
    {
        err = QString("Set 2/4 line Error !");
        break;
    }
    case PssRstSetFBError:
    {
        err = QString("Set F/B Error !");
        break;
    }
    case PssRstSetMasterError:
    {
        err = QString("Set master device Error !");
        break;
    }
    case PssRstSetTrigDirError:
    {
        err = QString("Set trig direction Error !");
        break;
    }
    case PssRstSetScanSrcError:
    {
        err = QString("Set sweep source Error !");
        break;
    }
    case PssRstSetScanStartError:
    {
        err = QString("Set sweep start value Error !");
        break;
    }
    case PssRstSetScanStopError:
    {
        err = QString("Set sweep stop value Error !");
        break;
    }
    case PssRstSetScanPointError:
    {
        err = QString("Set sweep points Error !");
        break;
    }
    case PssRstSetCustomValError:
    {
        err = QString("Set sweep custom value Error !");
        break;
    }
    case PssRstOverFlowError:
    {
        err = QString("Overflow Error !");
        break;
    }
    case PssRstRepeatError:
    {
        err = QString("Repeat operation Error !");
        break;
    }
    case PssRstSetValOverFlow:
    {
        err = QString("Set value overflow Error !");
        break;
    }
    case PssRstScpiFormatError:
    {
        err = QString("Scpi format Error !");
        break;
    }
    case PssRstScpiCmdError:
    {
        err = QString("Scpi cmd Error !");
        break;
    }
    case PssRstSetResolutinError:
    {
        err = QString("Set Resolutin Error !");
        break;
    }
    case PssRstCheckError:
    {
        err = QString("Checkout Error !");
        break;
    }
    case PssRstOverFlow:
    {
        err = QString("Overflow Error !");
        break;
    }
    case PssRstGPIBOpenError:
    {
        err = QString("Open GPIB Error !");
        break;
    }
    case PssRstGPIBNotOpen:
    {
        err = QString("GPIB not open Error !");
        break;
    }
    case PssRstGPIBWriteError:
    {
        err = QString("GPIB write Error !");
        break;
    }
    case PssRstGPIBReadError:
    {
        err = QString("GPIB read Error !");
        break;
    }
    case PssRstSocketBindError:
    {
        err = QString("Bind socket Error !");
        break;
    }
    case PssRstSocketConnectError:
    {
        err = QString("Socket connect Error !");
        break;
    }
    case PssRstUnknowPssBinFrm:
    {
        err = QString("Unknow pssbin Error !");
        break;
    }
    case PssRstAnlgOptError:
    {
        err = QString("Anlg board operation Error !");
        break;
    }
    case PssRstPwrOptError:
    {
        err = QString("Pwr board operation Error !");
        break;
    }
    case PssRstFileError:
    {
        err = QString("file operation Error !");
        break;
    }
    case PssRstUnknowDevice:
    {
        err = QString("Unknow device Error !");
        break;
    }
    case PssRstCalError:
    {
        err = QString("Cal device Error !");
        break;
    }
    case PssRstUartConfigError:
    {
        err = QString("Uart config Error !");
        break;
    }
    case PssRstOverFlowOfIndex:
    {
        err = QString("Overflow Error !");
        break;
    }
    case PssRstMySqlCmdErr:
    {
        err = QString("Sql statement Error !");
        break;
    }
    case PssRstMySqlUsrErr:
    {
        err = QString("Sql user Error !");
        break;
    }
    case PssRstMySqlPasswdErr:
    {
        err = QString("Sql password Error !");
        break;
    }
    default:
    {
        break;
    }
    }

    return err;
}

bool SetSystemTime(const QDateTime &time)
{
    QProcess proc;
    /* 设置系统时间 */
    QString cmd(QString("date -s \"%1\"").arg(time.toString("yyyy-MM-dd hh:mm:ss")));
    qDebug() << "set system time: " << cmd;
    int ret = system(cmd.toUtf8().constData());
    if(!CheckSystemCallResult(ret))
    {
        qCritical() << "exec" << cmd << " failed !";
        return false;
    }

    if(CheckRTC())
    {
        /* 同步系统时间和RTC时间 */
        cmd = QString("hwclock --systohc");
        ret = system(cmd.toUtf8().constData());
        if(!CheckSystemCallResult(ret))
        {
            qCritical() << "exec" << cmd << " failed !";
            return false;
        }
    }
    return true;
}
