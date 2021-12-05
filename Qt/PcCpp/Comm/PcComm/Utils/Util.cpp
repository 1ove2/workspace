#include "PssUtil.h"
#include "PssConfig.h"
#ifdef _PSS_GPIB_SUPPORT_
#include "visa.h"
#endif
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QApplication>
#include <QDebug>


using namespace Pss::Qt;
void PssUtil::LoadQss(const QString &dirPath)
{
    /* 单例读取配置文件 */
    QStringList qssFileNames = PssSingleton<PssConfig>::getInstance().GetQssFiles(dirPath);
    /* 需要把所有文件中的 QSS 合并在一起 */
    QString qss;

    foreach (QString name, qssFileNames) 
    {
        qDebug() << QString("=> 加载 QSS 文件 %1").arg(name);
    
        QFile file(name);
        if (!file.open(QIODevice::ReadOnly)) 
    	{
            qDebug() << QString("打开 QSS 文件 %1 失败");
            continue;
        }
        qss.append(file.readAll()).append("\n");
    }
    
    if (!qss.isEmpty()) 
    {
        qApp->setStyleSheet(qss);
    }
}

void PssUtil::SetIniValue(const QString &fileName, const QString &groupName, const QStringList &keys, const QStringList &values)
{
    PssSingleton<PssConfig>::getInstance().SetIniValue(fileName, groupName, keys, values);
}

QStringList PssUtil::GetIniValue(const QString &fileName, const QString &groupName)
{
    return PssSingleton<PssConfig>::getInstance().GetIniValue(fileName, groupName);
}

void PssUtil::RemoveIniValue(const QString &fileName, const QString &groupName, const QString &value)
{
    PssSingleton<PssConfig>::getInstance().RemoveIniValue(fileName, groupName, value);
}
#ifdef _PSS_GPIB_SUPPORT_
QStringList PssUtil::GetEnabledGpib()
{
        ViStatus status;
        ViSession DefaultGPIBRM = 0;
        QStringList GPIBList;
        /*
         * First we must call viOpenDefaultRM to get the resource manager
         * handle.  We will store this handle in defaultRM.
         */
        status = viOpenDefaultRM (&DefaultGPIBRM);

        static char instrDescriptor[VI_FIND_BUFLEN];
        static ViUInt32 numInstrs;
        static ViFindList findList;
        status = viFindRsrc(DefaultGPIBRM, "?*INSTR", &findList, &numInstrs, instrDescriptor);
    
        if(numInstrs > 0)
        {
            GPIBList.append(QString(instrDescriptor));
            while(--numInstrs)
            {
                /* stay in this loop until we find all instruments */
                status = viFindNext (findList, instrDescriptor);  /* find next desriptor */
                if(VI_SUCCESS == status)
                {
                    GPIBList.append(QString(instrDescriptor));
                }
            }
        }
        viClose(DefaultGPIBRM);
        return GPIBList;
}
#endif

bool PssUtil::GetSetValueVec(double startVal, double stopVal, int point, QVector<double> &vec)
{
    if (startVal > stopVal)
    {
        return false;
    }
    if (point > MAX_SCAN_POINT || point < MIN_SCAN_POINT)
	{
		return false;
	}
    /* 得到设定参数 */
    double stepVal = ( stopVal - startVal ) / ( point - 1 );
    for (int i = 0; i < point; ++i)
    {
        double val = startVal + i * stepVal;
        if (val > stopVal)
        {
            val = stopVal;
        }
        vec.append(val);
    }
    return true;
}

bool PssUtil::IsFileExist(const QString &fileName)
{
    QFileInfo fileInfo(fileName);
    if (fileInfo.isFile())
    {
        return true;
    }
    return false;
}

QString PssUtil::HostAddrToIPV4(const QHostAddress &ip)
{
    quint32 ip32 = ip.toIPv4Address();
    return QString("%1.%2.%3.%4")
            .arg((ip32 >> 24) & 0xFF)
            .arg((ip32 >> 16) & 0xFF)
            .arg((ip32 >> 8) & 0xFF)
            .arg(ip32 & 0xFF);
}

bool PssUtil::GpibAddrToInt(const QString &gpibAddr, int &cardAddr, int &devAddr)
{
    if(gpibAddr.isEmpty())
    {
        return false;
    }
    int pos1 = gpibAddr.indexOf("::");
    int pos2 = gpibAddr.lastIndexOf("::");

    if((pos1 < 0) || (pos2 < 0))
    {
        return false;
    }
    
    if(pos1 == pos2)
    {
        return false;
    }
    
    devAddr = gpibAddr.mid(pos1 + 2, pos2 - (pos1 + 2)).toUInt();
    
    int pos = gpibAddr.indexOf("B");
    if(pos >= pos1)
    {
        return false;
    }
    
    cardAddr = gpibAddr.mid(pos + 1, pos1 - (pos + 1)).toUInt();
    return true;
}

bool PssUtil::SyncSymbol(double &startVal, double &stopVal)
{
    if (startVal > stopVal)
    {
        return false;
    }
    /* 传入前stopVal >= startVal */
    if (0 == startVal && 0 == stopVal)
    {
        return true;
    }
    /* stopVal > startVal */
    if (0 == startVal)
    {
        /* stopVal >= 0 startVal == +0 */
        startVal = 1e-12;
    }
    else if (0 == stopVal)
    {
        stopVal = -1e-12;
    }
    return true;
}

bool PssUtil::ConverStepToPoint(double start, double stop, double &step, int &point)
{
    if (start > stop)
    {
        qDebug() << "start cant larger than stop!";
        return false;
    }
    if (step >= (stop - start) )
    {
        point = MIN_SCAN_POINT;
        step = stop - start;
        return true;
    }
    if (0 == step)
    {
        point = 2;
        step = stop - start;
        return true;
    }

    int num = round( (stop - start) / step );
    {
        if (num < MAX_SCAN_POINT)
        {
            point = num + 1;
        }
        else
        {
            point = MAX_SCAN_POINT;
        }
    }
    return true;
}

bool PssUtil::ConverPointToStep(bool isV, double start, double stop, double &step, QString &unit, int &point)
{
    /* 点数一定大于等于2,小于等于2000 */
    if (start > stop || point < 2 || point > 2000)
    {
        return false;
    }
    if (stop == start)
    {
        point = 2;
    }
    step = (stop - start) / (point - 1);
    if (isV)
    {
        if (step >= 1)
        {
            unit = " V";
        }
        else
        {
            step = step * 1000;
            unit = "mV";
        }
    }
    else
    {
        if (step >= 1)
        {
            unit = " A";
        }
        else if (step < 1 && step >= 1e-3)
        {
            step = step * 1e3;
            unit = "mA";
        }
        else if (step < 1e-3 && step >= 1e-6)
        {
            step = step * 1e6;
            unit = "uA";
        }
        else if (step < 1e-6)
        {
            step = step * 1e9;
            unit = "nA";
        }
    }
    return true;
}

QString PssUtil::GetFormatTime(ulong ms)
{
    int ss = 1000;
    int mi = ss * 60;
    int hh = mi * 60;
    int dd = hh * 24;

    ulong day = ms / dd;
    ulong hour = (ms - day * dd) / hh;
    ulong minute = (ms - day * dd - hour * hh) / mi;
    ulong second = (ms - day * dd - hour * hh - minute * mi) / ss;
    ulong milliSecond = ms - day * dd - hour * hh - minute * mi - second * ss;
    
    QString da = QString::number(day);
    QString hou = QString::number(hour);
    QString min = QString::number(minute);
    QString sec = QString::number(second);
    QString msec = QString::number(milliSecond);
    
    return da + "d" + hou + ":" + min + ":" + sec + "." + msec;
}

uint32_t PssUtil::TimeNsWithUnit(uint32_t time, const QString &unit)
{
    uint32_t realTime;
    uint32_t factor = GetUnitFactorNs(unit);
    realTime = time * factor;
    return realTime;
}

uint32_t PssUtil::GetUnitFactorNs(const QString &unit)
{
    uint32_t factor = 0;
    if ("ns" == unit)
    {
        factor = 1;
    }
    else if ("us" == unit)
    {
        factor = 1e3;
    }
    else if ("ms" == unit)
    {
        factor = 1e6;
    }
    else if ("s" == unit)
    {
        factor = 1e9;
    }
    return factor;
}