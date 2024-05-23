#ifndef UTILS_H
#define UTILS_H

#include "common.h"

/*
 *  UnicodeStrToWSTR() -
 *
 *  @PoolType:
 *  @Src:
 *  @Dest:
 *  @pSize:
 *
 *  Return:
 *    -
 *    -
 */
extern NTSTATUS
UnicodeStrToWSTR(
        _In_ POOL_TYPE PoolType,
        _In_ PUNICODE_STRING Src,
        _Out_ PWSTR* Dest,
        _Out_ PULONG pSize
    );

#endif  /* UTILS_H */
