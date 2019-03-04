#ifndef __OS__SPOT__C
#define __OS__SPOT__C

#include "os.h"

typedef struct OSexceptionStackFrame {
    volatile uint32_t r0;
    volatile uint32_t r1;
    volatile uint32_t r2;
    volatile uint32_t r3;
    volatile uint32_t r12;
    volatile uint32_t lr;
    volatile uint32_t pc;
    volatile uint32_t psr;
} OSexceptionStackFrame;

typedef struct OSregStackFrame {
    volatile uint32_t r4;
    volatile uint32_t r5;
    volatile uint32_t r6;
    volatile uint32_t r7;
    volatile uint32_t r8;
    volatile uint32_t r9;
    volatile uint32_t r10;
    volatile uint32_t r11;
} OSregStackFrame;

typedef struct OSspot {
    OSexceptionStackFrame exceptionFrame;
    OSregStackFrame regFrame;
    uint32_t returnFlag;
    uint32_t savedSP;
} OSspot;

extern OSspot os_tmpSpot;

void printSpot(OSspot * spot);
void SpotInit(volatile OSspot * spot);

#endif
