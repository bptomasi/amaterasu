
#include "amaterasu.h"

#ifdef ALLOC_PRAGMA
#   pragma alloc_text(INIT, DriverEntry)
#endif

static const FLT_OPERATION_REGISTRATION Callbacks[] = {
    {
        IRP_MJ_CREATE,
        0,
        AmaterasuFileDefaultPreCallback,
        AmaterasuFileDefaultPosCallback
    },
    {
        IRP_MJ_READ,
        0,
        AmaterasuFileDefaultPreCallback,
        AmaterasuFileDefaultPosCallback
    },
    {
        IRP_MJ_WRITE,
        0,
        AmaterasuFileDefaultPreCallback,
        AmaterasuFileDefaultPosCallback
    },
    {IRP_MJ_OPERATION_END}
};

/*
 *  'FLT_REGISTRATION' structure provides a framework for defining the behavior
 *  of a file system filter driver within the Windows Filter Manager.
 */
static const FLT_REGISTRATION FilterRegistration = {
    
    sizeof(FLT_REGISTRATION),
    FLT_REGISTRATION_VERSION,  

    /*
     *  'FLTFL_REGISTRATION_SUPPORT_NPFS_MSFS' specifies support for Named
     *  Pipes File System (NPFS) and Mailslot File System (MSFS), and
     *  'FLTFL_REGISTRATION_SUPPORT_DAX_VOLUME' specifies support for Direct
     *  Access (DAX) volumes.
     */
    FLTFL_REGISTRATION_SUPPORT_NPFS_MSFS | FLTFL_REGISTRATION_SUPPORT_DAX_VOLUME,
    ContextRegistration,
    Callbacks,
    AmaterasuUnload,

};

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
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {

    NTSTATUS status;

    /* Makes non-paged pools (kernel pools) allocations non-executable. */
    ExInitializeDriverRuntime(DrvRtPoolNxOptIn);

    /* Adds 'Amaterasu' to the global list of minifilter drivers. */
    status = FltRegisterFilter(DriverObject, &FilterRegistration, &Amaterasu.FilterHandle);
    if(!NT_SUCCESS(status)) {
        return status;
    }

    status = AmaterasuSetup(DriverObject, RegistryPath);
    if(!NT_SUCCESS(status)) {
        FltUnregisterFilter(Amaterasu.FilterHandle);
        return status;
    }

    /*
     *  'FltStartFiltering()' notifies the Filter Manager that 'Amaterasu' is
     *  ready to begin attaching to volumes and filtering I/O requests.
     */
    status = FltStartFiltering(Amaterasu.FilterHandle);
    if(!NT_SUCCESS(status)) {
        AmaterasuCleanup();
        FltUnregisterFilter(Amaterasu.FilterHandle);
    }

    return status;
}
