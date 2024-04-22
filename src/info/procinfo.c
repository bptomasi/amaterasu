
#include "procinfo.h"

/*
 *  GetRequestorProcStatus() -
 *
 *  @ProcStatus:
 *  @Data:
 *
 *  Return:
 *    -
 *    -
 */
static NTSTATUS GetRequestorProcStatus(_Out_ PBOOL ProcStatus, _In_ PFLT_CALLBACK_DATA Data) {

    NTSTATUS Status;

    return Status;
}

/*
 *  GetRequestorSID() -
 *
 *  @SID:
 *  @Data:
 *
 *  Return:
 *    -
 *    -
 */
static inline NTSTATUS GetRequestorSID(_Out_ PULONG SID, _In_ PFLT_CALLBACK_DATA Data) {

    /*
     *  Even though 'FltGetRequestorSessionId()' may return '-1', meaning
     *  that the process has no session, we consider it a success.
     */
    return FltGetRequestorSessionId(Data, SID);
}

/*
 *  GetRequestorPID() - Retrieves the Process ID.
 *
 *  @Data: Pointer to a 'FLT_CALLBACK_DATA' structure containing information
 *         about the callback.
 *
 *  Return:
 *    - Process ID.
 */
static inline ULONG GetRequestorPID(_In_ PFLT_CALLBACK_DATA Data) {

    /*
     *  Gets the process ID for the process associated with the thread that
     *  requested the I/O operation.
     */
    return FltGetRequestorProcessId(Data);
}

/*
 *  ProcInfoInit() -
 *
 *  @ProcInfo:
 *  @Data:
 *
 *  Return:
 *    -
 *    -
 */
NTSTATUS ProcInfoInit(_Out_ PPROC_INFO ProcInfo, _In_ PFLT_CALLBACK_DATA Data) {

    NTSTATUS Status;

    ProcInfo->PID = GetRequestorPID(Data);

    Status = GetRequestorSID(&ProcInfo->PID, Data);
    if(!NT_SUCCESS(Status)) {
        return Status;
    }

    Status = GetRequestorStatus(&ProcInfo->Status, Data);
    if(!NT_SUCCESS(Status)) {
        return Status;
    }

    return Status;
}
