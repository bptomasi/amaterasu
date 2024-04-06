
#include "Amaterasu.h"

static NTSTATUS
AmaterasuInitFileData(
        _Inout_ PFILE_DATA FileData,
        _In_ PFLT_FILE_NAME_INFORMATION NameInfo
    );

#ifdef ALLOCA_PRAGMA
#   pragma alloc_text(NONPAGED, AmaterasuInitFileData)
#   pragma alloc_text(NONPAGED, AmaterasuGetFileNameInfo)
#endif

/*
 *  AmaterasuInitFileData() -
 *  
 *  @FileData:
 *  @NameInfo:
 *
 *  Return:
 *    -
 *    -
 */
static NTSTATUS AmaterasuInitFileData(
        _Inout_ PFILE_DATA FileData,
        _In_ PFLT_FILE_NAME_INFORMATION NameInfo
    ) {

    NTSTATUS status;
    ULONG NameByteSize;
    ULONG FinalNameByteSize;

    status = STATUS_INSUFFICIENT_RESOURCES;

    /*
     *
     */
    FileData->PathSize = NameInfo->Name.Length;
    if(FileData->PathSize) {
        NameByteSize = FileData->PathSize * sizeof *FileData->Path;
        FileData->Path = ExAllocatePoolWithTag(NonPagedPool, NameByteSize, 'path');
        if(!FileData->Path) {
            DbgPrint("");
            FileData->Path = NULL;
            return status;
        }

        /*
         *
         */
        RtlCopyBytes(FileData->Path, NameInfo->Name.Buffer, NameByteSize);
    }

    /*
     *
     */
    FileData->FinalNameSize = NameInfo->FinalComponenet.Length;
    if(FileData->FileNameSize) {
        FinalNameByteSize = FileData->FinalNameSize * sizeof *FileData->FinalName;
        FileData->FinalName = ExAllocatePoolWithTag(NonPagedPool, FinalNameByteSize, 'fnme');
        if(!FileData->FinalName) {
            DbgPrint("");
            ExFreePoolWithTag(FileData->Path, 'path');
            FileData->Path = NULL;
            return status;
        }
        RtlCopyBytes(FileData->FinalName, NameInfo->FinalComponent.Buffer, FinalNameByteSize);
    }

    return STATUS_SUCCESS;
}

/*
 *  AmaterasuGetFileNameInfo() -
 *
 *  @Data:
 *  @FltObjects:
 *  @FileData:
 *
 *  Return:
 *    -
 *    -
 */
NTSTATUS AmaterasuGetFileNameInfo(
        _Inout_ PFLT_CALLBACK_DATA Data,
        _In_ PCFLT_RELATED_OBJECTS FltObjects,
        _Inout_ PFILE_DATA FileData
    ) {

    NTSTATUS status;
    PFLT_FILE_NAME_INFORMATION NameInfo;

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
            DbgPrint("");
            goto cleanup;
        }
    }

    status = FltParseNameInformation(NameInfo);
    if(!NT_SUCCESS(status)) {
        DbgPrint("");
        goto cleanup;
    }

    /*
     *
     */
    status = AmaterasuInitFileData(FileData, NameInfo);

cleanup:
    if(NameInfo) {
        FltReleaseFileNameInformation(NameInfo);
    }

    return status;
}
