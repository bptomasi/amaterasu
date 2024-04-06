#ifndef FILEDATA_H
#define FILEDATA_H

typedef struct FileData {

    ULONG PathSize;
    ULONG FinalNameSize;

    PWSTR Path;
    PWSTR FinalName;

} FILE_DATA, *PFILE_DATA;

#endif  /* FILEDATA_H */
