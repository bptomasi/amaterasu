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
NTSTATUS GetCreateProcessName(HANDLE processID, PUNICODE_STRING processName)
{
	PEPROCESS process;
	NTSTATUS status;
	PUNICODE_STRING imageName;

	status = PsLookupProcessByProcessId(processID, &process);
	if (!NT_SUCCESS(status)) {
		return status;
	}

	status = SeLocateProcessImageName(process, &imageName);
	if (!NT_SUCCESS(status)) {
		return status;
	}

	RtlCopyUnicodeString(processName, imageName);
	ExFreePool(imageName);

	ObDereferenceObject(process);
	return status;
}


/*
 * GetCreateProcessInfo - Fills in process creation info in the provided 'PTNOTIFY' structure.
 *
 * @pInfo: A pointer to the 'PTNOTIFY' structure to populate with process info.
 */
void GetCreateProcessInfo(_In_ PPTNOTIFY pInfo)
{
	PEPROCESS process;
	NTSTATUS status;

	HANDLE ParentID = NULL;
	HANDLE ChildID = NULL;
	BOOLEAN pCreate = 0;

	UNICODE_STRING parentProcessName;
	UNICODE_STRING childProcessName;

	if (pInfo == NULL) {
		return;
	}

	// Inicializacao das estruturas
	RtlInitEmptyUnicodeString(&parentProcessName, NULL, 0);
	RtlInitEmptyUnicodeString(&childProcessName, NULL, 0);

	// Inicializacao dos buffers
	parentProcessName.MaximumLength = 255 * sizeof(WCHAR);
	parentProcessName.Buffer = ExAllocatePoolWithTag(NonPagedPool, parentProcessName.MaximumLength, 'name');
	childProcessName.MaximumLength = 255 * sizeof(WCHAR);
	childProcessName.Buffer = ExAllocatePoolWithTag(NonPagedPool, childProcessName.MaximumLength, 'name');

	// Get Parent infos
	status = PsLookupProcessByProcessId(ParentID, &process);
	if (NT_SUCESS(status)) {
		pInfo->parentID = ParentID;
	}

	// Get Parent name infos
	status = GetCreateProcessName(pInfo->parentID, &parentProcessName);
	if (NT_SUCESS(status)) {
		pInfo->parentName = parentProcessName;
		pInfo->parentNameSize = parentProcessName.Length;
	}

	// Get Child infos
	status = PsLookupProcessByProcessId(ChildID, &process);
	if (NT_SUCESS(status)) {
		pInfo->ctID.childID = ChildID;
	}

	// Get Child name infos
	status = GetCreateProcessName(pInfo->ctID.childID, &childProcessName);
	if (NT_SUCESS(status)) {
		pInfo->ctID.childName = childProcessName;
		pInfo->ctID.childNameSize = childProcessName.Length;
	}

	pInfo->create = pCreate; // Create info

	// Free buffers
	if (parentProcessName.Buffer) {
		ExFreePoolWithTag(parentProcessName.Buffer, 'name');
	}
	if (childProcessName.Buffer) {
		ExFreePoolWithTag(childProcessName.Buffer, 'name');
	}
	ObDereferenceObject(process);
	return;
}


/*
 * GetCreateThreadInfo - Fills in thread creation info in the provided 'PTNOTIFY' structure.
 *
 * @tInfo: A pointer to the 'PTNOTIFY' structure to populate with thread info.
 */
void GetCreateThreadInfo(_In_ PPTNOTIFY tInfo)
{
	PEPROCESS process;

	HANDLE ParentID;
	HANDLE ThreadID;
	BOOLEAN tCreate;

	NTSTATUS status;

	if (tInfo == NULL) {
		return NULL;
	}

	// Get Parent Thread id
	status = PsLookupThreadByThreadId(ParentID, &process);
	if (NT_SUCESS(status)) {
		tInfo->parentID = ParentID;
	}

	// Get Thread id
	status = PsLookupThreadByThreadId(ThreadID, &process);
	if (!NT_SUCESS(status)) {
		tInfo->ctID.threadID = ThreadID;
	}

	tInfo->create = tCreate; // Create info

	ObDereferenceObject(process);
	return;
}

/*
 * ProcessCreateInit - Initializes a process notification structure.
 *
 * @PoolType: The type of pool memory to allocate.
 *
 * Return:
 *    - A pointer to the initialized 'PTNOTIFY' structure, or NULL if initialization fails.
 */
PPTNOTIFY ProcessCreateInit(_In_ POOL_TYPE PoolType)
{	
	PPTNOTIFY processInfo;
	processInfo = ProcessCreateAlloc(PoolType);
	if (processInfo == NULL) {
		retunr NULL;
	}

	GetCreateProcessInfo(processInfo);

	return processInfo;
}

/*
 * ThreadCreateInit - Initializes a thread notification structure.
 *
 * @PoolType: The type of pool memory to allocate.
 *
 * Return:
 *    - A pointer to the initialized 'PTNOTIFY' structure, or NULL if initialization fails.
 */
PPTNOTIFY ThreadCreateInit(_In_ POOL_TYPE PoolType)
{
	PPTNOTIFY threadInfo;
	threadInfo = ThreadCreateAlloc(PoolType);
	if (threadInfo == NULL) {
		return NULL;
	}

	GetCreateThreadInfo(threadInfo);

	return threadInfo;
}

/*
 * ProcessNameFree - Frees memory allocated for process names in the 'PTNOTIFY' structure.
 *
 * @ProcessInfo: A pointer to the 'PTNOTIFY' structure whose names are to be freed.
 */
void ProcessNameFree(PPTNOTIFY ProcessInfo)
{
	if (ProcessInfo->parentName.Buffer) {
		ExFreePoolWithTag(ProcessInfo->parentName.Buffer, 'name');
		ProcessInfo->parentName.Buffer = NULL;
	}

	if (ProcessInfo->ctID.childName.Buffer) {
		ExFreePoolWithTag(ProcessInfo->ctID.childName.Buffer, 'name');
		ProcessInfo->ctID.childName.Buffer = NULL;
	}

	return;
}

/*
 * ProcessCreateFree - Frees the process notification structure and its contents.
 *
 * @ProcessInfo: A double pointer to the 'PTNOTIFY' structure to be freed.
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
 * @ThreadInfo: A double pointer to the 'PTNOTIFY' structure to be freed.
 */
void ThreadCreateFree(_Inout_ PPTNOTIFY* ThreadInfo)
{
	ExFreePoolWithTag(*ThreadInfo, 'trnf');

	*ThreadInfo = NULL;

	return;
}