#include "CustomLogTable.h"
#include "xlsxdocument.h"
#include "xlsxcellrange.h"
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

const int PARA_COUNT = 3;
CustomLogTable::CustomLogTable(QWidget *parent)
    :QTableWidget(parent),
      mChanCount(0)
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    /* 隐藏默认的表头 */
    horizontalHeader()->setVisible(false);
    verticalHeader()->setVisible(false);

    setRowCount(50);
    setColumnCount(2);
    /* 点数列 */
    setItem(1,  0, new QTableWidgetItem("点数"));
    setItem(1,  1, new QTableWidgetItem("时间"));
    item(1,0)->setTextAlignment(Qt::AlignHCenter);
    item(1,1)->setTextAlignment(Qt::AlignHCenter);
    setColumnWidth(0, 50);
}

CustomLogTable::~CustomLogTable()
{

}

void CustomLogTable::AddChannel(const ChanInfo &info)
{
    mTableCfg.insert(info, mChanCount);
#if 1
    mChanCount++;
    mVecCount.append(0);

    setRowCount(2);
    setColumnCount(PARA_COUNT * mChanCount + 2) ;
    /* 合并通道号的单元格 */
    setSpan(0, PARA_COUNT * (mChanCount-1) + 2, 1, PARA_COUNT);
    /* 设置单元格 */
    setItem(0, PARA_COUNT * (mChanCount-1) + 2, new QTableWidgetItem(QString("通道%1").arg(mChanCount)));
    //setItem(1, PARA_COUNT * (mChanCount-1) + Date, new QTableWidgetItem("时间"));
    setItem(1, PARA_COUNT * (mChanCount-1) + Volt+1, new QTableWidgetItem("电压"));
    setItem(1, PARA_COUNT * (mChanCount-1) + Curr+1, new QTableWidgetItem("电流"));
    setItem(1, PARA_COUNT * (mChanCount-1) + Ohm+1, new QTableWidgetItem("电阻"));
    /* 设置居中显示 */
    item(0, PARA_COUNT * (mChanCount-1) + 2)->setTextAlignment(Qt::AlignHCenter);
    //item(1, PARA_COUNT * (mChanCount-1) + Date)->setTextAlignment(Qt::AlignHCenter);
    item(1, PARA_COUNT * (mChanCount-1) + Volt+1)->setTextAlignment(Qt::AlignHCenter);
    item(1, PARA_COUNT * (mChanCount-1) + Curr+1)->setTextAlignment(Qt::AlignHCenter);
    item(1, PARA_COUNT * (mChanCount-1) + Ohm+1)->setTextAlignment(Qt::AlignHCenter);
    /* 设置时间列宽度 */
    setColumnWidth(1, 180);
    /* Fixed 没设为ResizeToContents的原因是每次加入新数据时，QTableWidget要重新计算单元格宽度 会增加卡顿 */
    horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    //horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
}

void CustomLogTable::AddData(const QDateTime &time,  double valV, double valI, const ChanInfo &info)
{
    QString dateStr = time.toString("yyyy-MM-ddThh:mm:ss.z");
    int chanID = mTableCfg[info];
    mVecCount[chanID]++;
    /* 找到所有通道中最多的数据 */
    int max = *std::max_element(mVecCount.begin(), mVecCount.end());
    setRowCount(max + 2);
    setItem(max+1, 0, new QTableWidgetItem(QString::number(max)));
    setItem(max+1, 1, new QTableWidgetItem(dateStr));
    setItem(mVecCount.value(chanID)+1, chanID * PARA_COUNT + Volt+1, new QTableWidgetItem(QString(Value2Unit(valV) + "V")));
    setItem(mVecCount.value(chanID)+1, chanID * PARA_COUNT + Curr+1, new QTableWidgetItem(QString(Value2Unit(valI) + "A")));
    setItem(mVecCount.value(chanID)+1, chanID * PARA_COUNT + Ohm+1, new QTableWidgetItem(QString(Value2Unit(qAbs(valV / valI), 3) + "Ω")));
}

void CustomLogTable::DeleteChan(const ChanInfo &info)
{
    int cnt = mTableCfg[info];
    //removeColumn(PARA_COUNT * (cnt-1) + 2);
    removeColumn(PARA_COUNT * (mChanCount-1) + Volt+1);
    removeColumn(PARA_COUNT * (mChanCount-1) + Curr+1);
    removeColumn(PARA_COUNT * (mChanCount-1) + Ohm +1);

    mTableCfg.remove(info);
    mChanCount--;
}

void CustomLogTable::EnableChan(const ChanInfo &info, bool isEnable)
{
    int cnt = mTableCfg[info];
    //setColumnHidden(PARA_COUNT * (cnt-1) + 2, isEnable);
    setColumnHidden(PARA_COUNT * (mChanCount-1) + Volt+1, isEnable);
    setColumnHidden(PARA_COUNT * (mChanCount-1) + Curr+1, isEnable);
    setColumnHidden(PARA_COUNT * (mChanCount-1) + Ohm+1, isEnable);

//    removeColumn(PARA_COUNT * (cnt-1) + 2);
//    removeColumn(PARA_COUNT * (mChanCount-1) + Volt+1);
//    removeColumn(PARA_COUNT * (mChanCount-1) + Curr+1);
//    removeColumn(PARA_COUNT * (mChanCount-1) + Ohm+1);
}

void CustomLogTable::ExportData()
{
    QString path = QFileDialog::getSaveFileName(this, "保存数据", "data", "xlsx (*.xlsx)");
    if (path.isEmpty())
    {
        return;
    }
    QXlsx::Document xlsx;
    QXlsx::Format formatHeader;
    formatHeader.setHorizontalAlignment(QXlsx::Format::AlignHCenter);

    //写入表头
    for (int i = 0; i < mChanCount; ++i)
    {
        xlsx.mergeCells( QXlsx::CellRange(1,2 + PARA_COUNT * i, 1, 1 + PARA_COUNT * (1 + i)) ) ;
        xlsx.write(1, 2 + PARA_COUNT * i, QString("通道%1").arg(i+1), formatHeader);
    }
    xlsx.write(2, 1, QString("时间"));
    //写入单位
    for (int i = 0; i < mVecCount.count(); ++i)
    {
        //xlsx.write(2, 4 * i + 1, item(1, PARA_COUNT*i + Date)->text());
        xlsx.write(2, 3 * i + 2, item(1, PARA_COUNT*i + Volt + 1)->text());
        xlsx.write(2, 3 * i + 3, item(1, PARA_COUNT*i + Curr + 1)->text());
        xlsx.write(2, 3 * i + 4, item(1, PARA_COUNT*i + Ohm +  1)->text());
    }
    //时间
    for (quint64 i = 0; i < mVecCount.value(0); ++i)
    {
        xlsx.write(i + 3, 1, item(i+2, 1)->text());        //时间
    }
    
    //写入数据
    //遍历所有通道数
    for (int i = 0; i < mVecCount.count(); ++i)
    {
        for (quint64 j = 0; j < mVecCount.value(i); ++j)
        {
            xlsx.write(j + 3, 3 * i + 2, Unit2Value(item(j+2, 3*i+2)->text()));        //i通道的电压
            xlsx.write(j + 3, 3 * i + 3, Unit2Value(item(j+2, 3*i+3)->text()));        //i通道的电流
            xlsx.write(j + 3, 3 * i + 4, Unit2Value(item(j+2, 3*i+4)->text()));        //i通道的电阻
        }
    }
    bool isSucceed = xlsx.saveAs(path);
    if (!isSucceed)
    {
        QMessageBox::warning(this, "提示", "保存失败!");
    }
}

void CustomLogTable::Clear()
{
    setRowCount(2);
    for (int i = 0; i < mVecCount.count(); ++i)
    {
        mVecCount[i] = 0;
    }
}

QVector<quint64> CustomLogTable::GetVecCount()
{
    return mVecCount;
}

double CustomLogTable::Unit2Value(const QString &str)
{
    double factor = 1;
    QString unit;
    /* 去掉末尾单位 */
    unit = str.mid(0, str.length()-1);
    if (unit.endsWith("M"))
    {
        factor = 1e6;
    }
    else if (unit.endsWith("K"))
    {
        factor = 1e3;
    }
    else if (unit.endsWith("m"))
    {
        factor = 1e-3;
    }
    else if (unit.endsWith("u"))
    {
        factor = 1e-6;
    }
    else if (unit.endsWith("n"))
    {
        factor = 1e-9;
    }
    double val = unit.mid(0, unit.length()-1).toDouble();
    return val * factor;
}

QString CustomLogTable::Value2Unit(double val, int prec)
{
    QString str;
    double cVal;
    if (qAbs(val / 1e6) >= 1)
    {
        cVal = val / 1e6;
        str = QString::number(cVal,'f', prec);
        while(str.endsWith("0"))
        {
            str.remove(str.length()-1, 1);
        }
        str.append("M");
    }
    else if (qAbs(val / 1e3) >= 1)
    {
        cVal = val / 1e3;
        str = QString::number(cVal, 'f', prec);
        while(str.endsWith("0"))
        {
            str.remove(str.length()-1, 1);
        }
        str.append("K");
    }
    else if (qAbs(val) >= 1)
    {
        str = QString::number(val, 'f', prec);
        while(str.endsWith("0"))
        {
            str.remove(str.length()-1, 1);
        }
    }
    else if (qAbs(val / 1e-3) >= 1)
    {
        cVal = val / 1e-3;
        str = QString::number(cVal, 'f', prec);
        while(str.endsWith("0"))
        {
            str.remove(str.length()-1, 1);
        }
        str.append("m");
    }
    else if (qAbs(val / 1e-6) >= 1)
    {
        cVal = val / 1e-6;
        str = QString::number(cVal, 'f', prec);
        while(str.endsWith("0"))
        {
            str.remove(str.length()-1, 1);
        }
        str.append("u");
    }
    else
    {
        cVal = val / 1e-9;
        str = QString::number(cVal, 'f', prec);;
        while(str.endsWith("0"))
        {
            str.remove(str.length()-1, 1);
        }
        str.append("n");
    }
    return str;
}