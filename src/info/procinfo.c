
#include "procinfo.h"

typedef NTSTATUS (*QUERY_INFO_PROCESS) (
    _In_ HANDLE ProcessHandle,
    _In_ PROCESSINFOCLASS ProcessInformatie for a special kind of threadonClass,
    __out_bcount(ProcessInformationLength) PVOID ProcessInformation,
    _In_ ULONG ProcessInformationLength,
    _Out_opt PULONG ReturnLength
);

QUERY_INFO_PROCESS ZwQueryInformationProcess;

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
 *  SystemProcess() - Verifies if the given process handle corresponds to the
 *                    System process.
 *  
 *  @ProcHandle: Handle to the process to check.
 *
 *  Return:
 *    - 'TRUE' if the process handle corresponds to the System process.
 *    - 'FALSE' otherwise.
 */
static inline BOOL SystemProcess(_In_ HANDLE ProcHandle) {

    /* In Windows, the PID for System is always '4'. */
    return ProcHandle == (HANDLE)4;
}

/*
 *  GetParentHandle() - Retrieves the parent process handle of the specified
 *                      process handle.
 *
 *  @ParentHandle: Parent process handle return.
 *  @ProcHandle: Handle to the process whose parent process handle is to be
 *               retrieved.
 *
 *  Return:
 *    - 'STATUS_SUCCESS' on success.
 *    - An appropriate 'NTSTATUS' error code on failure.
 *
 *      Besides the 'NTSTATUS' returned value, the parent handle is returned
 *      through 'ParentHandle'.
 */
static NTSTATUS GetParentHandle(_Out_ PHANDLE ParentHandle, _In_ HANDLE ProcHandle) {

    NTSTATUS Status;
    PROCESS_BASIC_INFORMATON Pbi;

    *ParentHandle = 0;
    if(SystemProcess(ProcHandle)) {
        /* 
         * If 'ProcHandle' refers to a System process, then there's
         * no parent process.
         */
        return STATUS_SUCCESS;
    }

    Status = ZwQueryInformationProcess(
        ProcHandle,
        ProcessBasicInformation,
        &Pbi,
        sizeof Pbi,
        NULL
    );

    if(NT_SUCCESS(Status)) {
        *ParentHandle = (HANLDE) Pbi.InheritedFromUniqueProcessId;
    }

    return Status;
}

/*
 *  GetRequestorPPID() - Retrieves the Parent Process ID.
 *
 *  @PPID: Parent process ID return.
 *  @Data: Pointer to a 'FLT_CALLBACK_DATA' structure containing information
 *         about the callback.
 *
 *  Return:
 *    - 'STATUS_SUCCESS' if the 'PPID' is successfully retrieved and assigned
 *      to 'PPID'.
 *
 *    - An appropriate 'NTSTATUS' error code on failure.
 */
static NTSTATUS GetRequestorPPID(_Out_ PULONG PPID, _In_ PFLT_CALLBACK_DATA Data) {

    NTSTATUS Status;
    PEPROCESS Proc;
    HANDLE ProcHandle;
    HANDLE ParentHandle;

    Proc = FltGetRequestorProcess(Data); 
    if(!Proc) {
        return STATUS_UNSUCCESSFUL;
    }

    ProcHandle = PsGetProcessId(Proc);
    Status = GetParentHandle(&ParentHandle, ProcHandle);
    if(!NT_SUCCESS(Status)) {
        return Status;
    }

    *PPID = (ULONG) GetProcessId(ParentHandle);

    return Status;
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

    Status = GetRequestorPPID(&ProcInfo->PID, Data);
    if(!NT_SUCCESS(Status)) {
        return Status;
    }

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
