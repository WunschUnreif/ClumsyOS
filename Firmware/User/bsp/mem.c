#include "bsp.h"
#include <stdio.h>

void MemoryInit() {
    /* enable clocks */
    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG | 
        RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOD,
        ENABLE
    );
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    /* init pins */
    GPIO_InitTypeDef gpioInitStruct;
    gpioInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    /* address lines */
    gpioInitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
                              GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | 
                              GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOF, &gpioInitStruct);

    gpioInitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
                              GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_Init(GPIOG, &gpioInitStruct);

    gpioInitStruct.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_Init(GPIOD, &gpioInitStruct);

    /* data lines */
    gpioInitStruct.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_0 | GPIO_Pin_1 |
                              GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_Init(GPIOD, &gpioInitStruct);

    gpioInitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 |
                              GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 |
                              GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOE, &gpioInitStruct);

    /* control lines */
    gpioInitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOE, &gpioInitStruct);

    gpioInitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_Init(GPIOD, &gpioInitStruct);

    gpioInitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOG, &gpioInitStruct);

    /* FSMC init */
    FSMC_NORSRAMInitTypeDef sramInitStruct;
    FSMC_NORSRAMTimingInitTypeDef timeStruct;

    timeStruct.FSMC_AddressHoldTime = 0;
    timeStruct.FSMC_AddressSetupTime = 0;
    timeStruct.FSMC_BusTurnAroundDuration = 0;
    timeStruct.FSMC_CLKDivision = 0;
    timeStruct.FSMC_DataLatency = 0;
    timeStruct.FSMC_DataSetupTime = 2;
    timeStruct.FSMC_AccessMode = FSMC_AccessMode_A;

    sramInitStruct.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
    sramInitStruct.FSMC_Bank = FSMC_Bank1_NORSRAM3;
    sramInitStruct.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    sramInitStruct.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    sramInitStruct.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    sramInitStruct.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    sramInitStruct.FSMC_MemoryType = FSMC_MemoryType_SRAM;
    sramInitStruct.FSMC_ReadWriteTimingStruct = &timeStruct;
    sramInitStruct.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    sramInitStruct.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    sramInitStruct.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    sramInitStruct.FSMC_WrapMode = FSMC_WrapMode_Disable;
    sramInitStruct.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    sramInitStruct.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    sramInitStruct.FSMC_WriteTimingStruct = &timeStruct;

    FSMC_NORSRAMInit(&sramInitStruct);
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);
}

u8 MemoryCheck() {
    volatile uint32_t * memoryStart = (uint32_t *)MEMORY_BASE;

    /* continous access */
    for(uint32_t i = 0; i < (MEMORY_SIZE >> 2); ++i) {
        memoryStart[i] = 0;
    }
    for(uint32_t i = 0; i < (MEMORY_SIZE >> 2); ++i) {
        memoryStart[i] = i;
    }
    for(uint32_t i = 0; i < (MEMORY_SIZE >> 2); ++i) {
        memoryStart[i] = 0;
    }

    for(uint32_t i = 0; i < (MEMORY_SIZE >> 2); ++i) {
        memoryStart[i] = i;
    }

    u8 success = 1;
    for(uint32_t i = 0; i < (MEMORY_SIZE >> 2); i += 128) {
        if(memoryStart[i] != i) {
            printf("Mem check round 1 failed. [%ld] reading %ld\r\n", i, memoryStart[i]);
            success = 0;
        }
    }

    for(uint32_t i = 0; i < (MEMORY_SIZE >> 2); ++i) {
        memoryStart[i] = 0;
    }
    for(uint32_t i = 0; i < (MEMORY_SIZE >> 2); i += 128) {
        if(memoryStart[i] != 0) {
            printf("Mem check round 2 failed. [%ld] reading %ld\r\n", i, memoryStart[i]);
            success = 0;
        }
    }

    return success;
}

void MemoryCheckUntilGood() {
    do {
        printf("Mem checking...\r\n");
    } while(MemoryCheck() == 0);
    do {
        printf("Mem checking...\r\n");
    } while(MemoryCheck() == 0);
    do {
        printf("Mem checking...\r\n");
    } while(MemoryCheck() == 0);
    printf("Mem check finish.\r\n");
}
