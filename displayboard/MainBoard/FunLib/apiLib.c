/****************************************************************************************************/
/*			@FileName		:	timer.c															  	*/
/*			@Author			:	zhengbaiqiu															*/
/*			@Version		:	v1.0																*/
/*			@Date			:	2019.11.12  														*/
/****************************************************************************************************/


/* Includes --------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

/* Project ---------------------------------------------------------------------*/
#include "maindef.h"
#include "apiLib.h"
#include "lcd_driver.h"
#include "gui.h"
#include "display.h"

/* Variable --------------------------------------------------------------------*/
/* Function --------------------------------------------------------------------*/
u32 api_GetSystemTimePeriod(u32 StartTime);
u32 api_GetCurrentSystemTime(void);
void api_UART2_SendData(u16 vCont, u8 *pData);
void api_SendCMDData(u8 mode,u8 cmd,u8 status);
void api_UART2_ReceiveData(u8 vData);
void Scan_Uart_Receive(void);
void Scan_Key(void);
void Key_Process(void);





/*------------------------------------------------------------------------------

        时间相关库函数

--------------------------------------------------------------------------------*/

/*****************************************************************************
 * @name       :u32 api_GetSystemTimePeriod(u32 StartTime)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Get time difference
 * @parameters :start time
 * @retvalue   :time difference
******************************************************************************/
u32 api_GetSystemTimePeriod(u32 StartTime)
{
	u32 res;
	
	if(mMaininf.mSystem.mSystemTick >= StartTime)
	{
		res = mMaininf.mSystem.mSystemTick - StartTime;
	}
	else
	{
		res = SYSTEMTIMEMAX - StartTime + mMaininf.mSystem.mSystemTick;
	}
	
	return res;
}

/*****************************************************************************
 * @name       :u32 api_GetCurrentSystemTime(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Get clock ticks
 * @parameters :None
 * @retvalue   :time tick
******************************************************************************/
u32 api_GetCurrentSystemTime(void)
{
	return mMaininf.mSystem.mSystemTick;
}


/*------------------------------------------------------------------------------

        主板uart通讯

--------------------------------------------------------------------------------*/


/*****************************************************************************
 * @name       :void api_UART2_SendData(u16 vCont, u8 *pData)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :send data
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void api_UART2_SendData(u16 vCont, u8 *pData)
{
	u16 iCont ;
	mMaininf.mUart2.mSendSize = vCont;
	for (iCont=0;iCont < vCont;iCont ++)
	{
		mMaininf.mUart2.TxBuffer[iCont] = *(pData + iCont);
	}
	USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART2,USART_IT_TC,ENABLE);
}

/*****************************************************************************
 * @name       :void api_SendCMDData(u8 cmd)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :send data
 * @parameters :cmd
 * @retvalue   :None
******************************************************************************/
void api_SendCMDData(u8 mode,u8 cmd,u8 status)
{
	mMaininf.mUart2.SendBuf[0] = 0x55;
	mMaininf.mUart2.SendBuf[1] = 0xAA;
	mMaininf.mUart2.SendBuf[2] = mode;
	mMaininf.mUart2.SendBuf[3] = cmd;
	mMaininf.mUart2.SendBuf[4] = status;
	mMaininf.mUart2.SendBuf[5] = mMaininf.mUart2.SendBuf[2] ^ mMaininf.mUart2.SendBuf[3] ^
								 mMaininf.mUart2.SendBuf[4];
	
	api_UART2_SendData(6,mMaininf.mUart2.SendBuf);
}

/*****************************************************************************
 * @name       :void api_ClearUART2ReceiveFlag(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :clear uart2 receive flag
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void api_ClearUART2ReceiveFlag(void)
{
	mMaininf.mUart2.mReceiveStep = ST_0xAA;
	mMaininf.mUart2.mReceiveBCC = 0;
	mMaininf.mUart2.mReceiveSize = 0;
}

/*****************************************************************************
 * @name       :void api_UART2_ReceiveData(u8 vData)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :receive data
 * @parameters :data
 * @retvalue   :None
******************************************************************************/
void api_UART2_ReceiveData(u8 vData)
{
	if(mMaininf.mUart2.mReceiveTimeOut == 0)
	{
		api_ClearUART2ReceiveFlag();
	}
	switch(mMaininf.mUart2.mReceiveStep)
	{
		case ST_0x55 :
			if(vData == 0x55)
			{
				mMaininf.mUart2.mReceiveStep = ST_0xAA;
				mMaininf.mUart2.mReceiveTimeOut = 50;
			}
			else mMaininf.mUart2.mReceiveTimeOut = 0;
			break;
		case ST_0xAA :
			if(vData == 0xAA)
			{
				mMaininf.mUart2.mReceiveStep = ST_MODE;
				mMaininf.mUart2.mReceiveTimeOut = 50;
			}
			else mMaininf.mUart2.mReceiveTimeOut = 0;
			break;
		case ST_MODE :
			mMaininf.mUart2.mReceiveStep = ST_CMD;
			mMaininf.mUart2.mReceiveTimeOut = 50;
			mMaininf.mUart2.RxBuffer[mMaininf.mUart2.mReceiveSize ++] = vData;
			mMaininf.mUart2.mReceiveBCC = mMaininf.mUart2.mReceiveBCC ^ vData;
			break;
		case ST_CMD :
			mMaininf.mUart2.mReceiveStep = ST_STA;
			mMaininf.mUart2.mReceiveTimeOut = 50;
			mMaininf.mUart2.RxBuffer[mMaininf.mUart2.mReceiveSize ++] = vData;
			mMaininf.mUart2.mReceiveBCC = mMaininf.mUart2.mReceiveBCC ^ vData;
			break;
		case ST_STA :
			mMaininf.mUart2.mReceiveStep = ST_BCC;
			mMaininf.mUart2.mReceiveTimeOut = 50;
			mMaininf.mUart2.RxBuffer[mMaininf.mUart2.mReceiveSize ++] = vData;
			mMaininf.mUart2.mReceiveBCC = mMaininf.mUart2.mReceiveBCC ^ vData;
			break;
		case ST_BCC :
			if(mMaininf.mUart2.mReceiveBCC == vData)
			{
				for(mMaininf.mUart2.mReceiveCont = 0;mMaininf.mUart2.mReceiveCont < 3;mMaininf.mUart2.mReceiveCont ++)
				{
					mMaininf.mUart2.ReceiveBuf[mMaininf.mUart2.mReceiveCont] = mMaininf.mUart2.RxBuffer[mMaininf.mUart2.mReceiveCont];
				}
				
				mMaininf.mUart2.mReceiveFlag = 1;       //接收到数据
				
			}
			else mMaininf.mUart2.mReceiveTimeOut = 0;
			break;
		default :
			break;
	}
}

/*****************************************************************************
 * @name       :void Scan_Uart_Receive(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :scan uart receive
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Scan_Uart_Receive(void)
{
	if(mMaininf.mUart2.mReceiveFlag == 1)
	{
		mMaininf.mUart2.mReceiveFlag = 0;
		
		if(mMaininf.mUart2.ReceiveBuf[0] == 0 && mMaininf.mUart2.ReceiveBuf[1] == 0)
		{
			api_SendCMDData(mMaininf.mUart2.ReceiveBuf[0],mMaininf.mUart2.ReceiveBuf[1],mMaininf.mWork.mWorkCheckMODE);
		}
		else
		{
			api_SendCMDData(mMaininf.mUart2.ReceiveBuf[0],mMaininf.mUart2.ReceiveBuf[1],1);
		}
		Display_Test(mMaininf.mWork.mWorkMODE,mMaininf.mWork.mWorkCMD,mMaininf.mWork.mWorkSTATUS);
	}
}

/*****************************************************************************
 * @name       :void Scan_Key(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :scan key
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Scan_Key(void)
{
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == 1)
	{
		if(mMaininf.mWork.mWorkKeyPress == 0)
		{
			mMaininf.mWork.mKeyPressTime = 100;
		}
		
		mMaininf.mWork.mWorkKeyPress = 1;
	}
	else
	{
		if(mMaininf.mWork.mWorkKeyPress == 1)
		{
			if(mMaininf.mWork.mKeyPressTime == 0)
			{
				mMaininf.mWork.mKeyPressFlag = 1;
			}
		}
		
		mMaininf.mWork.mWorkKeyPress = 0;
	}
}

/*****************************************************************************
 * @name       :void Key_Process(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :key process
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Key_Process(void)
{
	if(mMaininf.mWork.mKeyPressFlag == 1)
	{
// 		Lcd_Clear(GRAY0);
// 		Gui_DrawFont_GBK16(16,56,BLUE,GRAY0,"检测中...");
		api_SendCMDData(0,0,0x08);
		mMaininf.mWork.mKeyPressFlag = 0;
	}
}


