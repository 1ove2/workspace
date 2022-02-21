#include "InstWorker.h"

InstWorker::InstWorker(int key, const QString &ip, QObject *parent)
    :QObject(parent),
      mIp(ip),
      mInstKey(key),
      mType(LAN),
      isOutputReady(false),
      isReadReady(false)
{
}

InstWorker::InstWorker(int key, const QString &comName, int comBaud, QObject *parent)
    :QObject(parent),
      mComName(comName),
      mBaud(comBaud),
      mInstKey(key),
      mType(RS232),
      isOutputReady(false),
      isReadReady(false)
{
}

InstWorker::InstWorker(int key, int cardAddr, int devAddr, QObject *parent)
    :QObject(parent),
      mCardAddr(cardAddr),
      mDevAddr(devAddr),
      mInstKey(key),
      mType(GPIB),
      isOutputReady(false),
      isReadReady(false)
{
}

InstWorker::~InstWorker()
{

}

void InstWorker::SetOutputReady(bool isReady)
{
    isOutputReady = isReady;
}

void InstWorker::SetReadReady(bool isReady)
{
    isReadReady = isReady;
}
