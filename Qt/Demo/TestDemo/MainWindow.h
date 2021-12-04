#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ConnectDlg.h"
#include "TestCfgWgt.h"
#include "DataQueryWgt.h"
#include "DataAnalysis.h"
#include "LoginDlg.h"
#include "UserManageWgt.h"
#include "ChangePwdWgt.h"
#include "DBCfgWgt.h"
#include "DataQueryWgt.h"
#include "DataAnalysis.h"
#include "RealTimeDataWgt.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    CreateWgt();
    InitMenuAction();
    void SetAuthority(QString strAuthority);
public slots:
    void sltTrigConnectDlg();
    void sltTrigNetCfgWgt();
    void sltTrigTestCfgWgt();
    void sltTrigChanCfgWgt();
    void sltTrigUserMgDlg();
    void sltTrigSwitchUserDlg();
    void sltTrigDbCfgWgt();
    void sltTrigDataViewWgt();
    void sltTrigDataAnalysis();
    void sltTrigReadTimeData();
    void sltShowCurrentUserName(QString UserName);
private:
    Ui::MainWindow *ui;
    ConnectDlg *m_pConnectDlg;
    TestCfgWgt *m_pTestCfgWgt;
    DataQueryWgt *m_pDataQueryWgt;
    DataAnalysis *m_pDataAnalysisWgt;
    LoginDlg *m_pLoginDlg;
    UserManageWgt *m_pUserManageWgt;
    DBCfgWgt *m_pDbCfgWgt;
    DataAnalysis *m_pDataAnalysis;
    RealTimeDataWgt *m_pRealTimeDataWgt;


    int m_bInTesting;
};

#endif // MAINWINDOW_H
