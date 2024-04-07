
#include "fileinfo.h"

#ifdef ALLOCA_PRAGMA
#   pragma alloc_text(NONPAGED, GetFileInfo)
#endif

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
PFILE_INFO GetFileInfo(
        __drv_strictTypeMatch(__drv_typeExpr)POOL_TYPE PoolType,
        _In_ PFLT_CALLBACK_DATA Data
    ) {

    NTSTATUS status;
    PFILE_INFO FileInfo;

    FileInfo = AllocFileInfo(PoolType);
    if(!FileInfo) {
        DbgPrint("at AllocFileInfo()\n");
        return NULL;
    }

    status = InitFileInfo(FileInfo, Data);
    if(!NT_SUCCESS(status)) {
        DbgPrint("at InitFileInfo()\n");

        /*
         *  If 'InitFileInfo()' fails, we deallocate the 'FILE_INFO'
         *  structure completely.
         */
        FreeFileInfo(&FileInfo);
        return NULL;
    }

    /* Change this? Maybe */
    /*
     *  If 'AllocFileInfo()' or 'InitFileInfo()' fails, 'Amaterasu'
     *  will not report any logging information.
     */

    return FileInfo;
}
