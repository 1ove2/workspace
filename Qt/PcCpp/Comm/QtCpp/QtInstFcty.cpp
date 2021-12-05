#include "QtInstFcty.h"
#include "Qt2450.h"
#include "Qt34470A.h"
#include "Sx00Instrument.h"
#include "ITool.h"
#include "RstDef.h"

const QString QtEmpytInst::mcName = QString("EmpytInst");

QtInstFcty::QtInstFcty(QObject *parent) :
    QObject(parent)
{
}

QtInstFcty::~QtInstFcty(void)
{
}

QtIInstrument *QtInstFcty::CreateInstrument(const QHostAddress &ip)
{
    QtIInstrument *tempInstrument = new QtEmpytInst(ip, "noName");
    bool done = tempInstrument->Connect();
    if(!done)
    {
        qCritical() << QString("%1:%2 have not instrument.").arg(ip.toString()).arg(QtIInstrument::GetScpiTcpServerPort());
        return nullptr;
    }

    QtIInstrument *realInstrunment = nullptr;
    QString name = tempInstrument->GetName();
    delete tempInstrument;
    tempInstrument = nullptr;
    if(name == Qt2450::GetIdName())
    {
        realInstrunment = new Qt2450(ip);
    }
    else if(name == Qt34470A::GetIdName())
    {
        realInstrunment = new Qt34470A(ip);
    }
    else if(name == Sx00Instrument::GetIdName())
    {
        realInstrunment = new Sx00Instrument(ip);
    }
    else if("S100" == Sx00Instrument::GetIdName())
    {
        realInstrunment = new Sx00Instrument(ip);
    }
    else if("S200" == Sx00Instrument::GetIdName())
    {
        realInstrunment = new Sx00Instrument(ip);
    }
    else if("S300" == Sx00Instrument::GetIdName())
    {
        realInstrunment = new Sx00Instrument(ip);
    }
    else
    {
        qCritical() << QString("%1:%2(%3) is not a valid instrument.")
                   .arg(ip.toString())
                   .arg(QtIInstrument::GetScpiTcpServerPort())
                   .arg(name);
    }
    
    return realInstrunment;
}


QtEmpytInst::QtEmpytInst(const QHostAddress &ip, const QString &name, QObject *parent) :
    Sx00Instrument(ip, QString(mcName), true, parent)
{
}

QtEmpytInst::~QtEmpytInst(void)
{
}

int QtEmpytInst::UpdateCfgToInstrument(void)
{
    qCritical() << QString("%1 not impl %2").arg("QtEmpytInst").arg(__func__);
    return PssRstNotImpl;
}
