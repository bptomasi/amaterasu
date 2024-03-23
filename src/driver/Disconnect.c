
#include "Amaterasu.h"

#ifdef ALLOCA_PRAGMA
#   pragma alloc_text(PAGED, AmaterasuDisconnect)
#endif

/*
 *  AmaterasuDisconnect() -
 */
void AmaterasuDisconnect(_In_opt_ PVOID ConnectionCookie) {

    KIRQL oldIrql;

    UNREFERENCED_PARAMETER(ConnectionCookie);
    PAGED_CODE();

    KeAcquireSpinLock(&Amaterasu.ConnectionLock, &oldIrql);

    /*
     *  Closes a communication client port that was created by the minifilter
     *  driver's ConnectNotifyCallback routine (AmaterasuConnect), setting it
     *  to 'NULL' afterwards.
     */
    FltCloseClientPort(Amaterasu.FilterHandle, &Amaterasu.ClientPort);
    Amaterasu.UserProcess = NULL;

    KeReleaseSpinLock(&Amaterasu.ConnectionLock, oldIrql);

    DbgPrint("!!! amaterasu.sys --- disconnected, 0x%p\n", Amaterasu.ClientPort);
}
