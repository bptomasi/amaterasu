#pragma once
#ifndef LOADIMAGE_H
#define LOADIMAGE_H

#include "common.h"
#include "amaterasu.h"

// Define the structure for image information
typedef struct _LoadImageInfo {
    IMAGE_INFO ImageInfo;           // Image information structure
    HANDLE PID;                     // Process ID
    ULONG FullImageNameSize;        // Size of the full image name
    ULONG FileNameSize;             // Size of the file name
    PWSTR FullImageName;            // Pointer to the full image name
    PWSTR FileName;                 // Pointer to the file name (if extended info is present)
    POOL_TYPE PoolType;             // Type of memory pool
} LOAD_IMAGE_INFO, * PLOAD_IMAGE_INFO;

// Function prototypes
PLOAD_IMAGE_INFO LoadImageInfoAlloc(_In_ POOL_TYPE PoolType);

NTSTATUS GetExtendedInfo(
    _In_ PLOAD_IMAGE_INFO LoadImageInfo,
    _In_ PIMAGE_INFO ImageInfo
);

NTSTATUS InitLoadImageInfoFields(
    _In_ PLOAD_IMAGE_INFO LoadImageInfo,
    _In_ PIMAGE_INFO ImageInfo,
    _In_ PUNICODE_STRING FullImageName
);

NTSTATUS LoadImageInfoInit(
    _In_ PLOAD_IMAGE_INFO LoadImageInfo,
    _In_ PIMAGE_INFO ImageInfo,
    _In_ PUNICODE_STRING FullImageName
);

void LoadImageInfoDeInit(
    _In_ PLOAD_IMAGE_INFO LoadImageInfo
);

void LoadImageInfoFree(
    _Inout_ PLOAD_IMAGE_INFO* LoadImageInfo
);

PLOAD_IMAGE_INFO LoadImageInfoGet(
    _In_ POOL_TYPE PoolType,
    _In_ PIMAGE_INFO ImageInfo,
    _In_ PUNICODE_STRING FullImageName,
    _In_ HANDLE PID
);

#endif // LOADIMAGE_H
