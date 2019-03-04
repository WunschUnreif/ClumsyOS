#ifndef __USART__H__
#define __USART__H__

#include "bsp.h"

extern volatile u8 serBuffer[256];
extern volatile u8 bufferStartPtr;
extern volatile u8 bufferEndPtr;

void UsartInit(void);
void UsartSendByte(u8 data);
void OnReceiveData(u8 data);

#endif // !__USART__H__
