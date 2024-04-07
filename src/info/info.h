#ifndef INFO_H
#define INFO_H

#include "fileinfo/fileinfo.h"
#include "procinfo/procinfo.h"

#include "infodefs.h"

typedef struct Info {

    POOL_TYPE PoolType;
    INFO_TYPE InfoType;
    
    ULONG ActionSize;
    ULONG TimeStampSize

    PWSTR Action;
    PWSTR TimeStamp;

    PROC_INFO ProcInfo;

    union {
        PFILE_INFO FileInfo;
    } Info;

} INFO, *PINFO;

/*
 *  AllocInfo() - Allocate a 'INFO' structure.
 *
 *  @PoolType: The type of memory pool to allocate from (paged or nonpaged).
 *
 *  Return:
 *    - Pointer to the allocated 'INFO' structure on success.
 *    - 'NULL' if memory allocation fails.
 */
extern PINFO
AllocInfo(
        __drv_strictTypeMatch(__drv_typeExpr)POOL_TYPE PoolType
    );

/*
 *  GetInfo() - Allocates and initializes a 'INFO' structure.
 *
 *  @PoolType: The type of memory pool to allocate from (paged or nonpaged).
 *  @Data: Pointer to a FLT_CALLBACK_DATA object.
 *
 *  'GetInfo()' is a wrapper for 'AllocInfo()' and 'InitInfo()', 
 *  providing a simple interface to obtain an allocated and initialized 
 *  'INFO' structure.
 *
 *  Returns:
 *    - Pointer to the allocated 'INFO' structure on success.
 *    - 'NULL' if memory allocation or initialization fails.
 */
extern PINFO
GetInfo(
        __drv_strictTypeMatch(__drv_typeExpr)POOL_TYPE PoolType
    );

/*
 *  InitInfo() - Initializes a 'INFO' structure based on 'FLT_CALLBACK_DATA'.
 *
 *  @Info: Pointer to the 'INFO' structure to be initialized.
 *  @Data: Pointer to the 'FLT_CALLBACK_DATA' structure containing file-related 
 *         and process related data.
 *
 *  Return:
 *    - Status code indicating the result of the initialization.
 *    - Upon success, returns 'STATUS_SUCCESS'.
 *    - The appropriate error code in case of failure.
 */
extern NTSTATUS
InitInfo(
        _Inout_ PINFO Info,
        _In_ PFLT_CALLBACK_DATA Data
    );

/*
 *  DeInitInfo() - Deinitializes and frees the internal resources
 *                 associated with a 'INFO' structure.
 *
 *  @Info: Pointer to the 'INFO' structure to be deinitialized.
 */
extern void
DeInitInfo(
        _Inout_ PINFO* Info
    );

/*
 *  FreeInfo() - Deallocates the memory associated with a 'INFO'
 *               struct dynamically allocated.
 *
 *  @Info: Pointer to a reference of a 'INFO' structure.
 */
extern void
FreeInfo(
        _Inout_ PINFO* Info
    );

#endif  /* INFO_H */
