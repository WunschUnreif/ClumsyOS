/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <stdio.h>

#include "os.h"
#include "bsp.h"
#include "ff.h"
FATFS fs;
FRESULT res;
FIL f;
FILINFO info;
DIR dir;
char buf[50];
char writebuf[] = "Hello, world";
int n;

int main(void)
{
    UsartInit();
    MemoryInit();
    MemoryCheckUntilGood();

    res = f_mount(&fs, "0:", 1);
    
    res = f_open(&f, "a.txt", FA_CREATE_ALWAYS | FA_WRITE);
    printf("open %d\r\n", res);
    f_write(&f, writebuf, 13, &n);
    f_close(&f);

    res = f_open(&f, "a.txt", FA_OPEN_EXISTING | FA_READ);
    printf("open %d\r\n", res);
    f_read(&f, buf, 50, &n);
    printf("%s\r\n", buf);

    res = f_opendir(&dir, "/");

    while((res = f_readdir(&dir, &info)) == FR_OK && info.fname[0]) {
        printf("%s\r\n", info.fname);
        if(info.fattrib == AM_DIR)
          f_unlink(info.fname);
    }

    f_mount(NULL, "0:", 1);

    while(1);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
