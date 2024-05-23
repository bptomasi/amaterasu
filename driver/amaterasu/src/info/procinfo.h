#ifndef PROC_INFO_H
#define PROC_INFO_H

#include "common.h"

struct ProcInfo {

    POOL_TYPE PoolType;
    
    ULONG PID;
    ULONG SID;
    ULONG TID;

    BOOLEAN IsElevated;
};

typedef struct ProcInfo PROC_INFO, *PPROC_INFO;

/*
 *  ProcInfoAlloc() - Allocate a 'PROC_INFO' structure.
 *
 *  @PoolType: The type of memory pool to allocate from (paged or nonpaged).
 *
 *  Return:
 *    - Pointer to the allocated 'PROC_INFO' structure on success.
 *    - 'NULL' if memory allocation fails.
 */
extern PPROC_INFO
ProcInfoAlloc(
    __drv_strictTypeMatch(__drv_typeExpr)POOL_TYPE PoolType
);

/*
 *  ProcInfoGet() - Allocates and initializes a 'FILE_INFO' structure.
 *
 *  @PoolType: The type of memory pool to allocate from (paged or nonpaged).
 *  @Data: Pointer to a FLT_CALLBACK_DATA object containing process-related data.
 *
 *  'ProcInfoGet()' is a wrapper for 'ProcInfoAlloc()' and 'ProcInfoInit()',
 *  providing a simple interface to obtain an allocated and initialized
 *  'PROC_INFO' structure.
 *
 *  Returns:
 *    - Pointer to the allocated 'PROC_INFO' structure on success.
 *    - 'NULL' if memory allocation or initialization fails.
 */
extern PPROC_INFO
ProcInfoGet(
    __drv_strictTypeMatch(__drv_typeExpr)POOL_TYPE PoolType,
    _In_ PFLT_CALLBACK_DATA Data
);

/*
 *  ProcInitInfo() - Initializes a 'PROC_INFO' structure based on 
 *                   'FLT_CALLBACK_DATA'.
 *
 *  @ProcInfo: Pointer to the 'PROC_INFO' structure to be initialized.
 *  @Data: Pointer to the 'FLT_CALLBACK_DATA' structure containing process 
 *         related data.
 *
 *  Return:
 *    - Status code indicating the result of the initialization.
 *    - Upon success, returns 'STATUS_SUCCESS'.
 *    - The appropriate error code in case of failure.
 */
extern NTSTATUS
ProcInfoInit(
    _Out_ PPROC_INFO ProcInfo,
    _In_ PFLT_CALLBACK_DATA Data
);

/*
 *  ProcInfoFree() - Deallocates the memory associated with a 'PROC_INFO'
 *                   struct dynamically allocated.
 *
 *  @ProcInfo: Pointer to a reference of a 'PROC_INFO' structure.
 */
extern void
ProcInfoFree(
    _Inout_  PPROC_INFO* ProcInfo
);

#endif  /* PROC_INFO_H */
