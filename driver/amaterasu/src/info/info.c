
#include "info.h"

/*
 *  InfoAlloc() - Allocate a 'INFO' structure.
 *
 *  @PoolType: The type of memory pool to allocate from (paged or nonpaged).
 *
 *  Return:
 *    - Pointer to the allocated 'INFO' structure on success.
 *    - 'NULL' if memory allocation fails.
 */
PINFO InfoAlloc(_PoolType_ POOL_TYPE PoolType) {

    PINFO Info;

    Info = ExAllocatePoolWithTag(PoolType, sizeof *Info, 'info');
    if(!Info) {
        return NULL;
    }

    RtlZeroMemory(Info, sizeof *Info);

    /*
     *  Store the pool type used for the allocation in 'Info' to ensure 
     *  correct memory handling, regardless of whether the caller routine is 
     *  paged or nonpaged.
     */
    Info->PoolType = PoolType;

    return Info;
}

/*
 *  InfoGet() - Allocates and initializes a 'INFO' structure.
 *
 *  @PoolType: The type of memory pool to allocate from (paged or nonpaged).
 *  @Data: Pointer to a FLT_CALLBACK_DATA object.
 *
 *  'InfoGet()' is a wrapper for 'InfoAlloc()' and 'InfoInit()',
 *  providing a simple interface to obtain an allocated and initialized
 *  'INFO' structure.
 *
 *  Returns:
 *    - Pointer to the allocated 'INFO' structure on success.
 *    - 'NULL' if memory allocation or initialization fails.
 */
PINFO InfoGet(_PoolType_ POOL_TYPE PoolType, _In_ PFLT_CALLBACK_DATA Data) {

    PINFO Info;
    NTSTATUS Status;

    Info = InfoAlloc(PoolType);
    if(!Info) {
        return NULL;
    }

    Status = InfoInit(Info, Data);
    if(!NT_SUCCESS(Status)) {
        InfoFree(&Info);
    }

    return Info;
}

/*
 *  InitTimeFields() - Initializes a TIME_FIELDS structure with the current
 *                     local time.
 *
 *  @TimeFields: Pointer to the TIME_FIELDS structure to initialize.
 *
 */
static inline void InitTimeFields(_Out_ PTIME_FIELDS TimeFields) {

    LARGE_INTEGER SysTime;
    LARGE_INTEGER LocalTime;

    KeQuerySystemTime(&SysTime);
    ExSystemTimeToLocalTime(&SysTime, &LocalTime);
    RltTimeToTimeFields(&LocalTime, TimeStamp);
}

/*
 *  InfoInit() - Initializes a 'INFO' structure allocate by 'InfoAlloc()'
 *               based on 'FLT_CALLBACK_DATA'.
 *
 *  @Info: Pointer to the 'INFO' structure to be initialized.
 *  @Data: Pointer to the 'FLT_CALLBACK_DATA' structure containing file-related
 *         and process related data.
 *
 *  Return:
 *    - Status code indicating the result of the initialization.
 *    - Upon success, returns 'STATUS_SUCCESS'.
 *    - The appropriate error code in case of failure.
 */
NTSTATUS InfoInit(_Out_ PINFO Info, _In_ PFLT_CALLBACK_DATA Data) {
    
    NTSTATUS Status;

    InitTimeFields(&Info->TimeFields);

    Status = MjFuncInit(&Info->MjFunc, Data->Iopb->MajorFunction);
    if(!NT_SUCCESS(Status)) {
        return Status;
    }

    Info->ProcInfo = ProcInfoGet(Info->PoolType, Data);
    if(!Info->ProcInfo) {
        return STATUS_UNSUCCESSFUL;
    }

    return Status;
}

/*
 *  InfoDeInit() - Deinitializes and frees the internal resources
 *                 associated with a 'INFO' structure.
 *
 *  @Info: Pointer to the 'INFO' structure to be deinitialized.
 */
void InfoDeInit(_Inout_ PINFO Info) {

    if(Info) {
        FileInfoFree(&Info->Info.FileInfo);
        ProcInfoFree(&Info->ProcInfo);
    }
}

/*
 *  InfoFree() - Deallocates the memory associated with a 'INFO'
 *               struct dynamically allocated.
 *
 *  @Info: Pointer to a reference of a 'INFO' structure.
 */
void InfoFree(_Inout_ PINFO* Info) {

    if(Info) {
        InfoDeInit(*Info);
        ExFreePoolWithTag(*Info, 'info');

       /*
        *  In order to avoid a dangling pointer, after deallocating the
        *  'INFO' structure, we set the reference to the 'INFO'
        *  struct to NULL.
        */
        *Info = NULL;
    }
}
