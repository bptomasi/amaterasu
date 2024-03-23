
#include "Amaterasu.h"

static void
AmaterasuGetOperation(
        _In_ PFLT_CALLBACK_DATA Data,
        _Inout_ PUNICODE_STRING op
    );

#ifdef ALLOCA_PRAGMA
#   pragma alloc_text(NONPAGED, AmaterasuPreCallback)
#endif

#define MajorFunction(Data) Data->Iopb->MajorFunction

/*
 *  AmaterasuGetOperation() -
 *
 */
static void AmaterasuGetOperation(_In_ PFLT_CALLBACK_DATA Data, __Inout_ PUNICODE_STRING op) {

    /*
     *
     */
    switch(MajorFunction(Data)) {
        
    }
}

static void AmaterasuGetFileName(void) {

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

    /* We are not interested on file system filter callback operations */
    if(FLT_IS_FS_FILTER_OPERATION(Data)) {
        /*
         *  There's no need to call the post operation callback for
         *  futher processing.
         */
        return FLT_PREOP_SUCCESS_NO_CALLBACK;
    }

    status = FltGetFileNameInformation(Data, );    
    if(!NT_SUCCESS(status)) {

    }


    return FLT_PREOP_SUCCESS_WITH_CALLBACK;
}
