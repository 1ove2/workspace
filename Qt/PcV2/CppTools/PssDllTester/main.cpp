#include "PssDll.h"
#include "PssCppLog.h"

#define _2400_

#include <iostream>
using namespace std;


int main(int argc, char *argv[])
{
    PSS_UNUSED(argc)
    PSS_UNUSED(argv)

#ifdef _2400_
    i32 dev = PssDllDevComOpen("COM4", PSS_DLL_BUADRATE_9600);
    if(dev)
    {
        fprintf(stderr, "PssDllDevNetOpen err:%d\n", dev);
        return 0;
    }
#else /* 2611B 模拟 2400 */
    i32 dev = PssDllDevNetOpen("192.168.3.204");
    if(dev)
    {
        fprintf(stderr, "PssDllDevNetOpen err:%d\n", dev);
        return 0;
    }
#endif

    i32 rst = PssDllSetVSrc(dev, 0, false);
    if(rst < 0)
    {
        fprintf(stderr, "PssDllSetVSrc err:%d\n", rst);
        return 0;
    }
    bool isVSrc = PssDllIsVSrc(dev, 0);
    fprintf(stderr, "PssDllIsVSrc:%s(%d).\n", isVSrc ? ("VSrc") : ("ISrc"), isVSrc);

    f64 vVal = 0;
    rst = PssDllGetVVal(dev, 0, &vVal);
    if(rst < 0)
    {
        fprintf(stderr, "PssDllGetVVal err:%d\n", rst);
        return 0;
    }

    f64 iVal = 0;
    rst = PssDllGetIVal(dev, 0, &iVal);
    if(rst < 0)
    {
        fprintf(stderr, "PssDllGetVVal err:%d\n", rst);
        return 0;
    }
    fprintf(stderr, "vVal:%7.4g, iVal:%7.4g.\n", vVal, iVal);

    rst = PssDllDevClose(dev);
    PssCLibLogI(PssCppLog::GetCLibLog(), "PssDllTester Run Done:%d,%d", dev, rst);
    return 0;
}
