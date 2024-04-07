
#include "fileinfo.h"

static NTSTATUS
InitFileInfoFields(
        _Inout_ PFILE_INFO FileInfo,
        _In_ PFLT_FILE_NAME_INFORMATION NameInfo
    );

#ifdef ALLOCA_PRAGMA
#   pragma alloc_text(NONPAGED, InitFileInfoFields)
#   pragma alloc_text(NONPAGED, InitFileInfo)
#endif
   
/*
 *  InitFileInfoFields() -
 *
 *  @FileInfo:
 *  @NameInfo:
 *
 *  Return:
 *    -
 *    -
 */
static NTSTATUS InitFileInfoFields(_Inout_ PFILE_INFO FileInfo, _In_ PFLT_FILE_NAME_INFORMATION NameInfo) {
 
    NTSTATUS status;

    ULONG NameByteSize;
    ULONG FinalNameByteSize;

    status = STATUS_INSUFFICIENT_RESOURCES;

    /*
     *
     */
    FileInfo->PathSize = NameInfo->Name.Length;
    if(FileInfo->PathSize) {
        NameByteSize = FileInfo->PathSize * sizeof *FileInfo->Path;
        FileInfo->Path = ExAllocataPoolWithTag(FileInfo->PoolType, NameByteSize, 'path'); 
        if(!FileInfo->Path) {
            DbgPrint("by ExAllocatePoolWithTag(), status: 0x%X.\n", status);
            return status;
        }

        RtlCopyBytes(FileInfo->Path, NameInfo->Name.Buffer, NameByteSize);
    }

    /*
     *
     */
    FileInfo->FinalNameSize = NameInfo->FinalComponent.Length;
    if(FileInfo->FinalNameSize) {
        FinalNameByteSize = FileInfo->FinalNameSize * sizeof *FileInfo->FinalName;
        FileInfo->FinalName = ExAllocatePoolWithTag(FileInfo->PoolType, FinalNameByteSize, 'name');
        if(!FileInfo->FinalName) {
            DbgPrint("by ExAllocatePoolWithTag(), status: 0x%X.\n", status);

            /*
             *  If the allocation of 'FileInfo->FinalName' fails, we deallocate
             *  the 'FileInfo->Path'.  In these cases, there is nothing more we
             *  can do due to insufficient resources, so we discard logging the
             *  information related to the current I/O operation.
             */
            ExFreePoolWithTag(FileInfo->Path, 'path');
            FileData->Path = NULL;
            return status;
        }

        RtlCopyBytes(FileInfo->FinalName, NameInfo->FinalComponent.Buffer, FinalNameByteSize);
    }

    return STATUS_SUCCESS;
}

/*
 *  InitFileInfo() - Initializes a 'FILE_INFO' structure based on 'FLT_CALLBACK_DATA'.
 *
 *  @FileInfo: Pointer to the 'FILE_INFO' structure to be initialized.
 *  @Data: Pointer to the 'FLT_CALLBACK_DATA' structure containing file-related data.
 *
 *  This function retrieves file name information from the 'FLT_CALLBACK_DATA'
 *  and initializes the 'FILE_INFO' structure accordingly.
 *
 *  Return:
 *    - Status code indicating the result of the initialization.
 *    - Upon success, returns 'STATUS_SUCCESS'.
 *    - The appropriate error code in case of failure.
 */
NTSTATUS InitFileInfo(_Inout_ PFILE_INFO FileInfo, _In_ PFLT_CALLBACK_DATA Data) { 

    NTSTATUS status;
    PFLT_FILE_NAME_INFORMATION NameInfo;

    NameInfo = NULL;
    status = FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED | Amaterasu.FileNameQueryMethod, NameInfo);
    if(!NT_SUCCESS(status)) {

        /*
         *  If it was not possible to get the "normalized" name, we try
         *  to get the "opened" name. The opened name refers to the name of the file 
         *  as it was specified when the file was opened, including the full path
         *  if available.
         */
        status = FltGetNameInformation(Data, FLT_FILE_OPENED | Amaterasu.FileNameQueryMethod, NameInfo);
        if(!NT_SUCCESS(status)) {
            DbgPrint("by FltGetNameInformation(), status: 0x%X.\n", status);
            goto cleanup;
        }
    }

    status = FltParseNameInformation(NameInfo);
    if(!NT_SUCCESS(status)) {
        DbgPrint("by FltParseNameInformation(), status: 0x%X.\n", status);
        goto cleanup;
    }

    status = InitFileInfoFields(FileInfo, NameInfo);
    if(!NT_SUCCESS(status)) {
        DbgPrint("at InitFileInfoFields()\n");
    }

cleanup:
    if(NameInfo) { 
        /*
         *  After obtaining the 'path' and 'name' of the file, 'NameInfo'
         *  is no longer necessary and should be deallocated.
         */
        FltReleaseFileNameInformation(NameInfo);
    }

    return status;
}
