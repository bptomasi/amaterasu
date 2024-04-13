
#include "amaterasu.h"

#ifdef ALLOCA_PRAGMA
#   pragma alloc_text(PAGED, AmaterasuDisconnect)
#endif

/*
 *  AmaterasuDisconnect() - Disconnects the user mode application from the
 *                          minifilter.
 *
 *  @ConnectionCookie: An optional pointer to a connection context.
 *                     This parameter is not used in this function.
 *
 *  'AmaterasuDisconnect()' is responsible for closing the communication client
 *  port that was created by 'AmaterasuConnect()'. Besides that, this function
 *  is automatically called before unloading the driver.
 */
void AmaterasuDisconnect(_In_opt_ PVOID ConnectionCookie) {

    KIRQL oldIrql;

    UNREFERENCED_PARAMETER(ConnectionCookie);
    PAGED_CODE();

    KeAcquireSpinLock(&Amaterasu.ConnectionLock, &oldIrql);

    /*
     *  Closes a communication client port that was created by the minifilter
     *  driver's ConnectNotifyCallback routine (AmaterasuConnect).
     */
    FltCloseClientPort(Amaterasu.FilterHandle, &Amaterasu.ClientPort);
    Amaterasu.UserProcess = NULL;

    KeReleaseSpinLock(&Amaterasu.ConnectionLock, oldIrql);

    DbgPrint("amaterasu.sys --- disconnected, 0x%p\n", Amaterasu.ClientPort);
}
