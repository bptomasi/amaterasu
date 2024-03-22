
#include "Amaterasu.h"
#include "FilterRegistration.h"

#ifdef ALLOC_PRAGMA
#   pragma alloc_text(INIT, DriverEntry)
#endif

/*
 *	DriverEntry() - 
 *
 *	@DriverObject:
 *	@RegistryPath:
 *
 *  Return:
 *    -
 *	  -
 */
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {

    NTSTATUS status;

    /*
     *  Makes non-paged pools (kernel pools) allocations non-executable.
     */
    ExInitializeDriverRuntime(DrvRtPoolNxOptIn);

    /*
     *  Adds 'Amaterasu' to the global list of minifilter drivers, as well as
     *  provides the Filter Manager with a list of callback functions and other
     *  information about the minifilter driver.
     *
     *  'FltRegisterFilter()' sets 'Amaterasu.FilterHandle'.
     */
    status = FltRegisterFilter(DriverObject, &FilterRegistration, &Amaterasu.FilterHandle);
    if(!NT_SUCCESS(status)) {
        DbgPrint("FltRegisterFilter()\n%s\nError!\n", __function__);
        return status;
    }

    status = AmaterasuSetup(RegistryPath);
    if(!NT_SUCCESS(status)) {
        DbgPrint("AmaterasuSetup()\n%s\nError!\n", __function__);

        /*
         *  If something has gone wrong, 'FltUnregisterFilter()' will
         *  unregister 'Amaterasu' from the Filter Manager so that it doesn't
         *  receive any further callback requests or filtering operations.
         *
         *  This function will be called from 'DriverUnload()' as well.
         */ 
        FltUnregisterFilter(Amaterasu.FilterHandle);
        return status;
    }

    /*
     *  'FltStartFiltering()' notifies the Filter Manager that 'Amaterasu' is
     *  ready to begin attaching to volumes and filtering I/O requests.
     *
     *  After this function call, the Filter Manager will treat the minifilter
     *  driver as being fully active, presenting it with volumes to attach to,
     *  as well as I/O requests.
     */
    status = FltStartFiltering(Amaterasu.FilterHandle);
    if(!NT_SUCCESS(status)) {
        DbgPrint("FltStartFiltering()\n%s\nError!\n", __function__);
        AmaterasuCleanup();
        FltUnregisterFilter(Amaterasu.FilterHandle);
    }

    return status;
}
