#ifndef INFO_H
#define INFO_H

#include "procinfo.h"
#include "fileinfo.h"
#include "mjfunc.h"

struct Info {

    POOL_TYPE   POOL_TYPE;
    MJFUNC      MjFunc;
    TIME_FIELDS TimeFields;
    PPROC_INFO  ProcInfo;

    union {
        PFILE_INFO FileInfo;
    } Info;

    LIST_ENTRY  ListEntry;
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

/*
 *  InfoGet() - Allocates and initializes a 'INFO' structure.
 *
 *  @PoolType: The type of memory pool to allocate from (paged or nonpaged).
 *  @Data: Pointer to a FLT_CALLBACK_DATA object.
 *
 *  'InfoGet()' is a wrapper for 'InfoAlloc()' and 'InfoInit()',
 *  providing a simple interface to obtain an allocated and initialized
 *  'INFO' structure.
 *
 *  Returns:
 *    - Pointer to the allocated 'INFO' structure on success.
 *    - 'NULL' if memory allocation or initialization fails.
 */
extern PINFO
InfoGet(
        __drv_stricttypematch(__drv_typeexpr)POOL_TYPE PoolType,
        _In_ PFLT_CALLBACK_DATA Data
    );

/*
 *  InfoInit() - Initializes a 'INFO' structure allocate by 'InfoAlloc()'
 *               based on 'FLT_CALLBACK_DATA'.
 *
 *  @Info: Pointer to the 'INFO' structure to be initialized.
 *  @Data: Pointer to the 'FLT_CALLBACK_DATA' structure containing file-related
 *         and process related data.
 *
 *  Return:
 *    - Status code indicating the result of the initialization.
 *    - Upon success, returns 'STATUS_SUCCESS'.
 *    - The appropriate error code in case of failure.
 */
extern NTSTATUS
InfoInit(
        _Out_ PINFO Info,
        _In_ PFLT_CALLBACK_DATA Data
    );

/*
 *  InfoDeInit() - Deinitializes and frees the internal resources
 *                 associated with a 'INFO' structure.
 *
 *  @Info: Pointer to the 'INFO' structure to be deinitialized.
 */
extern void
InfoDeInit(
        _Inout_ PINFO Info
    );

/*
 *  InfoFree() - Deallocates the memory associated with a 'INFO'
 *               struct dynamically allocated.
 *
 *  @Info: Pointer to a reference of a 'INFO' structure.
 */
extern void
InfoFree(
        _Inout_ PINFO* Info
    );

#endif  /* INFO_H */
