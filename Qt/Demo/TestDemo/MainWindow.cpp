#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_bInTesting(false),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CreateWgt();
    InitMenuAction();
}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::CreateWgt()
{
    m_pConnectDlg = new ConnectDlg();
    m_pTestCfgWgt = new TestCfgWgt();
    m_pDataQueryWgt = new DataQueryWgt();
    m_pDataAnalysisWgt = new DataAnalysis();
    m_pRealTimeDataWgt = new RealTimeDataWgt();
    m_pLoginDlg = new LoginDlg();
    m_pUserManageWgt = new UserManageWgt();
    m_pDbCfgWgt = new DBCfgWgt();
    m_pDataAnalysis = new DataAnalysis();
}

MainWindow::InitMenuAction()
{
    connect(ui->action_Conn, &QAction::triggered, this, &MainWindow::sltTrigConnectDlg);
    connect(ui->action_NetCfg, &QAction::triggered, this, &MainWindow::sltTrigNetCfgWgt);
    connect(ui->action_TestCfg, &QAction::triggered, this, &MainWindow::sltTrigTestCfgWgt);
    connect(ui->action_ChanlCfg, &QAction::triggered, this, &MainWindow::sltTrigChanCfgWgt);
    connect(ui->action_UserMg, &QAction::triggered, this, &MainWindow::sltTrigUserMgDlg);
    connect(ui->action_SwitchUser, &QAction::triggered, this, &MainWindow::sltTrigSwitchUserDlg);
    connect(ui->action_DbConfig, &QAction::triggered, this, &MainWindow::sltTrigDbCfgWgt);
    connect(ui->action_DataView, &QAction::triggered, this, &MainWindow::sltTrigDataViewWgt);
    connect(ui->action_DataAnalysis, &QAction::triggered, this, &MainWindow::sltTrigDataAnalysis);
    connect(ui->action_RealTimeData, &QAction::triggered, this, &MainWindow::sltTrigReadTimeData);
}

void MainWindow::sltTrigConnectDlg()
{
    m_pConnectDlg->exec();
}

void MainWindow::sltTrigNetCfgWgt()
{
    if(m_bInTesting)
    {
        QMessageBox::information(this, "提示", "当前正在测试中，无法更改设备IP!");
        return;
    }

}

void MainWindow::sltTrigTestCfgWgt()
{
    if(m_bInTesting)
    {
        QMessageBox::information(this,"提示","当前测试中，无法配置参数");
        return;
    }

    if(m_pTestCfgWgt)
    {
        m_pTestCfgWgt->show();
    }
}

void MainWindow::sltTrigChanCfgWgt()
{
    if(m_bInTesting)
    {
        QMessageBox::information(this, "提示", "当前测试中,无法配置通道参数");
        return;
    }
}

void MainWindow::sltTrigUserMgDlg()
{
    m_pUserManageWgt->exec();
}

void MainWindow::sltTrigSwitchUserDlg()
{
    m_pLoginDlg = new LoginDlg();
    connect(m_pLoginDlg,&LoginDlg::sigUserChange,this,&MainWindow::sltShowCurrentUserName);
    m_pLoginDlg->setWinTitle("切换用户","切换");
    int nRet = m_pLoginDlg->exec();
    if(nRet == QDialog::Accepted)
    {
        SetAuthority(m_pLoginDlg->m_strAuthority);
        m_pLoginDlg->close();
        m_pLoginDlg = nullptr;
    }
}

void MainWindow::sltTrigDbCfgWgt()
{
    m_pDbCfgWgt->exec();
}

void MainWindow::sltTrigDataViewWgt()
{
    m_pDataQueryWgt->activateWindow();
    m_pDataQueryWgt->setWindowState((this->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    m_pDataQueryWgt->show();
}

void MainWindow::sltTrigDataAnalysis()
{
    m_pDataAnalysis->activateWindow();
    m_pDataAnalysis->setWindowState((this->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    m_pDataAnalysis->show();
}

void MainWindow::sltTrigReadTimeData()
{
    m_pRealTimeDataWgt->activateWindow();
    m_pRealTimeDataWgt->setWindowState((this->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    m_pRealTimeDataWgt->show();
}

void MainWindow::sltShowCurrentUserName(QString UserName)
{
    ui->lab_UserName->setText(UserName);
}


void MainWindow::SetAuthority(QString strAuthority)
{
    if(strAuthority == "管理员")
    {
        ui->action_NetCfg->setVisible(true);
        ui->action_TestCfg->setVisible(true);
        ui->action_ChanlCfg->setVisible(true);
        ui->action_UserMg->setVisible(true);
    }
    else if(strAuthority == "操作员")
    {
        ui->action_NetCfg->setVisible(false);
        ui->action_TestCfg->setVisible(false);
        ui->action_ChanlCfg->setVisible(false);
        ui->action_UserMg->setVisible(false);
    }
}
