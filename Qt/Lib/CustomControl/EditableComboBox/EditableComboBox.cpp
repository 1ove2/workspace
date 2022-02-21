#include "EditableComboBox.h"
#include "CustomItemWgt.h"
EditableComboBox::EditableComboBox(QWidget *parent)
    :QComboBox(parent),
     mListWidget(new QListWidget)
{
    setEditable(true);                                                               /* 设置为可编辑 实现lineEdit效果 */
    setInsertPolicy(::QComboBox::NoInsert);                                          /* 设置为输完后值不能插入下拉框 */
    setModel(mListWidget->model());
    setView(mListWidget);
    setStyleSheet("QComboBox QAbstractItemView::item{height:24px;color:white;}");
}

EditableComboBox::~EditableComboBox()
{
    delete mListWidget;
    mListWidget = nullptr;
}

void EditableComboBox::AddItem(const QString &text, const QIcon &icon)
{
    /* 有重复内容就不新增 */
    int index = GetItemIndex(text);
    if (-1 != index)
    {
        return;
    }
    CustomItemWgt *itemWgt = new CustomItemWgt(mListWidget);
    itemWgt->SetText(text);
    itemWgt->SetBtnIcon(icon);
    connect(itemWgt, &CustomItemWgt::sigShowItem, this, &EditableComboBox::ShowItem);
    connect(itemWgt, &CustomItemWgt::sigRemoveItem, this, &EditableComboBox::RemoveItem);
    QListWidgetItem *item = new QListWidgetItem(mListWidget);
    mListWidget->setItemWidget(item, itemWgt);
    setEditText(text);
}

void EditableComboBox::AddItems(const QStringList &texts, const QIcon &icon)
{
    for (auto &text : texts)
    {
        AddItem(text, icon);
    }
}

QString EditableComboBox::GetItemText(int index) const
{
    QString text("");
    int count = mListWidget->count();
    if (index < 0 || index > count)
    {
        return text;
    }
    QListWidgetItem *item = mListWidget->item(index);
    CustomItemWgt *itemWgt = (CustomItemWgt *)(mListWidget->itemWidget(item));
    text = itemWgt->GetText();
    return text;
}

QStringList EditableComboBox::GetItemTexts() const
{
    QStringList texts;
    int count = mListWidget->count();
    for (int i = 0; i < count; ++i)
    {
        QString text = GetItemText(i);
        texts.append(text);
    }
    return texts;
}

void EditableComboBox::ShowItem(const QString &text)
{
    setEditText(text);
    hidePopup();                                                                     /* 隐藏下拉框 */
}

void EditableComboBox::RemoveItem(const QString &text)
{
    hidePopup();
    int index = GetItemIndex(text);
    if (-1 != index)
    {
        QListWidgetItem *item = mListWidget->takeItem(index);
        mListWidget->removeItemWidget(item);
        delete item;
    }
    int count = mListWidget->count();
    QString str = GetItemText(count - 1);
    setEditText(str);
}

int EditableComboBox::GetItemIndex(const QString &text) const
{
    /* 遍历框 判断是否存在重复内容 */
    int count = mListWidget->count();
    for(int i = 0; i < count; ++i)
    {
        QListWidgetItem *item = mListWidget->item(i);
        CustomItemWgt *itemWgt = (CustomItemWgt *)(mListWidget->itemWidget(item));
        QString currText = itemWgt->GetText();
        if(currText == text)
        {
            return i;
        }
    }
    return -1;
}
