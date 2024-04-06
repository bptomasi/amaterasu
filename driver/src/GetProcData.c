
#include "Amaterasu.h"

/*
 *  AmaterasuGetProcData() -
 *
 *  @Data:
 *  @FltObjects:
 *
 *  Return:
 *    -
 *    -
 */
PPROC_DATA AmaterasuGetProcData(
        _Inout_ PFLT_CALLBACK_DATA Data,
        _In_ PCFLT_RELATED_OBJECTS FltObjects
    ) {

    PPROC_DATA ProcData;
    PEPROCESS Process;

    ProcData = ExAllocatePoolWithTag(NonPagedPool, sizeof *ProcData, 'proc');
    if(!ProcData) {
        DbgPrint("");
        return NULL;
    }

    Process = IoThreadToProcess(Data->Thread);

    return ProcData;
}
