
#include "amaterasu.h"
#include "RegInfo.h"


PREG_INFO RegInfoAlloc(_PoolType_ POOL_TYPE PoolType) {

	PREG_INFO RegInfo;

	RegInfo = ExAllocatePool2(POOL_FLAG_NON_PAGED, sizeof * RegInfo, 'rifo');
	if (!RegInfo) {
		Assert(RegInfo);
		return NULL;
	}

	RtlZeroMemory(RegInfo, sizeof * RegInfo);

	RegInfo->PoolType = POOL_FLAG_NON_PAGED;

	DbgPrint("%s:RegInfo addr = %p\n", __func__, RegInfo);
	DbgPrint("%s:&RegInfo->PoolType = %p\n", __func__, &RegInfo->PoolType);
	DbgPrint("%s:&(*RegInfo).PoolType = %p\n", __func__, &(*RegInfo).PoolType);
	DbgPrint("%s:RegInfo->PoolType = %d\n", __func__, RegInfo->PoolType);

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

static NTSTATUS AcquireCompleteName(PREG_INFO RegInfo, PVOID Object, PUNICODE_STRING* Out) {

	NTSTATUS Status;

	Assert(RegInfo);
	Assert(Object);

	Status = STATUS_UNSUCCESSFUL;
	if (!RegInfo || !Object) {
		return Status;
	}

	DbgPrint("%s:Antes CmCallbackGetKeyObjectID()\n", __func__);
	Status = CmCallbackGetKeyObjectID(&Amaterasu.Cookie, Object, NULL, Out);
	DbgPrint("%s:Depois CmCallbackGetKeyObjectID()\n", __func__);
	if (!NT_SUCCESS(Status)) {
		Assert(NT_SUCCESS(Status), "CmCallbackGetKeyObjectID()");
		return Status;
	}

	return Status;
}

NTSTATUS SetValueInit(
	PREG_INFO RegInfo,
	PREG_SET_VALUE_KEY_INFORMATION RegStruct
) {

	NTSTATUS Status;
	PUNICODE_STRING QueryStr;

	Status = STATUS_UNSUCCESSFUL;
	if (RegInfo && RegStruct) {

		DbgPrint("%s:Entrando no AcquireCompleteName()\n", __func__);
		Status = AcquireCompleteName(RegInfo, ((PREG_SET_VALUE_KEY_INFORMATION)RegStruct)->Object, &QueryStr);
		DbgPrint("%s:Saiu do AcquireCompleteName()\n", __func__);
		if (!NT_SUCCESS(Status)) {
			Assert(NT_SUCCESS(Status), "AcquireCompleteName().");
			return Status;
		}

		Status = UnicodeStrToWSTR(RegInfo->PoolType, QueryStr, &RegInfo->CompleteName, &RegInfo->CompleteNameSize);
		if (!NT_SUCCESS(Status)) {
			Assert(NT_SUCCESS(Status), "UnicodeStrToWSTR().");
			ExFreePool(RegInfo->CompleteName);
			RegInfo->CompleteName = NULL;
			return Status;
		}

		DbgPrint("%s:After UnicodeStrToWSTR: %ws\n", __func__, RegInfo->CompleteName);
		GetKeyData(RegInfo, RegStruct);
	}

	return Status;
}

NTSTATUS DeleteValueInit(
	PREG_INFO RegInfo,
	PREG_DELETE_VALUE_KEY_INFORMATION RegStruct
) {

	NTSTATUS Status;

	Status = STATUS_UNSUCCESSFUL;

	if (RegInfo && RegStruct) {

		DbgPrint("%s:RegInfo addr = %p\n", __func__, RegInfo);
		DbgPrint("%s:&RegInfo->PoolType = %p\n", __func__, &RegInfo->PoolType);
		DbgPrint("%s:&(*RegInfo).PoolType = %p\n", __func__, &(*RegInfo).PoolType);
		DbgPrint("%s:RegInfo->PoolType = %d\n", __func__, RegInfo->PoolType);

		Status = AcquireCompleteName(RegInfo, RegStruct->Object, NULL);
		if (!NT_SUCCESS(Status)) {
			Assert(NT_SUCCESS(Status), "AcquireCompleteName().");
			return Status;
		}
		RegInfo->Data = NULL;
		RegInfo->DataSize = 0;
		RegInfo->DataType = 0;
	}


	return Status;
}

NTSTATUS RegInfoInit(PREG_INFO RegInfo, REG_NOTIFY_CLASS RegOperation, PVOID RegStruct) {

	NTSTATUS Status;

	Status = STATUS_UNSUCCESSFUL;
	DbgPrint("Entrou no RegInfoInit()\n");
	if (RegOperation == RegNtSetValueKey) {
		DbgPrint("Entrando no SetInit");
		Status = SetValueInit(RegInfo, ((PREG_SET_VALUE_KEY_INFORMATION)RegStruct)->Object);
	}
	else {
		//Status = DeleteValueInit(RegInfo, RegStruct);
	}
	
	DbgPrint("Saiu do RegInfoInit()\n");
	return Status;
}
