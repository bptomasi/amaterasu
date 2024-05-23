#ifndef MJFUNC_H
#define MJFUNC_H

struct MjFunc {
    
    UCHAR Code;
    ULONG Len;
    PWSTR Name;
};

typedef struct MjFunc MJFUNC, *PMJFUNC;

/*
 *  MjFuncInit() - Initializes a 'MJFUNTION' structure.
 *
 *  @MjFunc: Pointer to a 'MJFUNCTION' structure.
 *  @MjFuncCode: The function code to initialize the structure with.
 *
 *  Return:
 *    - 'STATUS_SUCCESS' if the initialization is successful.
 *    - 'STATUS_INVALID_PARAMETER' if the function is not implemented.
 */
extern NTSTATUS
MjFuncInit(
    _Out_ PMJFUNCTION MjFunc,
    _In_ UCHAR Code
);

#endif  /* MJFUNC_H */
