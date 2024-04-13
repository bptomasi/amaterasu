#ifndef INFO_H
#define INFO_H

#include "fileinfo/fileinfo.h"
#include "procinfo/procinfo.h"

#include "timestamp.h"
#include "mjfunc.h"

struct Info {

    POOL_TYPE   PoolType;
    INFO_TYPE   InfoType;

    MJFUNC      MjFunc;
    TIMESTAMP   TimeStamp;
    PROC_INFO   ProcInfo;

    union {
        PFILE_INFO FileInfo;
    } Info;

    LIST_ENTRY ListEntry;
};

typedef struct Info INFO, *PINFO;

/*
 *  InfoAlloc() - Allocate a 'INFO' structure.
 *
 *  @PoolType: The type of memory pool to allocate from (paged or nonpaged).
 *
 *  Return:
 *    - Pointer to the allocated 'INFO' structure on success.
 *    - 'NULL' if memory allocation fails.
 */
extern PINFO
InfoAlloc(
        __drv_stricttypematch(__drv_typeexpr)POOL_TYPE PoolType
    );

extern PINFO
InfoGet(
        __drv_stricttypematch(__drv_typeexpr)POOL_TYPE PoolType,
        _In_ PFLT_CALLBACK_DATA Data
    );

extern NTSTATUS
InfoInit(
        _Out_ PINFO Info,
        _In_ PFLT_CALLBACK_DATA Data
    );

extern void
InfoDeInit(
        _Inout_ PINFO Info
    );

extern void
InfoFree(
        _Inout_ PINFO* Info
    );

#endif  /* INFO_H */
