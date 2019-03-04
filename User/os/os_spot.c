#include "os.h"

OSspot os_tmpSpot;

void printSpot(OSspot * spot) {
    printf("[Spot->r0] =0x%lx\r\n", spot->exceptionFrame.r0);
    printf("[Spot->r1] =0x%lx\r\n", spot->exceptionFrame.r1);
    printf("[Spot->r2] =0x%lx\r\n", spot->exceptionFrame.r2);
    printf("[Spot->r3] =0x%lx\r\n", spot->exceptionFrame.r3);

    printf("[Spot->r4] =0x%lx\r\n", spot->regFrame.r4);
    printf("[Spot->r5] =0x%lx\r\n", spot->regFrame.r5);
    printf("[Spot->r6] =0x%lx\r\n", spot->regFrame.r6);
    printf("[Spot->r7] =0x%lx\r\n", spot->regFrame.r7);
    printf("[Spot->r8] =0x%lx\r\n", spot->regFrame.r8);
    printf("[Spot->r9] =0x%lx\r\n", spot->regFrame.r9);
    printf("[Spot->r10]=0x%lx\r\n", spot->regFrame.r10);
    printf("[Spot->r11]=0x%lx\r\n", spot->regFrame.r11);

    printf("[Spot->r12]=0x%lx\r\n", spot->exceptionFrame.r12);
    printf("[Spot->lr] =0x%lx\r\n", spot->exceptionFrame.lr);
    printf("[Spot->pc] =0x%lx\r\n", spot->exceptionFrame.pc);
    printf("[Spot->psr]=0x%lx\r\n", spot->exceptionFrame.psr);

    printf("[Spot->flg]=0x%lx\r\n", spot->returnFlag);

    printf("[Spot->sp] =0x%lx\r\n", spot->savedSP);
    printf("---------------------------------------------------\r\n");
}

void SpotInit(volatile OSspot * spot) {
    spot->exceptionFrame.r0 = 0;
    spot->exceptionFrame.r1 = 0;
    spot->exceptionFrame.r2 = 0;
    spot->exceptionFrame.r3 = 0;
    spot->exceptionFrame.r12 = 0;
    spot->exceptionFrame.lr = 0;
    spot->exceptionFrame.pc = 0;
    spot->exceptionFrame.psr = 0x1000000;

    spot->regFrame.r4 = 0;
    spot->regFrame.r5 = 0;
    spot->regFrame.r6 = 0;
    spot->regFrame.r7 = 0;
    spot->regFrame.r8 = 0;
    spot->regFrame.r9 = 0;
    spot->regFrame.r10 = 0;
    spot->regFrame.r11 = 0;

    spot->returnFlag = 0xfffffffd;
    spot->savedSP = 0x20010000;
}
