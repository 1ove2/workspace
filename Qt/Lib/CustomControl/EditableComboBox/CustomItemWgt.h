/******************************************************************************
 *
 * 文件名  ： CustomItemWidget
 * 负责人  ：
 * 创建日期： 20201201
 * 版本号  ： v1.0 20201201
 * 文件描述： 自定义窗口 作为PssEditableComboBox的下拉框
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef CUSTOMITEMWGT_H
#define CUSTOMITEMWGT_H
/************************************ 头文件 ***********************************/
#include <QWidget>
#include <QLabel>
#include <QPushButton>

/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/


/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/

class CustomItemWgt : public QWidget
{
    Q_OBJECT
public:
    explicit CustomItemWgt(QWidget *parent = 0);
    ~CustomItemWgt();
    void SetText(const QString &text);                                                                  /* 设置label值 */
    QString GetText() const;                                                                            /* 得到label值 */
    void SetBtnIcon(const QIcon &icon);
signals:
    void sigRemoveItem(const QString &text);                                                            /* 删除item的信号 */
    void sigShowItem(const QString &text);                                                              /* 显示label值的信号*/

protected:
    void mousePressEvent(QMouseEvent *event);                                                           /* 重写鼠标按下事件 记录按下状态 */
    void mouseReleaseEvent(QMouseEvent *event);                                                         /* 松开后显示label的值 */

private slots:
    void slotRemoveItem();                                                                              /* 发送移除item的信号 */

private:
    void InitWidget();

    QLabel *mLabel;                                                                                     /* 显示 */
    QPushButton *mDeleteButton;                                                                         /* 删除按钮 */
    bool mPressed;                                                                                      /* 记录鼠标是否被按下 */
};
/*********************************** 工具函数 **********************************/

#endif // CUSTOMITEMWGT_H






