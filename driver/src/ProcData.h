#ifndef PROCESSDATA_H
#define PROCESSDATA_H

typedef struct ProcessData {

    ULONG Pid;
    ULONG ProcSize;
    UCHAR Process[256];

} PROC_DATA, *PPROC_DATA;

#endif  /* PROCESSDATA_H */
