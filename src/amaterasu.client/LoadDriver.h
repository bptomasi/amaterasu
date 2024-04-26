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

BOOLEAN LoadDriver(_Out_ SC_HANDlE *hService, _Out_ SC_HANDLE *hSCManager, wchar_t* DriverName);

/*
 * UnloadDriver - Unloads Driver
 *
 * @hService: pointer to a SC_HANDLE for stoping the driver and be closed
 *
 * @hSCManager: pointer to a SC_HANDLE to be closed
 *
 */
void UnloadDriver(_In_ SC_HANDlE *hService, _In_ SC_HANDLE *hSCManager);
