#ifndef _PSS_QT_PARA_WGT_H_
#define _PSS_QT_PARA_WGT_H_

#include <QWidget>
#include "ParaDef.h"

namespace Ui {
class ParaWgt;
}

namespace Pss {
namespace Qt {

class ParaWgt : public QWidget
{
    Q_OBJECT
public:
    ParaWgt(QWidget *parent = 0);
    ~ParaWgt();
    void SetInfo(const ChanInfo &info);
    void SetChanID(int key, int cardID, int chanID);
    void SetTitle(const QString &str);
    int GetScanPara(MeasPara &para);
    void SetScanPara(const MeasPara &para) const;
    void SetNplc(const QString &text);
signals:
    void sigDeleteChan(const ChanInfo &info);
    void sigSearchChan(int chanID);
    void sigUpgrade(const ChanInfo &info);
    void sigNplcChanged(const ChanInfo &info,QString text);
private slots:
    void Upgrade();
    void SrcVToggled(bool isToggled);
    void DeleteChan();
    void SearchChan();
private:
    int GetRealChan(int chan);
    ChanInfo mInfo;
    int mKey;
    int mChanID;
    int mCardID;
    bool mEnable;
    Ui::ParaWgt *mUi;
};

}
}

#endif // _PSS_QT_PARA_WGT_H_