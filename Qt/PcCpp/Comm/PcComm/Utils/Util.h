#ifndef _PSS_UTIL_H_
#define _PSS_UTIL_H_

#include <QHostAddress>
#include <qmath.h>

class PssUtil
{
public:
    static void LoadQss(const QString &dirPath);
    static void SetIniValue(const QString &fileName, const QString &groupName, const QStringList &keys, const QStringList &values);
    static QStringList GetIniValue(const QString &fileName, const QString &groupName);
    static void RemoveIniValue(const QString &fileName, const QString &groupName, const QString &value);
#ifdef _PSS_GPIB_SUPPORT_
    static QStringList GetEnabledGpib();                                                                /* 获取可用GPIB */
#endif
    static bool GetSetValueVec(double startVal, double stopVal, int point, QVector<double> &vec); /* 根据起点、终点、点数算出数组 */
    static bool IsFileExist(const QString &fileName);  /* 判断文件是否存在 */
    static QString HostAddrToIPV4(const QHostAddress &ip);                                                                    /* 将IP转换为QString类型 */                                                                  /* 将整形数转为ip */
    static bool GpibAddrToInt(const QString &gpibAddr, int &cardAddr, int &devAddr);

    static bool SyncSymbol(double &startVal, double &stopVal);                                                     /* 手动限值参数为+0或-0 */
    static double GetResiVal(double v, double i){
        return qAbs(v / i);
    }
    static double GetPowerVal(double v, double i){
        return v * i;
    }
    static bool ConverStepToPoint(double start, double stop, double &step, int &point); /* 由步进算出点数 */
    static bool ConverPointToStep(bool isV, double start, double stop, double &step, QString &unit, int &point); /* 由点数算出步进 */
    
    static QString GetFormatTime(ulong ms);
    
    static uint32_t TimeNsWithUnit(uint32_t time, const QString &unit);
private:
    static uint32_t GetUnitFactorNs(const QString &unit);

    static const int MAX_SCAN_POINT = 2000;
    static const int MIN_SCAN_POINT = 2;
};



#endif // _PSS_UTIL_H_
