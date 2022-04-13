#include "PssCalRuner.h"

PssCalRuner::PssCalRuner(double start, double end, double stdLimt, double pssLimt,
                         double stdNplc, int calIndex, QObject *parent) :
    QObject(parent),
    mC04Dac(0),
    mC14Dac(0),
    mSqSum4Dac(0),
    mC04Adc(0),
    mC14Adc(0),
    mSqSum4Adc(0),
    mForCheck(true),
    mStart(0),
    mEnd(0),
    mCalIndex(calIndex),
    mStopCal(true),
    mChannel(1)
{

}

PssCalRuner::~PssCalRuner(void)
{

}

bool PssCalRuner::CalCurrAdc(PssDevice &pssDev, StdDevice &stdDev, int calPoints)
{
    mStopCal = false;
    /* step1: 采样 扫描 */

    /* step2: 校准系数计算 */

    /* step3: 上传校准数据 */

    /* step4: 检查 扫描 */
    return true;
}

bool PssCalRuner::CalVoltAdc(PssDevice &pssDev, StdDevice &stdDev, int calPoints)
{

}

bool PssCalRuner::CalCurrDac(PssDevice &pssDev, StdDevice &stdDev, int calPoints)
{

}

bool PssCalRuner::CalVoltDac(PssDevice &pssDev, StdDevice &stdDev, int calPoints)
{

}


void PssCalRuner::slotStopCal()
{
    mStopCal = true;
}

void PssCalRuner::AppendSetVLoad(double val)
{
    mSetVLoad.append(val);
}

void PssCalRuner::AppendSetILoad(double val)
{
    mSetILoad.append(val);
}

void PssCalRuner::AppendGetVLoad(double val)
{
    mGetVLoad.append(val);
}

void PssCalRuner::AppendGetILoad(double val)
{
    mGetILoad.append(val);
}

void PssCalRuner::AppendGetVAdc(double val)
{
    mGetVAdc.append(val);
}

void PssCalRuner::AppendGetIAdc(double val)
{
    mGetIAdc.append(val);
}

void PssCalRuner::AppendGetVDac(double val)
{
    mGetVDac.append(val);
}

void PssCalRuner::AppendGetIDac(double val)
{
    mGetIDac.append(val);
}

void PssCalRuner::AppendGetVReal(double val)
{
    mVReal.append(val);
}

void PssCalRuner::AppendGetIReal(double val)
{
    mIReal.append(val);
}



void PssCalRuner::CalibrationCompute4Dac(const QVector<double> &x, const QVector<double> &y)
{

}

void PssCalRuner::CalibrationCompute4Adc(const QVector<double> &x, const QVector<double> &y)
{

}
