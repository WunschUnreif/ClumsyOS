#include "os.h"

uint32_t os_tickNumber;

uint8_t userStack0[1024];

static inline void BSPinit() {
    UsartInit();
    MemoryInit();
}

static inline void ChangeToUserMode() {
    __set_PSP((uint32_t)(userStack0 + 1024));
    __set_CONTROL(0x3);
}

void OSinit() {
    printf("\r\n");

    BSPinit();
    printf("[OSinit] board supports init finished.\r\n");

    MemoryCheckUntilGood();

    printf("[OSinit] wait for input to start OS.\r\n");
    char c = getchar();
    printf("%c\r\n", c);

    CreateTestProcs();

    if(SysTick_Config(SystemCoreClock / 25)) {
        while(1);
    }
    os_tickNumber = 0;
    printf("[OSinit] systick init finished.\r\n");

    ChangeToUserMode();
    printf("[OSinit] mode changed.\r\n");

    root();
}

void onSysTick() {
    schedule();
}
