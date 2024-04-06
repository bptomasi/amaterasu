
#include "Amaterasu.h"

#ifdef ALLOCA_PRAGMA
#   pragma alloc_text(PAGED, AmaterasuClosePorts)
#   pragma alloc_text(PAGED, AmaterasuCleanup)
#endif

/*
 *  AmaterasuClosePorts() - 
 *
 *  Closes the ports opened by 'AmaterasuOpenPorts()', setting the ports to
 *  'NULL'. Besides that, in the case of 'Amaterasu.ClientPort', this port is
 *  closed automatically by the filter manager when it invokes the disconnect
 *  callback function.
 */
void AmaterasuClosePorts(void) {
    
    PAGED_CODE();

    FltCloseCommunicationPort(Amaterasu.ServerPort);
    Amaterasu.ServerPort = NULL;
}

/*
 *  AmaterasuCleanup() - 
 *
 */
void AmaterasuCleanup(void) {

    PAGED_CODE();

    AmaterasuClosePorts();
}
