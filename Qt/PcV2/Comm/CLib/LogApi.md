###   1. 日志级别
######       日志级别代表了日志的重要性，在 log 中由高到低，有如下几个日志级别：

| 级别                               | 名称             | 描述                                                         |
| ---------------------------------- | ---------------- | ------------------------------------------------------------ |
| PSS_MCU_MDLS_LOG_FILTER_LVL_SILENT | 静默（停止输出） | 停止输出日志                                                 |
| LOG_LVL_ERROR                      | 错误             | 发生严重的、不可修复的错误时输出的日志属于错误级别日日志     |
| LOG_LVL_WARNING                    | 警告             | 出现一些不太重要的、具有可修复性的错误时，会输出这些警告日志 |
| LOG_LVL_INFO                       | 信息             | 给本模块上层使用人员查看的重要提示信息日志，例如：初始化成功，当前工作状态等。该级别日志一般在量产时依旧保留 |
| LOG_LVL_DBG                        | 调试             | 给本模块开发人员查看的调试日志，该级别日志一般在量产时关闭   |
---
###  2. 日志初始化
#####   2.1 初始化API（注册）
    i32 PssMcuMdlsLogInit(PssMcuMdlsLogBackendDeviceEnumT type);
#####   2.2 去初始化API（注销）
    i32 PssMcuMdlsLogDeinit(PssMcuMdlsLogBackendDeviceEnumT type);
| 参数 | 描述         |
| ---- | ------------ |
| type | 后端硬件类型 |

| 后端类型                           | 描述     |
| ---------------------------------- | -------- |
| PSS_MCU_MDLS_LOG_BACKEND_TYPE_UART | UART输出 |
| PSS_MCU_MDLS_LOG_BACKEND_TYPE_LVDS | LVDS输出 |

| 返回值               | 描述                     |
| -------------------- | ------------------------ |
| PssRstParameterError | 失败，后端硬件类型不匹配 |
| PssRstSuccess        | 成功                     |

---

### 3. 日志过滤设置
#####   3.1 按级别过滤日志
    i8 PssMcuMdlsLogGlobalFilterLvlSet(u8 level)
| 参数  | 描述         |
| ----- | ------------ |
| level | 日志设定级别 |

| 返回值 | 描述                   |
| ------ | ---------------------- |
| PssRstParameterError     | 失败，日志等级设置失败 |
| PssRstSuccess     | 成功                   |
#####   3.2 按输出模块级别过滤日志
        i8 PssMcuMdlsLogTagLvlFilterSet(u8 mode)
| 参数 | 描述             |
| ---- | ---------------- |
| mode | 日志输出模块级别 |

| 返回值 | 描述                           |
| ------ | ------------------------------ |
| PssRstParameterError     | 失败，日志输出模块级别设置失败 |
| PssRstSuccess     | 成功                           |

---

##### 4. 日志输出 API
| API                                              | 描述         |
| ------------------------------------------------ | ------------ |
| PSS_MCU_MDLS_LOG_E(...)  或  PssMcuMdlsLogE(...) | 错误级别日志 |
| PSS_MCU_MDLS_LOG_W(...)  或  PssMcuMdlsLogW(...) | 警告级别日志 |
| PSS_MCU_MDLS_LOG_I(...)  或  PssMcuMdlsLogI(...) | 提示级别日志 |
| PSS_MCU_MDLS_LOG_D(...)  或  PssMcuMdlsLogD(...) | 调试级别日志 |
**说明：参数 ... 为日志内容，格式与 printf 一致**

---
### 5. 后端输出重定向
######    实现printf功能
#####     5.1 后端输出重定向
    void PssMcuMdlsLogOutput(PssMcuMdlsLogLevelEnumT level, const i8 *format, ...);
| 参数   | 描述       |
| ------ | ---------- |
|level      |日志输出级别|
| format | 打印输出值 |