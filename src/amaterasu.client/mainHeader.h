#include <stdio.h>
#include <windows.h>
#include <iostream>

#define CAPTURE_TIME 10000
#define FS_DRIVER_NAME L"MiniFilter"
#define UM_MSG "Aloooooooooo\n"

typedef struct ProcInfo PROC_INFO, * PPROC_INFO;

typedef struct FileInfo FILE_INFO, * PFILE_INFO;

typedef struct MjFunc MJFUNC, * PMJFUNC;

typedef struct Info INFO, * PINFO;

typedef int POOL_TYPE;

typedef struct _TIME_FIELDS {
    short Year;        // range [1601...]
    short Month;       // range [1..12]
    short Day;         // range [1..31]
    short Hour;        // range [0..23]
    short Minute;      // range [0..59]
    short Second;      // range [0..59]
    short Milliseconds;// range [0..999]
    short Weekday;     // range [0..6] == [Sunday..Saturday]
} TIME_FIELDS;
typedef TIME_FIELDS* PTIME_FIELDS;

#define IOCTL_FILE_LOG CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA);
#define IOCTL_PROC_LOG CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA);


struct ProcInfo {

    ULONG PID;
    ULONG SID;

    BOOLEAN IsElevated;

    POOL_TYPE PoolType;

};

struct FileInfo {
    POOL_TYPE PoolType;

    ULONG PathSize;
    ULONG FinalNameSize;

    PWSTR Path;
    PWSTR FinalName;
};

struct MjFunc {

    UCHAR Code;
    ULONG Len;
    PWSTR Name;
};



struct Info {

    MJFUNC      MjFunc;
    TIME_FIELDS TimeFields;
    PROC_INFO   ProcInfo;
    LIST_ENTRY  ListEntry;

    union {
        PFILE_INFO FileInfo;
    } Info;

    POOL_TYPE PoolType;

};

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

BOOLEAN LoadDriver(_Out_ SC_HANDLE* hService, _Out_ SC_HANDLE* hSCManager, wchar_t* DriverName);

/*
 * UnloadDriver - Unloads Driver
 *
 * @hService: pointer to a SC_HANDLE for stoping the driver and be closed
 *
 * @hSCManager: pointer to a SC_HANDLE to be closed
 *
 */
void UnloadDriver(_In_ SC_HANDLE* hService, _In_ SC_HANDLE* hSCManager);

VOID CALLBACK StopCapture();

void LogInfo(INFO Info, FILE *logFile, DWORD IOCTL_CODE);
