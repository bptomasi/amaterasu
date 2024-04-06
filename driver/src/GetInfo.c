
#include "Amaterasu.h"

#ifdef ALLOCA_PRAGMA
#   pragma alloc_text(NONPAGED, AmaterasuGetInfo)
#endif

/*
 *  AmaterasuGetInfo() -
 *  
 *  @Data:
 *  @FltObjects:
 *
 *  Return:
 *    -
 *    -
 */
PDATA AmaterasuGetInfo(
        _Inout_ PFLT_CALLBACK_DATA Data,
        _In_ PCFLT_RELATED_OBJECTS FltObjects,
    ) {

    ULONG Flags;
    PDATA Info;

    Info = ExAllocatePoolWithTag(NonPagedPool, sizeof *Info, 'info');
    if(!Info) {
        DbgPrint("");
        return NULL;
    }

    /*
     *
     */
    if(FltObjects->FileObjects) {

        NameInfo = NULL;
        Flags = FltObjects->FileObject->Flags;
 
        /*
         *
         */
        Info->ProcData = AmaterasuGetProcData(Data, FltObjects);
        if(!Info->ProcData) {
            DbgPrint("");
            ExFreePoolWithTag(Info, 'info');
            return NULL;
        }

       if(FlagOn(Flags, FO_FILE_OBJECT)) {

            /*
             *
             */
            Info->FileData = AmaterasuGetFileData(Data, FltObjects);
            if(!Info->FileData) {
                DbgPrint("");
                ExFreePoolWithTag(Info, 'info');
                return NULL:
            }
        } 
        else {
            if(FlagOn(Flags, FO_NAMED_PIPE)) {

            } else {
                if(FlagOn(Flags, FO_MAILSLOT)) {

                }
            }
        }
    }

    return Info;
}
