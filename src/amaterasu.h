#ifndef AMATERASU_H
#define AMATERASU_H

#include "callbacks/callbacks.h"
#include "info/info.h"

struct Amaterasu {

    PDRIVER_OBJECT  DriverObject;
    PFLT_FILTER     FilterHandle;

    KSPIN_LOCK      ConnectionLock;

    LIST_ENTRY      RecordList;
    
    PFLT_PORT       ServerPort;
    PFLT_PORT       ClientPort;

    PEPROCESS       UserProcess;

    struct {
        
        ULONG FileNameQueryMethod;

    } Options;

};

extern struct Amaterasu Amaterasu;

#endif  /* AMATERASU_H */
