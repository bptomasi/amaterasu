#ifndef FILE_INFO_H
#define FILE_INFO_H

typedef struct FileInfo {

    POOL_TYPE PoolType;

    ULONG PathSize;
    ULONG FinalNameSize;

    PWSTR Path;
    PWSTR FinalName;

} FILE_INFO, *PFILE_INFO;

/*
 *  AllocFileInfo() - Allocate a 'FILE_INFO' structure.
 *
 *  @PoolType: The type of memory pool to allocate from (paged or nonpaged).
 *
 *  Return:
 *    - Pointer to the allocated 'FILE_INFO' structure on success.
 *    - 'NULL' if memory allocation fails.
 */
extern PFILE_INFO
AllocFileInfo(
        __drv_strictTypeMatch(__drv_typeExpr)POOL_TYPE PoolType
    );

/*
 *  GetFileInfo() - Allocates and initializes a 'FILE_INFO' structure.
 *
 *  @PoolType: The type of memory pool to allocate from (paged or nonpaged).
 *  @Data: Pointer to a FLT_CALLBACK_DATA object containing file-related data.
 *
 *  'GetFileInfo()' is a wrapper for 'AllocFileInfo()' and 'InitFileInfo()', 
 *  providing a simple interface to obtain an allocated and initialized 
 *  'FILE_INFO' structure.
 *
 *  Returns:
 *    - Pointer to the allocated 'FILE_INFO' structure on success.
 *    - 'NULL' if memory allocation or initialization fails.
 */
extern PFILE_INFO
GetFileInfo(
        __drv_strictTypeMatch(__drv_typeExpr)POOL_TYPE PoolType,
        _In_ PFLT_CALLBACK_DATA Data
    );

/*
 *  InitFileInfo() - Initializes a 'FILE_INFO' structure based on 'FLT_CALLBACK_DATA'.
 *
 *  @FileInfo: Pointer to the 'FILE_INFO' structure to be initialized.
 *  @Data: Pointer to the 'FLT_CALLBACK_DATA' structure containing file-related data.
 *
 *  This function retrieves file name information from the 'FLT_CALLBACK_DATA' and
 *  initializes the 'FILE_INFO' structure accordingly.
 *
 *  Return:
 *    - Status code indicating the result of the initialization.
 *    - Upon success, returns 'STATUS_SUCCESS'.
 *    - The appropriate error code in case of failure.
 */
extern NTSTATUS
InitFileInfo(
        _Inout_ PFILE_INFO FileInfo,
        _In_ PFLT_CALLBACK_DATA Data
    );

/*
 *  DeInitFileInfo() - Deinitializes and frees the internal resources
 *                     associated with a 'FILE_INFO' structure.
 *
 *  @FileInfo: Pointer to the 'FILE_INFO' structure to be deinitialized.
 */
extern void
DeInitFileInfo(
        _Inout_ PFILE_INFO FileInfo
    );

/*
 *  FreeFileInfo() - Deallocates the memory associated with a 'FILE_INFO'
 *                   struct dynamically allocated.
 *
 *  @FileInfo: Pointer to a reference of a 'FILE_INFO' structure.
 */
extern void
FreeFileInfo(
        _Inout_ PFILE_INFO* FileInfo
    );

#endif  /* FILE_INFO_H *
