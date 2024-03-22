
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

    UNREFERENCED_PARAMETER(Flags);

    PAGED_CODE();

    AmaterasuCleanup();
    FltUnregisterFilter(Amaterasu.FilterHandle);

    Amaterasu.FilterHandle = NULL;

    return STATUS_SUCCESS;
}
