
#include "mjfunc.h"

#define NOT_IMPLEMENTED {0x0, 0x0, NULL}

static const MJFUNC MajorFunctions[] = {
    {0x00, 0x0e, L"IRP_MJ_CREATE"},
    NOT_IMPLEMENTED               ,
    NOT_IMPLEMENTED               ,
    {0x03, 0x0c, L"IRP_MJ_READ"  },
    {0x04, 0x0d, L"IRP_MJ_WRITE" },
    NOT_IMPLEMENTED               ,
    NOT_IMPLEMENTED               ,
    NOT_IMPLEMENTED               ,
    NOT_IMPLEMENTED               ,
    NOT_IMPLEMENTED               ,
    NOT_IMPLEMENTED               ,
    NOT_IMPLEMENTED               ,
    NOT_IMPLEMENTED               ,
    NOT_IMPLEMENTED               ,
    NOT_IMPLEMENTED               ,
    NOT_IMPLEMENTED
};

/*
 *  MjImplemented() - Checks if a specific major IRP function is implemented
 *                    in the driver.
 *
 *  @Code: The function code to check.
 *
 *  Return:
 *    - 'true' if the function is implemented.
 *    - 'false' otherwise
 */
static inline BOOLEAN MjImplemented(_In_ UCHAR Code) {
    
    return MajorFunctions[Code].Len;
}

/*
 *  MjFuncInit() - Initializes a 'MJFUNTION' structure.
 *
 *  @MjFunc: Pointer to a 'MJFUNCTION' structure.
 *  @Code: The function code to initialize the structure with.
 *
 *  Return:
 *    - 'STATUS_SUCCESS' if the initialization is successful.
 *    - 'STATUS_INVALID_PARAMETER' if the function is not implemented.
 */
NTSTATUS MjFuncInit(_Out_ PMJFUNCTION MjFunc, _In_ UCHAR Code) {

    if(!MjImplemented(Code)) {
        return STATUS_INVALID_PARAMETER;
    }

    RltCopyBytes(MjFunc, &MajorFunctions[Code], sizeof *MjFunc);

    return STATUS_SUCCESS;
}
