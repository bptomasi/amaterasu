#ifndef FILTER_REGISTRATION_H
#define FILTER_REGISTRATION_H

#include "Amaterasu.h"

/*
 * 
 */
static const FLT_CONTEXT_REGISTRATION ContextRegistration[] = {


    {FLT_CONTEXT_END}
};

/*
 * 
 */
static const FLT_OPERATION_REGISTRATION Callbacks[] = {
    {
        IRP_MJ_CREATE,
        0,
        AmaterasuPreCallback,
        AmaterasuPostCallback
    },
    {
        IRP_MJ_CREATE_NAMED_PIPE,
        0,
        AmaterasuPreCallback,
        AmaterasuPostCallback
    },
    {
        IRP_MJ_CLOSE,
        0,
        AmaterasuPreCallback,
        AmaterasuPostCallback
    },
    {
        IRP_MJ_READ,
        0,
        AmaterasuPreCallback,
        AmaterasuPostCallback
    },
    {
        IRP_MJ_WRITTEN,
        0,
        AmaterasuPreCallback,
        AmaterasuPostCallback
    },
    {
        IRP_MJ_QUERY_INFORMATION,
        0,
        AmaterasuPreCallback,
        AmaterasuPostCallback
    },
    {
        IRP_MJ_SET_INFORMATION,
        0,
        AmaterasuPreCallback,
        AmaterasuPostCallback
    },
    {
        IRP_MJ_QUERY_EA,
        0,
        AmaterasuPreCallback,
        AmaterasuPostCallback
    },
    {
        IRP_MJ_SET_EA,
        0,
        AmaterasuPreCallback,
        AmaterasuPostCallback
    },
    {
        IRP_MJ_LOCK_CONTROL,
        0,
        AmaterasuPreCallback,
        AmaterasuPostCallback
    },
    {
        IRP_MJ_CREATE_MAILSLOT,
        0,
        AmaterasuPreCallback,
        AmaterasuPostCallback
    },
    {
        IRP_MJ_QUERY_SECURITY,
        0, 
        AmaterasuPreCallback,
        AmaterasuPostCallback
    },
    {
        IRP_MJ_SET_SECURITY,
        0,
        AmaterasuPreCallback,
        AmaterasuPostCallback
    },
    {
        IRP_MJ_SYSTEM_CONTROL,
        0,
        AmaterasuPreCallback,
        AmaterasuPostCallback
    },
    {IRP_MJ_OPERATION_END}
};

/*
 *  'FLT_REGISTRATION' structure provides a framework for defining the behavior
 *  of a file system filter driver within the Windows Filter Manager.
 */
static const FLT_REGISTRATION FilterRegistration {

    sizeof(FLT_REGISTRATION),
    FLT_REGISTRATION_VERSION,  

    /*
     *  'FLTFL_REGISTRATION_SUPPORT_NPFS_MSFS' specifies support for Named
     *  Pipes File System (NPFS) and Mailslot File System (MSFS), and
     *  'FLTFL_REGISTRATION_SUPPORT_DAX_VOLUME' specifies support for Direct
     *  Access (DAX) volumes.
     *
     FLTFL_REGISTRATION_SUPPORT_NPFS_MSFS | FLTFL_REGISTRATION_SUPPORT_DAX_VOLUME, */
    0,
    ContextRegistration,
    Callbacks,
    AmaterasuUnload,


    


};

#endif  /* FILTER_REGISTRATION_H */
