#ifndef _PSS_QT_CUSTOMLOGTABLE_H_
#define _PSS_QT_CUSTOMLOGTABLE_H_

#include <QTableWidget>
#include <QDateTime>
#include "ParaDef.h"
/* 表格格式如下:
 *               通道号           通道号           ...
 * 点数 时间  电压  电流  电阻  电压  电流  电阻     ...
 */
 enum VariableType {
    Seq  = 0,
    //Date = 1,
    Volt = 1,
    Curr = 2,
    Ohm  = 3,
 };

class CustomLogTable : public QTableWidget
{
    Q_OBJECT
public:
    CustomLogTable(QWidget *parent = 0);
    ~CustomLogTable();
    void AddChannel(const ChanInfo &info); /* 新增设备通道 */
    void AddData(const QDateTime &time, double valV, double valI, const ChanInfo &info);
    void DeleteChan(const ChanInfo &info);
    void EnableChan(const ChanInfo &info, bool isEnable);
    void ExportData();
    void Clear();
    QVector<quint64> GetVecCount();
private:
    QMap<ChanInfo, int> mTableCfg;
    int mChanCount;
    QVector<quint64> mVecCount; /* 每个通道读到的数据不一定一样 采用一个数据记录各个通道读到的点数 */

    QString Value2Unit(double val, int prec = 6);
    double Unit2Value(const QString &str);
};

#endif // _PSS_QT_CUSTOMLOGTABLE_H_