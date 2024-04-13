
#include "fileinfo.h"

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
static NTSTATUS InitFileInfoFields(_Out_ PFILE_INFO FileInfo, _In_ PFLT_FILE_NAME_INFORMATION NameInfo) {

    NTSTATUS status;

    status = UnicodeStrToWSTR(FileInfo->PoolType, &NameInfo->Name, &FileInfo->Path, &FileInfo->PathSize);
    if(NT_SUCCESS(status)) {
        return status;
    }

    status = UnicodeStrToWSTR(FileName->PoolType, &NameInfo->FinalComponent, &FileInfo->FinalName, &FileInfo->FinalNameSize);
    if(!NT_SUCCESS(status)) {
        ExFreePoolWithTag(FileInfo->FinalName, 'wstr');
        return status;
    }

    return status;
}

/*
 *  GetNameInfo() -
 *
 *  @NameInfo:
 *  @Data:
 *  @QueryMethod:
 *
 *  Return:
 *    -
 *    -
 */
static NTSTATUS GetNameInfo(_Out_ PFLT_FILE_NAME_INFORMATION NameInfo, _In_ PFLT_CALLBACK_DATA Data, _In_ ULONG QueryMethod) {

    NTSTATUS status;

    status = FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED | QueryMethod, NameInfo);
    if(!NT_SUCCESS(status)) {

        /*
         *  If it was not possible to get the "normalized" name, we try
         *  to get the "opened" name. The opened name refers to the name of the file 
         *  as it was specified when the file was opened, including the full path
         *  if available.
         */
        status = FltGetNameInformation(Data, FLT_FILE_OPENED | QueryMethod, NameInfo);
        if(!NT_SUCCESS(status)) {
            return status;
        }
    }

    status = FltParseNameInformation(*pNameInfo);
    if(!NT_SUCCESS(status)) {
        FltReleaseFileNameInformation(NameInfo);
    }

    return status;
}

/*
 *  FileInfoInit() - Initializes a 'FILE_INFO' structure allocate by 'FileInfoAlloc()'
 *                   based on 'FLT_CALLBACK_DATA'.
 *
 *  @FileInfo: Pointer to the 'FILE_INFO' structure to be initialized.
 *  @NameQueryMethod: 
 *  @Data: Pointer to the 'FLT_CALLBACK_DATA' structure containing file-related 
 *         and process related data.
 *
 *  Return:
 *    - Status code indicating the result of the initialization.
 *    - Upon success, returns 'STATUS_SUCCESS'.
 *    - The appropriate error code in case of failure.
 */
NTSTATUS FileInfoInit(_Out_ PFILE_INFO FileInfo, _In_ ULONG NameQueryMethod, _In_ PFLT_CALLBACK_DATA Data) {

    NTSTATUS status;
    PFLT_FILE_NAME_INFORMATION NameInfo;

    status = GetNameInfo(Data, NameQueryMethod);
    if(!NT_SUCCESS(status)) {
        return status;
    }

    status = InitFileInfoFields(FileInfo, NameInfo);

    /* As 'NameInfo' is no longer necessary, we deallocate it. */
    FltReleaseFileNameInformation(NameInfo); 

    return status;
}
