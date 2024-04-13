#ifndef AMATERASU_H
#define AMATERASU_H

#include "callbacks/callbacks.h"
#include "info/info.h"

struct Amaterasu {

    PDRIVER_OBJECT  DriverObject;
    PFLT_FILTER     FilterHandle;

    KSPIN_LOCK      ConnectionLock;

    LIST_ENTRY      RecordList;

    ULONG           MaxRecords;
    volatile ULONG  RecordsAllocated;
    
    PFLT_PORT       ServerPort;
    PFLT_PORT       ClientPort;

    PEPROCESS       UserProcess;

    struct {
        
        ULONG NameQueryMethod;

        BOOL NaoDeleteArquivos;

    } Options;
};

extern struct Amaterasu Amaterasu;

/*
 *	DriverEntry() - Initializes the driver upon loading into memory.
 *
 *	@DriverObject: Pointer to the driver object created by the I/O manager.
 *	@RegistryPath: Path to the driver's registry key in the registry.
 *
 *  Return:
 *    - 'STATUS_SUCCESS' on success.
 *    - The appropriate 'NTSTATUS' is returned to indicate failure.
 */
extern NTSTATUS
DriverEntry(
        _In_ PDRIVER_OBJECT DriverObject,
        _In_ PUNICODE_STRING RegistryPath
    );

extern NTSTATUS
AmaterasuSetup(
        _In_ PDRIVER_OBJECT DriverObject,
        _In_ PUNICODE_STRING RegistryPath
    );



#endif  /* AMATERASU_H */
