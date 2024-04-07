#ifndef PROC_INFO_H
#define PROC_INFO_H

typedef struct ProcInfo {

    ULONG ProcId;
    /* Thread Id? */
    ULONG SessionId;
    ULONG ParentId;

    BOOL IsElevated;

} PROC_INFO, *PPROC_INFO;

/*
 *  ProcInitInfo() - Initializes a 'PROC_INFO' structure based on 'FLT_CALLBACK_DATA'.
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
extern NTSTATUS
InitProcInfo(
        _Inout_ PPROC_INFO ProcInfo,
        _In_ PFLT_CALLBACK_DATA Data
    );

#endif  /* PROC_INFO_H */
