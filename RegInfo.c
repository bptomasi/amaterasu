#include "RegInfo.h"


PREG_INFO RegInfoAlloc(_PoolType_ POOL_TYPE PoolType) {

	PREG_INFO RegInfo;

	RegInfo = ExAllocatePoolWithTag(PoolType, sizeof * RegInfo, 'reg');
	if (!RegInfo) {
		Assert(RegInfo != NULL, "at ExAllocatePoolWithTag().");
		return NULL;
	}

	RtlZeroMemory(RegInfo, sizeof * RegInfo);

	RegInfo->PoolType = PoolType;


	return RegInfo;
}


static inline void GetRegBinaryData(PUNICODE_STRING Data, PREG_SET_VALUE_KEY_INFORMATION RegStruct) {
	PVOID buffer = ExAllocatePoolWithTag(NonPagedPool, RegStruct->DataSize, 'buf');
	if (!buffer) {
		Assert(buffer != NULL, "at ExAllocatePoolWithTag().");
		return;
	}

	RtlCopyMemory(buffer, RegStruct->Data, RegStruct->DataSize);

	BinaryDataToUnicodeSTR(buffer, Data, RegStruct->DataSize);

	ExFreePoolWithTag(buffer, 'buf');
}

static inline void GetRegWORDData(PUNICODE_STRING Data, PREG_SET_VALUE_KEY_INFORMATION RegStruct) {
	PVOID buffer = ExAllocatePoolWithTag(NonPagedPool, RegStruct->DataSize, 'buf');
	if (!buffer) {
		Assert(buffer != NULL, "at ExAllocatePoolWithTag().");
		return;
	}

	RtlCopyMemory(buffer, RegStruct->Data, RegStruct->DataSize);

 	WORDDataToUnicodeSTR(buffer, Data, RegStruct->DataSize);

	ExFreePoolWithTag(buffer, 'buf');
}

static NTSTATUS GetKeyData(PREG_INFO RegInfo, PREG_SET_VALUE_KEY_INFORMATION RegStruct) {

	UNICODE_STRING Data;
	InitUnicodeString(&Data);

	RegInfo->DataType = RegStruct->Type;
	RegInfo->DataSize = RegStruct->DataSize;

	switch (RegStruct->Type) {
	case REG_SZ:
		RtlCopyMemory(Data.Buffer, RegStruct->Data, RegStruct->DataSize);
		break;
	case REG_EXPAND_SZ:
		RtlCopyMemory(Data.Buffer, RegStruct->Data, RegStruct->DataSize);
		break;
	case REG_MULTI_SZ: {
		ULONG min = min(Data.Length, RegStruct->DataSize);
		RtlCopyMemory(Data.Buffer, RegStruct->Data, min);
		break;
	}
	case REG_BINARY:
		GetRegBinaryData(&Data, RegStruct);
		break;
	case REG_DWORD:
		GetRegWORDData(&Data, RegStruct);
		break;
	case REG_QWORD:
		GetRegWORDData(&Data, RegStruct);
		break;

	}

	UnicodeStrToWSTR(RegInfo->PoolType, &Data, &RegInfo->Data, &RegInfo->DataSize);

	ExFreePoolWithTag(Data.Buffer, 'buf');

	return STATUS_SUCCESS;
}

NTSTATUS SetValueInit(
	PREG_INFO RegInfo,
	PREG_SET_VALUE_KEY_INFORMATION RegStruct,
	REG_NOTIFY_CLASS RegOperation
) {
	ULONG ReturnLength;
	NTSTATUS Status;

	UNICODE_STRING QueryStr;
	InitUnicodeString(&QueryStr);
	DbgPrint("SETTTTTTTT");
	Assert(RegInfo != NULL, "RegInfo is NULLLLLLL");

	Status = ObQueryNameString(RegStruct->Object, (POBJECT_NAME_INFORMATION)&QueryStr, QueryStr.MaximumLength, &ReturnLength);
	if (!NT_SUCCESS(Status)) {
		Assert(NT_SUCCESS(Status), "at ObQueryNameString().");
		ExFreePoolWithTag(QueryStr.Buffer, 'aux');
		return Status;
	}
	KdPrint(("%Deu boa\n"));
	QueryStr.Buffer[QueryStr.Length / sizeof * QueryStr.Buffer - 1] = 0;
	KdPrint(("QueryStr.Buffer = %ws\n", QueryStr.Buffer));

	DbgPrint("antes do pooltype\n\n")
	DbgPrint("passou do pooltype\n");

	DbgPrint("foe");
	PUNICODE_STRING test1 = &QueryStr;
	DbgPrint("test1\n");
	PWSTR* test2 = &RegInfo->CompleteName;
	DbgPrint("test2\n");
	PULONG test3 = &RegInfo->CompleteNameSize;
	DbgPrint("test3\n");

	Status = UnicodeStrToWSTR(NonPagedPool, &QueryStr, &RegInfo->CompleteName, &RegInfo->CompleteNameSize);
	Assert(RegInfo->CompleteName != NULL, "RegInfo->CompleteName is NULL");
	DbgPrint("SetValueInit: RegInfo->CompleteName %ws\n", RegInfo->CompleteName);
	if (!NT_SUCCESS(Status)) {
		Assert(NT_SUCCESS(Status), "by UnicodeStrToWSTR().");
		ExFreePoolWithTag(QueryStr.Buffer, 'aux');
	}
	GetKeyData(RegInfo, RegStruct);
	return Status;

}

NTSTATUS DeleteValueInit(
	PREG_INFO RegInfo,
	PREG_DELETE_VALUE_KEY_INFORMATION RegStruct,
	REG_NOTIFY_CLASS RegOperation
) {
	ULONG ReturnLength;
	NTSTATUS Status;

	DbgPrint("DELETEEEE");

	UNICODE_STRING QueryStr;
	InitUnicodeString(&QueryStr);

	Assert(RegInfo != NULL, "RegInfo is NULLLLLLL");

	Status = ObQueryNameString(RegStruct->Object, (POBJECT_NAME_INFORMATION)&QueryStr, QueryStr.MaximumLength, &ReturnLength);
	if (!NT_SUCCESS(Status)) {
		Assert(NT_SUCCESS(Status), "at ObQueryNameString().");
		ExFreePoolWithTag(QueryStr.Buffer, 'aux');
		return Status;
	}

	DbgPrint("antes do pooltype\n")
	DbgPrint("passou do pooltype\n");

	PUNICODE_STRING test1 = &QueryStr;
	DbgPrint("test1\n");
	PWSTR* test2 = &RegInfo->CompleteName;
	DbgPrint("test2\n");
	PULONG test3 = &RegInfo->CompleteNameSize;
	DbgPrint("test3\n");

	Status = UnicodeStrToWSTR(NonPagedPool, &QueryStr, &RegInfo->CompleteName, &RegInfo->CompleteNameSize);
	Assert(RegInfo->CompleteName != NULL, "RegInfo->CompleteName is NULL");
	DbgPrint("DeleteValueInit: RegInfo->CompleteName %ws\n", RegInfo->CompleteName);
	if (!NT_SUCCESS(Status)) {
		Assert(NT_SUCCESS(Status), "by UnicodeStrToWSTR");
		ExFreePoolWithTag(QueryStr.Buffer, 'aux');
		return Status;
	}


	RegInfo->Data = NULL;
	RegInfo->DataSize = 0;
	RegInfo->DataType = 0;

	return Status;
}

NTSTATUS RegInfoInit(
	PREG_INFO RegInfo,
	REG_NOTIFY_CLASS RegOperation,
	PVOID RegStruct
) {
	NTSTATUS Status;

	switch (RegOperation)
	{
	case RegNtSetValueKey:
		Status = SetValueInit(RegInfo, RegStruct, RegOperation);
		break;
	case RegNtDeleteValueKey:
		Status = DeleteValueInit(RegInfo, RegStruct, RegOperation);
		break;
	default:
		DbgPrint("RegInfoInit Default value.\n");
		Status = STATUS_UNSUCCESSFUL;
		break;
	}

	return Status;
}


void RegInfoDeInit(PREG_INFO RegInfo) {

	DbgPrint("Entrou no RegInfoDeInit()\n");
	if(RegInfo->CompleteName)
		ExFreePoolWithTag(RegInfo->CompleteName, 'wstr');

	if (RegInfo->Data)
		ExFreePoolWithTag(RegInfo->Data, 'wstr');

	DbgPrint("Saiu do RegInfoDeInit()");

}

void RegInfoFree(PREG_INFO* RegInfo) {

	if (!RegInfo || *RegInfo == NULL)
		return;

	DbgPrint("Entrou no RegInfoFree().\n");
	RegInfoDeInit(*RegInfo);
	DbgPrint("Saiu do RegInfoInig()\n");

	ExFreePoolWithTag(*RegInfo, 'reg');

	*RegInfo = NULL;
	DbgPrint("Saiu do RegInfoFree().\n");
}

PREG_INFO RegInfoGet(_PoolType_ POOL_TYPE PoolType, PREG_INFO_DATA RegInfoData) {

	PREG_INFO RegInfo;
	NTSTATUS Status;

	RegInfo = RegInfoAlloc(PoolType);
	if (!RegInfo) {
		Assert(RegInfo != NULL, "by RegInfoAlloc().");
		return NULL;
	}

	Status = RegInfoInit(RegInfo, RegInfoData->RegNotifyClass, RegInfoData->RegStruct);
	if (!NT_SUCCESS(Status)) {
		Assert(NT_SUCCESS(Status), "by RegInfoInit().");
		RegInfoFree(&RegInfo);
		return NULL;
	}

	return RegInfo;
}

void RegInfoCopy(PREG_INFO_STATIC StaticRegInfo, PREG_INFO RegInfo) {
	ULONG DataSize;

	if (StaticRegInfo && RegInfo) {
		DbgPrint("Entrou no if do RegInfoCopy().\n");
		StaticRegInfo->RegNotifyClass = RegInfo->RegNotifyClass;
		StaticRegInfo->CompleteNameSize = RegInfo->CompleteNameSize;
		StaticRegInfo->DataSize = RegInfo->DataSize;
		StaticRegInfo->DataType = RegInfo->DataType;
		DbgPrint("Passou das atribuicoes do RegInfoCopy().\n");

		Assert(RegInfo->CompleteName != NULL, "RegInfo->CompleteName is NULL");
		Assert(RegInfo->Data != NULL, "RegInfo->Data is NULL");

		DataSize = min(MAX_PATH, RegInfo->CompleteNameSize);
		if (DataSize) {
			DbgPrint("Entrou no primeiro copy.\n")
			RtlCopyMemory(StaticRegInfo->CompleteName, RegInfo->CompleteName, DataSize * sizeof * RegInfo->CompleteName);
		}
		DbgPrint("Passou do primeiro Copy\n");

		DataSize = min(MAX_DATA, RegInfo->DataSize);
		if (DataSize) {
			DbgPrint("Entrou no segundo copy.\n");
			RtlCopyMemory(StaticRegInfo->Data, RegInfo->Data, sizeof * RegInfo->Data * DataSize);
		}
		DbgPrint("Passou do segundo copy\n");
	}
}