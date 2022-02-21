/******************************************************************************
 *
 * 文件名  ： AnlgBrdCfg.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20190911
 * 版本号  ： v1.0 20190911
 * 文件描述： 模拟板配置
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _ANLG_BRD_CFG_H_
#define _ANLG_BRD_CFG_H_

/************************************ 头文件 ***********************************/
#ifndef _QT_PLX00_
#include "BinAppSx00AnlgEnum.h"
#endif
#include <QHash>
#include <QString>
#include <QVector>
#include <QDialog>
/*********************************** 命名空间 **********************************/
namespace Pss {
namespace Qt {

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 结构定义 **********************************/

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
/* 单例实现控件配置,设备的Cfg类填数据,Ui使用数据 */
class AnlgBrdCfg
{
    public:
        typedef enum _ProductTypeTag_
        {
            ProductTypeTBD              = 0x00,                                                         /* 未定义类型 */

            ProductTypeSx00             = 0x01,                                                         /* Sx00 */
            ProductTypePLx00            = 0x02,                                                         /* PLx00 */
            ProductTypeCx00             = 0x03,                                                         /* Cx00 */
        } ProductType;
    
        typedef enum _SX00TypeTag_
        {
            Sx00TypeTBD                 = 0x00,                                                         /* 未定义类型 */
            Sx00TypeS100                = 0x01,                                                         /* S100 */
            Sx00TypeS200                = 0x02,                                                         /* S200 */
            Sx00TypeS300                = 0x03,                                                         /* S300 */
        } Sx00Type;
    
        static AnlgBrdCfg *Get(void);                                                                   /* 获取实例 */
#ifndef _PSS_QT_NO_GUI_
        /* 样式表 */
        void SetQss(QDialog *dialog) const;                                                             /* 设置对话框样式表 */
#endif
        /* 设备版本 */
        void SetProductAndType(ProductType product, Sx00Type type);                                     /* 设置机器 配置 */
        void GetProductAndType(ProductType &product, Sx00Type &type) const;                             /* 获取机器 配置 */
#ifndef _PSS_QT_PLX00_
        PssBinAppSx00AnlgEnumRangeIndex RangeNameToRangeIndex(const QString &rangeStr) const;           /* 获取量程对应的协议索引 */
#endif
        QString FindRangeStrFromTable(bool isV, double val) const;                                      /* 查找量程字符串 */
#ifndef _PSS_QT_PLX00_
        QString RangeIndexToRangeName(PssBinAppSx00AnlgEnumRangeIndex index) const;                     /* 从量程索引求对应字符 */
#endif
        QStringList GetAllVRange(void) const;                                                           /* 获取所有电压挡位 */
        QStringList GetAllIRange(void) const;                                                           /* 获取所有电流挡位 */
        QString GetUnit(const QString &rangeStr) const;                                                 /* 获取量程单位 */
        int GetShowPrecison(const QString &rangeStr) const;                                             /* 获取显示精度  */
        int GetShowNumCount(const QString &rangeStr) const;                                             /* 获取显示数字字符数 */
        int GetTipsPrecison(const QString &rangeStr) const;                                             /* 获取提示精度 */
        int GetTipsNumCount(const QString &rangeStr) const;                                             /* 获取提示数字字符数 */
        double GetMin(const QString &rangeStr) const;                                                   /* 获取量程最小值 */
        double GetRangeValue(const QString &rangeStr) const;                                            /* 获取量程值 */
        double GetMax(const QString &rangeStr) const;                                                   /* 获取量程最大值 */
        double GetZero(const QString &rangeStr) const;                                                  /* 获取量程零点 */
        double GetVMaxValue(void) const;                                                                /* 获取设备电压最大值 */
        double GetIMaxValue(void) const;                                                                /* 获取设备电流最大值 */
    protected:
        AnlgBrdCfg(void);                                                                               /* 避免外部调用构造函数 */

    private:
        void SetZeroVaule(const QString &rangeStr, double val);                                         /* 设置最小零点值 */
        QStringList GetRange(const QString &unitStr) const;                                             /* 获取量程(V/A) */
        QStringList SortRange(const QStringList &rangeStrList) const;                                   /* 量程排序(小=>大) */
    
        ProductType mProduct;                                                                           /* 产品 */
        Sx00Type mType;                                                                                 /* 版本 */
        static AnlgBrdCfg *mSelf;                                                                       /* 实例 */
    
        QStringList mPlcList;                                                                           /* PLC表 */
        int mPlcIndex;                                                                                  /* 当前有效的PLC索引 */
    
        QVector<int> mBaudList;                                                                         /* 波特率表 */
        int mBaudIndex;                                                                                 /* 波特率索引 */
};

/*********************************** 工具函数 **********************************/

/*********************************** 工具变量 **********************************/

}
}

#endif // _PSS_ANLG_BRD_CFG_H_
