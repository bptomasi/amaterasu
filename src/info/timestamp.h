#ifndef TIMESTAMP_H
#define TIMESTAMP_H

struct TimeStamp {

    PWSTR Str;
    ULONG Len;
};

typedef struct TimeStamp TIMESTAMP, *PTIMESTAMP;

#endif  /* TIMESTAMP_H */
