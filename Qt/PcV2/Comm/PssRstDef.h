/********************************************************************************
*
* 文件名  ： PssRstDef.h
* 负责人  ： 彭鹏(516190948@qq.com)
* 创建日期： 20190318
* 版本号  ： v1.0
* 文件描述： 错误代码头文件
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_RST_DEF_H_
#define _PSS_RST_DEF_H_

/************************************ 头文件 ***********************************/

/************************************ 宏定义 ***********************************/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/*********************************** 类型定义 **********************************/
typedef enum {
    PssRstSuccess               =    0,                                                                 /* 成功 */
    PssRstParameterError        =   -1,                                                                 /* 参数输入错误 */
    PssRstDevCreateError        =   -2,                                                                 /* 设备构造失败 */
    PssRstDeviceNotInit         =   -3,                                                                 /* 设备未初始化*/
    PssRstScpiClientNotFound    =   -4,                                                                 /* 未找到scpiClient对象 */
    PssRstNotImpl               =   -5,                                                                 /* 未实现 */
    PssRstOutputFail            =   -6,                                                                 /* 设备Output失败 */
    PssRstDeviceNotConnect      =   -7,                                                                 /* 设备未连接 */
    PssRstSetSrcValError        =   -8,                                                                 /* 源值设置失败 */
    PssRstSetSrcRangeError      =   -9,                                                                 /* 源量程设置失败 */
    PssRstSetLmtValError        =   -10,                                                                /* 限值设置失败 */
    PssRstSetLmtRangeError      =   -11,                                                                /* 限量程设置失败 */
    PssRstSetSrcError           =   -12,                                                                /* 源设置失败 */
    PssRstSetNPLCError          =   -13,                                                                /* 设置NPLC指令失败 */
    PssRstSetMeasureSrcError    =   -14,                                                                /* 设置测试电压电流失败 */
    PssRstReadError             =   -15,                                                                /* 读取设备数据失败 */
    PssRstAlreadyConnect        =   -16,                                                                /* 设备已经初始化 */
    PssRstRecvFromIOError       =   -17,                                                                /* 接收IO数据失败 */
    PssRstIOEmptyError          =   -18,                                                                /* IO 为空 */
    PssRstOptTimeoutError       =   -19,                                                                /* 超时错误 */
    PssRstWriteToIOError        =   -20,                                                                /* 向IO写入数据失败 */
    PssRstReadFromIOError       =   -21,                                                                /* 读取IO数据失败 */
    PssRstMallocFailed          =   -22,                                                                /* 内存申请失败 */
    PssRstSetSensFuncError      =   -23,                                                                /* 限设置失败 */
    PssRstBufIsNotEnough        =   -24,                                                                /* 数据不足以支持后续解析 */
    PssRstResponseArrived       =   -25,                                                                /* 响应到达 */
    PssRstResponseMismatch      =   -26,                                                                /* 请求与响应不匹配 */
    PssRstSet24WError           =   -27,                                                                /* 设置2/4线失败 */
    PssRstSetFBError            =   -28,                                                                /* 设置前后面板失败 */
    PssRstSetMasterError        =   -29,                                                                /* 主从机设置失败 */
    PssRstSetTrigDirError       =   -30,                                                                /* 设置Trig方向失败 */
    PssRstSetScanSrcError       =   -31,                                                                /* 扫描源设置失败 */
    PssRstSetScanStartError     =   -32,                                                                /* 设置扫描起点失败 */
    PssRstSetScanStopError      =   -33,                                                                /* 设置扫描终点失败 */
    PssRstSetScanPointError     =   -34,                                                                /* 设置扫描点数失败 */
    PssRstSetCustomValError     =   -35,                                                                /* 设置自定义数据失败 */
    PssRstOverFlowError         =   -36,                                                                /* 溢出错误 */
    PssRstRepeatError           =   -37,                                                                /* 重复调用或设备错误 */
    PssRstSetValOverFlow        =   -38,                                                                /* 设置值超过极限值 */
    PssRstScpiFormatError       =   -39,                                                                /* scpi格式错误 */
    PssRstScpiCmdError          =   -40,                                                                /* scpi指令错误 */
    PssRstSetResolutinError     =   -41,                                                                /* 设置分辨率出错 */
    PssRstCheckError            =   -42,                                                                /* 检查出错 */
    PssRstOverFlow              =   -43,                                                                /* 缓存溢出 */
    PssRstGPIBOpenError         =   -44,                                                                /* GPIB打开失败 */
    PssRstGPIBNotOpen           =   -45,                                                                /* GPIB未打开 */
    PssRstGPIBWriteError        =   -46,                                                                /* GPIB写失败 */
    PssRstGPIBReadError         =   -47,                                                                /* GPIB读失败 */
    PssRstSocketBindError       =   -48,                                                                /* socket绑定错误 */
    PssRstSocketConnectError    =   -49,                                                                /* socket连接至服务端错误 */
    PssRstUnknowPssBinFrm       =   -50,                                                                /* 错误的PssBin协议帧 */
    PssRstAnlgOptError          =   -51,                                                                /* 模拟板底层操作出错 */
    PssRstPwrOptError           =   -52,                                                                /* 电源板底层操作出错 */
    PssRstFileError             =   -53,                                                                /* 文件异常错误 */
    PssRstUnknowDevice          =   -54,                                                                /* 未知设备类型 */
    PssRstCalError              =   -55,                                                                /* 校准失败 */
    PssRstUartConfigError       =   -56,                                                                /* 串口配置失败 */
    PssRstOverFlowOfIndex       =   -57,                                                                /* 索引溢出 */
    PssRstMySqlCmdErr           =   -58,                                                                /* 数据库MySql Sql语言错误 */
    PssRstMySqlUsrErr           =   -59,                                                                /* 数据库用户错误 */
    PssRstMySqlPasswdErr        =   -60,                                                                /* 数据库密码错误 */
    PssRstIOBrokenErr           =   -61,                                                                /* IO操作执行错误 */
    PssRstSysCallError          =   -62,                                                                /* system call error */
    PssRstReadEAGAIN            =   -63,                                                                /* read EAGAIN */
    PssRstReadEOF               =   -64,                                                                /* 文件 已经读取完毕 */
    PssRstPssBinNULLCallBack    =   -65,                                                                /* PssBin回调空 */
    PssRstLenIllegal            =   -66,                                                                /* 长度非法 */
    PssRstFrmTailErr            =   -67,                                                                /* 帧尾部错误 */
    PssRstCalParaWirteFail      =   -68,                                                                /* 校准系数写入失败 */
} PssRstT;

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/


#endif // _PSS_RST_DEF_H_

