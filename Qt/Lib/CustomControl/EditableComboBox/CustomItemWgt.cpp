#include "CustomItemWgt.h"
#include <QHBoxLayout>
#include <QMouseEvent>
CustomItemWgt::CustomItemWgt(QWidget *parent)
    : QWidget(parent),
      mLabel(new QLabel),
      mDeleteButton(new QPushButton),
      mPressed(false)
{
    InitWidget();
}

CustomItemWgt::~CustomItemWgt()
{
    delete mLabel;
    delete mDeleteButton;

    mLabel = nullptr;
    mDeleteButton = nullptr;
}

void CustomItemWgt::SetText(const QString &text)
{
    mLabel->setText(text);
}

QString CustomItemWgt::GetText() const
{
    return mLabel->text();
}

void CustomItemWgt::SetBtnIcon(const QIcon &icon)
{
    mDeleteButton->setIcon(icon);
}

void CustomItemWgt::mousePressEvent(QMouseEvent *event)
{
    if (::Qt::LeftButton == event->button())
    {
        mPressed = true;
    }
}

void CustomItemWgt::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (mPressed)
    {
        emit sigShowItem(mLabel->text());
        mPressed = false;
    }
}

void CustomItemWgt::slotRemoveItem()
{
    emit sigRemoveItem(mLabel->text());
}

void CustomItemWgt::InitWidget()
{
    /* 水平布局 */
    QHBoxLayout *hb = new QHBoxLayout(this);
    hb->addWidget(mLabel);
    hb->addStretch();
    hb->addWidget(mDeleteButton);
    hb->setContentsMargins(5,5,5,5);
    hb->setSpacing(5);
    setLayout(hb);
    mDeleteButton->setFlat(true);
    mDeleteButton->setIconSize(QSize(20,20));
    connect(mDeleteButton, &QPushButton::clicked, this, &CustomItemWgt::slotRemoveItem);
}
