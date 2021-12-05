#ifndef _PSS_QT_PARA_DEF_
#define _PSS_QT_PARA_DEF_

struct MeasPara {
    bool isSrcV;                 /* 源 */
    bool is4Wires;               /* 2/4线 */
    double srcVal;               /* 源值 */
    bool isSrcAuto;              /* 源自动量程 */
    double srcRange;             /* 源量程 */
    double lmtVal;               /* 限值 */
    bool isLmtAuto;              /* 限自动量程 */
    double lmtRange;             /* 限值 */
    double nplc;                 /* nplc */

    int instKey;                 /* 标识 */
    int cardID;                  /* 卡号 */
    int chanID;                  /* 通道号 */
    bool isEnable;               /* 使能 */
};

struct ChanInfo {
    int key;
    int card;
    int chan;
    bool operator < (const ChanInfo &info) const {
        return (key * 100 + card * 10 + chan) < (info.key * 100 + info.card * 10 + info.chan );
    }
};

#endif // _PSS_QT_PARA_DEF_