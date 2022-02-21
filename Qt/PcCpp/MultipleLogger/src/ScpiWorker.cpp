#include "ScpiWorker.h"

using namespace Pss::Qt;
ScpiWorker::ScpiWorker(int key, const QString &ip, QObject *parent)
    :QObject(parent),
      mInstKey(key),
      mIp(ip),
      mType(LAN),
      isReadReady(false)
{

}

ScpiWorker::ScpiWorker(int key, const QString &comName, int comBaud, QObject *parent)
    :QObject(parent),
      mInstKey(key),
      mComName(comName),
      mBaud(comBaud),
      mType(RS232),
      isReadReady(false)
{

}

ScpiWorker::ScpiWorker(int key, int cardAddr, int devAddr, QObject *parent)
    :QObject(parent),
      mInstKey(key),
      mCardAddr(cardAddr),
      mDevAddr(devAddr),
      mType(GPIB),
      isReadReady(false)
{

}

ScpiWorker::~ScpiWorker()
{

}

void ScpiWorker::SetReadReady(bool isReady)
{
    isReadReady = isReady;
}