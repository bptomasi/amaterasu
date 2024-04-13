
#include "fileinfo.h"

/*
 *  FileInfoGet() - Allocates and initializes a 'FILE_INFO' structure.
 *
 *  @PoolType: The type of memory pool to allocate from (paged or nonpaged).
 *  @Data: Pointer to a FLT_CALLBACK_DATA object containing file-related data.
 *
 *  'GetFileInfo()' is a wrapper for 'FileInfoAlloc()' and 'FileInfoInit()', 
 *  providing a simple interface to obtain an allocated and initialized 
 *  'FILE_INFO' structure.
 *
 *  Returns:
 *    - Pointer to the allocated 'FILE_INFO' structure on success.
 *    - 'NULL' if memory allocation or initialization fails.
 */

PFILE_INFO FileInfoGet(POOL_TYPE PoolType, _In_ PFLT_CALLBACK_DATA Data) {

    NTSTATUS status;
    PFILE_INFO FileInfo;

    FileInfo = FileInfoAlloc(PoolType);
    if(!FileInfo) {
        return NULL;
    }

    status = FileInfoInit(FileInfo, Data);
    if(!NT_SUCCESS(status)) {
        FileInfoFree(FileInfo);
        return NULL;
    }

    return FileInfo;
}
