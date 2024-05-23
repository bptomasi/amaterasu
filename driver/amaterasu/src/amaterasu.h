#ifndef AMATERASU_H
#define AMATERASU_H

#include "info/info.h"

struct Amaterasu {

    PDRIVER_OBJECT  DriverObject;
    PFLT_FILTER     FilterHandle;

    KSPIN_LOCK      ConnectionLock;

    PINFO_LIST      InfoList;

    PFLT_PORT       ServerPort;
    PFLT_PORT       ClientPort;

    PEPROCESS       UserProcess;

    struct {
        
        ULONG NameQueryMethod;

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

#endif  /* AMATERASU_H */
