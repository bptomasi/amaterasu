#ifndef AMATERASU_H
#define AMATERASU_H

/*
 *
 */
struct Amaterasu {

    PDRIVER_OBJECT  DriverObject;
    PFLT_FILTER     FilterHandle;

    KSPIN_LOCK      ListLock;
    KSPIN_LOCK      ConnectionLock;

    PFLT_PORT       ServerPort;
    PFLT_PORT       ClientPort;

    PEPROCESS       UserProcess;

    LIST_ENTRY      RegistryList;
    LIST_ENTRY      ProcessList;
    LIST_ENTRY      FileList;

    LONG            MaxRecords;
    volatile LONG   RecordsAllocated;
};

extern struct Amaterasu Amaterasu;

/*
 *  DriverEntry() -
 */
extern NTSTATUS
DriverEntry(
        _In_ PDRIVER_OBJECT DriverObject,
        _In_ PUNICODE_STRING RegistryPath
        );

/*
 *  AmaterasuSetup() -
 */
extern NTSTATUS
AmaterasuSetup(
        _In_ PUNICODE_STRING RegistryPath
        );  

/*
 *  AmaterasuConnect() -
 */
extern NTSTATUS 
AmaterasuConnect(
        _In_ PFLT_PORT ClientPort,
        _In_ PVOID ServerPortCookie,
        _In_reads_bytes_(SizeOfContext) PVOID ConnectionContext,
        _In_ ULONG SizeOfContext,
        _Flt_ConnectionCookie_Outptr_ PVOID* ConnectionCookie
    );

/*
 *  AmaterasuUnload() - 
 */
extern NTSTATUS 
AmaterasuUnload(
        _In_ FLT_FILTER_UNLOAD_FLAGS Flags
        );

/*
 *  AmaterasuPreCallback() -
 */
extern FLT_PREOP_CALLBACK_STATUS
AmaterasuPreCallback(
        _Inout_ PFLT_CALLBACK_DATA Data,
        _In_ PCFLT_RELATED_OBJECTS FltObjects,
        _Flt_CompletionContext_Outptr_ PVOID *CompletionContext
    );

/*
 *  AmaterasuClosePorts() - 
 */
extern void
AmaterasuClosePorts(
        void
        );

/*
 *  AmaterasuCleanup() - 
 */
extern void 
AmaterasuCleanup(
        void
        );

#endif  /* AMATERASU_H */
