
#include "info.h"

/*
 *  InfoAlloc() - Allocate a 'INFO' structure.
 *
 *  @PoolType: The type of memory pool to allocate from (paged or nonpaged).
 *
 *  Return:
 *    - Pointer to the allocated 'INFO' structure on success.
 *    - 'NULL' if memory allocation fails.
 */
PINFO InfoAlloc(POOL_TYPE PoolType) {

    PINFO Info;

    Info = ExAllocatePoolWithTag(PoolType, sizeof *Info, 'info');
    if(!Info) {
        return NULL;
    }

    RtlZeroMemory(Info, sizeof *Info);

    /*
     *  Store the pool type used for the allocation in 'Info' to ensure 
     *  correct memory handling, regardless of whether the caller routine is 
     *  paged or nonpaged.
     */
    Info->PoolType = PoolType;

    return Info;
}
