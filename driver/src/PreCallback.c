
#include "Amaterasu.h"

static void
AmaterasuGetOperation(
        _In_ PFLT_CALLBACK_DATA Data,
        _Inout_ PUNICODE_STRING op
    );

#ifdef ALLOCA_PRAGMA
#   pragma alloc_text(NONPAGED, AmaterasuPreCallback)
#endif

/*
 *  AmaterasuGetOperation() -
 *
 *  @Data
 *  @Op:
 *
 */
static void AmaterasuGetOperation(_In_ PFLT_CALLBACK_DATA Data, _Inout_ PUNICODE_STRING Op) {

    /*
     *
     */
    switch(Data->Iopb->MajorFunction) {
        
    }
}


/*
 *  AmaterasuPreCallback() -
 *
 *  Return:
 *    -
 *    -
 *    -
 */
FLT_PREOP_CALLBACK_STATUS AmaterasuPreCallback(
        _Inout_ PFLT_CALLBACK_DATA Data,
        _In_ PCFLT_RELATED_OBJECTS FltObjects,
        _Flt_CompletionContext_Outptr_ PVOID *CompletionContext
    ) {

    NTSTATUS status;
    PDATA Info;

    UNREFERENCED_PARAMETER(CompletionContext);

    /* We are not interested on file system filter callback operations */
    if(FLT_IS_FS_FILTER_OPERATION(Data)) {
        /*
         *  There's no need to call the post operation callback for
         *  futher processing.
         */
        return FLT_PREOP_SUCCESS_NO_CALLBACK;
    }

    /*
     *
     */
    status = AmaterasuGetInfo(Data, FltObjects, &Info);
    if(!NT_SUCCESS(status)) {

    }

    return FLT_PREOP_SUCCESS_WITH_CALLBACK;
}
