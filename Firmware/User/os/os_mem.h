#ifndef __OS__MEM__H
#define __OS__MEM__H

#include "os.h"

#define MEM_CHAIN_TAIL (0xffff)
#define MEM_STATE_NOTUSED (0)
#define MEM_STATE_USED (1)
#define MEM_SIZE_FOR_OS (1024 * 128)

typedef struct MemBlock {
    void * startAddr;
    uint32_t size;
    uint16_t nextBlock;
    uint8_t  state;
    uint8_t  pid;
} MemBlock;

extern MemBlock os_systemMemPool[512];
extern volatile uint16_t os_memRootBlock;


/* init the memory pool */
void OSinitMemPool();

/**
 * allocate memory, returns the index of memory block.
 * if no space or no block, returns 0xffff (MEM_CHAIN_TAIL).
 */
uint16_t OSallocateMemory(uint8_t pid, uint32_t size);

/**
 * deallocate memory block numbered `block`.
 * if success, returns the next block.
 * otherwise, returns 0xffff (MEM_CHAIN_TAIL)
 */
uint16_t OSdeallocateMemory(uint16_t block);

void Test_PrintBlocks();

#endif // !__OS__MEM__H
