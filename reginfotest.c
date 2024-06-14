
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
	DbgPrint("Entrou no GetKeyData()\n");
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

	DbgPrint("Saiu do GetKeyData()\n\n");
	return STATUS_SUCCESS;
}

static NTSTATUS AcquireCompleteName(PREG_INFO RegInfo, PVOID Object, POBJECT_NAME_INFORMATION* ObjectName) {
	DbgPrint("entrou em %s\n", __func__);
	NTSTATUS Status;
	ULONG ObjNameLen = 0, ReturnLen = 0;
	POBJECT_NAME_INFORMATION auxObjName = NULL;

	Assert(RegInfo);
	Assert(Object);

	DbgPrint("%s:%d\n", __func__, __LINE__);
	Status = STATUS_UNSUCCESSFUL;
	DbgPrint("%s:%d\n", __func__, __LINE__);
	if (!RegInfo || !Object) {
		DbgPrint("%s:%d\n", __func__, __LINE__);
		return Status;
	}


	DbgPrint("%s:%d\n", __func__, __LINE__);
	KIRQL irql = KeGetCurrentIrql();
	DbgPrint("%s:%d\n", __func__, __LINE__);
	DbgPrint("IRQL: %u %c %p\n", irql, irql, &irql);
	DbgPrint("%s:%d\n", __func__, __LINE__);

	*ObjectName = NULL;
	DbgPrint("%s:%d\n", __func__, __LINE__);
	DbgPrint("Setou ObjectName == NULL\n");


	DbgPrint("%s:%d\n", __func__, __LINE__);
	Status = ObQueryNameString(Object, NULL, 0, &ObjNameLen);
	DbgPrint("%s:%d\n", __func__, __LINE__);
	DbgPrint("ObQueryNameString to get size\n");
	if (Status = STATUS_INFO_LENGTH_MISMATCH) {
		DbgPrint("%s:%d\n", __func__, __LINE__);
		DbgPrint("mismatch\n");
		DbgPrint("%s:%d\n", __func__, __LINE__);
		ObjNameLen += sizeof(OBJECT_NAME_INFORMATION);
		DbgPrint("%s:%d\n", __func__, __LINE__);
		auxObjName = ExAllocatePool2(POOL_FLAG_NON_PAGED, ObjNameLen, 'obj');
		DbgPrint("%s:%d\n", __func__, __LINE__);
		if (!auxObjName) {
			DbgPrint("%s:%d\n", __func__, __LINE__);
			Status = STATUS_UNSUCCESSFUL;
			DbgPrint("%s:%d\n", __func__, __LINE__);
		}
		else {
			DbgPrint("%s:%d\n", __func__, __LINE__);
			Status = ObQueryNameString(Object, auxObjName, ObjNameLen, &ObjNameLen);
			DbgPrint("%s:%d\n", __func__, __LINE__);
			if (!NT_SUCCESS(Status)) {
				DbgPrint("%s:%d\n", __func__, __LINE__);
				ExFreePoolWithTag(auxObjName, 'obj');
				DbgPrint("%s:%d\n", __func__, __LINE__);
				return STATUS_UNSUCCESSFUL;
			}
			DbgPrint("%s:%d\n", __func__, __LINE__);
			*ObjectName = auxObjName;
			DbgPrint("%s:%d\n", __func__, __LINE__);
			DbgPrint("objectnaem : %s  %ws\n", __func__, (*ObjectName)->Name.Buffer);
		}
	}
	else if (NT_SUCCESS(Status)) {
		DbgPrint("%s:%d\n", __func__, __LINE__);
		DbgPrint("ObQuery deu sucesso\n");
		DbgPrint("%s:%d\n", __func__, __LINE__);
		ObjNameLen = sizeof(OBJECT_NAME_INFORMATION);
		DbgPrint("%s:%d\n", __func__, __LINE__);
		auxObjName = (POBJECT_NAME_INFORMATION)ExAllocatePool2(POOL_FLAG_NON_PAGED, ObjNameLen, 'obj');
		DbgPrint("%s:%d\n", __func__, __LINE__);
		if (auxObjName != NULL) {
			DbgPrint("%s:%d\n", __func__, __LINE__);
			RtlZeroMemory(auxObjName, ObjNameLen);
			DbgPrint("%s:%d\n", __func__, __LINE__);
			*ObjectName = auxObjName;
			DbgPrint("%s:%d\n", __func__, __LINE__);
			Status = STATUS_SUCCESS;
		}
	}


	//DbgPrint("%s:Antes CmCallbackGetKeyObjectID()\n", __func__);
	////try
	////{
	//	DbgPrint("ObQueryNameString para pegar o tamanho\n")
	//	//Status = ObQueryNameString(*Object, NULL, 0, &ReturnLen1);
	//	DbgPrint("ObQueryNameString para pegar o tamanho dps\n")

	//DbgPrint("Return len 1 %lu\n", ReturnLen1);

	//	//Status = CmCallbackGetKeyObjectIDEx(&Amaterasu.Cookie, Object, NULL, Out,0);
	//	DbgPrint("ObQueryNameString \n")
	//	Status = ObQueryNameString(Object, (PVOID)Out,Out->MaximumLength,&ReturnLen);
	//	DbgPrint("ObQueryNameString dps \n")
	///*}
	//except(REGISTRY_FILTER_DRIVER_EXCEPTION) {
	//	DbgPrint("REGISTRY_FILTER_DRIVER_EXCEPTION %x\n", Status);
	//	DbgPrint("reg path : %ws\n", (*Out)->Buffer);
	//};*/

	//DbgPrint("reg path : %ws\n", Out->Buffer);
	//DbgPrint("Return len %lu\n", ReturnLen);

	//DbgPrint("%s:Depois CmCallbackGetKeyObjectID()\n", __func__);
	//if (!NT_SUCCESS(Status)) {
	//	Assert(NT_SUCCESS(Status), "CmCallbackGetKeyObjectID()");
	//	return Status;
	//}
	//else {
	//	DbgPrint("deu boa o reg path : %ws\n", Out->Buffer);

	//}

	return Status;
}

NTSTATUS SetValueInit(
	PREG_INFO RegInfo,
	PREG_SET_VALUE_KEY_INFORMATION RegStruct
) {

	NTSTATUS Status;
	UNICODE_STRING QueryStr;
	InitUnicodeString(&QueryStr);

	POBJECT_NAME_INFORMATION ObjectName;

	Status = STATUS_UNSUCCESSFUL;
	if (RegInfo && RegStruct) {
		DbgPrint("%s:%d\n", __func__, __LINE__);
		DbgPrint("%s:%d: &RegStruct: %p\n", __func__, __LINE__, RegStruct);
		DbgPrint("%s:%d\n", __func__, __LINE__);
		DbgPrint("%s:%d: &RegStruct->ValueName: %p\n", __func__, __LINE__, RegStruct->ValueName);
		DbgPrint("%s:%d\n", __func__, __LINE__);
		if (!RegStruct->ValueName) {
			DbgPrint("%s:%d: RegStruct->ValueName is NULL!!!!\n", __func__, __LINE__);
			return STATUS_UNSUCCESSFUL;
		}

		Status = AcquireCompleteName(RegInfo, RegStruct->Object, &ObjectName);
		DbgPrint("%s:%d\n", __func__, __LINE__);
		if (!NT_SUCCESS(Status)) {
			DbgPrint("%s:%d\n", __func__, __LINE__);
			Assert(NT_SUCCESS(Status), "AcquireCompleteName().");
			DbgPrint("%s:%d\n", __func__, __LINE__);
			return Status;
		}

		DbgPrint("%s:%d\n", __func__, __LINE__);
		Status = UnicodeStrToWSTR(RegInfo->PoolType, &ObjectName->Name, &RegInfo->CompleteName, &RegInfo->CompleteNameSize);
		DbgPrint("%s:%d RegInfo->CompleteName : %p\n", __func__, __LINE__, RegInfo->CompleteName);
		ExFreePoolWithTag(ObjectName, 'obj');
		DbgPrint("%s:%d\n", __func__, __LINE__);
		if (!NT_SUCCESS(Status)) {
			DbgPrint("%s:%d\n", __func__, __LINE__);
			Assert(NT_SUCCESS(Status), "UnicodeStrToWSTR().");
			DbgPrint("%s:%d\n", __func__, __LINE__);
			ExFreePoolWithTag(RegInfo->CompleteName, 'wstr');
			DbgPrint("%s:%d\n", __func__, __LINE__);
			RegInfo->CompleteName = NULL;
			DbgPrint("%s:%d\n", __func__, __LINE__);
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

    DbgPrint("%s:%d: &RegStruct = %p\n", __func__, __LINE__, RegStruct);

	Status = STATUS_UNSUCCESSFUL;
	DbgPrint("Entrou no RegInfoInit()\n");
	if (RegOperation == RegNtPreSetValueKey) {
		DbgPrint("Entrando no SetInit");
		Status = SetValueInit(RegInfo, RegStruct);
	}
	else {
		//Status = DeleteValueInit(RegInfo, RegStruct);
	}
	
	DbgPrint("Saiu do RegInfoInit()\n");
	return Status;
}
