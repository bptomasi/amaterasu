
#include "Amaterasu.h"

#ifdef ALLOCA_PRAGMA
#   pragma alloc_text(PAGED, AmaterasuUnload)
#endif

/*
 *  AmaterasuUnload() -
 *
 *  @Flags
 *
 *  Return:
 *    -
 *    -
 */
NTSTATUS AmaterasuUnload(_In_ FLT_FILTER_UNLOAD_FLAGS Flags) {

    PAGED_CODE();
    UNREFERENCED_PARAMETER(Flags);

    AmaterasuCleanup();
    FltUnregisterFilter(Amaterasu.FilterHandle);

    return STATUS_SUCCESS;
}
