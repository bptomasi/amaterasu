#ifndef CALLBACKS_H
#define CALLBACKS_H

extern FLT_PREOP_CALLBACK_STATUS
AmaterasuDefaultPreCallback(
        _Inout_ PFLT_CALLBACK_DATA Data,
        _In_ PCFLT_RELATED_OBJECTS FltObjects,
        _Flt_CompletionContext_Outptr_ PVOID *CompletionContext
    );

#endif  /* CALLBACKS_H */
