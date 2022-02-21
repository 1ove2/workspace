#include "ConfigDlg.h"
#include "ui_ConfigDlg.h"
#include <QMessageBox>

const int MAX_ROW_CHACNNEL = 4;

using namespace Pss::Qt;
ConfigDlg::ConfigDlg(QWidget *parent) :
    QDialog(parent),
    mGridLayout(new QGridLayout),
    mUi(new Ui::ConfigDlg)
{
    mUi->setupUi(this);
    setWindowTitle(QStringLiteral("参数配置"));
    mUi->sb_AxisXRollTime->setEnabled(false);
    mUi->sb_AvePoint->setEnabled(false);
    mUi->groupBox->setLayout(mGridLayout);
    connect(mUi->pb_Cancel, &QPushButton::clicked, this, &ConfigDlg::hide);
    connect(mUi->pb_Confirm, &QPushButton::clicked, this, &ConfigDlg::ConfirmPara);
    connect(mUi->cb_AxisXRoll, &QCheckBox::toggled, [=](bool isToggled){
        mUi->sb_AxisXRollTime->setEnabled(isToggled);
    });
    connect(mUi->cb_Average, &QCheckBox::toggled, [=](bool isToggled){
        mUi->sb_AvePoint->setEnabled(isToggled);
    });
}

ConfigDlg::~ConfigDlg()
{
    delete mUi;
}

RollTimePara ConfigDlg::GetRollTimePara() const
{
    RollTimePara para;
    para.isRoll = mUi->cb_AxisXRoll->isChecked();
    para.rollTime = mUi->sb_AxisXRollTime->text().toInt();
    return para;
}

void ConfigDlg::SetRollTimePara(const RollTimePara &para) const
{
    mUi->cb_AxisXRoll->setChecked(para.isRoll);
    mUi->sb_AxisXRollTime->setValue(para.rollTime);
}

AveragePara ConfigDlg::GetAveragePara() const
{
    AveragePara para;
    para.isAverage = mUi->cb_Average->isChecked();
    para.avePoint = mUi->sb_AvePoint->value();
    return para;
}

void ConfigDlg::SetAveragePara(const AveragePara &para) const
{
    mUi->cb_Average->setChecked(para.isAverage);
    mUi->sb_AvePoint->setValue(para.avePoint);
}

void ConfigDlg::AddInst(const ChanInfo &info)
{
    ParaWgt *wgt = new ParaWgt(mUi->groupBox);
    connect(wgt, &ParaWgt::sigDeleteChan, this, &ConfigDlg::DeleteChan);
    connect(wgt, &ParaWgt::sigSearchChan, this, &ConfigDlg::sigSearchChan);
    connect(wgt, &ParaWgt::sigUpgrade, this, &ConfigDlg::sigUpgrade);
    connect(wgt, &ParaWgt::sigNplcChanged, [=](const ChanInfo &info, QString text){
        for (int i = 0; i < mParaWgts.count(); ++i)
        {
            if (mParaWgts.keys().value(i).card == info.card)
            {
                mParaWgts.values().value(i)->SetNplc(text);
            }
        }
    });
    wgt->SetInfo(info);

    int cnt = mParaWgts.count();
    int x = cnt / MAX_ROW_CHACNNEL;            /* 布局的行 */
    int y = cnt % MAX_ROW_CHACNNEL;            /* 布局的列 */
    wgt->SetTitle(QString("通道%1").arg(cnt+1));
    //mParaWgts.append(wgt);
    mParaWgts.insert(info, wgt);
    mGridLayout->addWidget(wgt, x, y);
}

bool ConfigDlg::GetScanParas(QVector<MeasPara> &paraVec)
{
#if 1
    for (int i = 0; i < mParaWgts.count(); ++i)
    {
        MeasPara para;
        int ret = mParaWgts.values().value(i)->GetScanPara(para);
        if (-1 == ret)
        {
            QMessageBox::warning(this, "提示", "限值不能小于0!");
            return false;
        }
        else if (-2 == ret)
        {
            QMessageBox::warning(this, "提示", "S100最大电压为30V,最大电流为1A!");
            return false;
        }
        else if (-3 == ret)
        {
            QMessageBox::warning(this, "提示", "CS400最大电压为10V,最大电流为200mA!");
            return false;
        }
        paraVec.append(para);
        ChanInfo info;
        info.key = para.instKey;
        info.card = para.cardID;
        info.chan = para.chanID;
//        QString info;
//        info.append(QString::number(para.instKey));
//        info.append(QString::number(para.cardID));
//        info.append(QString::number(para.chanID));
        emit sigIsChanEnable(info, para.isEnable);
    }
#endif
    return true;
}

void ConfigDlg::SetScanParas(const MeasPara &para)
{
    //mParaWgts.value(mParaWgts.count()-1)->SetScanPara(para);
}

void ConfigDlg::DeleteChan(const ChanInfo &info)
{
    /* 删除该窗口后刷新布局 */
    int wgtCnt = mParaWgts.count();
    for (int i = 0; i < wgtCnt; ++i)
    {
        mGridLayout->removeWidget(mParaWgts.values().value(i));
    }
    int chanCnt = 0;
    /* 判断是台式还是插卡式 */
    if (0 != info.card)
    {
        for (int i = 0; i < wgtCnt; ++i)
        {
            if (mParaWgts.keys().value(i).key == info.key)
            {
                chanCnt++;
                delete mParaWgts.take(mParaWgts.keys().value(i));
            }
        }
    }
    else
    {
        chanCnt = 1;
        delete mParaWgts.take(info);
    }


    for (int i = 0; i < wgtCnt-chanCnt; ++i)
    {
        int x = i / MAX_ROW_CHACNNEL;            /* 布局的行 */
        int y = i % MAX_ROW_CHACNNEL;            /* 布局的列 */
        mGridLayout->addWidget(mParaWgts.values().value(i), x, y);
        mParaWgts.values().value(i)->SetTitle(QString("通道%1").arg(i+1));
    }
    /* 提示主界面刷新绘图、表格 */
    emit sigDeleteChan(info);
}

void ConfigDlg::Resize()
{
    /* 250 * 280 */
    int cnt = mParaWgts.count();
    int width = (cnt % MAX_ROW_CHACNNEL) * 280 + 18;
    int height = (cnt / MAX_ROW_CHACNNEL + 1) *250 + 156;

    resize(width, height);
}

void ConfigDlg::ConfirmPara()
{
    QVector<MeasPara> paras;
    if (!GetScanParas(paras))
    {
        return;
    }
    emit sigSetScanPara(paras);
    hide();
}