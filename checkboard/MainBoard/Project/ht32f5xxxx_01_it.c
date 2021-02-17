/*********************************************************************************************************//**
 * @file    IP/Example/ht32f5xxxx_01_it.c
 * @version $Rev:: 4398         $
 * @date    $Date:: 2019-11-14 #$
 * @brief   This file provides all interrupt service routine.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "maindef.h"
#include "apiLib.h"
#include "bsp.h"

/** @addtogroup Project_Template Project Template
  * @{
  */

/** @addtogroup IP_Examples IP
  * @{
  */

/** @addtogroup Example
  * @{
  */


/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
 * @brief   This function handles NMI exception.
 * @retval  None
 ************************************************************************************************************/
void NMI_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles Hard Fault exception.
 * @retval  None
 ************************************************************************************************************/
void HardFault_Handler(void)
{
  #if 1

  static vu32 gIsContinue = 0;
  /*--------------------------------------------------------------------------------------------------------*/
  /* For development FW, MCU run into the while loop when the hardfault occurred.                           */
  /* 1. Stack Checking                                                                                      */
  /*    When a hard fault exception occurs, MCU push following register into the stack (main or process     */
  /*    stack). Confirm R13(SP) value in the Register Window and typing it to the Memory Windows, you can   */
  /*    check following register, especially the PC value (indicate the last instruction before hard fault).*/
  /*    SP + 0x00    0x04    0x08    0x0C    0x10    0x14    0x18    0x1C                                   */
  /*           R0      R1      R2      R3     R12      LR      PC    xPSR                                   */
  while (gIsContinue == 0)
  {
  }
  /* 2. Step Out to Find the Clue                                                                           */
  /*    Change the variable "gIsContinue" to any other value than zero in a Local or Watch Window, then     */
  /*    step out the HardFault_Handler to reach the first instruction after the instruction which caused    */
  /*    the hard fault.                                                                                     */
  /*--------------------------------------------------------------------------------------------------------*/

  #else

  /*--------------------------------------------------------------------------------------------------------*/
  /* For production FW, you shall consider to reboot the system when hardfault occurred.                    */
  /*--------------------------------------------------------------------------------------------------------*/
  NVIC_SystemReset();

  #endif
}

/*********************************************************************************************************//**
 * @brief   This function handles SVCall exception.
 * @retval  None
 ************************************************************************************************************/
void SVC_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles PendSVC exception.
 * @retval  None
 ************************************************************************************************************/
void PendSV_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles SysTick Handler.
 * @retval  None
 ************************************************************************************************************/
void SysTick_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles LVD & BOD interrupt.
 * @retval  None
 ************************************************************************************************************/
//void LVD_BOD_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles RTC interrupt.
 * @retval  None
 ************************************************************************************************************/
//void RTC_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles Flash interrupt.
 * @retval  None
 ************************************************************************************************************/
//void FLASH_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles Wake-up interrupt.
 * @retval  None
 ************************************************************************************************************/
//void EVWUP_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles EXTI interrupt.
 * @retval  None
 ************************************************************************************************************/
//void EXTI0_1_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles EXTI interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI2_3_IRQHandler(void)
{
	volatile uint32_t temp;
	temp = HT_EXTI->EDGEFLGR;
	//前下视
	if(HT_EXTI->EDGEFLGR & (1UL << EXTI_CHANNEL_2))
	{
		if(mMaininf.mWork.mFrontDropCheckEnable == 1)
		{
			mMaininf.mWork.mFrontDropFlag = 1;
		}
		
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_2);
	}
	
// 	if (EXTI_GetEdgeFlag(EXTI_CHANNEL_2))
// 	{
// 		
// 		EXTI_ClearEdgeFlag(EXTI_CHANNEL_2);
// 	}
}

/*********************************************************************************************************//**
 * @brief   This function handles EXTI interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI4_15_IRQHandler(void)
{
	volatile uint32_t temp;
	temp = HT_EXTI->EDGEFLGR;
// 	//左、右下视
// 	if(HT_EXTI->EDGEFLGR & (1UL << EXTI_CHANNEL_9))
// 	{
// 		if(mMaininf.mWork.mLeftRightDropCheckEnable == 1)
// 		{
// 			mMaininf.mWork.mLeftRightDropFlag = 1;
// 		}
// 		
// 		EXTI_ClearEdgeFlag(EXTI_CHANNEL_9);
// 	}
// 	
// 	//后下视
// 	if(HT_EXTI->EDGEFLGR & (1UL << EXTI_CHANNEL_15))
// 	{
// 		if(mMaininf.mWork.mBackDropCheckEnable == 1)
// 		{
// 			mMaininf.mWork.mBackDropFlag = 1;
// 		}
// 		
// 		EXTI_ClearEdgeFlag(EXTI_CHANNEL_15);
// 	}
	
// 	//左、右碰撞
// 	if(HT_EXTI->EDGEFLGR & (1UL << EXTI_CHANNEL_5))
// 	{
// 		if(mMaininf.mWork.mLeftRightBumpCheckEnable == 1)
// 		{
// 			mMaininf.mWork.mLeftRightBumpFlag = 1;
// 		}
// 		
// 		EXTI_ClearEdgeFlag(EXTI_CHANNEL_5);
// 	}
// 	
// 	//左、右离地
// 	if(HT_EXTI->EDGEFLGR & (1UL << EXTI_CHANNEL_8))
// 	{
// 		if(mMaininf.mWork.mLeftRightLeaveCheckEnable == 1)
// 		{
// 			mMaininf.mWork.mLeftRightLeaveFlag = 1;
// 		}
// 		
// 		EXTI_ClearEdgeFlag(EXTI_CHANNEL_8);
// 	}
	
// 	//尘盒
// 	if(HT_EXTI->EDGEFLGR & (1UL << EXTI_CHANNEL_10))
// 	{
// 		if(mMaininf.mWork.mDustBoxCheckEnable == 1)
// 		{
// 			mMaininf.mWork.mDustBoxFlag = 1;
// 		}
// 		
// 		EXTI_ClearEdgeFlag(EXTI_CHANNEL_10);
// 	}
	
	//左、右码盘
// 	if(HT_EXTI->EDGEFLGR & (1UL << EXTI_CHANNEL_12))
// 	{
// 		if(mMaininf.mWork.mLeftRightCodeCheckEnable == 1)
// 		{
// 			mMaininf.mWork.mLeftRightCodeFlag = 1;
// 		}
// 		
// 		EXTI_ClearEdgeFlag(EXTI_CHANNEL_12);
// 	}
	
// 	//按键
// 	if(HT_EXTI->EDGEFLGR & (1UL << EXTI_CHANNEL_11))
// 	{
// 		if(mMaininf.mWork.mPowerKeyCheckEnable == 1)
// 		{
// 			mMaininf.mWork.mPowerKeyFlag = 1;
// 		}
// 		
// 		EXTI_ClearEdgeFlag(EXTI_CHANNEL_11);
// 	}
	
	//左、右引导
	if(HT_EXTI->EDGEFLGR & (1UL << EXTI_CHANNEL_13))
	{
		if(mMaininf.mWork.mLeftRightGuideCheckEnable == 1)
		{
			mMaininf.mSystem.mGuideStartTimeT1 = mMaininf.mSystem.mGuideStartTimeT2;
			mMaininf.mSystem.mGuideStartTimeT2 = mMaininf.mSystem.mGuideStartTimeT3;
			mMaininf.mSystem.mGuideStartTimeT3 = mMaininf.mSystem.mSystemTick;
			if(((mMaininf.mSystem.mGuideStartTimeT2 - mMaininf.mSystem.mGuideStartTimeT1) >= 15) && 
				((mMaininf.mSystem.mGuideStartTimeT2 - mMaininf.mSystem.mGuideStartTimeT1) <= 25) && 
				((mMaininf.mSystem.mGuideStartTimeT3 - mMaininf.mSystem.mGuideStartTimeT2) >= 15) && 
				((mMaininf.mSystem.mGuideStartTimeT3 - mMaininf.mSystem.mGuideStartTimeT2) <= 25))
			{
				mMaininf.mWork.mLeftRightGuideFlag = 1;
			}
		}
		
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_13);
	}
	
	//后引导
	if(HT_EXTI->EDGEFLGR & (1UL << EXTI_CHANNEL_6))
	{
		if(mMaininf.mWork.mBackGuideCheckEnable == 1)
		{
			mMaininf.mSystem.mGuideStartTimeT1 = mMaininf.mSystem.mGuideStartTimeT2;
			mMaininf.mSystem.mGuideStartTimeT2 = mMaininf.mSystem.mGuideStartTimeT3;
			mMaininf.mSystem.mGuideStartTimeT3 = mMaininf.mSystem.mSystemTick;
			if(((mMaininf.mSystem.mGuideStartTimeT2 - mMaininf.mSystem.mGuideStartTimeT1) >= 15) && 
				((mMaininf.mSystem.mGuideStartTimeT2 - mMaininf.mSystem.mGuideStartTimeT1) <= 25) && 
				((mMaininf.mSystem.mGuideStartTimeT3 - mMaininf.mSystem.mGuideStartTimeT2) >= 15) && 
				((mMaininf.mSystem.mGuideStartTimeT3 - mMaininf.mSystem.mGuideStartTimeT2) <= 25))
			{
				mMaininf.mWork.mBackGuideFlag = 1;
			}
		}
		
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_6);
	}
}

/*********************************************************************************************************//**
 * @brief   This function handles CMP interrupt.
 * @retval  None
 ************************************************************************************************************/
//void COMP_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles ADC interrupt.
 * @retval  None
 ************************************************************************************************************/
void ADC_IRQHandler(void)
{
// 	if(m_gl_info.mSensorInfo.mADCCheckTimeOut != 0)
// 	{
// 		m_gl_info.mSensorInfo.mADCCheckTimeOut--;
// 	}
	
	
	//使能灰尘组件接收
// 	if(mMaininf.mWork.mDustCheckEnable == 1)
// 	{
// 		mMaininf.mWork.mSensorADC[4] = HT_ADC->DR[4] & 0x0FFF;       //灰尘检测传感器
// 	}
	
	//使能充电座组件接收
	if(mMaininf.mWork.mChargeBaseCheckEnable == 1)
	{
		mMaininf.mWork.mSensorADC[0] = HT_ADC->DR[0] & 0x0FFF;       //充电座A信号
		mMaininf.mWork.mSensorADC[1] = HT_ADC->DR[1] & 0x0FFF;       //充电座B信号
		mMaininf.mWork.mSensorADC[2] = HT_ADC->DR[2] & 0x0FFF;       //充电座TOP信号
		mMaininf.mWork.mSensorADC[3] = HT_ADC->DR[3] & 0x0FFF;       //充电座5V
	}
	
	//使能沿边组件接收
// 	if(mMaininf.mWork.mAlongSideCheckEnable == 1)
// 	{
// 		mMaininf.mWork.mSensorADC[5] = HT_ADC->DR[4] & 0x0FFF;       //沿边接收
// 	}
	
// 	mMaininf.mWork.mSensorADC[0] = HT_ADC->DR[0] & 0x0FFF;
// 	mMaininf.mWork.mSensorADC[1] = HT_ADC->DR[1] & 0x0FFF;
// 	mMaininf.mWork.mSensorADC[2] = HT_ADC->DR[2] & 0x0FFF;
// 	mMaininf.mWork.mSensorADC[3] = HT_ADC->DR[3] & 0x0FFF;
// 	mMaininf.mWork.mSensorADC[4] = HT_ADC->DR[4] & 0x0FFF;
// 	mMaininf.mWork.mSensorADC[5] = HT_ADC->DR[5] & 0x0FFF;
	
	ADC_ClearIntPendingBit(HT_ADC, ADC_FLAG_SINGLE_EOC);
}

/*********************************************************************************************************//**
 * @brief   This function handles MCTM interrupt.
 * @retval  None
 ************************************************************************************************************/
//void MCTM0_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles GPTM interrupt.
 * @retval  None
 ************************************************************************************************************/
//void GPTM0_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles GPTM interrupt.
 * @retval  None
 ************************************************************************************************************/
//void GPTM1_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles SCTM interrupt.
 * @retval  None
 ************************************************************************************************************/
//void SCTM0_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles SCTM interrupt.
 * @retval  None
 ************************************************************************************************************/
//void SCTM1_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles SCTM interrupt.
 * @retval  None
 ************************************************************************************************************/
//void SCTM2_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles SCTM interrupt.
 * @retval  None
 ************************************************************************************************************/
//void SCTM3_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles SLED0 interrupt.
 * @retval  None
 ************************************************************************************************************/
//void SLED0_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles SLED1 interrupt.
 * @retval  None
 ************************************************************************************************************/
//void SLED1_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles BFTM interrupt.
 * @retval  None
 ************************************************************************************************************/
void BFTM0_IRQHandler(void)         //0.2ms
{
	if(mMaininf.mWork.mGuideSendFlag == 1)
	{
		if(mMaininf.mWork.mGuideSendChangeFlag == 0)
		{
			PWM_Configuration(TRUE);
			api_Send_PWM(TRUE);
			mMaininf.mWork.mGuideSendChangeFlag = 1;
		}
		else
		{
			PWM_Configuration(FALSE);
			api_Send_PWM(FALSE);
			mMaininf.mWork.mGuideSendChangeFlag = 0;
		}
	}
	
	BFTM_ClearFlag(HT_BFTM0);
}

/*********************************************************************************************************//**
 * @brief   This function handles BFTM interrupt.
 * @retval  None
 ************************************************************************************************************/
void BFTM1_IRQHandler(void)         //1ms
{
	/*        用于生成1ms滴答定时器          */
	mMaininf.mSystem.mSystemTick ++;
	if(mMaininf.mSystem.mSystemTick >= SYSTEMTIMEMAX) mMaininf.mSystem.mSystemTick = 0;
	
	if(mMaininf.mWork.mGuideSendFlag == 1)
	{
		
	}
	
	BFTM_ClearFlag(HT_BFTM1);
}

/*********************************************************************************************************//**
 * @brief   This function handles I2C interrupt.
 * @retval  None
 ************************************************************************************************************/
//void I2C0_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles I2C interrupt.
 * @retval  None
 ************************************************************************************************************/
//void I2C1_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles I2C interrupt.
 * @retval  None
 ************************************************************************************************************/
//void I2C2_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles SPI interrupt.
 * @retval  None
 ************************************************************************************************************/
//void SPI0_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles SPI interrupt.
 * @retval  None
 ************************************************************************************************************/
//void SPI1_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles USART interrupt.
 * @retval  None
 ************************************************************************************************************/
//void USART0_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles USART interrupt.
 * @retval  None
 ************************************************************************************************************/
//void USART1_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles UART interrupt.
 * @retval  None
 ************************************************************************************************************/
void UART0_IRQHandler(void)
{
	/* Rx, move data from UART to buffer                                                                */
	if(USART_GetFlagStatus(HT_UART0, USART_FLAG_RXDR))
	{
		api_UART0_ReceiveData(USART_ReceiveData(HT_UART0));
		USART_ClearFlag(HT_UART0, USART_FLAG_RXDR);
	}
	/* Tx, move data from buffer to UART                                                                */
	if(USART_GetFlagStatus(HT_UART0, USART_FLAG_TXDE)) 
	{
		if(mMaininf.mUart0.mSendSize > 0)
		{
			USART_SendData(HT_UART0, mMaininf.mUart0.TxBuffer[mMaininf.mUart0.mSendCont ++]);
			if(mMaininf.mUart0.mSendCont == mMaininf.mUart0.mSendSize)
			{
				mMaininf.mUart0.mSendSize = 0;
				mMaininf.mUart0.mSendCont = 0;
				USART_IntConfig(HT_UART0, USART_INT_TXDE, DISABLE);
			}
		}
		else
		{
			mMaininf.mUart0.mSendSize = 0;
			mMaininf.mUart0.mSendCont = 0;
			USART_IntConfig(HT_UART0, USART_INT_TXDE, DISABLE);
		}
		
		USART_ClearFlag(HT_UART0, USART_FLAG_TXDE);
	}
}

/*********************************************************************************************************//**
 * @brief   This function handles UART interrupt.
 * @retval  None
 ************************************************************************************************************/
void UART1_IRQHandler(void)
{
	/* Rx, move data from UART to buffer                                                                */
	if(USART_GetFlagStatus(HT_UART1, USART_FLAG_RXDR))
	{
		
		USART_ClearFlag(HT_UART1, USART_FLAG_RXDR);
	}
	/* Tx, move data from buffer to UART                                                                */
	if(USART_GetFlagStatus(HT_UART1, USART_FLAG_TXDE)) 
	{
		
		USART_ClearFlag(HT_UART1, USART_FLAG_TXDE);
	}
}

/*********************************************************************************************************//**
 * @brief   This function handles UART interrupt.
 * @retval  None
 ************************************************************************************************************/
//void UART2_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles UART interrupt.
 * @retval  None
 ************************************************************************************************************/
//void UART3_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles UART interrupt.
 * @retval  None
 ************************************************************************************************************/
//void UART0_UART2_IRQHandler(void)
//{
//  #if (RETARGET_PORT == RETARGET_UART0) || (RETARGET_PORT == RETARGET_UART2)
//    #if (RETARGET_INT_MODE == 1)
//    RETARGET_UART_IRQHandler();
//    #endif
//  #endif
//}

/*********************************************************************************************************//**
 * @brief   This function handles UART interrupt.
 * @retval  None
 ************************************************************************************************************/
//void UART1_UART3_IRQHandler(void)
//{
//  #if (RETARGET_PORT == RETARGET_UART1) || (RETARGET_PORT == RETARGET_UART3)
//    #if (RETARGET_INT_MODE == 1)
//    RETARGET_UART_IRQHandler();
//    #endif
//  #endif
//}

/*********************************************************************************************************//**
 * @brief   This function handles SCI interrupt.
 * @retval  None
 ************************************************************************************************************/
//void SCI_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles I2S interrupt.
 * @retval  None
 ************************************************************************************************************/
//void I2S_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles USB interrupt.
 * @retval  None
 ************************************************************************************************************/
//void USB_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles PDMA interrupt.
 * @retval  None
 ************************************************************************************************************/
//void PDMA_CH0_1_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles PDMA interrupt.
 * @retval  None
 ************************************************************************************************************/
//void PDMA_CH2_5_IRQHandler(void)
//{
//}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
