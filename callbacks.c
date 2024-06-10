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
				InfoListAppend(&Amaterasu.InfoList, Data, INFO_FS);
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

	RegInfoData.RegNotifyClass = (REG_NOTIFY_CLASS)RegNotifyClass;
	RegInfoData.RegStruct = RegStruct;

	Status = InfoListAppend(Amaterasu.InfoList, &RegInfoData, INFO_REG);

	return Status;
}

void AmaterasuProcCallback(_In_ HANDLE PPID, _In_ HANDLE PID, _In_ BOOLEAN Active) {

	NTSTATUS Status;

	IDENTIFIER IDs = {
		PPID,
		PID,
		FALSE,
		Active
	};
	
	DbgPrint("foiiii");
	Status = InfoListAppend(Amaterasu.InfoList, &IDs, INFO_PROC);
	if (!NT_SUCCESS(Status)) {
		return;
	}
	KdPrint(("Proc deu bom %ul", PPID));

}

void AmaterasuThreadCallback(_In_ HANDLE PPID, _In_ HANDLE TID, _In_ BOOLEAN Active) {

	NTSTATUS Status;

	IDENTIFIER IDs = {
		PPID,
		TID,
		TRUE,
		Active
	};

	Status = InfoListAppend(Amaterasu.InfoList, &IDs, INFO_PROC);
	if (!NT_SUCCESS(Status)) {
		return;
	}
	KdPrint(("Thread deu bom %ul", PPID));
}




