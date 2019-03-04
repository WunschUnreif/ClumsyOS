#include "os.h"

volatile uint8_t currentProcID = 0;

volatile OSprocess procPool[256];

void root() {
    while(1);
}

volatile void testProc1() {
    long long sum = 0;
    for(int i = 1; i <= 10000; ++i) {
        sum += i;
        printf("A %d\r\n", i);
    }
    printf("[proc A] finished with ans %lld!\r\n", sum);
}

volatile void testProc2() {
    long long sum = 0;
    for(int i = 10000; i > 0; --i) {
        sum += i;
        printf("B %d\r\n", i);
    }
    printf("[proc B] finished with ans %lld!\r\n", sum);
}

volatile void killCurrProc() {
    procPool[currentProcID].state = DEAD;
    while(1);
}

void CreateTestProcs() {
    // SpotInit(&procPool[0].spot);
    // procPool[0].pid = 0;
    // procPool[0].state = ALIVE;
    // procPool[0].spot.exceptionFrame.pc = (uint32_t)&root & 0xfffffffe;
    // procPool[0].spot.exceptionFrame.lr = 0x01;
    // procPool[0].spot.savedSP = (uint32_t)userStack0 + 10240 - 32;

    // SpotInit(&procPool[1].spot);
    // procPool[1].pid = 1;
    // procPool[1].state = ALIVE;
    // procPool[1].spot.exceptionFrame.pc = (uint32_t)&testProc1 & 0xfffffffe;
    // procPool[1].spot.exceptionFrame.lr = (uint32_t)&killCurrProc | 1;
    // procPool[1].spot.savedSP = (uint32_t)userStack1 + 10240 - 32;

    // SpotInit(&procPool[2].spot);
    // procPool[2].pid = 2;
    // procPool[2].state = ALIVE;
    // procPool[2].spot.exceptionFrame.pc = (uint32_t)&testProc2 & 0xfffffffe;
    // procPool[2].spot.exceptionFrame.lr = (uint32_t)&killCurrProc | 1;
    // procPool[2].spot.savedSP = (uint32_t)userStack2 + 10240 - 32;
}

void schedule() {
    procPool[currentProcID++].spot = os_tmpSpot;
    while(procPool[currentProcID].state == DEAD) ++currentProcID;
    os_tmpSpot = procPool[currentProcID].spot;
}
