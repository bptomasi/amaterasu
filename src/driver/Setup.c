#include "Amaterasu.h"

#define AMATERASU_SERVER_PORT L"AmaterasuServerPort"

/* 
 * The 'static' function prototypes belos are just 
 * for the pragma as 'alloc_text' must be placed between a function
 * declarator and the function definition.
 */
static NTSTATUS
AmaterasuCreatePort(
        _In_ PSECURITY_DESCRIPTOR sd,
        _In_ PCWSTR PortName,
        _In_ PFLT_PORT Port
    );

static NTSTATUS
AmaterasuOpenPorts(
        void
    );

#ifdef ALLOCA_PRAGMA
#   pragma alloc_text(INIT, AmaterasuCreatePort)
#   pragma alloc_text(INIT, AmaterasuOpenPorts)
#   pragma alloc_text(INIT, AmaterasuSetup)
#endif

/*
 *  AmaterasuCreatePort() -
 *
 *  Return:
 *    -
 *    -
 */
static NTSTATUS AmaterasuCreatePort(_In_ PSECURITY_DESCRIPTOR sd, _In_ PCWSTR PortName, _In_ PFLT_PORT Port) {

    NTSTATUS status;
    UNICODE_STRING uniString;
    OBJECT_ATTRIBUTES objAttr;

    RtlInitUnicodeString(&uniString, PortName);

    InitializeObjectAttributes(
        &objAttr,
        &uniString,
        /*
         *  In Windows, object names are typically case insensitive, so
         *  'OBJ_CASE_INSENSITIVE' ensures consistency in how the port name is
         *  accessed and referenced.
         *
         *  'OBJ_KERNEL_HANDLE' specifies that the handle created for the
         *  communication port is intended for kernel-mode usage. This flag
         *  designates the handle for kernel-mode operations, ensuring it's
         *  managed appropriately within the kernel environment.
         */
        OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
        NULL,
        sd
    );

    status = FltCreateCommunicationPort(
        Amaterasu.FilterHandle,
        Port
        &objAttr,
        NULL,
        AmaterasuConnect,
        AmaterasuDisconnect,
        AmaterasuMessage,
        1
    );

    if(!NT_SUCCESS(status)) {
        Debug("FltCreateCommunicationPort()\n");
        return status;
    }

    return status;
}

/*
 *  AmaterasuOpenPorts() - 
 *
 *  Return:
 *    -
 *    -
 */
static NTSTATUS AmaterasuOpenPorts(void) {

    NTSTATUS status;
    PSECURITY_DESCRIPTOR sd;

    /*
     *  Build a default security descriptor for the filter port with all access
     *  rights.
     *
     *  A security descriptor contains security information associated with a
     *  securable object, defining access control settings, such as permissions
     *  granted to users and groups, ownership information, etc.
     *
     *  The security descriptor, in this function, is used to create
     *  communication ports.
     */
    status = FltBuildDefaultSecurityDescriptor(&sd, FLT_PORT_ALL_ACCESS);
    if(!NT_SUCCESS(status)) {
        Debug("FltBuildDefaultSecurityDescriptor()\n");
        return status;
    }

    status = AmaterasuCreatePort(sd, AMATERASU_SERVER_PORT, Amaterasu.ServerPort);
    if(!NT_SUCCESS) {
        Debug("AmaterasuCreatePort() - %s\n", AMATERASU_SERVER_PORT);
        FltFreeSecurityDescriptor(sd);
        return status;
    }

    /*
     *  After all the communication ports have been created
     *  the security descriptor is not needed anymore.
     */
    FltFreeSecurityDescriptor(sd);

    return status;
}

/*
 *  AmaterasuInitLists() -
 *
 */
static __forceinline void AmaterasuInitLists(void) {
    
    InitializeListHead(&Amaterasu.RegistryList);
    InitializeListHead(&Amaterasu.ProcessList);
    InitializeListHead(&Amaterasu.FileList);
}

/*
 *  AmaterasuSetup() - 
 *
 *  Return:
 *    -
 *    -
 */
NTSTATUS AmaterasuSetup(_In_ PUNICODE_STRING RegistryPath) {

    NTSTATUS status;

    Amaterasu.DriverObject = DriverObject;

    status = AmaterasuOpenPorts();
    if(!NT_SUCCESS) {
        Debug("AmaterasuOpenPorts()\n");
        return status;
    }

    AmaterasuInitLists();

    return status;
}
