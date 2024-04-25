
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

/*
static NTSTATUS GetRequestorProcStatus(_Out_ PBOOL ProcStatus, _In_ PFLT_CALLBACK_DATA Data) {

    NTSTATUS Status;

    return Status;
}
*/

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

static inline NTSTATUS GetEProcess(ULONG pid, PEPROCESS* eprocess) {
    return PsLookupProcessByProcessId((HANDLE)pid, eprocess);
}

/*
 *  AcquireElevationStatus() - Retrieves TOKEN_ELEVATION structure.
 *
 *  @proc: Pointer to a EPROCESS structure
 * 
 *  @tokenInfo: Pointer to TOKEN_ELEVATION to be filled
 *
 *  Return:
 *    - NTSTATUS .
 */
NTSTATUS AcquireElevationStatus(_In_ PEPROCESS proc, _Out_ TOKEN_ELEVATION* tokenInfo) {
    PACCESS_TOKEN ptoken;
    
    /*Acquires reference to the primary token*/
    ptoken= PsReferencePrimaryToken(proc);
    
    if (ptoken == NULL) {
        tokenInfo = NULL;
        return STATUS_SUCCESS;
    }

    return SeQueryInformationToken(ptoken, TokenElevation, &tokenInfo);
}

/*
 *  GetElevationStatus() - Retrieves Elevation status.
 *
 *  @proc: Pointer to a EPROCESS structure
 *
 *  Return:
 *    - NTSTATUS .
 */
NTSTATUS GetElevationStatus(_Out_ PPROC_INFO ProcInfo) {
    NTSTATUS Status;
    TOKEN_ELEVATION tokenInfo;
    PEPROCESS eprocess;
    
    Status = GetEProcess(ProcInfo->PID, &eprocess);
    if (!NT_SUCCESS(Status))
        return Status;

    Status = AcquireElevationStatus(eprocess, &tokenInfo);
    if (!NT_SUCCESS(Status))
        return Status;

    ProcInfo->IsElevated = tokenInfo.TokenIsElevated;

    return STATUS_SUCCESS;
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
    PVOID tokenInfo;
    PEPROCESS eprocess;

    ProcInfo->PID = GetRequestorPID(Data);

    Status = GetRequestorSID(&ProcInfo->PID, Data);
    if(!NT_SUCCESS(Status))
        return Status;

    Status = GetElevationStatus(ProcInfo);
    if (!NT_SUCCESS(Status))
        return Status;

    /*
    Status = GetRequestorStatus(&ProcInfo->Status, Data);
    if(!NT_SUCCESS(Status)) {
        return Status;
    }
    */

    return Status;
}

PPROC_INFO ProcInfoGet(_Out_ PPROC_INFO ProcInfo, _In_ PFLT_CALLBACK_DATA Data) {

}
