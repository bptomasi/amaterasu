
#include "callbacks.h"

static void AddPidToHandleArr(_In_ HANDLE PID) {

    KIRQL OldIrql;

    KeAcquireSpinLock(&Amaterasu.HandleArrLock, &OldIrql);

    if(Amaterasu.HandleArrSize < 1024) {
        Amaterasu.HandleArr[Amaterasu.HandleArrSize++] = PID;
    }

    KeReleaseSpinLock(&Amaterasu.HandleArrLock, OldIrql);
}

static BOOLEAN AreWeTrackingIt(_In_ HANDLE PID) {

    BOOLEAN Ret;
    NTSTATUS Status;
    PEPROCESS eProc;
    PUNICODE_STRING ImageName;

    if(Amaterasu.HandleArrSize) {
        for(SIZE_T i = 0; i < Amateras.HandleArrSize; i++) {
            if(PID == Amateras.HandleArr[i]) {
                return TRUE;
            }
        }
    } 

    Status = PsLookupProcessById(PID, &eProc);
    if(NT_SUCCESS(Status)) {
        Status = SeLocateProcessImageName(eProc, &ImageName);
        if(NT_SUCCESS(Status)) {
            if (wcsstr(ImageName->Buffer, Amaterasu.DriverSettings->TargetName)) {
                Ret = TRUE;
            }
        }
        ObDereferenceObject(eProc);
    }

    return Ret;
}

FLT_PREOP_CALLBACK_STATUS AmaterasuDefaultPreCallback(
    _Inout_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _Flt_CompletionContext_Outptr_ PVOID* CompletionContext
) {

    HANDLE PID;

    PID = FltGetRequestorProcessId(Data);

    if(AreWeTrackingIt(PID)) {
        InfoListAppend(&Amaterasu.InfoList, Data, INFO_FS); 
    }

    return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

void AmaterasuLoadImageCallback(
	PUNICODE_STRING FullImageName,
	HANDLE ProcessId,
	PIMAGE_INFO ImageInfo
) {

	LOAD_IMAGE_DATA LoadImageData;

    if(AreWeTrackingIt(ProcessId)) {

        LoadImageData.FullImageName = FullImageName;
        LoadImageData.ProcessId = ProcessId;
        LoadImageData.ImageInfo = ImageInfo;

        InfoListAppend(Amaterasu.InfoList, &LoadImageData, INFO_LOAD);
    }

	return;
}

NTSTATUS AmaterasuRegCallback(
	PVOID CallbackContext,
	PVOID RegNotifyClass,
	PVOID RegStruct
){

	NTSTATUS Status;
    HANDLE PID;
	REG_INFO_DATA RegInfoData;
    REG_NOTIFY_CLASS RegClass;

	Status = STATUS_SUCCESS;

    RegClass = (REG_NOTIFY_CLASS)RegNotifyClass;
    if(RegClass == RegNtSetValueKey) {
        PID = ((PREG_SET_VALUE_KEY_INFORMATION)RegStruct)->ProcessId;
    } else {
        if(RegClass == RegNtDeleteValueKey) {
            PID = ((PREG_DELETE_VALUE_KEY_INFORMATION)RegStruct)->ProcessId;
        } else {
            return Status;
        }
    }

    if(AreWeTrackingIt(PID)) {

        RegInfoData.RegNotifyClass = RegClass;
        RegInfoData.RegStruct = RegStruct;

        Status = InfoListAppend(Amaterasu.InfoList, &RegInfoData, INFO_REG);
        if(!NT_SUCCESS(Status)) {
            Status = STATUS_UNSUCCESSFUL;
        }
    }

	return Status;
}

void AmaterasuProcCallback(_In_ HANDLE PPID, _In_ HANDLE PID, _In_ BOOLEAN Active) {

	NTSTATUS Status;
	IDENTIFIER IDs;

    if(!AreWeTrackingIt(PID)) {
        if(AreWeTrackingIt(PPID)) {
             AddPidToHandleArr(PID);
        } else {
            return;
        }
    }

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

    if(AreWeTrackingIt(PPID)) {

        IDs.Active = Active;
        IDs.PPID = PPID;
        IDs.Id.TID = TID;
        IDs.isThread = TRUE;

        Status = InfoListAppend(Amaterasu.InfoList, &IDs, INFO_PROC);
        if (!NT_SUCCESS(Status)) {
            return;
        }
    }
}
