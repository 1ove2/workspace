#ifndef _ERROR_DEF_H_
#define _ERROR_DEF_H_

typedef enum {
    SetSuccess = 0,
    ConnectError = -1,
    SetTrigLineError = -2,
    SetMasterError = -3,
    SetTrigInputError = -4,
    SetTraceError = -5,
    Set4WError = -6,
    SetFError = -7,
    SetNplcError = -8,
    SetMeasureQueryError = -9,
    SetVSrcError = -10,
    SetSrcRangeError = -11,
    SetSrcValError = -12,
    SetLmtRangeError = -13,
    SetLmtValError = -14,
    SetScanTypeError = -15,
    SetScanStartValError = -16,
    SetScanStopValError = -17,
    SetScanPointError = -18,
    SetAppendListValError = -19,
    SetOutputError = -20,
    SetReadError = -21,
    RecvStrOnError = -22,
    SetOutputModeError = -23,
    SetPulseDelayError = -24,
    SetPulseWidthError = -25,
    SetPulsePeriodError = -26,
    SetNetUpgradePortError = -27,
} PssErrorDef;



#endif //_ERROR_DEF_H_
