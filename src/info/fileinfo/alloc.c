
#include "fileinfo.h"

#ifdef ALLOCA_PRAGMA
#   pragma alloc_text(NONPAGED, FileInfoAlloc)
#endif

/*
 *  FileInfoAlloc() - Allocate a 'FILE_INFO' structure.
 *
 *  @PoolType: The type of memory pool to allocate from (paged or nonpaged).
 *
 *  Return:
 *    - Pointer to the allocated 'FILE_INFO' structure on success.
 *    - 'NULL' if memory allocation fails.
 */
PFILE_INFO FileInfoAlloc(_In_ POOL_TYPE PoolType) {

    PFILE_INFO FileInfo;

    FileInfo = ExAllocatePoolWithTag(PoolType, sizeof *FileInfo, 'file');
    if(!FileInfo) {
        return NULL;
    }
    
    RtlZeroMemory(FileInfo, sizeof *FileInfo);

    /*
     *  Store the pool type used for the allocation in 'FileInfo' to ensure 
     *  correct memory handling, regardless of whether the caller routine is 
     *  paged or nonpaged.
     */
    FileInfo->PoolType = PoolType;

    return FileInfo;
}
