#include "bsp.h"
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>

volatile u8 serBuffer[256];
volatile u8 bufferStartPtr = 0, bufferEndPtr = 0;

void UsartInit(void) {
    USART_InitTypeDef usartInitStruct;
    GPIO_InitTypeDef gpioInitStruct;
    NVIC_InitTypeDef nvicInitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    gpioInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    gpioInitStruct.GPIO_Pin = GPIO_Pin_9;
    gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpioInitStruct);

    gpioInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpioInitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &gpioInitStruct);

    usartInitStruct.USART_BaudRate = 115200;
    usartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usartInitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    usartInitStruct.USART_Parity = USART_Parity_No;
    usartInitStruct.USART_StopBits = USART_StopBits_1;
    usartInitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &usartInitStruct);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    nvicInitStruct.NVIC_IRQChannel = USART1_IRQn;
    nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
    nvicInitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&nvicInitStruct);

    USART_Cmd(USART1, ENABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    bufferStartPtr = 0, bufferEndPtr = 0;
}

void UsartSendByte(u8 data) {
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, data);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void OnReceiveData(u8 data) {
    serBuffer[bufferEndPtr++] = data;
}

extern int errno;
extern int  _end;
caddr_t _sbrk ( int incr )
{
  static unsigned char *heap = NULL;
  unsigned char *prev_heap;
  if (heap == NULL) {
    heap = (unsigned char *)&_end;
  }
  prev_heap = heap;
  heap += incr;
  return (caddr_t) prev_heap;
}
int link(char *old, char *new) {
return -1;
}
int _close(int file)
{
  return -1;
}
int _fstat(int file, struct stat *st)
{
  st->st_mode = S_IFCHR;
  return 0;
}
int _isatty(int file)
{
  return 1;
}
int _lseek(int file, int ptr, int dir)
{
  return 0;
}

int _write(int fd, char *pBuffer, int size) {
    for(int i = 0; i < size; ++i) {
        UsartSendByte(pBuffer[i]);
    }
    return size;
}

int _read(int fd, char *pBuffer, int size) {
    int i = 0;
    for(i = 0; i < size; ++i) {
        while(bufferStartPtr == bufferEndPtr);
        pBuffer[i] = serBuffer[bufferStartPtr++];
        if(pBuffer[i] == '\n' || pBuffer[i] == '\r') break;
    }
    return i + 1;
}
