
#include "info.h"

/*
 *  InfoIni() -
 *
 *  @Info:
 *  @Data:
 *
 *  Return:
 *    -
 *    -
 */
NTSTATUS InfoInit(_Out_ PINFO Info, _In_ PFLT_CALLBACK_DATA Data) {

    NTSTATUS status;

    status = TimesStampInit(&Info->TimeStamp);
    if(!NT_SUCCESS(status)) {
        return status;
    }

    return status;
}
