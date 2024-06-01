#pragma once
#ifndef PTNOTIFY_H
#define PTNOTIFY_H

#include "common.h"
#include "amaterasu.h"

typedef struct PTNotify {
	POOL_TYPE PoolType;

	ULONG parentID;
	ULONG parentNameSize;
	UNICODE_STRING parentName;

	union {
		ULONG childID;
		ULONG childNameSize;
		UNICODE_STRING childName;

		ULONG threadID;
	} ctID;

	BOOLEAN create;
};

typedef struct PTNotify PTNOTIFY, *PPTNOTIFY;


/*
 * ProcessCreateAlloc - Allocates and initializes a PTNOTIFY structure for a process.
 *
 * @PoolType: Type of pool memory to allocate from.
 *
 * Return:
 *    - Pointer to the allocated PTNOTIFY structure.
 *    - NULL if allocation fails.
 */
PPTNOTIFY ProcessCreateAlloc(_In_ POOL_TYPE PoolType);


/*
 * ThreadCreateAlloc - Allocates and initializes a PTNOTIFY structure for a thread.
 *
 * @PoolType: Type of pool memory to allocate from.
 *
 * Return:
 *    - Pointer to the allocated PTNOTIFY structure.
 *    - NULL if allocation fails.
 */
PPTNOTIFY ThreadCreateAlloc(_In_ POOL_TYPE PoolType);


/*
 * GetCreateProcessName - Retrieves the name of the process given its ID.
 *
 * @processID: ID of the process.
 * @processName: Pointer to the 'UNICODE_STRING' to receive the process name.
 *
 * Return:
 *    - Status code indicating the result of the operation.
 *    - Upon success, returns STATUS_SUCCESS.
 *    - The appropriate error code in case of failure.
 */
NTSTATUS GetCreateProcessName(
	HANDLE processID,
	PUNICODE_STRING processName
);


/*
 * GetCreateProcessInfo - Fills in process creation info in the provided 'PTNOTIFY' structure.
 *
 * @pInfo: A pointer to the 'PTNOTIFY' structure to populate with process info.
 */
void GetCreateProcessInfo(
	_In_ PPTNOTIFY pInfo
);


/*
 * GetCreateThreadInfo - Fills in thread creation info in the provided 'PTNOTIFY' structure.
 *
 * @tInfo: A pointer to the 'PTNOTIFY' structure to populate with thread info.
 */
void GetCreateThreadInfo(
	_In_ PPTNOTIFY tInfo
);


/*
 * ProcessCreateInit - Initializes a process notification structure.
 *
 * @PoolType: The type of pool memory to allocate.
 *
 * Return:
 *    - A pointer to the initialized 'PTNOTIFY' structure, or NULL if initialization fails.
 */
PPTNOTIFY ProcessCreateInit(
	_In_ POOL_TYPE PoolType
);


/*
 * ThreadCreateInit - Initializes a thread notification structure.
 *
 * @PoolType: The type of pool memory to allocate.
 *
 * Return:
 *    - A pointer to the initialized 'PTNOTIFY' structure, or NULL if initialization fails.
 */
PPTNOTIFY ThreadCreateInit(
	_In_ POOL_TYPE PoolType
);


/*
 * ProcessNameFree - Frees memory allocated for process names in the 'PTNOTIFY' structure.
 *
 * @ProcessInfo: A pointer to the 'PTNOTIFY' structure whose names are to be freed.
 */
void ProcessNameFree(PPTNOTIFY ProcessInfo);


/*
 * ProcessCreateFree - Frees the process notification structure and its contents.
 *
 * @ProcessInfo: A double pointer to the 'PTNOTIFY' structure to be freed.
 */
void ProcessCreateFree(_Inout_ PPTNOTIFY* ProcessdInfo);


/*
 * ThreadCreateFree - Frees the thread notification structure.
 *
 * @ThreadInfo: A double pointer to the 'PTNOTIFY' structure to be freed.
 */
void ThreadCreateFree(_Inout_ PPTNOTIFY* ThreadInfo);

#endif // PTNOTIFY_H
