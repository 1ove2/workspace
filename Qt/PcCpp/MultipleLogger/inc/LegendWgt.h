/******************************************************************************
 *
 * 文件名  ： LegendWgt.h
 * 负责人  ： zsf(2672921140@qq.com)
 * 创建日期： 20210601
 * 版本号  ： v1.0 20210601
 * 文件描述： 标签类
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_QT_LEGENDWGT_H_
#define _PSS_QT_LEGENDWGT_H_

/************************************ 头文件 ***********************************/
#include <QWidget>
#include "ParaDef.h"
/*********************************** 命名空间 **********************************/
namespace Ui {
class LegendWgt;
}

namespace Pss {
namespace Qt {

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class LegendWgt : public QWidget
{
    Q_OBJECT
public:
    LegendWgt(const ChanInfo &info,QWidget *parent = 0);
    ~LegendWgt();
    void SetChanInfo(const ChanInfo &info);
    ChanInfo GetChanInfo() const;

    void SetIconColor(const QColor &color);             /* 设置图标颜色 */
    QColor GetIconColor() const;                        /* 获取图标颜色 */
    
    void SetChanName(const QString &name);              /* 设置曲线名 */
    QString GetChanName() const;
signals:
    void sigChanChecked(const ChanInfo &info, bool isChecked);
    void sigColorChanged(const ChanInfo &info, const QColor &color);
private slots:
    void ChanStatusChanged(bool isChecked);
private:
    bool eventFilter(QObject *obj, QEvent *e);
private:
    ChanInfo mInfo;
    QColor mColor;
    Ui::LegendWgt *mUi;
};
/*********************************** 工具函数 **********************************/
}
}
#endif // _PSS_QT_LEGENDWGT_H_