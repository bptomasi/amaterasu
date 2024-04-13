
#include "fileinfo.h"

#ifdef ALLOCA_PRAGMA
#   pragma alloc_text(NONPAGED, FileInfoFree)
#endif

/*
 *  FileInfoFree() - Deallocates the memory associated with a 'FILE_INFO'
 *                   struct dynamically allocated.
 *
 *  @FileInfo: Pointer to a reference of a 'FILE_INFO' structure.
 */
void FileInfoFree(_Inout_ PFILE_INFO* FileInfo) {

    if(FileInfo) {
        FileInfoDeInit(*FileInfo);
        ExFreePoolWithTag(*FileInfo, 'file');

       /*
        *  In order to avoid a dangling pointer, after deallocating the
        *  'FILE_INFO' structure, we set the reference to the 'FILE_INFO'
        *  struct to NULL.
        */
        *FileInfo = NULL;
    }
}
