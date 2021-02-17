/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
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
#include "maindef.h"
#include "apiLib.h"

 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
}



void TIM2_IRQHandler(void)        //1ms interrupt
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	{
		/*        用于生成1ms滴答定时器          */
		mMaininf.mSystem.mSystemTick ++;
		if(mMaininf.mSystem.mSystemTick >= SYSTEMTIMEMAX) mMaininf.mSystem.mSystemTick = 0;
		
		//接收超时
		if(mMaininf.mUart2.mReceiveTimeOut > 0) mMaininf.mUart2.mReceiveTimeOut --;
		
		//按键去抖
		if(mMaininf.mWork.mKeyPressTime > 0) mMaininf.mWork.mKeyPressTime --;
		
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)     /*   接收数据   */
	{
		api_UART2_ReceiveData(USART_ReceiveData(USART2));
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
	if(USART_GetITStatus(USART2,USART_IT_TC)!=RESET)      /*   发送数据   */
	{
		if(mMaininf.mUart2.mSendSize > 0)
		{
			USART_SendData(USART2,mMaininf.mUart2.TxBuffer[mMaininf.mUart2.mSendCont ++]);
			if(mMaininf.mUart2.mSendCont == mMaininf.mUart2.mSendSize)
			{
				mMaininf.mUart2.mSendSize = 0;
				mMaininf.mUart2.mSendCont = 0;
				USART_ITConfig(USART2,USART_IT_TC,DISABLE);
				USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
			}
		}
		else
		{
			mMaininf.mUart2.mSendSize = 0;
			mMaininf.mUart2.mSendCont = 0;
			USART_ITConfig(USART2,USART_IT_TC,DISABLE);
			USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
		}
		USART_ClearITPendingBit(USART2,USART_IT_TC);
	}
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
