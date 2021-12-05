#include "ParaWgt.h"
#include "ui_ParaWgt.h"
#include "PssQtHal.h"

using namespace Pss::Qt;
ParaWgt::ParaWgt(QWidget *parent) :
    QWidget(parent),
    mEnable(true),
    mUi(new Ui::ParaWgt)
{
    mUi->setupUi(this);
    SrcVToggled(true);
    connect(mUi->rb_SrcV, &QRadioButton::toggled, this, &ParaWgt::SrcVToggled);
    connect(mUi->pb_Search, &QPushButton::clicked, this, &ParaWgt::SearchChan);
    connect(mUi->cb_Enable, &QCheckBox::toggled,[this](bool isToggled){
        mEnable = isToggled;
        mUi->gb_Para->setEnabled(isToggled);
    });
    connect(mUi->pb_Delete, &QPushButton::clicked, this, &ParaWgt::DeleteChan);
    connect(mUi->pb_Upgrade, &QPushButton::clicked, this, &ParaWgt::Upgrade);
    connect(mUi->le_Plc, &QLineEdit::textChanged, [=](QString text){
        emit sigNplcChanged(mInfo, text);
    });
    mUi->pb_Upgrade->setVisible(false);
    //mUi->cb_Enable->setVisible(false);
    mUi->pb_Delete->setVisible(false);
}

ParaWgt::~ParaWgt()
{
    delete mUi;
}

void ParaWgt::SetInfo(const ChanInfo &info)
{
    mInfo = info;
    if (0 != info.card)
    {
        mUi->pb_Upgrade->setVisible(true);
        mUi->pb_Search->setVisible(false);
        mUi->lb_Addr->setText(QString("1003C卡%1通道%2").arg(info.card).arg(info.chan));
    }
    mKey = info.key;
    mCardID = info.card;
    mChanID = info.chan;
}

void ParaWgt::SetTitle(const QString &str)
{
    mUi->groupBox->setTitle(str);
}

int ParaWgt::GetScanPara(MeasPara &para)
{
    double val = ValWithUnitToStdVal(mUi->le_Src->text().toDouble(), mUi->cb_Src->currentText());
    double lmt = ValWithUnitToStdVal(mUi->le_Lmt->text().toDouble(), mUi->cb_Lmt->currentText());
    if (lmt < 0)
    {
        return -1;
    }
    if (mUi->rb_SrcV->isChecked())
    {
        /* 插卡设备 */
        if (0 != mCardID)
        {
            if (val > 10 * 1.05)
            {
                return -3;
            }
            if (lmt > 0.2 * 1.05)
            {
                return -3;
            }
        }
        /* 台式源表 */
        else
        {
            if (val > 30 * 1.05)
            {
                return -2;
            }
            if (lmt > 1 * 1.05)
            {
                return -2;
            }
        }
    }
    else
    {
        /* 插卡设备 */
        if (0 != mCardID)
        {
            if (val > 0.2 * 1.05)
            {
                return -3;
            }
            if (lmt > 10 * 1.05)
            {
                return -3;
            }
        }
        /* 台式源表 */
        else
        {
            if (val > 1 * 1.05)
            {
                return -2;
            }
            if (lmt > 30 * 1.05)
            {
                return -2;
            }
        }
    }
    para.isSrcV = mUi->rb_SrcV->isChecked();
    para.is4Wires = mUi->rb_4Wires->isChecked();
    para.isLmtAuto = false;
    para.isSrcAuto = false;
    para.srcVal = ValWithUnitToStdVal(mUi->le_Src->text().toDouble(), mUi->cb_Src->currentText());
    para.lmtVal = ValWithUnitToStdVal(mUi->le_Lmt->text().toDouble(), mUi->cb_Lmt->currentText());
    para.lmtRange = ValWithUnitToStdVal(mUi->le_Lmt->text().toDouble(), mUi->cb_Lmt->currentText());
    para.srcRange = qAbs(ValWithUnitToStdVal(mUi->le_Src->text().toDouble(), mUi->cb_Src->currentText()));
    para.nplc = mUi->le_Plc->text().toDouble();
    para.instKey = mKey;
    para.cardID = mCardID;
    para.chanID = GetRealChan(mChanID);
    para.isEnable = mEnable;
    return 0;
}

void ParaWgt::SetScanPara(const MeasPara &para) const
{
    mUi->rb_SrcV->setChecked(para.isSrcV);
    mUi->rb_SrcI->setChecked(!para.isSrcV);
    mUi->le_Src->setText(QString::number(para.srcVal));
    mUi->le_Lmt->setText(QString::number(para.lmtVal));
    mUi->le_Plc->setText(QString::number(para.nplc));
}

void ParaWgt::SetNplc(const QString &text)
{
    mUi->le_Plc->setText(text);
}

void ParaWgt::Upgrade()
{
    emit sigUpgrade(mInfo);
}

void ParaWgt::SrcVToggled(bool isToggled)
{
    static QStringList vUnits = GetAllVUnit();
    static QStringList iUnits = GetAllIUnit();
    if (isToggled)
    {
        mUi->cb_Src->clear();
        mUi->cb_Src->addItems(vUnits);

        mUi->cb_Lmt->clear();
        mUi->cb_Lmt->addItems(iUnits);
    }
    else
    {
        mUi->cb_Src->clear();
        mUi->cb_Src->addItems(iUnits);
    
        mUi->cb_Lmt->clear();
        mUi->cb_Lmt->addItems(vUnits);
    }
}

void ParaWgt::DeleteChan()
{
    if (QMessageBox::Yes == QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("确定删除该通道?\n(插卡设备将删除所有卡通道!)"), QMessageBox::Yes | QMessageBox::No))
    {
        emit sigDeleteChan(mInfo);
    }
}

void ParaWgt::SearchChan()
{
    emit sigSearchChan(mKey);
}

int ParaWgt::GetRealChan(int chan)
{
    int realChan = chan;
    if (2 == chan)
    {
        realChan = 3;
    }
    else if (3 == chan)
    {
        realChan = 4;
    }
    else if (4 == chan)
    {
        realChan = 2;
    }
    return realChan;
}