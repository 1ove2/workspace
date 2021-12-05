#ifndef _PSS_MAINWINDOW_H_
#define _PSS_MAINWINDOW_H_

#include <QMainWindow>
#include <QVBoxLayout>
#include "ConfigDlg.h"
#include "CustomLogPlot.h"
#include <QDateTime>
#include "InstManager.h"
#include "LegendWgt.h"
#include "UpgradeDlg.h"
#define MAX_INST (15)

struct DataPara {
    double volt;
    double curr;
    double ohm;
};

namespace Ui {
class MainWindow;
}

namespace Pss {
namespace Qt {

class ConnectDlg;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *e);
private:
    void AddInst(const InstPara &para);
    void StartRead();
    void StopRead();
    void EnableUi(bool isEnabled);

private slots:
    void ShowConfigDlg();
    void ShowCommDlg();
    void EnableReadTime(bool isEnable);
    void EnableButton(bool isEnable);
    void ParseData(QDateTime date, const QVector<DataTag> &data);
    void Refresh();
    void IsInstConnect(bool isConnect, const QString &ip, InstPara para);
    void QuickConnect();
    void Clear();
    void DeleteChan(const ChanInfo &info);
    void IsChanEnable(const ChanInfo &info, bool isEnable);
    void ShowUpgradeDlg(const ChanInfo &info);
    void ShowErrorMessBox();
private:
    DataPara GetAverData(const QVector<DataPara> &paraVec);
    void LoadConfig(const QString &path);
    void SaveConfig(const QString &path);
    bool isSend;
    bool isClear;

    int allTime;
    QDateTime mDateTime;
    QStringList colorList;
    AveragePara mAverPara;
    QMap<ChanInfo, QVector<DataPara>> mDataVec;
    QMap<ChanInfo, LegendWgt*> mLegendWgts;
    //QVector<DataPara> mDataVec[MAX_INST];
    RollTimePara mRollPara;
    quint64 mTotalTime;
    InstManager *mInstManager;
    ConfigDlg *mConfigDlg;
    ConnectDlg *mCommDlg;
    QVBoxLayout *mWgtLegendLayout;
    QTimer *mTimer;


    QVector<QLabel*> mLabelList;                                                                        /* 显示采样率 */
    Ui::MainWindow *mUi;
};
}
}

#endif // _PSS_MAINWINDOW_H_