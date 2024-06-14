
#include "RegInfo.h"


void RegInfoDeInit(PREG_INFO RegInfo) {

	DbgPrint("Entrou no RegInfoDeInit()\n");
	//if (RegInfo) {
	//	if (RegInfo->Data) {
	//		DbgPrint("%s:%d: RegInfo->Data : %ws\n", __func__, __LINE__, RegInfo->Data);
	//		DbgPrint("%s:%d: Entrou no segundo if\n", __func__, __LINE__);
	//		ExFreePoolWithTag(RegInfo->Data, 'wstr');
	//		DbgPrint("%s:%d: Saiu do segundo if\n", __func__, __LINE__);
	//	}

	//	if (RegInfo->CompleteName) {
	//		DbgPrint("%s:%d: Entrou no primeiro if\n", __func__, __LINE__);
	//		DbgPrint("%s:%d RegInfo->CompleteName : %p\n", __func__, __LINE__, RegInfo->CompleteName);
	//		DbgPrint("%s:%d: RegInfo->CompleteName: %ws\n", __func__, __LINE__, RegInfo->CompleteName);
	//		ExFreePoolWithTag(RegInfo->CompleteName, 'wstr');
	//		DbgPrint("%s:%d: Saiu do primeiro if\n", __func__, __LINE__);
	//	}
	//}
	DbgPrint("Saiu do RegInfoDeInit()");
}

void RegInfoFree(PREG_INFO* RegInfo) {

	if (!RegInfo || *RegInfo == NULL)
		return;

	DbgPrint("%s:%d (*RegInfo)->CompleteName : %p\n", __func__, __LINE__, (*RegInfo)->CompleteName);
	DbgPrint("%s:%d: (*RegInfo)->CompleteName: %ws\n", __func__, __LINE__, (*RegInfo)->CompleteName);

	DbgPrint("Entrou no RegInfoFree().\n\n");
	RegInfoDeInit(*RegInfo);
	DbgPrint("Saiu do RegInfoInig()\n");

	ExFreePoolWithTag(*RegInfo, 'reg');

	*RegInfo = NULL;
	DbgPrint("Saiu do RegInfoFree().\n");
}

PREG_INFO RegInfoGet(_PoolType_ POOL_TYPE PoolType, PREG_INFO_DATA RegInfoData) {

	PREG_INFO RegInfo;
	NTSTATUS Status = STATUS_SUCCESS;
	
	PREG_SET_VALUE_KEY_INFORMATION RegStruct = RegInfoData->RegStruct;
    DbgPrint("%s:%d: &RegStruct = %p\n", __func__, __LINE__, RegStruct);

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

		DbgPrint("%s completename %ws\n", __func__, RegInfo->CompleteName);
		DataSize = min(MAX_PATH, RegInfo->CompleteNameSize);
		if (DataSize) {
			DbgPrint("Entrou no primeiro copy.\n")
			RtlCopyMemory(StaticRegInfo->CompleteName, RegInfo->CompleteName, DataSize * sizeof * RegInfo->CompleteName);
		}
		DbgPrint("Passou do primeiro Copy\n");
		
		DbgPrint("%s completename %ws\n", __func__, RegInfo->CompleteName);

		DataSize = min(MAX_DATA, RegInfo->DataSize);
		if (DataSize) {
			DbgPrint("Entrou no segundo copy.\n");
			RtlCopyMemory(StaticRegInfo->Data, RegInfo->Data, sizeof * RegInfo->Data * DataSize);
		}
		DbgPrint("Passou do segundo copy\n");
		DbgPrint("%s:%d:RegInfo->RegNotifyClass:%d\n", __func__, __LINE__, RegInfo->RegNotifyClass);
	}
}


