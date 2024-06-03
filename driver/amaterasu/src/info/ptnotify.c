#include "ptnotify.h"

/*
 * ProcessCreateAlloc - Allocates and initializes a PTNOTIFY structure for a process.
 *
 * @PoolType: Type of pool memory to allocate from.
 *
 * Return:
 *    - Pointer to the allocated PTNOTIFY structure.
 *    - NULL if allocation fails.
 */
PPTNOTIFY ProcessCreateAlloc(_In_ POOL_TYPE PoolType)
{
	PPTNOTIFY ProcessInfo;

	// Allocate memory for the PTNOTIFY structure
	ProcessInfo = ExAllocatePoolWithTag(PoolType, sizeof(*ProcessInfo), 'prnf');
	if (!ProcessInfo) {
		return NULL;
	}

	RtlZeroMemory(ProcessInfo, sizeof(PTNOTIFY));

	ProcessInfo->PoolType = PoolType;

	return ProcessInfo;
}

/*
 * ThreadCreateAlloc - Allocates and initializes a PTNOTIFY structure for a thread.
 *
 * @PoolType: Type of pool memory to allocate from.
 *
 * Return:
 *    - Pointer to the allocated PTNOTIFY structure.
 *    - NULL if allocation fails.
 */
PPTNOTIFY ThreadCreateAlloc(_In_ POOL_TYPE PoolType)
{
	PPTNOTIFY ThreadInfo;

	// Allocate memory for the PTNOTIFY structure
	ThreadInfo = ExAllocatePoolWithTag(PoolType, sizeof(*ThreadInfo), 'trnf');
	if (!ThreadInfo) {
		return NULL;
	}

	RtlZeroMemory(ThreadInfo, sizeof(PTNOTIFY));

	ThreadInfo->PoolType = PoolType;

	return ThreadInfo;
}

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
NTSTATUS GetCreateProcessName(_In_ HANDLE processID, _Out_ PUNICODE_STRING processName)
{
	PEPROCESS process;
	NTSTATUS status;
	PUNICODE_STRING imageName;

	// Look up the process by its ID
	status = PsLookupProcessByProcessId(processID, &process);
	if (!NT_SUCCESS(status)) {
		return status;
	}

	// Locate the image name of the process
	status = SeLocateProcessImageName(process, &imageName);
	if (!NT_SUCCESS(status)) {
		ObDereferenceObject(process);
		return status;
	}

	// Copy the process name to the provided PUNICODE_STRING
	RtlCopyUnicodeString(processName, &imageName);
	ExFreePool(imageName);

	ObDereferenceObject(process);
	return status;
}


/*
 * GetCreateProcessInfo - Fills in process creation info in the provided 'PTNOTIFY' structure.
 *
 * @ParentId: ID of the parent process.
 * @ChildId: ID of the child process.
 * @pCreate: Pointer to process creation information.
 * @pInfo: Pointer to the 'PTNOTIFY' structure to populate with process info.
 */
void GetCreateProcessInfo(_In_ HANDLE ParentId, _In_ HANDLE ChildId, _In_ HANDLE pCreate, _Inout_ PPTNOTIFY pInfo)
{
	PEPROCESS process;
	NTSTATUS status;

	PUNICODE_STRING parentProcessName;
	PUNICODE_STRING childProcessName;

	if (pInfo == NULL) {
		return;
	}

	// Initialize the UNICODE_STRING
	RtlInitEmptyUnicodeString(&parentProcessName, NULL, 0);
	RtlInitEmptyUnicodeString(&childProcessName, NULL, 0);

	// Initialize the buffers for the UNICODE_STRING
	parentProcessName->MaximumLength = 512 * sizeof(WCHAR);
	parentProcessName->Buffer = ExAllocatePoolWithTag(NonPagedPool, parentProcessName->MaximumLength, 'name');
	childProcessName->MaximumLength = 512 * sizeof(WCHAR);
	childProcessName->Buffer = ExAllocatePoolWithTag(NonPagedPool, childProcessName->MaximumLength, 'name');

	// Get Parent infos
	status = PsLookupProcessByProcessId(ParentId, &process);
	if (!NT_SUCCESS(status)) {
		return;
	}
	pInfo->parentID = ParentId;

	// Get Parent name infos
	status = GetCreateProcessName(pInfo->parentID, parentProcessName);
	if (!NT_SUCCESS(status)) {
		ObDereferenceObject(process);
		return;
	}
	status = UnicodeStrToWSTR(pInfo->PoolType, parentProcessName, &pInfo->parentName, &pInfo->parentNameSize);
	if (!NT_SUCCESS(status)) {
		ExFreePoolWithTag(pInfo->parentName, 'wstr');
		ObDereferenceObject(process);
		return;
	}

	// Get Child infos
	status = PsLookupProcessByProcessId(ChildId, &process);
	if (!NT_SUCCESS(status)) {
		ExFreePoolWithTag(pInfo->parentName, 'wstr');
		ExFreePoolWithTag(parentProcessName->Buffer, 'name');
		return;
	}
	pInfo->ctID.childID = ChildId;

	// Get Child name infos
	status = GetCreateProcessName(pInfo->ctID.childID, childProcessName);
	if (!NT_SUCCESS(status)) {
		ExFreePoolWithTag(pInfo->parentName, 'wstr');
		ExFreePoolWithTag(parentProcessName->Buffer, 'name');
		ObDereferenceObject(process);
		return;
	}
	status = UnicodeStrToWSTR(pInfo->PoolType, childProcessName, &pInfo->childName, &pInfo->childNameSize);
	if (!NT_SUCCESS(status)) {
		ExFreePoolWithTag(pInfo->parentName, 'wstr');
		ExFreePoolWithTag(parentProcessName->Buffer, 'name');
		ExFreePoolWithTag(childProcessName->Buffer, 'name');
		ObDereferenceObject(process);
		return;
	}

	pInfo->create = pCreate; // Create info

	ObDereferenceObject(process);
	return;
}


/*
 * GetCreateThreadInfo - Fills in thread creation info in the provided 'PTNOTIFY' structure.
 *
 * @ParentId: ID of the parent thread.
 * @ThreadId: ID of the thread.
 * @tCreate: Pointer to thread creation information.
 * @tInfo: Pointer to the 'PTNOTIFY' structure to populate with thread info.
 */
void GetCreateThreadInfo(_In_ HANDLE ParentId, _In_ HANDLE ThreadId, _In_ HANDLE tCreate, _Inout_ PPTNOTIFY tInfo)
{
	PEPROCESS process;
	NTSTATUS status;

	if (tInfo == NULL) {
		return NULL;
	}

	// Get Parent Thread id
	status = PsLookupThreadByThreadId(ParentId, &process);
	if (!NT_SUCCESS(status)) {
		return;
	}
	tInfo->parentID = ParentId;

	// Get Thread id
	status = PsLookupThreadByThreadId(ThreadId, &process);
	if (!NT_SUCCESS(status)) {
		return;
	}
	tInfo->ctID.threadID = ThreadId;

	tInfo->create = tCreate; // Create info

	ObDereferenceObject(process);
	return;
}

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
PPTNOTIFY ProcessCreateInit(_In_ POOL_TYPE PoolType, _In_ HANDLE ParentId, _In_ HANDLE ChildId, _In_ HANDLE pCreate)
{	
	PPTNOTIFY processInfo;
	processInfo = ProcessCreateAlloc(PoolType);
	if (processInfo == NULL) {
		retunr NULL;
	}

	GetCreateProcessInfo(ParentId, ChildId, pCreate, processInfo);

	return processInfo;
}

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
PPTNOTIFY ThreadCreateInit(_In_ POOL_TYPE PoolType, _In_ HANDLE ParentId, _In_ HANDLE ThreadId, _In_ HANDLE tCreate)
{
	PPTNOTIFY threadInfo;
	threadInfo = ThreadCreateAlloc(PoolType);
	if (threadInfo == NULL) {
		return NULL;
	}

	GetCreateThreadInfo(ParentId, ThreadId, tCreate, threadInfo);

	return threadInfo;
}

/*
 * ProcessNameFree - Frees memory allocated for process names in the 'PTNOTIFY' structure.
 *
 * @ProcessInfo: Pointer to the 'PTNOTIFY' structure whose names are to be freed.
 */
void ProcessNameFree(_In_ PPTNOTIFY ProcessInfo)
{
	if (ProcessInfo) {
		if (ProcessInfo->parentName) {
			ExFreePoolWithTag(ProcessInfo->parentName, 'wstr');
		}
		if (ProcessInfo->childName) {
			ExFreePoolWithTag(ProcessInfo->childName, 'wstr');
		}
	}
	return;
}

/*
 * ProcessCreateFree - Frees the process notification structure and its contents.
 *
 * @ProcessInfo: Double pointer to the 'PTNOTIFY' structure to be freed.
 */
void ProcessCreateFree(_Inout_ PPTNOTIFY* ProcessInfo)
{
	ProcessNameFree(*ProcessInfo);
	ExFreePoolWithTag(*ProcessInfo, 'prnf');

	*ProcessInfo = NULL;

	return;
}

/*
 * ThreadCreateFree - Frees the thread notification structure.
 *
 * @ThreadInfo: Double pointer to the 'PTNOTIFY' structure to be freed.
 */
void ThreadCreateFree(_Inout_ PPTNOTIFY* ThreadInfo)
{
	ExFreePoolWithTag(*ThreadInfo, 'trnf');

	*ThreadInfo = NULL;

	return;
}