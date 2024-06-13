#include "RegInfo.h"


PREG_INFO RegInfoAlloc2(_PoolType_ POOL_TYPE PoolType) {

	DbgPrint("Entrou no RegInfoAlloc().\n");
	PREG_INFO RegInfo;

	RegInfo = ExAllocatePool2(POOL_FLAG_NON_PAGED, sizeof(REG_INFO), 'reg');
	if (!RegInfo) {
		Assert(RegInfo != NULL, "at ExAllocatePoolWithTag().");
		return NULL;
	}
	
	DbgPrint("RegInfo Addr:%p %llx", (void*)RegInfo, (void*)RegInfo);

	RtlZeroMemory(RegInfo, sizeof * RegInfo);

	RegInfo->PoolType = PoolType;
	DbgPrint("RegInfo->PoolType: %d\n", RegInfo->PoolType);

	DbgPrint("Saiu do RegInfoAlloc().\n");
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
    DbgPrint("teste 1\n");
}

NTSTATUS SetValueInit2(
	PREG_INFO RegInfo,
	PREG_SET_VALUE_KEY_INFORMATION RegStruct,
	REG_NOTIFY_CLASS RegOperation
) {
	ULONG ReturnLength;
	NTSTATUS Status;
	UNICODE_STRING QueryStr;
	InitUnicodeString(&QueryStr);

	Status = ObQueryNameString(RegStruct->Object, (POBJECT_NAME_INFORMATION)&QueryStr, QueryStr.MaximumLength, &ReturnLength);
	if (!NT_SUCCESS(Status)) {
		Assert(NT_SUCCESS(Status), "at ObQueryNameString().");
		ExFreePoolWithTag(QueryStr.Buffer, 'aux');
		return Status;
	}

	Status = UnicodeStrToWSTR(NonPagedPool, &QueryStr, &RegInfo->CompleteName, &RegInfo->CompleteNameSize);
	if (!NT_SUCCESS(Status)) {
		Assert(NT_SUCCESS(Status), "by UnicodeStrToWSTR().");
		ExFreePoolWithTag(QueryStr.Buffer, 'aux');
	}
	GetKeyData(RegInfo, RegStruct);
	return Status;
}

NTSTATUS DeleteValueInit2(
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



	Status = UnicodeStrToWSTR(NonPagedPool, &QueryStr, &RegInfo->CompleteName,&ReturnLength);
	Assert(RegInfo->CompleteName != NULL, "RegInfo->CompleteName is NULL");
	RegInfo->CompleteNameSize = ReturnLength;
	DbgPrint("atribui o complete name size\n");
	
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

NTSTATUS RegInfoInit2(
	PREG_INFO RegInfo,
	REG_NOTIFY_CLASS RegOperation,
	PVOID RegStruct
) {
	NTSTATUS Status;
	Status = STATUS_SUCCESS;

	if (RegInfo) {
		DbgPrint("RegInfoInit(): Acessando PoolType\n");
		DbgPrint("RegInfoInit(): Reg->PoolType: %d\n", RegInfo->PoolType);
		DbgPrint("RegInfoInit(): Saiu do PoolType\n.");
		switch (RegOperation)
		{
		case RegNtSetValueKey: {
			DbgPrint("Caiu no SetValueInit().\n");

			ULONG ReturnLength;
			NTSTATUS Status;
			UNICODE_STRING QueryStr;
			InitUnicodeString(&QueryStr);

			Status = ObQueryNameString(((PREG_SET_VALUE_KEY_INFORMATION)RegStruct)->Object, (POBJECT_NAME_INFORMATION)&QueryStr, QueryStr.MaximumLength, &ReturnLength);
			DbgPrint("Passou do ObQueryNameString().\n");
			if (!NT_SUCCESS(Status)) {
				Assert(NT_SUCCESS(Status), "at ObQueryNameString().");
				ExFreePoolWithTag(QueryStr.Buffer, 'aux');
				return Status;
			}
				
			SIZE_T Len = QueryStr.Length + sizeof * QueryStr.Buffer;
			RegInfo->CompleteName = ExAllocatePool2(RegInfo->PoolType, Len, 'sint');
			DbgPrint("Passou do ExAllocatePool2().\n");
			if (!RegInfo->CompleteName) {
				DbgPrint("RegInfo->CompleteName is NULL\n");
				return STATUS_UNSUCCESSFUL;
			}

			RtlCopyMemory(RegInfo->CompleteName, QueryStr.Buffer, QueryStr.Length);

			SIZE_T Ind = Len / sizeof * RegInfo->CompleteName;
			RegInfo->CompleteName[Len - 1] = 0;
		
			//Status = UnicodeStrToWSTR(NonPagedPool, &QueryStr, &RegInfo->CompleteName, &RegInfo->CompleteNameSize);
			//DbgPrint("Passou do UnicodeStrToWSTR.\n");
			//if (!NT_SUCCESS(Status)) {
			//	Assert(NT_SUCCESS(Status), "by UnicodeStrToWSTR().");
			//	ExFreePoolWithTag(QueryStr.Buffer, 'aux');
			//}
			
			DbgPrint("Entrando Get Key Data\n");
			GetKeyData(RegInfo, RegStruct);
			Status = STATUS_SUCCESS;
			break;
		}
		case RegNtDeleteValueKey:
			//DbgPrint("Caiu no DeleteValueInit().\n");
			//Status = DeleteValueInit(RegInfo, RegStruct, RegOperation);
			//break;
		default:
			DbgPrint("RegInfoInit Default value.\n");
			Status = STATUS_UNSUCCESSFUL;
			break;
		}
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
	NTSTATUS Status = STATUS_SUCCESS;
	
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