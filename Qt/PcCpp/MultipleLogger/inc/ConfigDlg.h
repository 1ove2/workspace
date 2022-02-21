#ifndef _PSS_QT_CONFIGDLG_H_
#define _PSS_QT_CONFIGDLG_H_

#include <QDialog>
#include <QMap>
#include <QGridLayout>
#include "ParaWgt.h"

struct RollTimePara {
    bool isRoll;
    int rollTime;
};

struct AveragePara {
    bool isAverage;
    int avePoint;
};

namespace Ui {
class ConfigDlg;
}

namespace Pss {
namespace Qt {

class ConfigDlg : public QDialog
{
    Q_OBJECT

public:
    ConfigDlg(QWidget *parent = 0);
    ~ConfigDlg();
    RollTimePara GetRollTimePara() const;                                /* 获取用户设置的滚动参数 */
    void SetRollTimePara(const RollTimePara &para) const;                /* 根据配置文件加载用户之前设置的滚动参数 */
    AveragePara GetAveragePara() const;
    void SetAveragePara(const AveragePara &para) const;

    void AddInst(const ChanInfo &info);                                      /* 新增设备 */
    bool GetScanParas(QVector<MeasPara> &paraVec);                              /* 获取设备参数 */
    void SetScanParas(const MeasPara &para);                       /* 设置设备参数 */
    void DeleteChan(const ChanInfo &info);
signals:
    void sigDeleteChan(const ChanInfo &info);
    void sigSearchChan(int chanID);
    void sigSetScanPara(const QVector<MeasPara> &paras);
    void sigIsChanEnable(const ChanInfo &info, bool isEnable);
    void sigUpgrade(const ChanInfo &info);
private:
    void Resize();

    void ConfirmPara();
    QMap<ChanInfo, ParaWgt*> mParaWgts;
    QGridLayout *mGridLayout;
    Ui::ConfigDlg *mUi;
};
}
}

#endif // _PSS_QT_CONFIGDLG_H_