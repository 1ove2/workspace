#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "ConnectDlg.h"
#include "PssUtil.h"
#include "PssQtHal.h"
#include <QApplication>
#include <QThread>
#include <QMessageBox>

const QVector<QColor> colorVec = {QColor(Qt::red), QColor(Qt::black), QColor(Qt::blue), QColor("#d55e00"),
                                  QColor(Qt::green), QColor(75,172,198), QColor(128,100,162), QColor("#009e73"),
                                  QColor("#8799ff"), QColor("#e3efd4"), QColor("#f43a14"), QColor("#ddee17"),
                                  QColor("#42adb1"), QColor("#f12e2a"), QColor("#a43ef1"), QColor("#e6bae2")};

using namespace Pss::Qt;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    isSend(false),
    isClear(true),
    mTotalTime(0),
    mInstManager(new InstManager),
    mConfigDlg(new ConfigDlg),
    mCommDlg(new ConnectDlg),
    mWgtLegendLayout(new QVBoxLayout),
    mTimer(new QTimer),
    mUi(new Ui::MainWindow)
{
    mUi->setupUi(this);

    setWindowTitle(QString("多通道记录仪(%1)").arg(_GIT_REPO_HASH_));
    mUi->spinBox->setEnabled(false);
    mUi->wgt_Legend->setLayout(mWgtLegendLayout);
    mUi->wgt_Rate->setVisible(false);
    mLabelList.append(mUi->lb_Rate1);
    mLabelList.append(mUi->lb_Rate2);
    mLabelList.append(mUi->lb_Rate3);
    mLabelList.append(mUi->lb_Rate4);
    mLabelList.append(mUi->lb_Rate5);
    mLabelList.append(mUi->lb_Rate6);
    mLabelList.append(mUi->lb_Rate7);
    mLabelList.append(mUi->lb_Rate8);
    EnableUi(false);
//    InstPara para;
//    para.key = 0;
//    para.type = CS400;
//    QMap<int, QVector<int>> chan;
//    chan.insert(2,{1,2,3,4});
//    chan.insert(3,{1,2,3,4});
//    para.channels = chan;
//    AddInst(para);
    connect(mUi->tb_Comm, &QToolButton::clicked, this, &MainWindow::ShowCommDlg);
    connect(mUi->tb_Config, &QToolButton::clicked, this, &MainWindow::ShowConfigDlg);
    connect(mUi->checkBox, &QCheckBox::toggled, this, &MainWindow::EnableReadTime);
    connect(mCommDlg, &ConnectDlg::sigConnectToNetReq, mInstManager, &InstManager::ConnectToLan);
    connect(mCommDlg, &ConnectDlg::sigConnectToComReq, mInstManager, &InstManager::ConnectToRs232);
    connect(mCommDlg, &ConnectDlg::sigConnectToGpibReq, mInstManager, &InstManager::ConnectToGpib);
    connect(mInstManager, &InstManager::sigIsInstConnect, this, &MainWindow::IsInstConnect);
    connect(mInstManager, &InstManager::sigResultReady, this, &MainWindow::ParseData);
    connect(mUi->tb_Start, &QToolButton::clicked, this, &MainWindow::StartRead);
    connect(mUi->tb_Stop, &QToolButton::clicked, this, &MainWindow::StopRead);
    connect(mUi->tb_Clear, &QToolButton::clicked, this, &MainWindow::Clear);
    connect(mUi->tb_Export, &QToolButton::clicked, mUi->tableWidget, &CustomLogTable::ExportData);
    connect(mConfigDlg, &ConfigDlg::sigSearchChan, mInstManager, &InstManager::SearchChan);
    connect(mConfigDlg, &ConfigDlg::sigSetScanPara, mInstManager, &InstManager::SendScanPara);
    connect(mConfigDlg, &ConfigDlg::sigDeleteChan, this, &MainWindow::DeleteChan);
    connect(mUi->tb_QuickConnect, &QToolButton::clicked, this, &MainWindow::QuickConnect);
    connect(mConfigDlg, &ConfigDlg::sigIsChanEnable, this, &MainWindow::IsChanEnable);
    connect(mConfigDlg, &ConfigDlg::sigUpgrade, this, &MainWindow::ShowUpgradeDlg);
    connect(mTimer, &QTimer::timeout, this, &MainWindow::Refresh);
    connect(mInstManager, &InstManager::sigTimeout, this, &MainWindow::ShowErrorMessBox);
    QShortcut *cut = new QShortcut(QKeySequence(::Qt::Key_Home), this);
    connect(cut, &QShortcut::activated, [=](){
        if (mUi->wgt_Rate->isHidden())
        {
            mUi->wgt_Rate->setVisible(true);
        }
        else
        {
            mUi->wgt_Rate->setVisible(false);
        }
    });
    if (PssUtil::IsFileExist("config.ini"))
    {
        LoadConfig("config.ini");
    }
}

MainWindow::~MainWindow()
{
    if (mTimer->isActive())
    {
        mTimer->stop();
    }
    delete mTimer;
    mTimer = nullptr;

    delete mCommDlg;
    mCommDlg = nullptr;
    
    delete mConfigDlg;
    mConfigDlg = nullptr;
    
    delete mInstManager;
    mInstManager = nullptr;
    
    delete mUi;
    mUi = nullptr;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    /* 重写关闭事件， 退出前保存连接成功的ip */
    QStringList ipList = mCommDlg->GetIPConfig();
    QStringList keyList;
    for (int i = 0; i < ipList.size(); ++i)
    {
        keyList.append(QString("IP%1").arg(i));
    }
    PssUtil::SetIniValue("config.ini", "IP", keyList, ipList);
    SaveConfig("config.ini");
    QMainWindow::closeEvent(e);
}

void MainWindow::AddInst(const InstPara &para)
{
    EnableUi(true);
    mCommDlg->AddItem();

    if (para.type == CS400)
    {
        for (int i = 0; i < para.channels.size(); ++i)
        {
            for (int j = 0; j < para.channels.values().value(i).size(); ++j)
            {
                int cnt = mUi->plotWidget->axisRectCount();
    
                ChanInfo info;
                info.key = para.key;
                info.card =para.channels.keys().value(i);
                info.chan =  para.channels.values().value(i).value(j);
    
                mUi->plotWidget->AppendAxisRect(info);
                mUi->plotWidget->graph(cnt)->setPen(QPen(colorVec.value(cnt)));
    
                LegendWgt *legend = new LegendWgt(info, this);
                mLegendWgts.insert(info, legend);
    
                legend->SetChanName(QString("通道%1card%2:chan:%3").arg(mLegendWgts.count()).arg(info.card).arg(info.chan));
                legend->SetIconColor(colorVec.value(cnt));
                connect(legend, &LegendWgt::sigChanChecked, mUi->plotWidget, &CustomLogPlot::ChanChecked);
                connect(legend, &LegendWgt::sigColorChanged, mUi->plotWidget, &CustomLogPlot::ColorChanged);
                mWgtLegendLayout->addWidget(legend);
    
                mConfigDlg->AddInst(info);
                mUi->tableWidget->AddChannel(info);
                QVector<DataPara> para;
                mDataVec.insert(info, para);
            }
        }
    }
    else if (para.type == S100)
    {
        int cnt = mUi->plotWidget->axisRectCount();
        ChanInfo info;
        info.key = para.key;
        info.card =0;
        info.chan =  0;
    
        mUi->plotWidget->AppendAxisRect(info);
        mUi->plotWidget->graph(cnt)->setPen(QPen(colorVec.value(cnt)));
    
        LegendWgt *legend = new LegendWgt(info, this);
        mLegendWgts.insert(info, legend);
        legend->SetChanName(QString("通道%1").arg(mLegendWgts.count()));
        legend->SetIconColor(colorVec.value(cnt));
        connect(legend, &LegendWgt::sigChanChecked, mUi->plotWidget, &CustomLogPlot::ChanChecked);
        connect(legend, &LegendWgt::sigColorChanged, mUi->plotWidget, &CustomLogPlot::ColorChanged);
        mWgtLegendLayout->addWidget(legend);
    
        mConfigDlg->AddInst(info);
    
        mUi->tableWidget->AddChannel(info);
    }
}

void MainWindow::StartRead()
{
    allTime = 0;
    mUi->tabWidget->setCurrentIndex(0);
    //mUi->tabWidget->setEnabled(false);
    /* 启动UI刷新定时器 */
    mTimer->start(60);
    mRollPara = mConfigDlg->GetRollTimePara();
    mAverPara = mConfigDlg->GetAveragePara();
    mUi->wgt_Timing->setEnabled(false);
    EnableButton(false);
    if (isClear)
    {
        /* 再读数 */
        mDateTime = QDateTime::currentDateTime();
        isClear = false;
    }
    mInstManager->StartRead();
}

void MainWindow::StopRead()
{
    mUi->tabWidget->setEnabled(true);
    mTimer->stop();
    mUi->wgt_Timing->setEnabled(true);
    EnableButton(true);
    mInstManager->StopRead();
}

void MainWindow::ShowConfigDlg()
{
    mConfigDlg->exec();
}

void MainWindow::ShowCommDlg()
{
    mCommDlg->exec();
}

void MainWindow::EnableReadTime(bool isEnable)
{
    mUi->spinBox->setEnabled(isEnable);
}

void MainWindow::EnableButton(bool isEnable)
{
    mUi->tb_Start->setEnabled(isEnable);
    mUi->tb_Clear->setEnabled(isEnable);
    mUi->tb_Comm->setEnabled(isEnable);
    mUi->tb_Config->setEnabled(isEnable);
    mUi->tb_Export->setEnabled(isEnable);
    mUi->tb_QuickConnect->setEnabled(isEnable);
}

void MainWindow::ParseData(QDateTime date, const QVector<DataTag> &data)
{
    mTotalTime = (date.toMSecsSinceEpoch() - mDateTime.toMSecsSinceEpoch());
    double totalTimeS = mTotalTime / 1000.0;

    int chanCnt = data.size();
    for (int i = 0; i < chanCnt; ++i)
    {
        DataTag dataP = data.value(i);
        QVector<RecvDataPara> paras = dataP.para;
        //allTime += dataP.time;
        for (int j = 0; j < paras.size(); ++j)
        {
            ChanInfo info;
            info.key = paras.value(j).key;
            info.card = paras.value(j).card;
            info.chan = paras.value(j).channel;
    
            if (mAverPara.isAverage)
            {
                if ( (mAverPara.avePoint) > mDataVec[info].count() )
                {
                    DataPara dPara;
                    dPara.volt = paras.value(j).volt;
                    dPara.curr = paras.value(j).curr;
                    dPara.ohm = qAbs(paras.value(j).volt / paras.value(j).curr);
                    mDataVec[info].append(dPara);
                }
                else if ((mAverPara.avePoint) == mDataVec[info].count())
                {
                    qDebug() << "point:" << mDataVec[info].count();
                    DataPara dPara = GetAverData(mDataVec[info]);
    
                    mUi->plotWidget->AddData(info, totalTimeS, dPara.ohm);
                    mUi->tableWidget->AddData(date, dPara.volt, dPara.curr, info);
                    mDataVec[info].clear();
                }
            }
            else
            {
                qDebug() << "info:" << info.key << info.card << info.chan << " time:" << totalTimeS;
                mUi->plotWidget->AddData(info, totalTimeS, qAbs(paras.value(j).volt / paras.value(j).curr));
                mUi->tableWidget->AddData(date, paras.value(j).volt, paras.value(j).curr, info);
            }
        }
    }
    /* 如果设置了定时关闭 */
    if (mUi->checkBox->isChecked())
    {
        uint32_t setupTime = mUi->spinBox->text().toUInt();
        if (mTotalTime >= setupTime * 1000)
        {
            StopRead();
        }
    }
}

void MainWindow::Refresh()
{
    /* 未输出状态 */
    if (mUi->tb_Start->isEnabled())
    {
        return;
    }
    for (int i = 0; i < mUi->plotWidget->axisRectCount(); ++i)
    {
        /* 设置了滚动 */
        if (mRollPara.isRoll)
        {
            double totalTimeS = mTotalTime / 1000.0;
            if (totalTimeS > mRollPara.rollTime)
            {
               mUi->plotWidget->axisRect(i)->axis(QCPAxis::atBottom)->setRange(totalTimeS - mRollPara.rollTime, totalTimeS);
            }
            else
            {
                mUi->plotWidget->axisRect(i)->axis(QCPAxis::atBottom)->rescale(true);
            }
        }
        else
        {
            mUi->plotWidget->axisRect(i)->axis(QCPAxis::atBottom)->rescale(true);
        }
    }
    mUi->plotWidget->replot();
    //mUi->tableWidget->setUpdatesEnabled(true);
    mUi->tableWidget->scrollToBottom();
}

void MainWindow::IsInstConnect(bool isConnect, const QString &ip, InstPara para)
{
    mCommDlg->SetConnectBtnEnable(true);
    if (!isConnect)
    {
        isSend = true;
        QMessageBox::warning(this, "提示", QString("%1连接失败!").arg(ip));
        return;
    }
    isSend = true;
    mCommDlg->hide();

    AddInst(para);
}

void MainWindow::QuickConnect()
{
    mUi->wgt_Func->setEnabled(false);
    QStringList ipList = mCommDlg->GetIPConfig();
    for (int i = 0; i < ipList.count(); ++i)
    {
        isSend = false;
        QString ip = ipList.value(i);
        emit mCommDlg->sigConnectToNetReq(ip);
        while(!isSend)
        {
            QCoreApplication::processEvents();
            QThread::msleep(1);
        }
    }
    mUi->wgt_Func->setEnabled(true);
}

void MainWindow::Clear()
{
    isClear = true;

    for (int i = 0; i <mDataVec.keys().size(); ++i)
    {
        mDataVec[mDataVec.keys().value(i)].clear();
    }
    mTotalTime = 0;
    for (int i = 0; i < mUi->plotWidget->axisRectCount(); ++i)
    {
        mUi->plotWidget->graph(i)->data().data()->clear();
    }
    mUi->plotWidget->replot();
    mUi->tableWidget->Clear();
}

void MainWindow::DeleteChan(const ChanInfo &info)
{
    /* 绘图删除 */
    mUi->plotWidget->RemoveChan(info);

    /*  legend */
    for (int i = 0; i < mLegendWgts.count(); ++i)
    {
        if (mLegendWgts.keys().value(i).key== info.key)
        {
            mWgtLegendLayout->removeWidget(mLegendWgts[mLegendWgts.keys().value(i)]);
            delete mLegendWgts.take(mLegendWgts.keys().value(i));
        }
    }
    
    /* 表格删除 */
    
    mUi->tableWidget->DeleteChan(info);
    /* 设备删除 */
    mInstManager->RemoveChan(info);
}

void MainWindow::IsChanEnable(const ChanInfo &info, bool isEnable)
{
    //mUi->plotWidget->EnableChan(info, isEnable);
    //mLegendWgts[info]->setVisible(isEnable);
    //mUi->tableWidget->EnableChan(info, isEnable);
}

void MainWindow::ShowUpgradeDlg(const ChanInfo &info)
{
    /* 得到ip */
    QString ip = mInstManager->GetIP(info.key);
    InstType type;
    if (0 != info.card)
    {
        type = CS400;
    }
    else
    {
        type = S100;
    }
    UpgradeDlg *dlg = new UpgradeDlg(ip,info,type);
    connect(dlg, &UpgradeDlg::sigSetNetgradePort,mInstManager, &InstManager::SetUpgradePort);
    dlg->setAttribute(::Qt::WA_DeleteOnClose);
    dlg->exec();
}

void MainWindow::ShowErrorMessBox()
{
    QMessageBox::warning(this, QString("提示"), QString("读数超时!"));
    StopRead();
    return;
}

DataPara MainWindow::GetAverData(const QVector<DataPara> &paraVec)
{
    DataPara avePara;
    double ohmVal = 0.0, vVal = 0.0, iVal = 0.0;
    int cnt = paraVec.count();
    qDebug() << "cnt::" << cnt;
    for (int i = 0; i < cnt; ++i)
    {
        //值平均
        vVal += paraVec.value(i).volt;
        iVal += paraVec.value(i).curr;
        ohmVal += paraVec.value(i).ohm;
    }
    avePara.volt = vVal / cnt;
    avePara.curr = iVal / cnt;
    avePara.ohm = ohmVal / cnt;
    return avePara;
}

void MainWindow::LoadConfig(const QString &path)
{
    QSettings ini(path, QSettings::IniFormat);
    RollTimePara paraTime;
    paraTime.isRoll = ini.value("isRoll").toBool();
    paraTime.rollTime = ini.value("rollTime").toString().toInt();
    mConfigDlg->SetRollTimePara(paraTime);

    mUi->checkBox->setChecked(ini.value("isTiming").toBool());
    mUi->spinBox->setValue(ini.value("time").toString().toInt());
}

void MainWindow::SaveConfig(const QString &path)
{
    QSettings ini(path, QSettings::IniFormat);

    /* 每个通道的颜色 */
    ini.beginGroup("color");
    for (int i = 0; i < colorList.count(); ++i)
    {
        ini.setValue(QString("color%1").arg(i), colorList.value(i));
    }
    ini.endGroup();
    /* 是否定时及定时时间 */
    ini.setValue("isTiming", mUi->checkBox->isChecked());
    ini.setValue("time", mUi->spinBox->text());
    /* 设置界面的参数 */
    QVector<MeasPara> paraVec;
    //mConfigDlg->GetScanParas(paraVec);
    RollTimePara paraTime = mConfigDlg->GetRollTimePara();
    ini.beginGroup("para");
    for (int i = 0; i < paraVec.count(); ++i)
    {
        ini.setValue(QString("para%1Src").arg(i), paraVec.value(i).isSrcV);
        ini.setValue(QString("para%1SrcVal").arg(i), paraVec.value(i).srcVal);
        ini.setValue(QString("para%1LmtVal").arg(i), paraVec.value(i).lmtVal);
        ini.setValue(QString("para%1plc").arg(i), paraVec.value(i).nplc);
    }
    ini.endGroup();
    ini.setValue("isRoll", paraTime.isRoll);
    ini.setValue("rollTime", paraTime.rollTime);
}

void MainWindow::EnableUi(bool isEnabled)
{
    mUi->tb_Comm->setEnabled(true);
    mUi->wgt_Timing->setEnabled(isEnabled);
    mUi->tb_Clear->setEnabled(isEnabled);
    mUi->tb_Config->setEnabled(isEnabled);
    mUi->tb_Export->setEnabled(isEnabled);
    mUi->tb_Start->setEnabled(isEnabled);
    mUi->tb_Stop->setEnabled(isEnabled);
    mUi->tb_QuickConnect->setEnabled(true);
}