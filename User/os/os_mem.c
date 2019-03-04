#include "os.h"

#define MEM_LOCK_LOCKED (1)
#define MEM_LOCK_UNLOCK (0)

                    MemBlock    os_systemMemPool[512];
        volatile    uint16_t    os_memRootBlock;
        const       uint16_t    os_memBlockMaxNum = 512;
static  volatile    uint8_t     os_memLock;

/* init the memory pool */
void OSinitMemPool() {
    for(uint16_t i = 0; i < os_memBlockMaxNum; ++i) {
        os_systemMemPool[i].state = MEM_STATE_NOTUSED;
    }

    os_memRootBlock = 0;
    os_systemMemPool[os_memRootBlock].state = MEM_STATE_USED;
    /* first 128 Kib for OS */
    os_systemMemPool[os_memRootBlock].startAddr = MEMORY_BASE + MEM_SIZE_FOR_OS; 
    os_systemMemPool[os_memRootBlock].size = MEMORY_SIZE - MEM_SIZE_FOR_OS;
    /* 0 for root process */
    os_systemMemPool[os_memRootBlock].pid = 0;
    os_systemMemPool[os_memRootBlock].nextBlock = MEM_CHAIN_TAIL;

    os_memLock = MEM_LOCK_UNLOCK;
}

static inline uint16_t FindUnuesdBlock() {
    for(uint16_t i = 0; i < os_memBlockMaxNum; ++i) {
        if(os_systemMemPool[i].state == MEM_STATE_NOTUSED) {
            return i;
        }
    }
    return MEM_CHAIN_TAIL;
}

/**
 * allocate memory, returns the index of memory block.
 * if no space or no block, returns 0xffff (MEM_CHAIN_TAIL).
 */
uint16_t OSallocateMemory(uint8_t pid, uint32_t size) {
    uint16_t ptr = os_memRootBlock;
    uint16_t ans;

    /* lock the memlock until allocator finishes */
    /* because other processes may raise an allocating request */
    /* when this process isn't finished */
    while(os_memLock == MEM_LOCK_LOCKED);
    os_memLock = MEM_LOCK_LOCKED;

    /* find a proper block */
    while(os_systemMemPool[ptr].size < size) {
        ptr = os_systemMemPool[ptr].nextBlock;
    }

    if(ptr == MEM_CHAIN_TAIL) {
        return MEM_CHAIN_TAIL;
    }

    /* find an unused block */
    ans = FindUnuesdBlock();
    if(ans == MEM_CHAIN_TAIL) {
        return MEM_CHAIN_TAIL;
    }
    
    /* allocate this block to [pid] */
    os_systemMemPool[ans].state = MEM_STATE_USED;
    os_systemMemPool[ans].nextBlock = MEM_CHAIN_TAIL;
    os_systemMemPool[ans].pid = pid;
    os_systemMemPool[ans].size = size;
    os_systemMemPool[ans].startAddr = os_systemMemPool[ptr].startAddr;

    /* shrink the size of free block */
    os_systemMemPool[ptr].startAddr += size;
    os_systemMemPool[ptr].size -= size;

    /* delete node if necessary */
    if(os_systemMemPool[ptr].size == 0 && ptr != os_memRootBlock) {
        uint16_t father = os_memRootBlock;
        while(os_systemMemPool[father].nextBlock != ptr) {
            father = os_systemMemPool[father].nextBlock;
        }
        os_systemMemPool[father].nextBlock = os_systemMemPool[ptr].nextBlock;

        os_systemMemPool[ptr].state = MEM_STATE_NOTUSED;
    }

    /* unlock the memlock */
    os_memLock = MEM_LOCK_UNLOCK;

    return ans;
}

/**
 * deallocate memory block numbered `block`.
 * if success, returns the next block.
 * otherwise, returns 0xffff (MEM_CHAIN_TAIL)
 */
uint16_t OSdeallocateMemory(uint16_t block) {
    if(os_systemMemPool[block].state == MEM_STATE_NOTUSED ||
         os_systemMemPool[block].pid == 0) {
        return MEM_CHAIN_TAIL;
    }

    /* lock the memlock until deallocator finishes */
    while(os_memLock == MEM_LOCK_LOCKED);
    os_memLock = MEM_LOCK_LOCKED;

    uint16_t ans = os_systemMemPool[block].nextBlock;

    uint16_t father = MEM_CHAIN_TAIL,
             curr   = os_memRootBlock;

    /* find the correct position to insert block */
    /* <father --> curr> ==> <father --> block --> curr> */ 
    while(os_systemMemPool[block].startAddr > os_systemMemPool[curr].startAddr
          && curr != MEM_CHAIN_TAIL) {
        father = curr;
        curr = os_systemMemPool[curr].nextBlock;
    }

    if(curr == MEM_CHAIN_TAIL) {
        return MEM_CHAIN_TAIL;
    }

    /* do the insertion */
    os_systemMemPool[block].nextBlock = curr;
    os_systemMemPool[block].pid = 0;
    
    if(father == MEM_CHAIN_TAIL) {
        os_memRootBlock = block;
    } else {
        os_systemMemPool[father].nextBlock = block;
    }

    /* check if can merge <block <-- curr> */
    if(os_systemMemPool[block].startAddr + os_systemMemPool[block].size == 
            os_systemMemPool[curr].startAddr) {
        os_systemMemPool[block].size += os_systemMemPool[curr].size;
        os_systemMemPool[block].nextBlock = os_systemMemPool[curr].nextBlock;
        os_systemMemPool[curr].state = MEM_STATE_NOTUSED;
    }

    /* check if can merge <father <-- block> */
    if(father != MEM_CHAIN_TAIL && 
            os_systemMemPool[father].startAddr + os_systemMemPool[father].size == 
            os_systemMemPool[block].startAddr) {
        os_systemMemPool[father].size += os_systemMemPool[block].size;
        os_systemMemPool[father].nextBlock = os_systemMemPool[block].nextBlock;
        os_systemMemPool[block].state = MEM_STATE_NOTUSED;
    }

    /* unlock the memlock */
    os_memLock = MEM_LOCK_UNLOCK;

    return ans;
}

/* print 0~9 blocks. */
void Test_PrintBlocks() {
    printf("------------Root: %d------------\r\n", os_memRootBlock);
    for(u8 i = 0; i < 10; ++i) {
        printf("```````Block %d:\r\n", i);
        printf("Start : 0x%p\r\n", os_systemMemPool[i].startAddr);
        printf("Size  : %ld Byte(s)\r\n", os_systemMemPool[i].size);
        printf("PID   : %d\r\n", os_systemMemPool[i].pid);
        printf("Next  : %d\r\n", os_systemMemPool[i].nextBlock);
        printf("State : %d\r\n", os_systemMemPool[i].state);
    }
}
