
#include "procinfo.h"

#ifdef ALLOC_PRAGMA
#   pragma alloc_text(NONPAGED, InitProcInfo)
#endif

static NTSTATUS GetParentProcId(_Inout_ ) {

}

/*
 *  InitProcInfo() -
 *
 *  @ProcInfo:
 *  @Data:
 *
 *  Return:
 *    -
 *    -
 */
NTSTATUS InitProcInfo(
        _Inout_ PPROC_INFO ProcInfo, 
        _In_ PFLT_CALLBACK_DATA Data,
    ) {

    NTSTATUS status;

    PEPROCESS Process;

    ProcInfo->ProcId = FltGetRequestProcessId(Data);
    Process = IoThreadToProcess(Data->Thread);

    return status;
}
