#ifndef _PSS_QT_UPGRADEDLG_H_
#define _PSS_QT_UPGRADEDLG_H_
#include "InstManager.h"
#include "QtTcpClient.h"
#include <QDialog>
#include <QHostAddress>

namespace Ui {
class UpgradeDlg;
}

#pragma pack(push)
#pragma pack(1)

#define _UPGRADE_DATA_HEAD              (0xaa5555aa)                                                    /* 网络数据包头 */
#define _UPGRADE_DATA_TAIL              (0x55aaaa55)                                                    /* 网络数据包尾 */
typedef struct _SpecialUpgradeDataTag_
{
    uint        Head;                                                                                   /* 数据头 */
    char        Cmd;                                                                                    /* 命令格式 */
    int         PkgLength;                                                                              /* 数据包长度 */
    int         PayLength;                                                                              /* 有效数据长度 */
    short       CurrentPkgID;                                                                           /* 当前数据包序号 */
    short       TotalPkgNums;                                                                           /* 数据包总数 */
    char        Payload;                                                                                /* 有效数据首地址 */
} SpecialUpgradeData;
#pragma pack(pop)

namespace Pss {
namespace Qt {

enum PssNetPacketCmd {
    FileName = 0x1,
    FileData = 0x2
};

class UpgradeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit UpgradeDlg(const QString &ip, const ChanInfo &info, InstType type, QWidget *parent = 0);
    ~UpgradeDlg();

signals:
    void sigSetNetgradePort(const ChanInfo &info,int port);
private slots:
    void slotGetDirPath();                                                                              /* 获取升级文件路径 */
    void slotStartUpgrade();                                                                            /* 开始发送升级文件 */

private:
    void Init();
    int ParseUpgradeResult(const QString &str, QString &board, int &rate);
    bool SendUpgradeScip();                                                                             /* 发送准备升级命令 */
    bool NetUpgrade(const QString &filePath);
    bool SendUpgradeData(QtTcpClient* client,const QString &fileName);                                  /* 发送升级文件 */
    bool SendUpgradeEndCmd(QtTcpClient *client);                                                        /* 发送结束升级命名 */
    void SetProcessBarValue(int value);                                                                 /* 设置进度条进度 */
    void LockUI(bool isLocked);                                                                         /* UI锁 */

    int Recv(QString &str, QtTcpClient *client, uint32_t delayMs = 1000, bool check = true) const;         /* 以QString格式接收 */
    int mKey;
    int mUpgradePort;                                                                                   /* 用户自定义升级端口 */
    QString mIP;
    ChanInfo mInfo;
    InstType mType;
    Ui::UpgradeDlg *mUi;
};
}
}

#endif // _PSS_QT_UPGRADEDLG_H_