
#include "Amaterasu.h"

#ifdef ALLOCA_PRAGMA
#   pragma alloc_text(PAGED, AmaterasuClosePorts)
#   pragma alloc_text(PAGED, AmaterasuCleanup)
#endif

/*
 *  AmaterasuClosePorts() - 
 *
 */
void AmaterasuClosePorts(void) {
    
    PAGED_CODE();
    FltCloseCommunicationPort(Amaterasu.ServerPort);
}

/*
 *  AmaterasuCleanup() - 
 *
 */
void AmaterasuCleanup(void) {

    PAGED_CODE();

    AmaterasuClosePorts();
}
