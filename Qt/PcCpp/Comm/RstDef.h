/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _RST_DEF_H
#define _RST_DEF_H
                                                                                                      /* 注释定位 */
/*********************************** 类型定义 **********************************/
typedef enum {
    RstSuccess               =    0,                                            /* 成功 */
    RstParameterError        =   -1,                                            /* 参数输入错误 */
    RstDevCreateError        =   -2,                                            /* 设备构造失败 */
    RstDeviceNotInit         =   -3,                                            /* 设备未初始化*/
    RstScpiClientNotFound    =   -4,                                            /* 未找到scpiClient对象 */
    RstNotImpl               =   -5,                                            /* 未实现 */
    RstOutputFail            =   -6,                                            /* 设备Output失败 */
    RstDeviceNotConnect      =   -7,                                            /* 设备未连接 */
    RstSetSrcValError        =   -8,                                            /* 源值设置失败 */
    RstSetSrcRangeError      =   -9,                                            /* 源量程设置失败 */
    RstSetLmtValError        =   -10,                                           /* 限值设置失败 */
    RstSetLmtRangeError      =   -11,                                           /* 限量程设置失败 */
    RstSetSrcError           =   -12,                                           /* 源设置失败 */
    RstSetNPLCError          =   -13,                                           /* 设置NPLC指令失败 */
    RstSetMeasureSrcError    =   -14,                                           /* 设置测试电压电流失败 */
    RstReadError             =   -15,                                           /* 读取设备数据失败 */
    RstAlreadyConnect        =   -16,                                           /* 设备已经初始化 */
    RstRecvFromIOError       =   -17,                                           /* 接收IO数据失败 */
    RstIOEmptyError          =   -18,                                           /* IO 为空 */
    RstOptTimeoutError       =   -19,                                           /* 超时错误 */
    RstWriteToIOError        =   -20,                                           /* 向IO写入数据失败 */
    RstReadFromIOError       =   -21,                                           /* 读取IO数据失败 */
    RstMallocFailed          =   -22,                                           /* 内存申请失败 */
    RstSetSensFuncError      =   -23,                                           /* 限设置失败 */
    RstBufIsNotEnough        =   -24,                                           /* 数据不足以支持后续解析 */
    RstResponseArrived       =   -25,                                           /* 响应到达 */
    RstResponseMismatch      =   -26,                                           /* 请求与响应不匹配 */
    RstSet24WError           =   -27,                                           /* 设置2/4线失败 */
    RstSetFBError            =   -28,                                           /* 设置前后面板失败 */
    RstSetMasterError        =   -29,                                           /* 主从机设置失败 */
    RstSetTrigDirError       =   -30,                                           /* 设置Trig方向失败 */
    RstSetScanSrcError       =   -31,                                           /* 扫描源设置失败 */
    RstSetScanStartError     =   -32,                                           /* 设置扫描起点失败 */
    RstSetScanStopError      =   -33,                                           /* 设置扫描终点失败 */
    RstSetScanPointError     =   -34,                                           /* 设置扫描点数失败 */
    RstSetCustomValError     =   -35,                                           /* 设置自定义数据失败 */
    RstOverFlowError         =   -36,                                           /* 溢出错误 */
    RstRepeatError           =   -37,                                           /* 重复调用或设备错误 */
    RstSetValOverFlow        =   -38,                                           /* 设置值超过极限值 */
    RstScpiFormatError       =   -39,                                           /* scpi格式错误 */
    RstScpiCmdError          =   -40,                                           /* scpi指令错误 */
    RstSetResolutinError     =   -41,                                           /* 设置分辨率出错 */
    RstCheckError            =   -42,                                           /* 检查出错 */
    RstOverFlow              =   -43,                                           /* 缓存溢出 */
    RstGPIBOpenError         =   -44,                                           /* GPIB打开失败 */
    RstGPIBNotOpen           =   -45,                                           /* GPIB未打开 */
    RstGPIBWriteError        =   -46,                                           /* GPIB写失败 */
    RstGPIBReadError         =   -47,                                           /* GPIB读失败 */
    RstSocketBindError       =   -48,                                           /* socket绑定错误 */
    RstSocketConnectError    =   -49,                                           /* socket连接至服务端错误 */
    RstUnknowPssBinFrm       =   -50,                                           /* 错误的PssBin协议帧 */
    RstAnlgOptError          =   -51,                                           /* 模拟板底层操作出错 */
    RstPwrOptError           =   -52,                                           /* 电源板底层操作出错 */
    RstFileError             =   -53,                                           /* 文件异常错误 */
    RstUnknowDevice          =   -54,                                           /* 未知设备类型 */
    RstCalError              =   -55,                                           /* 校准失败 */
    RstUartConfigError       =   -56,                                           /* 串口配置失败 */
    RstOverFlowOfIndex       =   -57,                                           /* 索引溢出 */
    RstMySqlCmdErr           =   -58,                                           /* 数据库MySql Sql语言错误 */
    RstMySqlUsrErr           =   -59,                                           /* 数据库用户错误 */
    RstMySqlPasswdErr        =   -60,                                           /* 数据库密码错误 */
    RstIOBrokenErr           =   -61,
    RstSysCallError          =   -62,                                           /* system call error */
} RstT;

#endif // _RST_DEF_H
