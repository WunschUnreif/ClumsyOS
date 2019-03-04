#ifndef __MEM__H__
#define __MEM__H__

#include "bsp.h"

#define MEMORY_BASE ((void*)(0x68000000))
#define MEMORY_SIZE ((uint32_t)1 << 20)

void MemoryInit();
u8 MemoryCheck();
void MemoryCheckUntilGood();

#endif // !__MEM__H__
