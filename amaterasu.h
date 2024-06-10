#ifndef AMATERASU_H
#define AMATERASU_H

#include "info.h"
#include "infolist.h"
#include "callbacks.h"
#include "ioctl.h"

#include "utils.h"

#define DRIVER_NAME L"\\Device\\MiniFilter"
#define DEVICE_SYM_LINK L"\\DosDevices\\MiniFilter"
#define CALLBACK_NUMBER 4


enum CallbacksTypes {
    FS_CALLBACK,
    PROC_CALLBACK,
    LOAD_IMAGE_CALLBACK,
    REG_CALLBACK
};

typedef enum CallbacksTypes CALLBACK_TYPES, * PCALLBACK_TYPES;


struct Amaterasu {

    PDRIVER_OBJECT  DriverObject;
    PFLT_FILTER     FilterHandle;
    
    PINFO_LIST      InfoList;

    BOOLEAN EnabledCallbacks[CALLBACK_NUMBER];

    LARGE_INTEGER Cookie;

};

extern struct Amaterasu Amaterasu;



NTSTATUS Create(PDEVICE_OBJECT DeviceObject, PIRP Irp);

NTSTATUS Close(PDEVICE_OBJECT DeviceObject, PIRP Irp);

/*
 *	DriverEntry() - Initializes the driver upon loading into memory.
 *
 *	@DriverObject: Pointer to the driver object created by the I/O manager.
 *	@RegistryPath: Path to the driver's registry key in the registry.
 *
 *  Return:
 *    - 'STATUS_SUCCESS' on success.
 *    - The appropriate 'NTSTATUS' is returned to indicate failure.
 */
extern NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath
);

#endif  /* AMATERASU_H */
