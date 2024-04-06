#include "Amaterasu.h"

#define AMATERASU_SERVER_PORT L"AmaterasuServerPort"

/* 
 * The 'static' function prototypes below are just 
 * for the pragma as 'alloc_text' must be placed between a function
 * declarator and the function definition.
 */
static NTSTATUS
AmaterasuCreatePort(
        _In_ PSECURITY_DESCRIPTOR sd,
        _In_ PCWSTR PortName,
        _In_ PFLT_PORT Port,
        _In_ PFLT_CONNECT_NOTIFY ConnectCallback,
        _In_ PFLT_DISCONNECT_NOTIFY DisconnectCallback,
        _In_ PFLT_MESSAGE_NOTIFY MessageCallback,
        _In_ LONG MaxConnections
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
 *  AmaterasuCreatePort() - Creates a communication port for inter-process
 *                          communication.
 *
 *  @sd      : Security descriptor.
 *  @PortName: Name of the communication port.
 *  @Port    : Pointer to a FLT_PORT structure that will receive the created
 *             port handle.
 *
 *  @ConnectCallback: Callback routine to be called when a client connects
 *                    to the port.
 *
 *  @DisconnectCallback: Callback routine to be called when a client
 *                       disconnects from the port.
 *
 *  @MessageCallback: Callback routine to be called when a message is
 *                    received on the port.
 *
 *  @MaxConnections: Maximum number of concurrent connections allowed on the
 *                   port.
 *
 *  Return:
 *    - 'STATUS_SUCCESS' on success;
 *    - The appropriate 'NTSTATUS' is returned to indicate failure.
 */
static NTSTATUS AmaterasuCreatePort(
        _In_ PSECURITY_DESCRIPTOR sd,
        _In_ PCWSTR PortName,
        _In_ PFLT_PORT Port,
        _In_ PFLT_CONNECT_NOTIFY ConnectCallback,
        _In_ PFLT_DISCONNECT_NOTIFY DisconnectCallback,
        _In_ PFLT_MESSAGE_NOTIFY MessageCallback,
        _In_ LONG MaxConnections
    ) {

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
        ConnectCallback,
        DisconnectCallback,
        MessageCallback,
        MaxConnections
    );

    if(!NT_SUCCESS(status)) {
        DbgPrint("FltCreateCommunicationPort()\n");
        return status;
    }

    return status;
}

/*
 *  AmaterasuOpenPorts() - Opens communication ports for the Amaterasu filter
 *                         driver.
 *
 *  'AmaterasuOpenPorts()' opens the communication ports needed to 
 *  communicate with user mode applications. More specifically, this function
 *  opens 'Amaterasu.ServerPort'.
 *
 *  Besides that, 'AmaterasuOpenPorts()' is essentially just a wrapper
 *  to 'AmaterasuCreatePort()', serving as a unified interface for port creation,
 *  facilitating the expansion to open other communication ports if needed.
 *
 *  Return:
 *    - STATUS_SUCCESS if the communication ports are successfully opened.
 *    - An appropriate NTSTATUS error code if an error occurs.
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
        DbgPrint("FltBuildDefaultSecurityDescriptor()\n");
        return status;
    }

    status = AmaterasuCreatePort(
        sd,
        AMATERASU_SERVER_PORT,
        Amaterasu.ServerPort,
        AmaterasuConnect,
        AmaterasuDisconnect,
        AmaterasuMessage,
        1
    );

    if(!NT_SUCCESS(status)) {
        DbgPrint("AmaterasuCreatePort() - %s\n", AMATERASU_SERVER_PORT);
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
 */
static void AmaterasuInitLists(void) {
    
    InitializeListHead(&Amaterasu.RegistryList);
    InitializeListHead(&Amaterasu.ProcessList);
    InitializeListHead(&Amaterasu.FileList);
}

/*
 *  AmaterasuInitLocks() - 
 */
static void AmaterasuInitLocks(void) {

    KeInitializeLock(Amaterasu.ConnectionLock);
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

    /*
     *  'AmaterasuSetup()' may be expanded to handle informations
     *  stored in the system registry, pointed by 'RegistryPath'.
     */
    UNREFERENCED_PARAMETER(RegistryPath);

    Amaterasu.DriverObject = DriverObject;

    /* TODO: get this info from registry */
    Amaterasu.FileNameQueryMethod = FLT_FILE_NAME_QUERY_ALWAYS_ALLOW_CACHE_LOOKUP;

    status = AmaterasuOpenPorts();
    if(!NT_SUCCESS(status)) {
        DbgPrint("AmaterasuOpenPorts()\n");
        return status;
    }

    AmaterasuInitLocks();
    AmaterasuInitLists();

    return status;
}
