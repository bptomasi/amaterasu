
#include "Amaterasu.h"

#ifdef ALLOCA_PRAGMA
#   pragma alloc_text(PAGED, AmaterasuDisconnect)
#endif

/*
 *  AmaterasuDisconnect() -
 */
void AmaterasuDisconnect(_In_opt_ PVOID ConnectionCookie) {

    UNREFERENCED_PARAMETER(ConnectionCookie);

    PAGED_CODE();

    DbgPrint("!!! amaterasu.sys --- disconnected, 0x%p\n", Amaterasu.ClientPort);

    /*
     *  Closes a communication client port that was created by the minifilter
     *  driver's ConnectNotifyCallback routine (AmaterasyConnect), setting it
     *  to 'NULL' afterwards.
     */
    FltCloseClientPort(Amaterasu.FilterHandle, &Amaterasu.ClientPort);
    Amaterasu.UserProcess = NULL;
}
