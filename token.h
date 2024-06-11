#pragma once

#ifndef TOKEN_INFO_H
#define TOKEN_INFO_H

#include "common.h"

#define MAX_PRIV_COUNT 35

// Structure to hold token information
struct TokenInfo {
	PTOKEN_TYPE Type;
	PTOKEN_PRIVILEGES Privileges;
	PTOKEN_ELEVATION TokenElevation;
	PSECURITY_IMPERSONATION_LEVEL ImpersonationLevel;
};


struct privileges {
	DWORD PrivilegeCount;
	LUID_AND_ATTRIBUTES Privileges[MAX_PRIV_COUNT];
};

typedef struct privileges PRIVILEGES, * PPRIVILEGES;

struct TokenInfoStatic {
	TOKEN_TYPE Type;
	PRIVILEGES Privileges;
	TOKEN_ELEVATION TokenElevation;
	SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;
};

typedef struct TokenInfo TOKEN_INFO, * PTOKEN_INFO;

typedef struct TokenInfoStatic TOKEN_INFO_STATIC, * PTOKEN_INFO_STATIC;


// Function prototypes
NTSTATUS AcquireTokenInfo(
	_In_ HANDLE TokenHandle,
	_Out_ PTOKEN_INFO TokenInfo);

NTSTATUS TokenInfoGet(
	_Inout_ PTOKEN_INFO TokenInfo,
	_In_ HANDLE PID);

void TokenInfoCopy(
	PTOKEN_INFO_STATIC Dest,
	PTOKEN_INFO Src
);

#endif // TOKEN_INFO_H
