#include "ConnectDlg.h"
#include "ui_ConnectDlg.h"
#include "PssUtil.h"
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QLineEdit>
#include <QSettings>
#include <QThread>
#include <QDebug>

//const QIcon mIcon(":/icons/DeleteIP.svg");   /* 会导致再其他电脑上运行时提示cant load qt plugin windows!!! */
const QStringList baudList = {"115200","9600"};

using namespace Pss::Qt;
ConnectDlg::ConnectDlg(QWidget *parent)
    :QDialog(parent),

    mUi(new Ui::ConnectDlg)
{
    mUi->setupUi(this);
    setWindowTitle("连接设备");
    setWindowFlags(::Qt::CustomizeWindowHint | ::Qt::WindowCloseButtonHint);

    mUi->label_Tip->setText("添加一个LAN连接设备");
    mUi->widget->setEnabled(false);
    
    mUi->lineEdit_CardAddr->setValidator(new QIntValidator(0,2,this));
    mUi->lineEdit_DevAddr->setValidator(new QIntValidator(0,30,this));

//    /* 安装自定义的IP下拉框 */
    InstallIPLineEdit();

    /* 信号槽连接 */
    InitConnections();
#ifdef _PSS_GPIB_SUPPORT_
#else
    mUi->tab_2->setEnabled(false);
#endif
}

ConnectDlg::~ConnectDlg()
{
    delete mUi;
    mUi = nullptr;
}

QStringList ConnectDlg::GetIPConfig() const
{
    return mUi->comboBox_Net->GetItemTexts();
}

void ConnectDlg::SetConnectBtnEnable(bool isEnabled)
{
    mUi->tabWidget->setEnabled(isEnabled);
    mUi->pushButton_Connect->setEnabled(isEnabled);
    mUi->pushButton_Cancel->setEnabled(isEnabled);
    if (isEnabled)
    {
        mUi->label_ConnectTip->setText("");
    }
}

void ConnectDlg::AddItem()
{
    if ("..." != mUi->comboBox_Net->currentText().trimmed())
    {
        mUi->comboBox_Net->AddItem(mUi->comboBox_Net->currentText().trimmed(), QIcon(":/icons/DeleteIP.svg"));
    }
}

void ConnectDlg::slotConnectTo()
{
    /* 点击连接后 按钮被禁用 */
    mUi->tabWidget->setEnabled(false);
    mUi->pushButton_Connect->setEnabled(false);
    mUi->pushButton_Cancel->setEnabled(false);
    mUi->label_ConnectTip->setText("正在连接...");
    repaint();

    if (0 == mUi->tabWidget->currentIndex())
    {
        emit sigConnectToNetReq(mUi->comboBox_Net->currentText().trimmed());
    }
    else if (1 == mUi->tabWidget->currentIndex())
    {
        emit sigConnectToComReq(mUi->comboBox_Com->currentText(), mUi->comboBox_Baud->currentText().toInt());
    }
    else if (2 == mUi->tabWidget->currentIndex())
    {
#ifdef _PSS_GPIB_SUPPORT_
#else
        QMessageBox::information(this,"提示","该版本不支持GPIB!");
        mUi->tabWidget->setEnabled(true);
        mUi->pushButton_Connect->setEnabled(true);
        mUi->pushButton_Cancel->setEnabled(true);
        mUi->label_ConnectTip->setText("");
        return;
#endif
        if (mUi->radioButton_Auto->isChecked())
        {
            emit sigConnectToGpibReq(mUi->comboBox_Gipb->currentText());
        }
        else
        {
            QString gpibStr = QString("GPIB%1::%2::INSTR").arg(mUi->lineEdit_CardAddr->text().toInt()).arg(mUi->lineEdit_DevAddr->text().toInt());
            emit sigConnectToGpibReq(gpibStr);
        }
    }
}

void ConnectDlg::InitConnections()
{
    /* 按键信号槽连接 */
    connect(mUi->pushButton_Cancel, &QPushButton::clicked, this, &ConnectDlg::close);
    connect(mUi->pushButton_Connect, &QPushButton::clicked, this, &ConnectDlg::slotConnectTo);

    /* IP回车键连接 */
    connect(mUi->comboBox_Net->lineEdit(), &QLineEdit::returnPressed,this, &ConnectDlg::slotConnectTo);
    
    connect(mUi->tabWidget, &QTabWidget::tabBarClicked, [this](int index){
        if (0 == index)
        {
            mUi->label_Tip->setText("添加一个LAN连接设备");
        }
        else if (1 == index)
        {
            mUi->label_Tip->setText("添加一个RS232连接设备");
            mUi->comboBox_Baud->clear();
            mUi->comboBox_Baud->addItems(baudList);
    
            /* 查找可用串口并添加至下拉框 */
            mUi->comboBox_Com->clear();                                                                     /* 添加前先清空 */
            foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
            {
                mUi->comboBox_Com->addItem(info.portName());
            }
        }
        else if (2 == index)
        {
            mUi->label_Tip->setText("添加一个GPIB连接设备");
            mUi->comboBox_Gipb->clear();
#ifdef _PSS_GPIB_SUPPORT_
            QStringList GpibList = PssUtil::GetEnabledGpib();
            mUi->comboBox_Gipb->addItems(GpibList);
#endif
        }
    });
    connect(mUi->radioButton_Auto, &QRadioButton::toggled, [this](bool isToggled){
        mUi->comboBox_Gipb->setEnabled(isToggled);
        mUi->widget->setEnabled(!isToggled);
    });
}

void ConnectDlg::InstallIPLineEdit()
{
    mUi->comboBox_Net->lineEdit()->setInputMask("000.000.000.000");
    mUi->comboBox_Net->lineEdit()->setMaxLength(16);                                                    /* 设置最大长度 */

    /* 实现光标自动跳过"." */
    connect(mUi->comboBox_Net->lineEdit(), &QLineEdit::cursorPositionChanged, [=](int oldPos, int newPos){
        /* 右移 */
        if (newPos > oldPos)
        {
            if (3 == newPos || 7 == newPos || 11 == newPos)
            {
                mUi->comboBox_Net->lineEdit()->setCursorPosition(newPos + 1);
            }
        }
        /* 左移 */
        else
        {
            if (3 == newPos || 7 == newPos || 11 == newPos)
            {
                mUi->comboBox_Net->lineEdit()->setCursorPosition(newPos - 1);
            }
        }
    });
    
    /* 从配置文件读取ip 加载到下拉框 */
    QStringList ipList = PssUtil::GetIniValue("config.ini","IP");
    mUi->comboBox_Net->AddItems(ipList, QIcon(":/icons/DeleteIP.svg"));
}