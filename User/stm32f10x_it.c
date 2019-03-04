/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

#include "bsp.h"
#include "os.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
    printf("NMI\r\n");
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    printf("HardFault\r\n");
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    printf("BusFault\r\n");
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    printf("UsageFault\r\n");
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

void SysTick_Handler(void)
{
    /* a temp reg varible at r0 */
    register uint32_t tmp __ASM("r0") = (uint32_t)(&os_tmpSpot.regFrame);

    /* store the higher regs into temp regFrame*/
    __ASM("str r4, [%[val], #0]\n\t"
          "str r5, [%[val], #4]\n\t"
          "str r6, [%[val], #8]\n\t"
          "str r7, [%[val], #12]\n\t"
          "str r8, [%[val], #16]\n\t"
          "str r9, [%[val], #20]\n\t"
          "str r10, [%[val], #24]\n\t"
          "str r11, [%[val], #28]\n\t"
          "str lr, [%[val], #32]\n\t"
          "mov %[val], lr\n\t"
          :
          : [val] "r"(tmp));

    /* read out autosaved SP into `tmp` */
    /* tmp = (saved lr) at this stage */
    if (tmp & 0x04)
    {
        /* if lr[2] is 1, then the except frame stores in psp */
        __ASM("mrs %[val], psp\n\t"
              : [val] "=r"(tmp));
    }
    else
    {
        /* if lr[2] is 0, then the except frame stores in msp */
        __ASM("mrs %[val], msp\n\t"
              "add %[val], %[val], #8"
              : [val] "=r"(tmp));
    }

    /* store the exception stack frame into temp spot */
    os_tmpSpot.savedSP = tmp;
    os_tmpSpot.exceptionFrame.r0 = ((OSexceptionStackFrame *)(tmp))->r0;
    os_tmpSpot.exceptionFrame.r1 = ((OSexceptionStackFrame *)(tmp))->r1;
    os_tmpSpot.exceptionFrame.r2 = ((OSexceptionStackFrame *)(tmp))->r2;
    os_tmpSpot.exceptionFrame.r3 = ((OSexceptionStackFrame *)(tmp))->r3;
    os_tmpSpot.exceptionFrame.r12 = ((OSexceptionStackFrame *)(tmp))->r12;
    os_tmpSpot.exceptionFrame.lr = ((OSexceptionStackFrame *)(tmp))->lr;
    os_tmpSpot.exceptionFrame.pc = ((OSexceptionStackFrame *)(tmp))->pc;
    os_tmpSpot.exceptionFrame.psr = ((OSexceptionStackFrame *)(tmp))->psr;

    /* call systick delegation for context switch */
    onSysTick();

    /*  at this stage the <os_tmpSpot> has been filled with 
        the context for next process.
    */

    /* read the saved lr for next process */
    tmp = os_tmpSpot.returnFlag;

    /* recover the LR for next process 
        and do the stack pop mannually
   */
    __ASM("add sp, sp, #8\n\t"
          "mov lr, %[val]\n\t"
          :
          : [val] "r"(tmp));

    /* set the SP for next process correctly based on saved LR */
    if (tmp & 0x04)
    {
        /* if lr[2] is 1, then the except frame should store in psp */
        tmp = os_tmpSpot.savedSP;
        __ASM("msr psp, %[val]\n\t"
              :
              : [val] "r"(tmp));
    }
    else
    {
        /* if lr[2] is 0, then the except frame should store in msp */
        tmp = os_tmpSpot.savedSP;
        __ASM("msr msp, %[val]\n\t"
              :
              : [val] "r"(tmp));
    }

    /* fill the autosaved spot for next process */
    tmp = os_tmpSpot.savedSP;
    ((OSexceptionStackFrame *)(tmp))->r0 = os_tmpSpot.exceptionFrame.r0;
    ((OSexceptionStackFrame *)(tmp))->r1 = os_tmpSpot.exceptionFrame.r1;
    ((OSexceptionStackFrame *)(tmp))->r2 = os_tmpSpot.exceptionFrame.r2;
    ((OSexceptionStackFrame *)(tmp))->r3 = os_tmpSpot.exceptionFrame.r3;
    ((OSexceptionStackFrame *)(tmp))->r12 = os_tmpSpot.exceptionFrame.r12;
    ((OSexceptionStackFrame *)(tmp))->lr = os_tmpSpot.exceptionFrame.lr;
    ((OSexceptionStackFrame *)(tmp))->pc = os_tmpSpot.exceptionFrame.pc;
    ((OSexceptionStackFrame *)(tmp))->psr = os_tmpSpot.exceptionFrame.psr;

    /* load the higher regs mannually
        then return from interrupt to next process
     */
    tmp = (uint32_t)(&os_tmpSpot.regFrame);
    __ASM("ldr r4,  [%[val], #0]\n\t"
          "ldr r5,  [%[val], #4]\n\t"
          "ldr r6,  [%[val], #8]\n\t"
          "ldr r7,  [%[val], #12]\n\t"
          "ldr r8,  [%[val], #16]\n\t"
          "ldr r9,  [%[val], #20]\n\t"
          "ldr r10, [%[val], #24]\n\t"
          "ldr r11, [%[val], #28]\n\t"
          "bx lr\n\t"
          :
          : [val] "r"(tmp));
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);

        OnReceiveData(USART_ReceiveData(USART1));

        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    }
}

void SDIO_IRQHandler(void) {
    SD_ProcessIRQSrc();
}

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
