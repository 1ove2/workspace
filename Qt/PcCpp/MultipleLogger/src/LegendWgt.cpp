#include "LegendWgt.h"
#include "ui_LegendWgt.h"
#include <QColorDialog>

using namespace Pss::Qt;
LegendWgt::LegendWgt(const ChanInfo &info, QWidget *parent) :
    QWidget(parent),
    mInfo(info),
    mColor(::Qt::red),
    mUi(new Ui::LegendWgt)
{
    mUi->setupUi(this);
    SetIconColor(mColor);
    mUi->lb_Color->installEventFilter(this);
    mUi->lb_Name->installEventFilter(this);
    connect(mUi->cb_Visible, &QCheckBox::toggled, this, &LegendWgt::ChanStatusChanged);
}

LegendWgt::~LegendWgt()
{
    delete mUi;
}

void LegendWgt::SetChanInfo(const ChanInfo &info)
{
    mInfo = info;
}

ChanInfo LegendWgt::GetChanInfo() const
{
    return mInfo;
}

void LegendWgt::SetIconColor(const QColor &color)
{
    mColor = color;
    mUi->lb_Color->setPalette(QPalette(color));
    mUi->lb_Color->setAutoFillBackground(true);
}

QColor LegendWgt::GetIconColor() const
{
    return mColor;
}

void LegendWgt::SetChanName(const QString &name)
{
    mUi->lb_Name->setText(name);
}

QString LegendWgt::GetChanName() const
{
    return mUi->lb_Name->text();
}

void LegendWgt::ChanStatusChanged(bool isChecked)
{
    emit sigChanChecked(mInfo, isChecked);
}

bool LegendWgt::eventFilter(QObject *obj, QEvent *e)
{
    if (mUi->lb_Color == obj && QEvent::MouseButtonPress == e->type())
    {
        QColor color = QColorDialog::getColor(mColor, this, QStringLiteral("选择颜色"));
        if (color.isValid())
        {
            emit sigColorChanged(mInfo, color);
            SetIconColor(color);
        }
        return true;
    }
    return false;
}