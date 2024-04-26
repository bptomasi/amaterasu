#include "LoadDriver.h"

/*
 * LoadDriver - Loads Driver
 *
 * @hService: pointer to a SC_HANDLE for opening a service
 *
 * @hSCManager: pointer to a SC_HANDLE to estabilish a connection to the SC manager
 *
 * Return: 
 * 	- TRUE the driver was loaded 
 * 	- FALSE failed to load the driver
 */

BOOLEAN LoadDriver(_Out_ SC_HANDlE *hService, _Out_ SC_HANDLE *hSCManager, wchar_t* DriverName){
	BOOLEAN status = FALSE;
	/*
	 * Get handle to servive manager with desired acesss
	 */

	*hSCManager = OpenSCManager(NULL,NULL, GENERIC_WRITE);
	
	if (*hSCManager != NULL){
		*hService = OpenService(*hSCManager, DriverName, GENERIC_EXECUTE | DELETE);
		if (*hService != NULL)
			if (StartService(*hService,0,NULL))
				status = TRUE;
			else
				printf("Failed to start service %d",GetLastError());
		else
			printf("Failed to get service handle %d",GetLastError());
	}
	else
		printf("Failed to get service manager handle %d",GetLastError());
	
	return status;
}

/*
 * UnloadDriver - Unloads Driver
 *
 * @hService: pointer to a SC_HANDLE for stoping the driver and be closed
 *
 * @hSCManager: pointer to a SC_HANDLE to be closed
 *
 */
void UnloadDriver(_In_ SC_HANDlE *hService, _In_ SC_HANDLE *hSCManager){
	SERVICE_STATUS status;

	/*
	 * Sends control code to the driver to stop it
	 */

	ControlService(*hService, SERVICE_CONTROL_STOP, &status);
	
	CloseServiceHandle(*hService);
	CloseServiceHandle(*hSCManager);
}


