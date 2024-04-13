
#include "timestamp.h"

/*
 *  GetTimeFieldsInfo() -
 *
 *  @TimeField:
 *  @UniString:
 *
 *  Return:
 *    -
 *    -
 */
static NTSTATUS GetTimeFieldsInfo(_In_ PTIME_FIELDS TimeField, _Out_ PUNICODE_STRING UniString) {

}

/*
 *  TimeStampInit() -
 *
 *  @TimeStamp:
 *
 *  Return:
 *    -
 *    -
 */
NTSTATUS TimeStampInit(_Out_ PTIMESTAMP TimeStamp) {

    NTSTATUS status;

    LARGE_INTEGER SysTime;
    LARGE_INTEGER LocalTime;
    TIME_FIELDS TimeField;

    UNICODE_STRING UniString;

    KeQuerySystemTime(&SysTime);
    ExSystemTimeToLocalTime(&SysTime, &LocalTime);
    RltTimeToTimeFields(&LocalTime, &TimeField);

    status = UnicodeStrToWSTR();
    

    return status;
};
