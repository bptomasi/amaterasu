
#include "fileinfo.h"

#ifdef ALLOCA_PRAGMA
#   pragma alloc_text(NONPAGED, FileInfoDeInit)
#endif

/*
 *  FileInfoDeInit() - Deinitialize and frees the internal resources
 *                     associated with a 'FILE_INFO' structure.
 *
 *  @FileInfo: Pointer to the 'FILE_INFO' structure to be deinitialized.
 */
void DeInitFileInfo(_Inout_ PFILE_INFO FileInfo) {

    if(FileInfo) {
        ExFreePoolWithTag(FileInfo->Path, 'wstr');
        ExFreePoolWithTag(FileInfo->FinalName, 'wstr');

        /* Resets 'FileInfo' values. */
        FileInfo->Path = NULL;
        FileInfo->FinalName = NULL;
    }
}
