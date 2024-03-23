
#include "Amaterasu.h"

#ifdef ALLOCA_PRAGMA
#   pragma alloc_text(PAGED, AmaterasuConnect)
#endif

/*
 *  AmaterasuConnect() - 
 *
 *  @ClientPort:
 *  @ServerPort:
 *  @ConnectionContext:
 *  @SizeOfContext:
 *  @ConnectionCookie:
 *
 *  Return:
 *    -
 *    -
 */
NTSTATUS AmaterasuConnect(
        _In_ PFLT_PORT ClientPort,
        _In_ PVOID ServerPortCookie,
        _In_reads_bytes_(SizeOfContext) PVOID ConnectionContext,
        _In_ ULONG SizeOfContext,
        _Flt_ConnectionCookie_Outptr_ PVOID* ConnectionCookie
    ) {

    NTSTATUS status;
    KIRQL oldIrql;

    UNREFERENCED_PARAMETER(ServerPortCookie);
    UNREFERENCED_PARAMETER(ConnectionContext);
    UNREFERENCED_PARAMETER(SizeOfContext);
    UNREFERENCED_PARAMETER(ConnectionCookie);

    PAGED_CODE();

    /*  
     *  Initialize the spin lock to ensure both thread synchronization and
     *  synchronized access to the 'Amaterasu.ClientPort' and
     *  'Amaterasu.UserProcess' fields.
     */
    KeAcquireSpinLock(&Amaterasu.ConnectionLock, &oldIrql);

    Amaterasu.ClientPort  = ClientPort;
    Amaterasu.UserProcess = PsGetCurrentProcess();

    KeReleaseSpinLock(&Amaterasu.ConnectionLock, oldIrql):

    DbgPrint("!!! amaterasu.sys --- connected, port=0x%p\n", ClientPort);

    return status;
}
