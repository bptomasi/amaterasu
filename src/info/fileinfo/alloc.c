
#include "fileinfo.h"

#ifdef ALLOCA_PRAGMA
#   pragma alloc_text(NONPAGED, AllocFileInfo)
#endif

/*
 *  AllocFileInfo() - Allocate a 'FILE_INFO' structure.
 *
 *  @PoolType: The type of memory pool to allocate from (paged or nonpaged).
 *
 *  Return:
 *    - Pointer to the allocated 'FILE_INFO' structure on success.
 *    - 'NULL' if memory allocation fails.
 */
PFILE_INFO AllocFileInfo(__drv_strictTypeMatch(__drv_typeExpr)POOL_TYPE PoolType) {

    PFILE_INFO FileInfo;

    FileInfo = ExAllocatePoolWithTag(PoolType, sizeof *FileInfo, 'file');
    if(!FileInfo) {
        DbgPrint("by ExAllocatePoolWithTag(), status: 0x%X.\n", status);
        return NULL;
    }
    
    /* Initialize 'FileInfo' fields to zero. */
    RtlZeroMemory(FileInfo, sizeof *FileInfo);

    /*
     *  Store the pool type used for allocation in 'FileInfo' to ensure 
     *  correct handling, regardless of whether the caller routine is 
     *  paged or nonpaged.
     */
    FileInfo->PoolType = PoolType;

    return FileInfo;
}
