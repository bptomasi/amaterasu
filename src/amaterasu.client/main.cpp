#include "mainHeader.h"

// Global variable to control capture state
BOOLEAN captureOn = TRUE;

int main() {
    // Handle declarations
    SC_HANDLE hService, hSCManager;
    HANDLE timerHandle, DriverHandle;
    // Structure for information
    INFO Info;
    // Driver name
    wchar_t DriverName[] = DRIVER_NAME;

    // File pointer for logging
    FILE *logFile = fopen("Log.txt", "w");

    // Load the driver
    LoadDriver(&hService, &hSCManager, DriverName);

    // Get a handle to the driver
    DriverHandle = GetDriverHandle(DriverName);

    // Create a timer to stop capture
    CreateTimerQueueTimer(&timerHandle, NULL, (WAITORTIMERCALLBACK)StopCapture,
                          NULL, CAPTURE_TIME, 0, WT_EXECUTEDEFAULT);

    // Main loop for capturing
    while (captureOn) {
        // Send IOCTL to driver to capture information
        DeviceIoControl(DriverHandle, IOCTL_FILE_LOG,
                        UM_MSG, sizeof(UM_MSG), &Info,
                        sizeof(Info), &BytesReturned,
                        NULL);

        // Log captured information if any
        if (BytesReturned > 0)
            LogInfo(Info, logFile, IOCTL_FILE_LOG);
    }

    // Delete the timer
    (void)DeleteTimerQueueTimer(NULL, &timerHandle, NULL);

    // Unload the driver
    UnloadDriver(&hService, &hSCManager);

    // Close the driver handle
    CloseHandle(DriverHandle);

    return 0;
}

