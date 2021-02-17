/****************************************************************************************************/
/*			@FileName		:	MAIN.c															  	*/
/*			@Author			:	zhengbaiqiu															*/
/*			@Version		:	v1.0																*/
/*			@Date			:	2019.11.12  														*/
/****************************************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

/* Project -------------------------------------------------------------------*/
#include "maindef.h"
#include "bsp.h"
#include "gpioredef.h"
#include "lcd_driver.h"
#include "gui.h"
#include "apiLib.h"

#define LOG_TAG    "main"
#include <elog.h>
#include "display.h"


/*        Global variable.            */
_ST_MAINMANAGER mMaininf;
extern void Display_FontDrop(u8 item,u8 status);
extern void Gui_DrawPoint(u16 x,u16 y,u16 Data);


int main(void)
{
	prvSetupHardware();
	Lcd_Init();
	
	/* initialize EasyLogger */
	elog_init();
	/* set EasyLogger log format */
	elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
	elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
	elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
	elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
	elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
	elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
	/* start EasyLogger */
	elog_start();
	
	/* dynamic set enable or disable for output logs (true or false) */
//    elog_set_output_enabled(false);
    /* dynamic set output logs's level (from ELOG_LVL_ASSERT to ELOG_LVL_VERBOSE) */
//    elog_set_filter_lvl(ELOG_LVL_WARN);
    /* dynamic set output logs's filter for tag */
//    elog_set_filter_tag("main");
    /* dynamic set output logs's filter for keyword */
//    elog_set_filter_kw("Hello");
    /* dynamic set output logs's tag filter */
//    elog_set_filter_tag_lvl("main", ELOG_LVL_WARN);

	
	LCD_LED_SET;//通过IO控制背光亮		
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(16,56,BLUE,GRAY0,"初始化...");
	mMaininf.mWork.mWorkCheckMODE = (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) << 2) | 
									(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4) << 1) |
									 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3);
	mMaininf.mSystem.mScreenUpdateStartTime = api_GetCurrentSystemTime();
	while(1)
	{
		/*    定时复位看门狗   */
		IWDG_ReloadCounter();
		
		if(mMaininf.mWork.mPowerOnFlag == 0)    //初始化中
		{
			if(mMaininf.mUart2.mReceiveFlag == 1)
			{
				mMaininf.mUart2.mReceiveFlag = 0;
				
				if(mMaininf.mUart2.ReceiveBuf[0] == 0 && mMaininf.mUart2.ReceiveBuf[1] == 0 && mMaininf.mUart2.ReceiveBuf[2] == 0)
				{
					api_SendCMDData(0x00,0x01,mMaininf.mWork.mWorkCheckMODE);
				}
				else if(mMaininf.mUart2.ReceiveBuf[0] == 0 && mMaininf.mUart2.ReceiveBuf[1] == 0 && mMaininf.mUart2.ReceiveBuf[2] == 1)
				{
					Gui_DrawFont_GBK16(16,56,BLUE,GRAY0,"初始化OK");
					mMaininf.mWork.mPowerOnFlag = 1;
				}
			}
		}
		else                                    //初始化成功
		{
			if(mMaininf.mUart2.mReceiveFlag == 1)
			{
				mMaininf.mUart2.mReceiveFlag = 0;
				
				if(mMaininf.mUart2.ReceiveBuf[0] == 0 && mMaininf.mUart2.ReceiveBuf[1] == 0 && mMaininf.mUart2.ReceiveBuf[2] == 1)
				{
					//按键反馈
					Lcd_Clear(GRAY0);
					Gui_DrawFont_GBK16(16,56,BLUE,GRAY0,"检测中...");
				}
				else
				{
					api_SendCMDData(mMaininf.mUart2.ReceiveBuf[0],mMaininf.mUart2.ReceiveBuf[1],1);
					Display_Test(mMaininf.mUart2.ReceiveBuf[0],mMaininf.mUart2.ReceiveBuf[1],mMaininf.mUart2.ReceiveBuf[2]);
				}
			}
			
			Scan_Key();
			Key_Process();
		}
		
// 		if(mMaininf.mUart2.mReceiveFlag == 1)
// 		{
// 			mMaininf.mUart2.mReceiveFlag = 0;
// 			
// 			if(mMaininf.mUart2.ReceiveBuf[0] == 0 && mMaininf.mUart2.ReceiveBuf[1] == 0 && mMaininf.mUart2.ReceiveBuf[2] == 0)
// 			{
// 				api_SendCMDData(0x00,0x01,mMaininf.mWork.mWorkCheckMODE);
// // 				if(mMaininf.mUart2.ReceiveBuf[2] == 0)      //读取板子类型
// // 				{
// // 					api_SendCMDData(0x00,0x01,mMaininf.mWork.mWorkCheckMODE);
// // 				}
// // 				else                                        //按键反馈
// // 				{
// // // 					Lcd_Clear(GRAY0);
// // // 					Gui_DrawFont_GBK16(16,56,BLUE,GRAY0,"检测中...");
// // 				}
// 			}
// 			else if(mMaininf.mUart2.ReceiveBuf[0] == 0 && mMaininf.mUart2.ReceiveBuf[1] == 0 && mMaininf.mUart2.ReceiveBuf[2] == 1)
// 			{
// 				//按键反馈
// 				Lcd_Clear(GRAY0);
// 				Gui_DrawFont_GBK16(16,56,BLUE,GRAY0,"检测中...");
// 			}
// 			else
// 			{
// 				api_SendCMDData(mMaininf.mUart2.ReceiveBuf[0],mMaininf.mUart2.ReceiveBuf[1],1);
// 				Display_Test(mMaininf.mUart2.ReceiveBuf[0],mMaininf.mUart2.ReceiveBuf[1],mMaininf.mUart2.ReceiveBuf[2]);
// 			}
// 			//Display_Test(mMaininf.mWork.mWorkMODE,mMaininf.mWork.mWorkCMD,mMaininf.mWork.mWorkSTATUS);
// 			//Display_Test(mMaininf.mUart2.ReceiveBuf[0],mMaininf.mUart2.ReceiveBuf[1],mMaininf.mUart2.ReceiveBuf[2]);
// 		}
// 		
// //		Scan_Uart_Receive();
// 		
// 		Scan_Key();
// 		Key_Process();
		
		if(api_GetSystemTimePeriod(mMaininf.mSystem.mScreenUpdateStartTime) > 1000)
		{
			mMaininf.mSystem.mScreenUpdateStartTime = api_GetCurrentSystemTime();
			Gui_DrawPoint(0,0,GRAY0);
		}

//		Display_FontDrop(DROP,0);

	}
// 		elog_a("elog", "%d\n", mMaininf.mDrop.mDropADCVAL[0]);
// 		log_e("Hello EasyLogger! %d",ADCValue[0]);
//	return 0;
}




































