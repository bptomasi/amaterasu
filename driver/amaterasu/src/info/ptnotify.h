#pragma once
#ifndef PTNOTIFY_H
#define PTNOTIFY_H

#include "common.h"
#include "amaterasu.h"

typedef struct PTNotify {
	POOL_TYPE PoolType;

	ULONG parentID;
	ULONG parentNameSize;
	PWSTR parentName;

	ULONG childNameSize;
	PWSTR childName;

	union {
		ULONG childID;
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
	_In_ HANDLE processID,
	_Out_ PUNICODE_STRING processName
);


/*
 * GetCreateProcessInfo - Fills in process creation info in the provided 'PTNOTIFY' structure.
 *
 * @ParentId: ID of the parent process.
 * @ChildId: ID of the child process.
 * @pCreate: Pointer to process creation information.
 * @pInfo: Pointer to the 'PTNOTIFY' structure to populate with process info.
 */
void GetCreateProcessInfo(
	_In_ HANDLE ParentId,
	_In_ HANDLE ChildId,
	_In_ HANDLE pCreate,
	_Inout_ PPTNOTIFY pInfo
);


/*
 * GetCreateThreadInfo - Fills in thread creation info in the provided 'PTNOTIFY' structure.
 *
 * @ParentId: ID of the parent thread.
 * @ThreadId: ID of the thread.
 * @tCreate: Pointer to thread creation information.
 * @tInfo: Pointer to the 'PTNOTIFY' structure to populate with thread info.
 */
void GetCreateThreadInfo(
	_In_ HANDLE ParentId,
	_In_ HANDLE ThreadId,
	_In_ HANDLE tCreate,
	_Inout_ PPTNOTIFY tInfo
);


/*
 * ProcessCreateInit - Initializes a process notification structure.
 *
 * @PoolType: The type of pool memory to allocate.
 * @ParentId: ID of the parent process.
 * @ChildId: ID of the child process.
 * @pCreate: Pointer to process creation information.
 *
 * Return:
 *    - Pointer to the initialized 'PTNOTIFY' structure, or NULL if initialization fails.
 */
PPTNOTIFY ProcessCreateInit(
	_In_ POOL_TYPE PoolType,
	_In_ HANDLE ParentId,
	_In_ HANDLE ChildId,
	_In_ HANDLE pCreate
);


/*
 * ThreadCreateInit - Initializes a thread notification structure.
 *
 * @PoolType: The type of pool memory to allocate.
 * @ParentId: ID of the parent thread.
 * @ThreadId: ID of the thread.
 * @tCreate: Pointer to thread creation information.
 *
 * Return:
 *    - Pointer to the initialized 'PTNOTIFY' structure, or NULL if initialization fails.
 */
PPTNOTIFY ThreadCreateInit(
	_In_ POOL_TYPE PoolType,
	_In_ HANDLE ParentId,
	_In_ HANDLE ThreadId,
	_In_ HANDLE tCreate
);


/*
 * ProcessNameFree - Frees memory allocated for process names in the 'PTNOTIFY' structure.
 *
 * @ProcessInfo: Pointer to the 'PTNOTIFY' structure whose names are to be freed.
 */
void ProcessNameFree(_In_ PPTNOTIFY ProcessInfo);


/*
 * ProcessCreateFree - Frees the process notification structure and its contents.
 *
 * @ProcessInfo: Double pointer to the 'PTNOTIFY' structure to be freed.
 */
void ProcessCreateFree(_Inout_ PPTNOTIFY* ProcessdInfo);


/*
 * ThreadCreateFree - Frees the thread notification structure.
 *
 * @ThreadInfo: Double pointer to the 'PTNOTIFY' structure to be freed.
 */
void ThreadCreateFree(_Inout_ PPTNOTIFY* ThreadInfo);

#endif // PTNOTIFY_H
