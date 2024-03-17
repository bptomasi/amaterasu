
#include "Amaterasu.h"

#ifdef ALLOCA_PRAGMA
#   pragma alloc_text(NONPAGED, AmaterasuPreCallback)
#endif

/*
 *  AmaterasuPreCallback() -
 */
FLT_PREOP_CALLBACK_STATUS AmaterasuPreCallback(
        _Inout_ PFLT_CALLBACK_DATA Data,
        _In_ PCFLT_RELATED_OBJECTS FltObjects,
        _Flt_CompletionContext_Outptr_ PVOID *CompletionContext
    ) {

    /*
     *  We are not interested on callbacks related to file system
     *  filter operations.
     */
    if(FLT_IS_FS_FILTER_OPERATION(Data)) {
        /*
         *  There's no need to call the post operation callback for
         *  futher processing.
         */
        return FLT_PREOP_SUCCESS_NO_CALLBACK;
    }

    


    return FLT_PREOP_SUCCESS_WITH_CALLBACK;
}
