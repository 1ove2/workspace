#include "IQtBrd.h"
#include "QtCom.h"
#include "Bytes.h"
#include <QDebug>
#include <QThread>
#include <QApplication>
#include "PssQtHal.h"
#include "PssBinAppCommEnum.h"

using namespace Pss;
using namespace Pss::Qt;

IQtBrd::IQtBrd(const QString &comName, int baudrate, const QString &brdIdStr, QObject *parent):
    QObject(parent),
    mIIo(new QtCom(comName, baudrate, brdIdStr, parent)),
    mVerStr(QString("Not Get Yet.")),
    mBrdIdStr(brdIdStr),
    mDeleteIoFlag(true),
    mConnected(false)
{

}

IQtBrd::IQtBrd(IIo *io, const QString &brdIdStr, QObject *parent):
    QObject(parent),
    mIIo(io),
    mVerStr(QString("Not Get Yet.")),
    mBrdIdStr(brdIdStr),
    mDeleteIoFlag(false),
    mConnected(false)
{

}

IQtBrd::~IQtBrd(void)
{
    if(mDeleteIoFlag)
    {
        delete mIIo;
        mIIo = nullptr;
    }
}

IIo *IQtBrd::GetIo(void) const
{
    return mIIo;
}

PssRstT IQtBrd::Upgrade(const QByteArray &binDataA, const QByteArray &binDataB)
{

    PssRstT ret = PssRstSuccess;
    /* step2: 获取当前面对应的升级文件 */
    bool shouldWriteA = false;
    ret = ShouldWriteA(shouldWriteA);
    if(PssRstSuccess != ret)
    {
        qCritical() << "Get AB info failed: " << ret;
        UpdateUi(-1, QStringLiteral("获取AB面失败！"));
        return ret;
    }
    QByteArray binData;
    if(shouldWriteA)
    {
        binData = binDataA;
    }
    else
    {
        binData = binDataB;
    }
    /* step3: 拆包 */
    QVector<QByteArray> dataVector;
    int upgradeDataSize = PssBinAppCommQtUpgradeData::GetUpgradeDataSize();
    SplitBinData(dataVector, binData, upgradeDataSize);
    UpdateUi(2, QStringLiteral("升级文件拆包成功."));
    /* step4: 擦FLASH */
    UpdateUi(3, QStringLiteral("Flash开始擦除..."));
    ret = Erase();
    if(PssRstSuccess != ret)
    {
        qCritical() << "Erase failed: " << ret;
        UpdateUi(-1, QStringLiteral("擦除Flash失败！"));
        return ret;
    }
    UpdateUi(10, QStringLiteral("Flash擦除成功."));
    /* 此处必须延时等待擦除完成(4s为经验值) */
    QThread::msleep(4000);
    
    /* step5: 遍历烧写所有升级数据 */
    ret = Fire(dataVector, 11, 95);
    if(PssRstSuccess != ret)
    {
        qCritical() << "Fire failed: " << ret;
        UpdateUi(-1, QStringLiteral("烧写失败！"));
        return ret;
    }
    UpdateUi(95, QStringLiteral("烧写成功."));
    QThread::msleep(1000);
    
    /* step6: 设置标记,使能APP区域 */
    ret = SetFlag();
    if(PssRstSuccess != ret)
    {
        qCritical() << "SetFlag failed: " << ret;
        UpdateUi(-1, QStringLiteral("置标记失败！"));
        return ret;
    }
    UpdateUi(96, QStringLiteral("APP标记置成功."));
    QThread::msleep(1000);
    
    /* step7: 重启 */
    ret = Reboot();
    if(PssRstSuccess != ret)
    {
        qCritical() << "Reboot failed: " << ret;
        UpdateUi(-1, QStringLiteral("重启失败，请手动重启设备."));
        return ret;
    }
    UpdateUi(98, QStringLiteral("重启成功."));
    QCoreApplication::processEvents();
    QThread::msleep(300);
    UpdateUi(99, QStringLiteral("重启成功."));
    QCoreApplication::processEvents();
    QThread::msleep(300);
    UpdateUi(100, QStringLiteral("升级完成."));
    QCoreApplication::processEvents();
    QThread::msleep(300);
    return PssRstSuccess;
}

PssRstT IQtBrd::Upgrade(const QByteArray &binDataA, const QByteArray &binDataB, const uint32_t &address)
{
//    qDebug()<<__FUNCTION__<<" File1 len:"<<binDataA.length()
//           <<" File2 len:"<<binDataB.length();
    PSS_UNUSED(address);
    PssRstT ret = PssRstSuccess;
#if 0
    /* step2: 获取当前面对应的升级文件 */
    bool shouldWriteA = false;
    ret = ShouldWriteA(shouldWriteA);
    if(PssRstSuccess != ret)
    {
        qCritical() << "Get AB info failed: " << ret;
        UpdateUi(-1, QStringLiteral("获取AB面失败！"));
        return ret;
    }

    QByteArray binData;
    if(shouldWriteA)
    {
        binData = binDataA;
    }
    else
    {
        binData = binDataB;
    }
#endif

    /* step3: 拆包 */
    QVector<QByteArray> dataVector;
    /* 第一包文件长度 */
    int firstPackLen = 0;
    int upgradeDataSize = PssBinAppCommQtUpgradeData::GetUpgradeDataSize();
    SplitBinData(dataVector, binDataA, upgradeDataSize);
    firstPackLen = dataVector.count();
    SplitBinData(dataVector, binDataB, upgradeDataSize);
    UpdateUi(2, QStringLiteral("升级文件拆包成功."));
    
    /* step4: 擦FLASH */
    UpdateUi(3, QStringLiteral("Flash开始擦除..."));
    ret = Erase();
    if(PssRstSuccess != ret)
    {
        qCritical() << "Erase failed: " << ret;
        UpdateUi(-1, QStringLiteral("擦除Flash失败！"));
        return ret;
    }
    UpdateUi(10, QStringLiteral("Flash擦除成功."));
    /* 此处必须延时等待擦除完成(4s为经验值) */
    QThread::msleep(16000);
    
    /* step5: 遍历烧写所有升级数据 */
    ret = Fire(dataVector, 11, 95,firstPackLen);
    if(PssRstSuccess != ret)
    {
        qCritical() << "Fire failed: " << ret;
        UpdateUi(-1, QStringLiteral("烧写失败！"));
        return ret;
    }
    UpdateUi(95, QStringLiteral("烧写成功."));
    QThread::msleep(1000);
    
    /* step6: 设置标记,使能APP区域 */
    ret = SetFlag();
    if(PssRstSuccess != ret)
    {
        qCritical() << "SetFlag failed: " << ret;
        UpdateUi(-1, QStringLiteral("置标记失败！"));
        return ret;
    }
    UpdateUi(96, QStringLiteral("APP标记置成功."));
    QThread::msleep(1000);
    
    /* step7: 重启 */
    ret = Reboot();
    if(PssRstSuccess != ret)
    {
        qCritical() << "Reboot failed: " << ret;
        UpdateUi(-1, QStringLiteral("重启失败，请手动重启设备."));
        return ret;
    }
    UpdateUi(98, QStringLiteral("重启成功."));
    QCoreApplication::processEvents();
    QThread::msleep(300);
    UpdateUi(99, QStringLiteral("重启成功."));
    QCoreApplication::processEvents();
    QThread::msleep(300);
    UpdateUi(100, QStringLiteral("升级完成."));
    QCoreApplication::processEvents();
    QThread::msleep(300);
    return PssRstSuccess;
}

void IQtBrd::UpdateUi(int rate, const QString &msgStr)
{
    emit sigUpdateUpgradeProcess(rate, msgStr);
}

PssRstT IQtBrd::ShouldWriteA(bool & isA)
{
    PssRstT ret = PssRstSuccess;
    /* 请求AB面信息 */
    IPssBinAppCommQt *pssBin = GetPssBin();
    ret = pssBin->SendCommReq(isA);
    if(PssRstSuccess != ret)
    {
        qCritical() << "Send Req AB info failed: " << ret;
        return ret;
    }
    return ret;
}

void IQtBrd::SplitBinData(QVector<QByteArray> &dataVector, const QByteArray &binData, int maxUpgradeDataSize)
{
    const char *ptrBinData = binData.constData();
    int dataCount = binData.count();
    int upgradeDataNums = floor(1.0 * dataCount / maxUpgradeDataSize);

    int i = 0;
    for(i = 0; i < upgradeDataNums; i++)
    {
        const char *ptrData = ptrBinData + maxUpgradeDataSize * i;
        QByteArray aUpgradeData(ptrData, maxUpgradeDataSize);
        dataVector.append(aUpgradeData);
    }
    
    /* 处理尾部 未填满的帧 */
    int offset = upgradeDataNums * maxUpgradeDataSize;
    int leftSize = dataCount - offset;
    if(leftSize > 0)
    {
        const char *ptrData = ptrBinData + offset;
        QByteArray aUpgradeData(ptrData, leftSize);
        dataVector.append(aUpgradeData);
    }
}

PssRstT IQtBrd::Erase(void)
{
    PssRstT ret = PssRstSuccess;
    /* 发送擦除命令 */
    IPssBinAppCommQt *pssBin = GetPssBin();
    ret = pssBin->SendCommCtrl(PssBinAppCommEnumCtrlUpgradeCmdErase);
    return ret;
}

PssRstT IQtBrd::Fire(const QVector<QByteArray> &dataVector, int startRate, int endRate)
{
    PssRstT ret = PssRstSuccess;
    int i = 0;
    int iMax = dataVector.count();
    double progressStep = 1.0 * (endRate - startRate) / (iMax - 1);
    QString str = QString(QStringLiteral("共计 %1 包需要烧写.")).arg(iMax, 3);
    QThread::msleep(2000);
    UpdateUi(startRate, str);
    for(i = 0; i < iMax; i++)
    {
        /* step1: 构造升级数据 */
        QByteArray upgradeBinData = dataVector[i];
        const uint8_t *ptrData = (const uint8_t *)(upgradeBinData.constData());
        uint32_t size = upgradeBinData.count();
        uint32_t id = static_cast<uint32_t>(i);
        PssBinAppCommQtUpgradeData data(id, ptrData, size);


        QThread::msleep(100);
        /* step2: 烧写 */
        ret = FireAUpgradeData(data);
        if(PssRstSuccess != ret)
        {
            qCritical() << "Fire Upgrade data failed: " << ret;
            return ret;
        }
    
        /* step3: 更新UI */
        int rate = startRate + progressStep * i;
        str = QString(QStringLiteral("%1's 包 已烧写.")).arg(i, 3);
    
        /* 10包一更新 */
        if(0 == i % 10)
        {
            UpdateUi(rate, str);
        }
    }
    
    return PssRstSuccess;
}

PssRstT IQtBrd::Fire(const QVector<QByteArray> &dataVector, int startRate, int endRate, int firstPackLen)
{
    /*
     * firstPackLen:第一包长度
    */
    PssRstT ret = PssRstSuccess;
    int i = 0;
    int iMax = dataVector.count();
    double progressStep = 1.0 * (endRate - startRate) / (iMax - 1);
    QString str = QString(QStringLiteral("共计 %1 包需要烧写.")).arg(iMax, 3);
    QThread::msleep(2000);
    UpdateUi(startRate, str);
    int curRate = 0;
    for(i = 0; i < iMax; i++)
    {
        /* step1: 构造升级数据 */
        QByteArray upgradeBinData = dataVector[i];
        const uint8_t *ptrData = (const uint8_t *)(upgradeBinData.constData());
        uint32_t size = upgradeBinData.count();
        uint32_t id = static_cast<uint32_t>(i);
        QThread::msleep(10);
        if(i < firstPackLen)
        {
            PssBinAppCommQtUpgradeData data(id, ptrData, size);
            /* step2: 烧写 */
            ret = FireAUpgradeData(data);
        }else{
            PssBinAppCommQtUpgradeData data(id - firstPackLen, ptrData, size);
            /* step2: 烧写 */
            ret = FireAUpgradeData(data);
        }

        if(PssRstSuccess != ret)
        {
            qCritical() << "Fire Upgrade data failed: " << ret;
            return ret;
        }
    
        /* step3: 更新UI */
        int rate = startRate + progressStep * i;
        str = QString(QStringLiteral("%1 / %2 包 已烧写.")).arg(i, 3).arg(iMax);
    
        /* 10包一更新 */
        if((0 == i % 10)
                && (curRate != rate))
        {
            UpdateUi(rate, str);
            curRate = rate;
        }
    }
    return PssRstSuccess;
}

PssRstT IQtBrd::FireAUpgradeData(const PssBinAppCommQtUpgradeData &upgradeData)
{
    PssRstT ret = PssRstSuccess;
    /* 烧写 */
    IPssBinAppCommQt *pssBin = GetPssBin();
    ret = pssBin->SendCommUpgradeData(upgradeData);
    return ret;
}

PssRstT IQtBrd::SetFlag(void)
{
    PssRstT ret = PssRstSuccess;
    /* 发送置标记命令 */
    IPssBinAppCommQt *pssBin = GetPssBin();
    ret = pssBin->SendCommCtrl(PssBinAppCommEnumCtrlUpgradeCmdSetFlag);
    return ret;
}


PssRstT IQtBrd::Connect(void)
{
    Bytes verBytes;
    IPssBinAppCommQt *pssBin = GetPssBin();
    PssRstT ret = pssBin->SendCommReq(verBytes);

    if(PssRstSuccess != ret)
    {
        qCritical() << QString("%1 connect failed !").arg(mBrdIdStr);
        return ret;
    }
    qDebug() << QString("%1 connected !").arg(mBrdIdStr);
    QByteArray ba;
    ba.append((const char*)(verBytes.ConstData()), verBytes.GetValidLen());
    mVerStr.clear();
    mVerStr.append(ba);
    mConnected = true;
    return PssRstSuccess;
}

bool IQtBrd::IsConnected(void)
{
    return mConnected;
}

QString IQtBrd::GetBrdVer(void) const
{
    return mVerStr;
}

QString IQtBrd::GetBrdIdStr(void) const
{
    return mBrdIdStr;
}

PssRstT IQtBrd::GetAtsha204aStatus(PssBinAppCommMcuAtsha204aStatus &status) const
{
    IPssBinAppCommQt *pssBin = GetPssBin();
    PssRstT done = pssBin->SendCommReq(status);
    return done;
}

PssRstT IQtBrd::Upgrade(const QString &filePath, const QString& infoFileName)
{
    /* 读取并校验文件 */
    QString msg = QString(QStringLiteral("开始校验 %1 镜像.")).arg(mBrdIdStr);
    qDebug() << mBrdIdStr << ": filePath: " << filePath << ", infoFile: " << infoFileName;
    emit sigUpdateUpgradeProcess(0, msg);
    QByteArray imageA;
    QByteArray imageB;
    PssRstT ret = PssRstSuccess;
    QString fileName = filePath + "/" + mBrdIdStr;
    QString infoName = filePath + "/" + GetFileNameFromPath(infoFileName);
    if(!CheckAndGetMcuImageFilePath(imageA, imageB, fileName, infoName))
    {
        QString msg = QString(QStringLiteral("%1 升级文件获取失败")).arg(mBrdIdStr);
        emit sigUpdateUpgradeProcess(-1, msg);
        qCritical() << msg;
        return PssRstFileError;
    }
    /* 与子板通过PssBin升级 */
    ret = Upgrade(imageA, imageB);
    return ret;
}

#ifdef _PSS_QT_PLX00_
PssRstT IQtBrd::Upgrade(const QString &filePath, const QString& infoFileName,const QString &str)
{
    PSS_UNUSED(str);
    /* 读取并校验文件 */
    QString msg = QString(QStringLiteral("开始校验 %1 镜像.")).arg(mBrdIdStr);
    emit sigUpdateUpgradeProcess(0, msg);
    QByteArray imageA;
    QByteArray imageB;
    PssRstT ret = PssRstSuccess;
    QString fileName = filePath + "/" + mBrdIdStr;
    QString infoName = filePath + "/" + GetFileNameFromPath(infoFileName);
    if(!CheckAndGetMcuImageFilePath(imageA, imageB, fileName, infoName))
    {
        QString msg = QString(QStringLiteral("%1 升级文件获取失败")).arg(mBrdIdStr);
        emit sigUpdateUpgradeProcess(-1, msg);
        qCritical() << msg;
        return PssRstFileError;
    }
    /* 与子板通过PssBin升级 */
    ret = Upgrade(imageA, imageB,0x400000);  /*PLX00项目:两文件地址 A:0x00 B:0x400000*/
    return ret;
}
#endif

PssRstT IQtBrd::Reboot(void)
{
    /* 重启 */
    IPssBinAppCommQt *pssBin = GetPssBin();
    PssRstT ret = pssBin->SendCommCtrl(PssBinAppCommEnumCtrlReboot);
    if(PssRstSuccess != ret)
    {
        qCritical() << "Send Reboot failed: " << ret;
        return ret;
    }
    /* 等待重启完成 */
    QThread::msleep(mcWaitMcuRebootMs);
    return ret;
}

PssRstT IQtBrd::SendCommAtsha204aDataBrd(const PssBinAppCommQtFireAtsha204a &data)
{
    IPssBinAppCommQt *pssBin = GetPssBin();
    PssRstT ret = pssBin->SendCommAtsha204aData(data);
    if(PssRstSuccess != ret)
    {
        qCritical() << "Burn failed: " << ret;
        return ret;
    }
    return ret;
}

PssRstT IQtBrd::SendCommReqBrd(PssBinAppCommEnumStatus &status)
{
    IPssBinAppCommQt *pssBin = GetPssBin();
    PssRstT ret = pssBin->SendCommReq(status);
    if(PssRstSuccess != ret)
    {
        qCritical() << "Req(Updata) failed: " << ret;
        return ret;
    }
    return ret;
}