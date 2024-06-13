#include "token.h"
#include "common.h"

static inline void ImpersonationLevelCopy(
	PSECURITY_IMPERSONATION_LEVEL Dest,
	PSECURITY_IMPERSONATION_LEVEL Src,
	TOKEN_TYPE Type
) {

	if (!Dest || !Src)
		return;

	DbgPrint("Entrou no ImpersonationLevelCopy\n");

	if (Type == TokenImpersonation)
		RtlCopyMemory(Dest, Src, sizeof *Src);
	
}

void TokenInfoCopy(PTOKEN_INFO_STATIC Dest, PTOKEN_INFO Src) {

	if (!Dest || !Src)
		return;

	DbgPrint("Entrou no TokenInfoCopy\n");

	DbgPrint("Src: %p\nSrc->Privileges: %p\n", Src, Src->Privileges);
	DbgPrint("Src->Privileges->PrivilegeCount: %lu\n", Src->Privileges->PrivilegeCount);
	DbgPrint("*Src->Privileges->Privileges: %zu\n", sizeof * Src->Privileges->Privileges);

	ULONG PrivSize = Src->Privileges->PrivilegeCount * sizeof *Src->Privileges->Privileges;
	DbgPrint("PrivSize ta certo\n");

	RtlCopyMemory(&Dest->Type, Src->Type, sizeof *Src->Type);
	DbgPrint("Dest->Type: %d\n", Dest->Type);
	DbgPrint("Src->Type: %d\n", *Src->Type);
	DbgPrint("RtlCopyMemory 1\n");
	RtlCopyMemory(&Dest->TokenElevation, Src->TokenElevation, sizeof *Src->TokenElevation);
	DbgPrint("Dest->TokenElevation: %d\n", Dest->TokenElevation.TokenIsElevated);
	DbgPrint("Src->TokenElevation: %d\n", Src->TokenElevation->TokenIsElevated);
	DbgPrint("RtlCopyMemory 2\n");
	
	DbgPrint("Copiou tipo e elevation\n");

	ImpersonationLevelCopy(&Dest->ImpersonationLevel, Src->ImpersonationLevel, *Src->Type);
	DbgPrint("Saiu do ImpersonationLevelCopy\n");

	Dest->Privileges.PrivilegeCount = Src->Privileges->PrivilegeCount;

	RtlCopyMemory(Dest->Privileges.Privileges, Src->Privileges->Privileges, PrivSize);


}

/**
 * AcquireTokenInfo - Acquires various token information.
 * @TokenHandle: Handle to the token.
 * @TokenInfo: Pointer to a TOKEN_INFO structure to receive the token information.
 *
 * This function queries the specified token for its type, privileges, elevation,
 * and impersonation level, and stores this information in the provided TOKEN_INFO
 * structure. It returns an NTSTATUS code indicating success or failure.
 *
 * Return: NTSTATUS code.
 */
NTSTATUS AcquireTokenInfo(
	_In_ HANDLE TokenHandle,
	_Out_ PTOKEN_INFO TokenInfo)
{
	NTSTATUS Status;
	ULONG ReturnLength;

	Status = NtQueryInformationToken(TokenHandle, TokenType, &TokenInfo->Type, sizeof(TokenInfo->Type), &ReturnLength);
	if (!NT_SUCCESS(Status)) {
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "NtQueryInformationToken failed 1\n");
		return Status;
	}

	Status = NtQueryInformationToken(TokenHandle, TokenPrivileges, &TokenInfo->Privileges, sizeof(TokenInfo->Privileges), &ReturnLength);
	if (!NT_SUCCESS(Status)) {
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "NtQueryInformationToken failed 2\n");
		return Status;
	}

	Status = NtQueryInformationToken(TokenHandle, TokenElevation, &TokenInfo->TokenElevation, sizeof(TokenInfo->TokenElevation), &ReturnLength);
	if (!NT_SUCCESS(Status)) {
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "NtQueryInformationToken failed 3\n");
		return Status;
	}

	if (*TokenInfo->Type == TokenImpersonation) {
		Status = NtQueryInformationToken(TokenHandle, TokenImpersonationLevel, &TokenInfo->ImpersonationLevel, sizeof(TokenInfo->ImpersonationLevel), &ReturnLength);
		if (!NT_SUCCESS(Status)) {
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "NtQueryInformationToken failed 4 info : %x %d\n", Status, TokenInfo->Type);
			return Status;
		}
	}

	return STATUS_SUCCESS;
}

static NTSTATUS AcquireTokenInfo2(_In_ PACCESS_TOKEN pToken, _Inout_ PTOKEN_INFO TokenInfo) {

	NTSTATUS Status;

	DbgPrint("TokenInfo: %p\n", TokenInfo);
	Status = SeQueryInformationToken(pToken, TokenType, &TokenInfo->Type);
	if (!NT_SUCCESS(Status)) {
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "SeQueryInformationToken Type\n");
		return Status;
	}
	DbgPrintSt("Buffer Type: %p", TokenInfo->Type, Status);

	Status = SeQueryInformationToken(pToken, TokenPrivileges, &TokenInfo->Privileges);
	if (!NT_SUCCESS(Status)) {
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "SeQueryInformationToken Privileges\n");
		return Status;
	}
	DbgPrintSt("Buffer Privileges: %p", TokenInfo->Privileges, Status);

	Status = SeQueryInformationToken(pToken, TokenElevation, &TokenInfo->TokenElevation);
	if (!NT_SUCCESS(Status)) {
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "NtQueryInformationToken TokenElevation\n");
		return Status;
	}

	DbgPrintSt("Buffer Elevation: %p", TokenInfo->TokenElevation, Status);

	if (TokenInfo->Type == TokenImpersonation) {
		Status = SeQueryInformationToken(pToken, TokenImpersonation, &TokenInfo->ImpersonationLevel);
		if (!NT_SUCCESS(Status)) {
			DbgPrintSt("SeQueryInformationToken TokenImpersonation\n", Status);
			return Status;
		}
		DbgPrintSt("Buffer ImpersonationLevel: %p", TokenInfo->ImpersonationLevel, Status);
	}
	return Status;
}

/**
 * TokenInfoGet - Retrieves token information for a specified process.
 * @TokenInfo: Pointer to a TOKEN_INFO structure to receive the token information.
 * @PID: Handle to the process.
 *
 * This function opens the process token for the specified process and retrieves
 * various token information by calling AcquireTokenInfo. It returns an NTSTATUS
 * code indicating success or failure.
 *
 * Return: NTSTATUS code.
 */
NTSTATUS TokenInfoGet(
	_Inout_ PTOKEN_INFO TokenInfo,
	_In_ HANDLE PID)
{
	NTSTATUS Status;
	HANDLE TokenHandle;
	PACCESS_TOKEN pToken;
	PEPROCESS eProc;

	Status = PsLookupProcessByProcessId(PID, &eProc);
	if (!NT_SUCCESS(Status)) {
		DbgPrintSt("PsLookupProcessByProcessId failed No token info get\n", Status);
		return Status;
	}
		
	pToken = PsReferencePrimaryToken(eProc);
	if (!pToken) {
		DbgPrint("PsReferencePrimaryToken failed\n");
		return Status;
	}

	Status = AcquireTokenInfo2(pToken, TokenInfo);
	if (!NT_SUCCESS(Status)) {
		DbgPrintSt("AcquireTokenInfo2 failed", Status);
		return Status;
	}
	
	ObDereferenceObject(eProc);
	//Status = NtOpenProcessToken(PID, TOKEN_QUERY | TOKEN_QUERY_SOURCE, &TokenHandle);
	//if (!NT_SUCCESS(Status)) {
	//	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "NtQueryInformationToken failed 5 %x\n", Status);
	//	return Status;
	//}

	//Status = AcquireTokenInfo2(TokenHandle, TokenInfo);
	//NtClose(TokenHandle); // Ensure to close the token handle after use

	return Status;
}