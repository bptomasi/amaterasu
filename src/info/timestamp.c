
#include "timestamp.h"

/*
 *  TimeStampInit() -
 *
 *  @TimeStamp:
 *
 *  Return:
 *    -
 */
void TimeStampInit(_Out_ PTIME_FIELDS TimeStamp) {

    LARGE_INTEGER SysTime;
    LARGE_INTEGER LocalTime;

    KeQuerySystemTime(&SysTime);
    ExSystemTimeToLocalTime(&SysTime, &LocalTime);
    RltTimeToTimeFields(&LocalTime, TimeStamp);
};
