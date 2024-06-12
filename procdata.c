
#include "procdata.h"
#include "utils.h"

/*
 *  ProcDataAlloc() - Allocate a 'PROC_DATA' structure.
 *
 *  @PoolType: The type of memory pool to allocate from (paged or nonpaged).
 *
 *  Return:
 *    - Pointer to the allocated 'PROC_DATA' structure on success.
 *    - 'NULL' if memory allocation fails.
 */
PPROC_DATA ProcDataAlloc(_In_ POOL_TYPE PoolType) {

    PPROC_DATA ProcData;

    ProcData = ExAllocatePoolWithTag(PoolType, sizeof * ProcData, 'prnf');
    if (!ProcData) {
        return NULL;
    }

    RtlZeroMemory(ProcData, sizeof * ProcData);

    /*
     *  Store the pool type used for the allocation in 'ProcData' to ensure
     *  correct memory handling, regardless of whether the caller routine is
     *  paged or nonpaged.
     */
    ProcData->PoolType = PoolType;

    return ProcData;
}

/*
 *  ProcDataGet() - Allocates and initializes a 'FILE_INFO' structure.
 *
 *  @PoolType: The type of memory pool to allocate from (paged or nonpaged).
 *  @Data: Pointer to a IDENTIFIR struct containing process related data.
 *
 *  'ProcDataGet()' is a wrapper for 'ProcDataAlloc()' and 'ProcDataInit()',
 *  providing a simple interface to obtain an allocated and initialized
 *  'PROC_DATA' structure.
 *
 *  Returns:
 *    - Pointer to the allocated 'PROC_DATA' structure on success.
 *    - 'NULL' if memory allocation or initialization fails.
 */
PPROC_DATA ProcDataGet(_In_ POOL_TYPE PoolType, _In_ PIDENTIFIER Data) {

    NTSTATUS Status;
    PPROC_DATA ProcData;

    ProcData = ProcDataAlloc(PoolType);
    if (!ProcData) {
        DbgPrint("ProcDataAlloc failed\n");
        return NULL;
    }

    Status = ProcDataInit(ProcData, Data);
    if (!NT_SUCCESS(Status)) {
        DbgPrintSt("ProcDataInit failed", Status);
        ProcDataFree(&ProcData);
    }

    return ProcData;
}

/*
 *  GetImageName() -
 *
 *  @ID:
 *  @ImageBuf:
 *
 *  Return:
 *    -
 *    -
 */
static NTSTATUS GetImageName(_In_ BOOLEAN isThread,_In_ HANDLE ID, _Inout_ WCHAR ImageBuf[MAX_PATH]) {

    NTSTATUS Status;
    PEPROCESS eProc;
    UNICODE_STRING ImageName, *pImageName;
    ULONG ImageNameSize = MAX_PATH;

    pImageName = &ImageName;
    InitUnicodeString(pImageName);


    Status = PsLookupProcessByProcessId(ID, &eProc);
    if (!NT_SUCCESS(Status)) {
        DbgPrintSt("PsLookupProcessByProcessId failed no get image name %d", isThread,Status);
        return Status;
    }

    Status = SeLocateProcessImageName(eProc, &pImageName);
    if (!NT_SUCCESS(Status)) {
        DbgPrintSt("SeLocalteProcessImageName failed", Status);
        ObDereferenceObject(eProc);
        return Status;
    }
    
    UnicodeStrToStaticWSTR(ImageBuf, pImageName, &ImageNameSize);

    ExFreePoolWithTag(ImageName.Buffer,'buf');

    ObDereferenceObject(eProc);

    return Status;
}

/*
 *  ProcInitInfo() - Initializes a 'PROC_DATA' struct.
 *
 *  @ProcData: Pointer to the 'PROC_DATA' structure to be initialized.
 *  @Data: Pointer to a 'IDENTIFIER' structure containing process
 *         related data.
 *
 *  Return:
 *    - Status code indicating the result of the initialization.
 *    - Upon success, returns 'STATUS_SUCCESS'.
 *    - The appropriate error code in case of failure.
 */
NTSTATUS ProcDataInit(_Out_ PPROC_DATA ProcData, _In_ PIDENTIFIER Data) {

    NTSTATUS Status;

    Status = TokenInfoGet(&ProcData->TokenInfo, Data->PPID);
    if (!NT_SUCCESS(Status)) {
        DbgPrintSt("TokenInfoGet failed", Status);
        return Status;
    }

    RtlCopyMemory(&ProcData->Ids, Data, sizeof ProcData->Ids);

    DbgPrint("Active: %d\n", Data->Active);
	Status = GetImageName(ProcData->Ids.isThread,Data->PPID, ProcData->ParentName);
	if (!NT_SUCCESS(Status)) {
		DbgPrintSt("GetImageName 1 failed -- %ws", ProcData->ParentName, Status);
	}
	else {
		DbgPrintSt("GetImageName 1 succeded", Status);
	}

 
    if (!ProcData->Ids.isThread) {
        Status = GetImageName(ProcData->Ids.isThread, Data->Id.ID, ProcData->ChildName);
        if (!NT_SUCCESS(Status)) {
            DbgPrintSt("GetImageName 2 failed", Status);
        }
        else {
            DbgPrintSt("GetImageName 2 succeded", Status);
        }
    }

    return Status;
}

/*
 *  ProcDataFree() - Deallocates the memory associated with a 'PROC_DATA'
 *                   struct dynamically allocated.
 *
 *  @ProcData: Pointer to a reference of a 'PROC_DATA' structure.
 */
void ProcDataFree(_Inout_ PPROC_DATA* ProcData) {

    if (ProcData && *ProcData) {
        ExFreePoolWithTag(*ProcData, 'prnf');

        /*
         *  In order to avoid a dangling pointer, after deallocating the
         *  'PROC_INFO' structure, we set the reference to the 'PROC_INFO'
         *  struct to NULL.
         */
        *ProcData = NULL;
    }
}

/*
 *  IdentifierCopy() -
 *
 *  @Dest:
 *  @Src:
 */
static void IdentifierCopy(_Out_ PIDENTIFIER Dest, _In_ PIDENTIFIER Src) {

    if (Dest && Src) {

        RtlCopyMemory(&Dest->PPID, &Src->PPID, sizeof Dest->PPID);
        RtlCopyMemory(&Dest->Id.ID, &Src->Id.ID, sizeof Dest->Id.ID);
        RtlCopyMemory(&Dest->isThread, &Src->isThread, sizeof Dest->isThread);
        RtlCopyMemory(&Dest->Active, &Src->Active, sizeof Dest->Active);
    }
}

/*
 *  ProcDataCopy() -
 *
 *  @Dest:
 *  @Src:
 */
void ProcDataCopy(_Out_ PPROC_DATA_STATIC Dest, _In_ PPROC_DATA Src) {

    if (Dest && Src) {

        IdentifierCopy(&Dest->Ids, &Src->Ids);
        TokenInfoCopy(&Dest->TokenInfo, &Src->TokenInfo);
        RtlCopyMemory(Dest->ParentName, Src->ParentName, sizeof Dest->ParentName);
        RtlCopyMemory(Dest->ChildName, Src->ChildName, sizeof Dest->ChildName);
    }
}
