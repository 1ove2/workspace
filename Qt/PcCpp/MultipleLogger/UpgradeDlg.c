#include "UpgradeDlg.h"
#include "ui_UpgradeDlg.h"
#include "PssQtHal.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTime>
#include <QThread>

using namespace Pss::Qt;
UpgradeDlg::UpgradeDlg(const QString &ip, const ChanInfo &info, InstType type,QWidget *parent) :
    QDialog(parent),
    mIP(ip),
    mInfo(info),
    mType(type),
    mUi(new Ui::UpgradeDlg)
{
    mUi->setupUi(this);
    setWindowTitle("设备升级(LAN)");
    Init();
    connect(mUi->pb_Path, &QPushButton::clicked, this, &UpgradeDlg::slotGetDirPath);
    connect(mUi->pb_Confirm, &QPushButton::clicked, this, &UpgradeDlg::slotStartUpgrade);
    connect(mUi->pb_Cancel, &QPushButton::clicked, this, &UpgradeDlg::close);
}

UpgradeDlg::~UpgradeDlg()
{
    delete mUi;
}

void UpgradeDlg::slotGetDirPath()
{
    QString dirPath = QFileDialog::getExistingDirectory(this,"选择文件路径","./");
    if (dirPath.isEmpty())
    {
        return;
    }
    mUi->le_Path->setText(dirPath);
}

int UpgradeDlg::ParseUpgradeResult(const QString &str,QString &board, int &rate)
{
    if (!str.contains(":"))
    {
        return -2;
    }
    QStringList list = str.split(":");
    if (list.size() < 5)
    {
        return -2;
    }
    QString boardStr = list.value(2);
    if ("ANLG" == boardStr)
    {
        board = QString("模拟板");
    }
    else if ("FNT" == boardStr)
    {
        board = QString("前面板");
    }
    else if ("PWR" == boardStr)
    {
        board = QString("电源板");
    }
    QString rateStr = list.value(4);
    qDebug() << "rateStr:" <<  rateStr;
    rate = rateStr.split("\"").value(1).toInt();
    qDebug() << "rate:" << rate;
    if ("PWR" == boardStr && 100 == rate)
    {
        return 0;
    }
    if (100 == rate)
    {
        return 1;
    }
    return -1;
}

void UpgradeDlg::slotStartUpgrade()
{
    mUi->progressBar->setValue(0);
    mUpgradePort = mUi->le_Port->text().toInt();

    LockUI(true);
    /* step2 发送准备升级指令 */
    if (!SendUpgradeScip())
    {
        LockUI(false);
        return;
    }
    /* step3 发送升级文件 */
    if (!NetUpgrade(mUi->le_Path->text()))
    {
        LockUI(false);
        return;
    }
    
    //QMessageBox::information(this,"提示","升级文件传输完毕，请等待设备自动升级\n 设备升级期间请勿操作设备!");
}

void UpgradeDlg::Init()
{
    mUi->progressBar->setMaximum(100);
    mUi->progressBar->setMinimum(0);
    mUi->progressBar->setValue(0);
}

void UpgradeDlg::SetProcessBarValue(int value)
{
    mUi->progressBar->setValue(value);
}

void UpgradeDlg::LockUI(bool isLocked)
{
    mUi->pb_Path->setEnabled(!isLocked);
    mUi->pb_Confirm->setEnabled(!isLocked);
    mUi->pb_Cancel->setEnabled(!isLocked);
    mUi->le_Port->setEnabled(!isLocked);
}

int UpgradeDlg::Recv(QString &str, QtTcpClient *client, uint32_t delayMs, bool check) const
{
    if (!client)
    {
        return -1;
    }
    const int bufSize = 1024 * 50;
    char buf[bufSize] = {0};
    QTime startTime = QTime::currentTime();
    int recvLen = 0;
    startTime.start();
    do
    {
        int recvBytesNums = client->Recv(buf + recvLen, bufSize - recvLen, delayMs);
        qDebug() << "buf:" << buf;
        if(recvBytesNums < 0)
        {
            //qCritical() << "scpi client recv failed: " << recvBytesNums;
            str.clear();
            return recvBytesNums;
        }
        recvLen += recvBytesNums;
        if((recvLen) && ('\n' == buf[recvLen-1]))
        {
            break;
        }
        if(startTime.elapsed() > delayMs)
        {
            qDebug() << "time:" << startTime.elapsed() << delayMs;
            Utf82QString(str, buf, recvLen);
            qDebug() << "recv:" << recvLen << "str:" << str;
            if(check)
            {
                return PssRstOptTimeoutError;
            }
            if(recvLen > 0)
            {
                break;
            }
            qCritical() << "scpi recv timeout for " << delayMs << " ms !";
            return PssRstOptTimeoutError;
        }
        if((!check) && (recvLen > 0))
        {
            break;
        }
    }while(1);

    Utf82QString(str, buf, recvLen);
    qDebug() << "ScpiClient Recv:" + str;
    return recvLen;
}

bool UpgradeDlg::NetUpgrade(const QString &filePath)
{
    QtTcpClient* client = nullptr;
    QTime time;
    time.start();
    do
    {
        QHostAddress mDeviceIP(mIP);
        client = new QtTcpClient(mDeviceIP, mUpgradePort, "upgrade client");
        if(client->IsOpen())
        {
            break;
        }
        if(time.elapsed() >= 2 * 1000)
        {
            QMessageBox::information(this, "连接设备：", "连接设备超时!");
            delete client;
            client = nullptr;
            return false;
        }
    }while(1);

    QDir::setCurrent(filePath);
    
    QString file(filePath);
    file += "/DaemonProcess";
    
    if(!SendUpgradeData(client, file))
    {
        mUi->plainTextEdit->appendPlainText(QString("升级文件发送失败!"));
        delete client;
        client = nullptr;
        return false;
    }
    SetProcessBarValue(10);
    
    file.clear();
    file.append(filePath);
    file.append("/PwrProcess");
    
    if(!SendUpgradeData(client, file))
    {
        delete client;
        client = nullptr;
        return false;
    }
    SetProcessBarValue(20);
    
    if (S100 == mType)
    {
    
        file.clear();
        file.append(filePath);
        file.append("/UIProcess");
        if(!SendUpgradeData(client, file))
        {
            delete client;
            client = nullptr;
            return false;
        }
        SetProcessBarValue(30);
    }
    
    /* 模拟板镜像 */
    file.clear();
    file.append(filePath);
    if (S100 == mType)
    {
        file.append("/SX00Anlg.bin.a");
    }
    else if (CS400 == mType)
    {
        file.append("/CSX00Anlg.bin.a");
    }
    if(!SendUpgradeData(client, file))
    {
        delete client;
        client = nullptr;
        return false;
    }
    SetProcessBarValue(40);
    
    file.clear();
    file.append(filePath);
    if (S100 == mType)
    {
        file.append("/SX00Anlg.bin.b");
    }
    else if (CS400 == mType)
    {
        file.append("/CSX00Anlg.bin.b");
    }
    if(!SendUpgradeData(client, file))
    {
        delete client;
        client = nullptr;
        return false;
    }
    SetProcessBarValue(50);
    /* 电源板镜像 */
    file.clear();
    file.append(filePath);
    if (S100 == mType)
    {
        file.append("/SX00Pwr.bin.a");
    }
    else if (CS400 == mType)
    {
        file.append("/CSX00Pwr.bin.a");
    }
    if(!SendUpgradeData(client, file))
    {
        delete client;
        client = nullptr;
        return false;
    }
    SetProcessBarValue(60);
    
    file.clear();
    file.append(filePath);
    if (S100 == mType)
    {
        file.append("/SX00Pwr.bin.b");
    }
    else if (CS400 == mType)
    {
        file.append("/CSX00Pwr.bin.b");
    }
    if(!SendUpgradeData(client, file))
    {
        delete client;
        client = nullptr;
        return false;
    }
    SetProcessBarValue(70);
    if (S100 == mType)
    {
        /* 前面板镜像 */
        file.clear();
        file.append(filePath);
    
        file.append("/SX00Fnt.bin.a");
    
        if(!SendUpgradeData(client, file))
        {
            delete client;
            client = nullptr;
            return false;
        }
        SetProcessBarValue(80);
    
        file.clear();
        file.append(filePath);
    
        file.append("/SX00Fnt.bin.b");
    
        if(!SendUpgradeData(client, file))
        {
            delete client;
            client = nullptr;
            return false;
        }
    }
    if (CS400 == mType)
    {
        /* ko文件 */
        file.clear();
        file.append(filePath);
        file.append("/e1000e.ko");
        if (!SendUpgradeData(client, file))
        {
            delete client;
            client = nullptr;
            return false;
        }
    }
    
    SetProcessBarValue(90);
    
    /* Info文件 */
    file.clear();
    file.append(filePath);
    file.append("/info");
    if(!SendUpgradeData(client, file))
    {
        delete client;
        client = nullptr;
        return false;
    }
    SetProcessBarValue(100);
    
    /* 发送完毕 结束*/
    if(!SendUpgradeEndCmd(client))
    {
        delete client;
        client = nullptr;
        return false;
    }
    
    mUi->plainTextEdit->appendPlainText(QString("升级文件发送成功, 设备正在升级!"));
    mUi->progressBar->setValue(0);
    int rlt = -1; //rate = 100代表升级完成。
    int rate = 0;
    QString board;
    time.start();
    int cnt = 1;
    do {
        QApplication::processEvents();
        if (time.elapsed() > 30000)
        {
            mUi->plainTextEdit->appendPlainText(QString("升级进度信息返回超时!"));
            break;
        }
        QString recvStr;
        int ret = Recv(recvStr,client, 5000);
        if (0 > ret)
        {
            continue;
        }
        rlt = ParseUpgradeResult(recvStr, board, rate);
        //mUi->plainTextEdit->appendPlainText(QString("%1当前升级进度:%2%").arg(board).arg(rate));
        if (-1 == rate)
        {
            mUi->plainTextEdit->appendPlainText(QString("%1升级失败!").arg(board));
            break;
        }
        if (1 == rlt)
        {
            mUi->progressBar->setValue(rate);
            if (board == QString("模拟板"))
            {
                cnt ++;
                board.append(QString::number(cnt));
            }
            mUi->plainTextEdit->appendPlainText(QString("%1升级完成").arg(board));
        }
        if (-1 == rlt)
        {
            if (board == QString("模拟板"))
            {
                board.append(QString::number(cnt));
            }
            mUi->progressBar->setValue(rate);
            mUi->plainTextEdit->appendPlainText(QString("%1升级中...已完成%2%").arg(board).arg(rate));
        }
        if (-2 == rlt)
        {
            mUi->plainTextEdit->appendPlainText(QString("升级进度信息返回失败!"));
            break;
        }
        if (0 == rlt)
        {
            mUi->progressBar->setValue(rate);
            QMessageBox::warning(this, "提示", "设备升级完成,请重启设备和软件!");
            mUi->plainTextEdit->appendPlainText(QString("设备升级完成!"));
            break;
        }
        time.start();
    } while(1);
    
    delete client;
    client = nullptr;
    return true;
}

bool UpgradeDlg::SendUpgradeScip()
{
    if(mUi->le_Path->text().isEmpty())
    {
        QMessageBox::information(this, "设备升级：", "请选择正确的升级路径!");
        return false;
    }
    emit sigSetNetgradePort(mInfo, mUpgradePort);
    return true;
}

bool UpgradeDlg::SendUpgradeData(QtTcpClient* client,const QString &fileName)
{
    qDebug() << "SendUpgradeData ： " << fileName;
    SpecialUpgradeData* upgradeData = (SpecialUpgradeData*)malloc(1024*1024);

    /* 读取本地文件 传输 */
    QFile upgradeFile;
    upgradeFile.setFileName(fileName);
    if(!upgradeFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, "升级设备：", QString("读取升级文件: %1 失败：").arg(upgradeFile.errorString()));
        free(upgradeData);
        upgradeData = nullptr;
        return false;
    }
    
    QByteArray data;
    /* 发送文件名 */
    data.append(upgradeFile.fileName());
    upgradeData->PayLength = data.size();
    data.append(0x0001);
    int tail = _UPGRADE_DATA_TAIL;
    memcpy(&(upgradeData->Payload), data.constData(), data.size());
    upgradeData->Head = _UPGRADE_DATA_HEAD;
    upgradeData->Cmd = 0X1;
    upgradeData->PkgLength = upgradeData->PayLength + sizeof(SpecialUpgradeData) + 4;
    memcpy((char*)upgradeData + upgradeData->PkgLength - 4, &tail, sizeof(tail));
    int ret = client->Send((const char*)upgradeData, upgradeData->PkgLength, 500);
    if(ret <= 0)
    {
        QMessageBox::information(this, "升级设备：", QString("发送升级文件失败：%1").arg(ret));
        free(upgradeData);
        upgradeData = nullptr;
        return false;
    }
    //void *memset(void *s,int ch, size_t n);
    /* 如果数据超过1k, 分包处理 */
    /* 发送数据 */
    memset(upgradeData, 0x0, 1024*1024);
    data.clear();
    data = upgradeFile.readAll();
    
    for(int i = 0;data.size() > 0;i++)
    {
        QByteArray subData = data.mid(0, 1000);
        data.remove(0, 1000);
        upgradeData->PayLength = subData.size();
        if(data.isEmpty())
        {
            /* 传输结束标志置位 */
            subData.append(0x01);
        }
        else
        {
            subData.append(0x0, 1);
        }
        //subData.append(_UPGRADE_DATA_TAIL);
        memcpy(&(upgradeData->Payload), subData.constData(), subData.size());
        upgradeData->Head = _UPGRADE_DATA_HEAD;
        upgradeData->Cmd = 0X2;
        upgradeData->PkgLength = upgradeData->PayLength + sizeof(SpecialUpgradeData) + 4;
        memcpy((char*)upgradeData + upgradeData->PkgLength - 4, &tail, sizeof(tail));
        ret = client->Send((const char*)upgradeData, upgradeData->PkgLength, 500);
        if(ret <= 0)
        {
            QMessageBox::information(this, "升级设备：", QString("发送升级文件失败：%1").arg(ret));
            free(upgradeData);
            upgradeData = nullptr;
            return false;
        }
    
    }
    upgradeFile.close();
    free(upgradeData);
    upgradeData = nullptr;
    
    client->FlushOut();
    return true;
}

bool UpgradeDlg::SendUpgradeEndCmd(QtTcpClient *client)
{
    SpecialUpgradeData* upgradeData = (SpecialUpgradeData*)malloc(1024*1024);
    QByteArray data;
    /* 发送传输结束命令 */
    data.append("END");
    upgradeData->PayLength = data.size();
    data.append(0x0001);
    int tail = _UPGRADE_DATA_TAIL;
    //data.append(_UPGRADE_DATA_TAIL);
    memcpy(&(upgradeData->Payload), data.constData(), data.size());
    upgradeData->Head = _UPGRADE_DATA_HEAD;
    upgradeData->Cmd = 0X3;
    upgradeData->PkgLength = upgradeData->PayLength + sizeof(SpecialUpgradeData) + 4;
    memcpy((char*)upgradeData + upgradeData->PkgLength - 4, &tail, sizeof(tail));
    int ret = client->Send((const char*)upgradeData, upgradeData->PkgLength, 500);
    if(ret <= 0)
    {
        QMessageBox::information(this, "升级设备：", QString("发送升级文件失败：%1").arg(ret));
        free(upgradeData);
        upgradeData = nullptr;
        return false;
    }
    free(upgradeData);
    upgradeData = nullptr;

    client->FlushOut();
    return true;
}