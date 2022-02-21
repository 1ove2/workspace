#ifndef _SCAN_PARA_H_
#define _SCAN_PARA_H_
#include <QString>
#include <QVector>

const int MAX_TIMEOUT_MS  = 500000;
const int MAX_READTIMEOUT_MS  = 5000;

/* 设备型号 */
enum InstModel {
    S100,
    S200,
    S300,
    CS300,
    CS400,
    P100,
    P200,
    P300,
    Pl100
};
/* 通信方式 */
enum ConnectType {
    LAN,
    RS232,
    GPIB
};

/* 窗口类型 */
typedef enum _WidgetTypeTag_ {
    WidgetTypeSwe,
    WidgetTypeList,
    WidgetTypeLog,
    WidgetTypeMos,
    WidgetTypeBjt,
    WidgetTypeLiv,
    WidgetTypeLiv2,
    WidgetTypeLiv3,
    WidgetTypeApd
} WidgetType;

/* 扫描参数结构体 */
struct ScanPara
{
    enum ScanType{
        Swe,
        List,
        Read,
        Mos,
        Apd
    };
    double limitIn;
    double limitOut;
    double startVal;
    double stopVal;
    double nplc;
    QVector<double> startList;
    QVector<double> stopList;
    QVector<double> lmtList;
    QVector<double> srcRangeList;
    QVector<double> lmtRangeList;
    QVector<int> pointList;

    bool isPulse;
    int pulseWidth;  /* us */
    int pulsePeriod; /* us */
    int pulseDelay;  /* ns */
    
    bool isSourceV;
    bool is4Wires;
    bool isFrontboard;
    bool isMasterDevice;
    bool TrigOutput;
    bool hasCache;
    bool isIndependent;
    
    int dir;
    int pointCount;
    int chanID;
    int timeout;
    
    QVector<double> listData;


    bool isSrcAuto;
    bool isLmtAuto;
    
    ScanType type;
};

struct LANPara {
    QString ipAddr;
    uint32_t port;
};

struct RS232Para {
    QString comName;
    uint32_t baud;
};

struct GPIBPara {
    uint8_t cardAddr;
    uint8_t devAddr;
};

struct ConnectPara {
    ConnectType type;
    union {
        LANPara lanPara;
        RS232Para rs232Para;
        GPIBPara gpibPara;
    };
};

/* 窗口参数结构体 */
struct ConfigPara {
    QString startVal;
    QString stopVal;
    QString stepVal;
    QString limitVal;

    int startUnitIndex;
    int stopUnitIndex;
    int stepUnitIndex;
    int limitUnitIndex;
    
    QString nplc;
    QString point;
    
    bool isSourceV;
    bool is4Wires;
    bool isFront;
    
    bool hasR;
    bool hasP;
};
#endif // _SCAN_PARA_H_