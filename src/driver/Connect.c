
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

    PAGED_CODE();

    Amaterasu.ClientPort  = ClientPort;
    Amaterasu.UserProcess = PsGetCurrentProcess();

    DbgPrint("!!! amaterasu.sys --- connected, port=0x%p\n", ClientPort);

    return status;
}
