/******************************************************************************
 *
 * 文件名  ： AnlgBrdCfg.cpp
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20190911
 * 版本号  ： v1.0 20190911
 * 文件描述： 模拟板配置
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "ITool.h"
#include "PssQtHal.h"
#include "AnlgBrdCfg.h"
#include <QMap>
#include <QFile>
#include <math.h>

using namespace Pss;
using namespace Pss::Qt;

/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
/* 注释定位 */

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
/* 仅本CPP实用 */
typedef struct _RangeItemTag_
{
    QString         unitStr;                                                                    /* 量程 单位 */
    int             showPrecision;                                                              /* 显示精度(小数点后位数) */
    int             showNumCount;                                                               /* 显示数字字符数(小数点+数字) */
    int             tipsPrecision;                                                              /* 提示精度 */
    int             tipsNumCount;                                                               /* 提示数字字符数 */
    double          zeroVal;                                                                    /* 量程零点值 */
    double          minVal;                                                                     /* 量程最小值 */
    double          maxVal;                                                                     /* 量程最大值 */
#ifndef _PSS_QT_PLX00_
    PssBinAppSx00AnlgEnumRangeIndex  protRangeIndex;                                            /* 量程的协议索引 */
#endif
} RangeItem;

typedef struct _RangeHashItemTag_
{
    QString         key;                                                                        /* 量程名 */
    RangeItem       item;                                                                       /* 值 */
} RangeHashItemTag;

RangeHashItemTag sRangeHashTable [] =
{
#ifndef _PSS_QT_PLX00_
    /* 量程: 单位,显示精度,显示数字个数,提示精度,提示数字个数,零点(0.001),最小值，最大值,量程协议索引,十万一(分辨率),格式例子 */
    {"100nA", {"nA", 3, 7, 1, 5, 1e-10, -1.01e-7, +1.01e-7, PssBinAppSx00AnlgEnumRangeIndexI100nA}},    //0.001nA,  +000.000nA
    {"1uA",   {"uA", 5, 7, 3, 5, 1e-9,  -1.01e-6, +1.01e-6, PssBinAppSx00AnlgEnumRangeIndexI1uA}},      //0.00001uA,+0.00000uA
    {"10uA",  {"uA", 4, 7, 2, 5, 1e-8,  -1.01e-5, +1.01e-5, PssBinAppSx00AnlgEnumRangeIndexI10uA}},     //0.0001uA, +00.0000uA
    {"100uA", {"uA", 3, 7, 1, 5, 1e-7,  -1.01e-4, +1.01e-4, PssBinAppSx00AnlgEnumRangeIndexI100uA}},    //0.001uA,  +000.000uA
    {"1mA",   {"mA", 5, 7, 3, 5, 1e-6,  -1.01e-3, +1.01e-3, PssBinAppSx00AnlgEnumRangeIndexI1mA}},      //0.00001mA,+0.00000mA
    {"10mA",  {"mA", 4, 7, 2, 5, 1e-5,  -1.01e-2, +1.01e-2, PssBinAppSx00AnlgEnumRangeIndexI10mA}},     //0.0001mA, +00.0000mA
    {"100mA", {"mA", 3, 7, 1, 5, 1e-4,  -1.01e-1, +1.01e-1, PssBinAppSx00AnlgEnumRangeIndexI100mA}},    //0.001mA,  +000.000mA
    {"1A",    {" A", 5, 7, 3, 5, 1e-3,  -1.01e-0, +1.01e-0, PssBinAppSx00AnlgEnumRangeIndexI1A}},       //0.00001A, +0.00000 A

    /*{"30mV",  {"mV", 4, 7, 2, 5, 3e-5,  -3.03e-2, +3.03e-2, AnlgRangeIndexV30mV}},   //0.0003mV, +00.0000mV */
    {"300mV", {"mV", 3, 7, 1, 5, 3e-4,  -3.03e-1, +3.03e-1, PssBinAppSx00AnlgEnumRangeIndexV300mV}},    //0.003mV,  +000.000mV
    {"3V",    {" V", 5, 7, 3, 5, 3e-3,  -3.03e-0, +3.03e-0, PssBinAppSx00AnlgEnumRangeIndexV3V}},       //0.00003V, +0.00000 V
    {"30V",   {" V", 4, 7, 2, 5, 3e-2,  -3.03e+1, +3.03e+1, PssBinAppSx00AnlgEnumRangeIndexV30V}},      //0.0003V,  +00.0000 V
    
    {"100V",  {" V", 3, 7, 1, 5, 3e-1,  -1.03e+2, +1.03e+2, PssBinAppSx00AnlgEnumRangeIndexV300V}},      //0.003V,   +000.000 V
    {"300V",  {" V", 3, 7, 1, 5, 3e-1,  -3.03e+2, +3.03e+2, PssBinAppSx00AnlgEnumRangeIndexV300V}},      //0.003V,   +000.000 V
    #endif
};

static QHash<QString, RangeItem> sRangeHash;

/********************************** 变量实现区 *********************************/
AnlgBrdCfg *AnlgBrdCfg::mSelf = new AnlgBrdCfg();

/********************************** 函数实现区 *********************************/
AnlgBrdCfg *AnlgBrdCfg::Get(void)
{
    return mSelf;
}

AnlgBrdCfg::AnlgBrdCfg(void)
{
    sRangeHash.clear();
    mPlcList.clear();
    mBaudList.clear();
}

void AnlgBrdCfg::SetZeroVaule(const QString &rangeStr, double val)
{
#if 1
    PSS_UNUSED(rangeStr);
    PSS_UNUSED(val);
#else
    sRangeHash[rangeStr].zeroVal = val;
#endif
    return ;
}

#ifndef _PSS_QT_NO_GUI_
/* 设置对话框样式表 */
void AnlgBrdCfg::SetQss(QDialog *dialog) const
{
    QFile file(":/qss/qrc/arm.qss");
    file.open(QFile::ReadOnly);
    QString stylesheet = file.readAll();
    dialog->setStyleSheet(stylesheet);
    file.close();
}
#endif

void AnlgBrdCfg::SetProductAndType(ProductType product, Sx00Type type)
{
    mProduct = product;
    mType = type;
#ifndef _PSS_QT_PLX00_
    /*
     * S100: 30V
     * S200: 100V
     * S300: 300V
     * */
    int i = 0;
    int size = sizeof(sRangeHashTable) / sizeof(sRangeHashTable[0]);
    size -= 2;
    for(i = 0; i < size; i++)
    {
        sRangeHash.insert(sRangeHashTable[i].key, sRangeHashTable[i].item);
        /* 设置所有档位零点值 */
        SetZeroVaule(sRangeHashTable[i].key, 0);
    }

    /*
     * S200: +100V
     * S300: +100V +300V
     * */
    if(Sx00TypeS200 == type)
    {
        sRangeHash.insert(sRangeHashTable[i].key, sRangeHashTable[i].item);
        /* 设置所有档位零点值 */
        SetZeroVaule(sRangeHashTable[i].key, 0);
        i++;
    }
    if(Sx00TypeS300 == type)
    {
        i++;
        sRangeHash.insert(sRangeHashTable[i].key, sRangeHashTable[i].item);
        /* 设置所有档位零点值 */
        SetZeroVaule(sRangeHashTable[i].key, 0);
    }
#endif
}

void AnlgBrdCfg::GetProductAndType(ProductType &product, Sx00Type &type) const
{
    product = mProduct;
    type = mType;
}
#ifndef _PSS_QT_PLX00_
PssBinAppSx00AnlgEnumRangeIndex AnlgBrdCfg::RangeNameToRangeIndex(const QString &rangeStr) const
{
    return sRangeHash[rangeStr].protRangeIndex;
}
#endif
QString AnlgBrdCfg::FindRangeStrFromTable(bool isV, double val) const
{
    /* 获取已排序的量程列表 */
    QStringList allRange;
    if(isV)
    {
        allRange = GetAllVRange();
    }
    else
    {
        allRange = GetAllIRange();
    }

    /* 遍历构造 */
    QString rangeStr;
    int iMax = allRange.count();
    for(int i = 0; i < iMax; i++)
    {
        rangeStr = allRange[i];
        double maxVal = GetMax(rangeStr);
    
        if(fabs(val) <= maxVal)
        {
            break;
        }
    }
    
    return rangeStr;
}
#ifndef _PSS_QT_PLX00_
QString AnlgBrdCfg::RangeIndexToRangeName(PssBinAppSx00AnlgEnumRangeIndex index) const
{
    QStringList allKeys = sRangeHash.keys();
    int i = 0;
    int iMax = allKeys.count();

    for(i = 0; i < iMax; i++)
    {
        QString key = allKeys[i];
        if(index == RangeNameToRangeIndex(key))
        {
            return key;
        }
    }
    
    return QString("NAN");
}
#endif
QStringList AnlgBrdCfg::GetAllVRange(void) const
{
    QStringList allRange = GetRange("V");
    return SortRange(allRange);
}

QStringList AnlgBrdCfg::GetAllIRange(void) const
{
    QStringList allRange = GetRange("A");
    return SortRange(allRange);
}

QString AnlgBrdCfg::GetUnit(const QString &rangeStr) const
{
    return sRangeHash[rangeStr].unitStr;
}

int AnlgBrdCfg::GetShowPrecison(const QString &rangeStr) const
{
    return sRangeHash[rangeStr].showPrecision;
}

int AnlgBrdCfg::GetShowNumCount(const QString &rangeStr) const
{
    return sRangeHash[rangeStr].showNumCount;
}

int AnlgBrdCfg::GetTipsPrecison(const QString &rangeStr) const
{
    return sRangeHash[rangeStr].tipsPrecision;
}

int AnlgBrdCfg::GetTipsNumCount(const QString &rangeStr) const
{
    return sRangeHash[rangeStr].tipsNumCount;
}

double AnlgBrdCfg::GetMin(const QString &rangeStr) const
{
    return sRangeHash[rangeStr].minVal;
}

double AnlgBrdCfg::GetRangeValue(const QString &rangeStr) const
{
    return UnitVal2StdVal(rangeStr);
}

double AnlgBrdCfg::GetMax(const QString &rangeStr) const
{
    return sRangeHash[rangeStr].maxVal;
}

double AnlgBrdCfg::GetZero(const QString &rangeStr) const
{
    return sRangeHash[rangeStr].zeroVal;
}

double AnlgBrdCfg::GetVMaxValue(void) const
{
    QStringList list = GetAllVRange();
    //qDebug() << "all v range list = " << list;
    return GetMax(list.last());
}

double AnlgBrdCfg::GetIMaxValue(void) const
{
    QStringList list = GetAllIRange();
    //qDebug() << "all i range list = " << list;
    return GetMax(list.last());
}

QStringList AnlgBrdCfg::GetRange(const QString &unitStr) const
{
    QStringList list = sRangeHash.keys();
    return list.filter(unitStr);
}

QStringList AnlgBrdCfg::SortRange(const QStringList &rangeStrList) const
{
    /* 按照挡位值排序 */
    /* 构造{量程值:量程字符串} 的QMap */
    QMap<double, QString> rangeVal2Str;
    /* 遍历构造 */
    int iMax = rangeStrList.count();
    for(int i = 0; i < iMax; i++)
    {
        QString rangeStr = rangeStrList[i];
        double val = GetMax(rangeStr);
        rangeVal2Str[val] = rangeStr;
    }

    /* 返回值 列表(已经 小=>大 排序) */
    return rangeVal2Str.values();
}