/******************************************************************************
 *
 * 文件名  ： ConnectDlg.h
 * 负责人  ： zsf(2672921140@qq.com)
 * 创建日期： 20201201
 * 版本号  ： v1.0 20201201
 * 文件描述： 连接窗口
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_ConnectDlg_H_
#define _PSS_ConnectDlg_H_

/************************************ 头文件 ***********************************/
#include <QDialog>
#include <QListWidget>
namespace Ui {
class ConnectDlg;
}

/*********************************** 命名空间 **********************************/
namespace Pss {
namespace Qt {

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class ConnectDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDlg(QWidget *parent = 0);
    ~ConnectDlg();
    QStringList GetIPConfig() const;
    void SetConnectBtnEnable(bool isEnabled);                                                           /* 使能连接按钮 */
    void AddItem();                                                                                     /* 将IP添加到IP下拉框 */

signals:
    void sigConnectToGpibReq(const QString &address);                                                   /* GPIB请求 */
    void sigConnectToComReq(const QString &com, int baud);                                              /* 串口请求 */
    void sigConnectToNetReq(const QString &ip);                                                         /* 网口请求 */

private slots:
    void slotConnectTo();                                                                               /* 判断连接的类型 */
private:
    void InitConnections();                                                                             /* 初始化信号槽 */
    void InstallIPLineEdit();                                                                           /* 安装自定义IP下拉框 */
    Ui::ConnectDlg *mUi;
};
/*********************************** 工具函数 **********************************/

}
}

#endif // _PSS_ConnectDlg_H_