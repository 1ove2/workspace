#ifndef EDITABLECOMBOBOX_H
#define EDITABLECOMBOBOX_H
/************************************ 头文件 ***********************************/
#include <QComboBox>
#include <QListWidget>

/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/                                                                                                        /* 注释定位 */

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/

class EditableComboBox : public QComboBox
{
    Q_OBJECT
public:
    EditableComboBox(QWidget *parent=0);
    ~EditableComboBox();
    void AddItem(const QString &text, const QIcon &icon);
    void AddItems(const QStringList &texts, const QIcon &icon);
    QString GetItemText(int index) const;
    QStringList GetItemTexts() const;

private slots:
    void ShowItem(const QString &text);
    void RemoveItem(const QString &text);

private:
    int GetItemIndex(const QString &text) const;
    QListWidget *mListWidget;
};

#endif // EDITABLECOMBOBOX_H


