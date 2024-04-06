
#include "Amaterasu.h"

#ifdef ALLOCA_PRAGMA
#   pragma alloc_text(NONPAGED, AmaterasuGetFileData)
#endif

/*
 *  AmaterasuGetFileData() -
 *
 *  @Data:
 *  @FltObjects:
 *
 *  Return:
 *    -
 *    -
 */
PFILE_DATA AmaterasuGetFileData(
        _Inout_ PFLT_CALLBACK_DATA Data,
        _In_ PCFLT_RELATED_OBJECTS FltObjects,
    ) {

    NTSTATUS status;
    PFILE_DATA FileData;

    FileData = ExAllocatePoolWithTag(NonPagedPool, sizeof *FileData, 'fdta');
    if(!FileData) {
        DbgPrint("");
        return NULL;
    }

    /*
     *
     */
    status = AmaterasuGetFileNameInfo(Data, FltObjects, FileData);
    if(NT_SUCCESS(status)) {
        DbgPrint("");
        ExFreePoolWithTag(FileData, 'fdta');
        return NULL;
    }


    return FileData;
}
