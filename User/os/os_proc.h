#ifndef __OS__PROC__H
#define __OS__PROC__H

#include "os.h"

#define ALIVE (0x01)
#define DEAD  (0x00)

typedef struct OSprocess {
    OSspot spot;
    uint8_t pid;
    uint8_t state;
} OSprocess;

void root();
void CreateTestProcs();
void schedule();

#endif // !__OS__PROC__H
