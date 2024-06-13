#include "utils.h"


void InitUnicodeString(PUNICODE_STRING Str) {
    Str->Buffer = ExAllocatePoolWithTag(NonPagedPool, MAX_STR_SIZE, 'buf');
    Str->Length = 0;
    Str->MaximumLength = MAX_STR_SIZE;
}

void BinaryDataToUnicodeSTR(PVOID BinData, PUNICODE_STRING UnicodeStr, ULONG DataSize) {
    UNICODE_STRING hexValue;
    InitUnicodeString(&hexValue);

    for (ULONG i = 0; i < DataSize; ++i) {
        RtlIntegerToUnicodeString(((UCHAR*)BinData)[i], 16, &hexValue);

        RtlUnicodeStringCat(UnicodeStr, &hexValue);
        RtlUnicodeStringCatString(UnicodeStr, L" ");
    }

    ExFreePoolWithTag(hexValue.Buffer, 'buf');
}

void WORDDataToUnicodeSTR(PVOID WordData, PUNICODE_STRING UnicodeStr, ULONG DataSize) {
    UNICODE_STRING hexValue;
    InitUnicodeString(&hexValue);

    for (ULONG i = DataSize; i > 0; --i) {
        RtlIntegerToUnicodeString(((UCHAR*)WordData)[i], 16, &hexValue);

        RtlUnicodeStringCat(UnicodeStr, &hexValue);
        RtlUnicodeStringCatString(UnicodeStr, L" ");
    }

    ExFreePoolWithTag(hexValue.Buffer, 'buf');
}

/*
 *  UnicodeStrToWSTR() -
 *
 *  @PoolType:
 *  @Src:
 *  @Dest:
 *  @pSize:
 *
 *  Return:
 *    -
 *    -
 */
NTSTATUS UnicodeStrToWSTR(_In_ POOL_TYPE PoolType, _In_ PUNICODE_STRING Src, _Out_ PWSTR* Dest, _Out_ PULONG pSize) {

    ULONG Size;
    PWSTR Wstr;

    DbgPrint("Entrou no UnicodeStrToWSTR");

    /*
     *  As 'UNICODE_STRING::Length' does not include the '/0', we
     *  add '1' to it.
     */
    Size = Src->Length + sizeof *Wstr;
    
    DbgPrint("Src_>lenghth %ul\n", Size );
    Wstr = ExAllocatePoolWithTag(PoolType, Size, 'wstr');
    
    DbgPrint("teste 2\n");
    if(!Wstr) {
        Assert(Wstr != NULL, "at ExAllocatePoolWithTag().\n");
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    DbgPrint("teste 3\n");
    SIZE_T Ind = Size / sizeof * Wstr;
    DbgPrint("Ind; %lu", Ind);


    DbgPrint("teste 4\n");
    /* null terminate the string. */
    Wstr[Ind - 1] = 0;
    
    DbgPrint("teste 5\n");
    RtlCopyMemory(Wstr, Src->Buffer, Src->Length);

    DbgPrint("pSize %p %llx\n",pSize, pSize );
    *pSize = Size;
    DbgPrint("teste 7\n");
    *Dest = Wstr;
    DbgPrint("teste 8\n");


    DbgPrint("Saiu do UnicodeStrToWSTR");
    return STATUS_SUCCESS;
}

NTSTATUS UnicodeStrToStaticWSTR(_Inout_ WCHAR Dest[MAX_PATH], _In_ PUNICODE_STRING Src, _Inout_ PULONG pSize) {
    ULONG minSize = min(MAX_PATH-1, Src->Length/sizeof(WCHAR));

    DbgPrint("Entrou no UnicodeStrToStaticWSTR");
    /* null terminate the string. */
    
    RtlZeroMemory(Dest, MAX_PATH * sizeof * Dest);

    RtlCopyMemory(Dest, Src->Buffer, minSize * sizeof(WCHAR));

    *pSize = minSize;

    DbgPrint("Saiu do UnicodeStrToStaticWSTR");
    return STATUS_SUCCESS;
}

