#include "amaterasu.h"

#define ARQ_MONITORADO L"target.exe"

AmaterasuDefaultPreCallback(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Flt_CompletionContext_Outptr_ PVOID* CompletionContext
)
{

		NTSTATUS status;
		PFILE_INFO fileInfo;
		HANDLE pid = PsGetCurrentProcessId();
		PEPROCESS proc;
		PUNICODE_STRING nome;
		WCHAR targetProcNome[] = ARQ_MONITORADO;
		KIRQL irq;
	
		PPROC_INFO ProcInfo;
		PINFO Info;
		
		if (FLT_IS_FS_FILTER_OPERATION(Data)) {
			return FLT_PREOP_SUCCESS_NO_CALLBACK;
		}
	
		//Get Process name 
		PsLookupProcessByProcessId(pid, &proc);
		SeLocateProcessImageName(proc, &nome);
		if (nome->Buffer != NULL) {
			if (wcsstr(nome->Buffer, targetProcNome) != NULL) {
				/*irq = KeGetCurrentIrql();
				KdPrint(("%ws %c %d\n", irq, irq, irq));
				if (irq == PASSIVE_LEVEL)
					KdPrint(("nivel passivo\n"));
				if (irq == DISPATCH_LEVEL)
					KdPrint(("nivel de despacho\n"));
				fileInfo = FileInfoGet(NonPagedPool, FLT_FILE_NAME_QUERY_DEFAULT, Data);
				if (fileInfo != NULL) {
					KdPrint(("------------------------------------\n"));
					KdPrint(("PID: %lu %p\n", pid, pid));
					KdPrint(("Proc Name: %wZ\n", nome->Buffer));
					KdPrint(("Path: %ws\n", fileInfo->Path));
					KdPrint(("Path size: %ld\n", fileInfo->PathSize));
					KdPrint(("File Name: %ws\n", fileInfo->FinalName));
					KdPrint(("File Name Size: %ld\n", fileInfo->FinalNameSize));
					FileInfoFree(&fileInfo);
				}
				ProcInfo = ProcInfoGet(NonPagedPool, Data);
				if (ProcInfo != NULL) {
					KdPrint(("------------------------------------\n"));
					KdPrint(("PID: %ld\n", ProcInfo->PID));
					KdPrint(("SID: % ld\n", ProcInfo->SID));
					KdPrint(("IsElevated: % ld\n", ProcInfo->IsElevated));
					ProcInfoFree(&ProcInfo);
				}
				else
					KdPrint(("ProcInfoGet failed\n"));*/
				InfoListAppend(Amaterasu.InfoList, Data, INFO_FS);
				Info = InfoGet(NonPagedPool, Data, INFO_FS);
				//InsertHeadList(&RecordList, &(Info->ListEntry));
				if (Info != NULL) {
					KdPrint(("------------------------------------\n"));
					KdPrint(("PID: %lu %p\n", pid, pid));
					KdPrint(("Day: %hu\n", Info->TimeFields.Day));
					KdPrint(("Hour: %hu\n", Info->TimeFields.Hour));
					KdPrint(("Minute: %hu\n", Info->TimeFields.Minute));
					KdPrint(("Seconds: %hu\n", Info->TimeFields.Second));
					KdPrint(("Seconds: %hu\n", Info->TimeFields.Second));
					KdPrint(("Seconds: %hu\n", Info->TimeFields.Second));
					InfoFree(&Info);
	
				}
	
			}
		}
	
	
		return FLT_PREOP_SUCCESS_NO_CALLBACK;


}


void AmaterasuLoadImageCallback(
	PUNICODE_STRING FullImageName,
	HANDLE ProcessId,
	PIMAGE_INFO ImageInfo
) {

	LOAD_IMAGE_DATA LoadImageData;
	
	LoadImageData.FullImageName = FullImageName;
	LoadImageData.ProcessId = ProcessId;
	LoadImageData.ImageInfo = ImageInfo;

	InfoListAppend(Amaterasu.InfoList, &LoadImageData, INFO_LOAD);

	return;
}

NTSTATUS AmaterasuRegCallback(
	PVOID CallbackContext,
	PVOID RegNotifyClass,
	PVOID RegStruct
){
	NTSTATUS Status;
	
	REG_INFO_DATA RegInfoData;

	Status = STATUS_SUCCESS;

	PREG_SET_VALUE_KEY_INFORMATION RegSt = RegStruct;

	switch ((REG_NOTIFY_CLASS)RegNotifyClass)
	{
	case RegNtSetValueKey:
		DbgPrint("reg callbacks path : %ws\n",((PREG_SET_VALUE_KEY_INFORMATION)RegStruct)->ValueName->Buffer)
		DbgPrint("%s:%d\n", __func__, __LINE__);
		DbgPrint("%s:%d: &RegStruct: %p\n", __func__, __LINE__, RegSt);
		DbgPrint("%s:%d\n", __func__, __LINE__);
		DbgPrint("%s:%d: &RegStruct->ValueName: %p\n", __func__, __LINE__, RegSt->ValueName);
		DbgPrint("%s:%d\n", __func__, __LINE__);
		DbgPrint("%s:%d\n", __func__, __LINE__);
		DbgPrint("%s:%d: &RegStruct->ValueName->Buffer: %p\n", __func__, __LINE__, RegSt->ValueName->Buffer);
		DbgPrint("%s:%d: &RegStruct->ValueName->Length: %\n", __func__, __LINE__, RegSt->ValueName->Length);
		break;
	case RegNtDeleteValueKey:
		break;

	default:
		return Status;
	}

	
	RegInfoData.RegNotifyClass = (REG_NOTIFY_CLASS)RegNotifyClass;
	RegInfoData.RegStruct = RegStruct;



	Status = InfoListAppend(Amaterasu.InfoList, &RegInfoData, INFO_REG);

	return Status;
}

void AmaterasuProcCallback(_In_ HANDLE PPID, _In_ HANDLE PID, _In_ BOOLEAN Active) {

	NTSTATUS Status;

	IDENTIFIER IDs;

	IDs.Active = Active;
	IDs.PPID = PPID;
	IDs.Id.PID = PID;
	IDs.isThread = FALSE;
	
	Status = InfoListAppend(Amaterasu.InfoList, &IDs, INFO_PROC);
	if (!NT_SUCCESS(Status)) {
		return;
	}

}

void AmaterasuThreadCallback(_In_ HANDLE PPID, _In_ HANDLE TID, _In_ BOOLEAN Active) {

	NTSTATUS Status;

	IDENTIFIER IDs;

	IDs.Active = Active;
	IDs.PPID = PPID;
	IDs.Id.TID = TID;
	IDs.isThread = TRUE;

	Status = InfoListAppend(Amaterasu.InfoList, &IDs, INFO_PROC);
	if (!NT_SUCCESS(Status)) {
		return;
	}
}




