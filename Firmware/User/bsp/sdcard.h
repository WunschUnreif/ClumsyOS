#ifndef __SDCARD__H__
#define __SDCARD__H__

#include "bsp.h"

#define SDIO_TRANSFER_CLK_DIV (1)
#define SDIO_INIT_CLK_DIV (0xB2)
#define SDIO_FIFO_ADDRESS ((uint32_t)0x40018080)

extern uint8_t * sdcardBuffer;
extern uint16_t  sdcardBufferSize;

void SD_LowLevel_Init();
void SD_LowLevel_DeInit();
void SD_LowLevel_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize);
void SD_LowLevel_DMA_RxConfig(uint32_t *BufferSRC, uint32_t BufferSize);
uint32_t SD_DMAEndOfTransferStatus(void);

u8 SDcardInit();
void Test_printCardInfo();
void Test_readBlock();

#endif // !__SDCARD__H__
