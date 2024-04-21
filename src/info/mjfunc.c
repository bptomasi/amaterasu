
#include "mjfunc.h"

#define NOT_IMPLEMENTED {0x0, 0x0, NULL}

/*
 *
 */
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
 *  MjImplemented() -
 *
 *  @Code:
 *
 *  Return:
 *    -
 *    -
 */
static inline BOOL MjImplemented(_In_ UCHAR Code) {
    
    return MajorFunctions[Code].Len;
}

/*
 *  MjFuncInit() - 
 *
 *  @MjFunc: 
 *  @MjFuncCode:
 *
 *  Return:
 *    -
 *    -
 */
NTSTATUS MjFuncInit(_Out_ PMJFUNCTION MjFunc, _In_ UCHAR Code) {

    if(!MjImplemented(Code)) {
        return STATUS_INVALID_PARAMETER;
    }

    RltCopyBytes(MjFunc, &MajorFunctions[Code], sizeof *MjFunc);

    return STATUS_SUCCESS;
}
