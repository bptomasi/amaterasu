
#include "procinfo.h"

/*
 *  ProcInitInfo() - Initializes a 'PROC_INFO' structure based on 
 *                   'FLT_CALLBACK_DATA'.
 *
 *  @Info: Pointer to the 'INFO' structure to be initialized.
 *  @Data: Pointer to the 'FLT_CALLBACK_DATA' structure containing process 
 *         related data.
 *
 *  Return:
 *    - Status code indicating the result of the initialization.
 *    - Upon success, returns 'STATUS_SUCCESS'.
 *    - The appropriate error code in case of failure.
 */
NTSTATUS ProcInfoInit(_Out_ PPROC_INFO ProcInfo, _In_ PFLT_CALLBACK_DATA Data) {

    NTSTATUS status;

    PEPROCESS Process;
    HANDLE Handle;

    Process = FltGetRequestorProcess(Data);
    if(!Process) {
        return 'STATUS_CODE';
    }

    ProcInfo->PID = FltGetRequestProcessId(Data);

    return status;
}
