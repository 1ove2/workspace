/******************************************************************************
 *
 * 文件名  ： ESSx00Worker.h
 * 负责人  ： zsf(2672921140@qq.com)
 * 创建日期： 20201201
 * 版本号  ： v1.0 20201201
 * 文件描述： 扫描线程类
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_ES_SX00_WORKER_H_
#define _PSS_ES_SX00_WORKER_H_

/************************************ 头文件 ***********************************/
#include "ESInstWorker.h"

/*********************************** 命名空间 **********************************/
namespace Pss {
namespace Qt {

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class Sx00Instrument;
class ESSx00Worker : public ESInstWorker
{
    Q_OBJECT
public:
    ESSx00Worker(int key, const QString &ip, QObject *parent = 0);
    ESSx00Worker(int key, const QString &comName, int comBaud, QObject *parent = 0);
    ESSx00Worker(int key, int cardAddr, int devAddr, QObject *parent = 0);
    ~ESSx00Worker();
public:             
    void Init() override;
    void SetScanPara(const ScanPara &para) override;
    void SetScanPara(int key, const ScanPara &para) override;
    void SetScanPara(int key, const QVector<ScanPara> &paras);
    void Connect() override;
    void SetNetUpgrade(int port) override;

private:
    void GetReadData(int point, int timeout);
    int SetReadPara(const ScanPara &para);
    int SetListPara(const ScanPara &para);
    int SetSwePara(const ScanPara &para);
    int SetApdPara(const ScanPara &para);
    int SendRst(bool isV);
    int SendCommScpiList(const ScanPara &para);
    int SendSweCommScpiList(int i, const ScanPara &para, bool isSwe = true);
    int SendSweScpiList(int i,const ScanPara &para);
    int SendListScpiList(const ScanPara &para);
    int SendReadScpiList(const ScanPara &para);
    bool OutputHasRecvOn();

    Sx00Instrument *mSx00;                                                                              /* 设备指针 */
};
/*********************************** 工具函数 **********************************/

}
}

#endif // _PSS_ES_SX00_WORKER_H_