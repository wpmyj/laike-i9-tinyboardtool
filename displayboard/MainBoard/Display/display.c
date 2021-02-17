/****************************************************************************************************/
/*			@FileName		:	display.c														  	*/
/*			@Author			:	zhengbaiqiu															*/
/*			@Version		:	v1.0																*/
/*			@Date			:	2019.11.12  														*/
/****************************************************************************************************/


/* Includes --------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Project ---------------------------------------------------------------------*/
#include "lcd_driver.h"
#include "gui.h"
#include "delay.h"
#include "picture.h"
#include "display.h"
#include "maindef.h"

/* Variable --------------------------------------------------------------------*/
/* Function --------------------------------------------------------------------*/
void Display_Test(u8 test_mode,u8 test_cmd,u8 test_status);





/*****************************************************************************
 * @name       :void Display_FontDrop(u8 item,u8 status)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :显示前下视
 * @parameters :item:测试项  
 *				status:状态
 * @retvalue   :None
******************************************************************************/
void Display_FontDrop(u8 item,u8 status)     //   前下视
{
	switch(item)
	{
		case DROP :
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"下视:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"下视:PASS");
			}
			break;
		case ALL_TEST :
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"下视:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"下视:PASS");
			}
			break;
		default :
			Gui_DrawPoint(0,0,GRAY0);
			break;
	}
}

/*****************************************************************************
 * @name       :void Display_Wheel(u8 item,u8 status)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :显示大轮线束
 * @parameters :item:测试项
 *				status:状态
 * @retvalue   :None
******************************************************************************/
void Display_Wheel(u8 item,u8 status)     //   大轮线束
{
	switch(item)
	{
		case DROP :            //下视
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"下视:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"下视:PASS");
			}
			break;
		case BUMP :           //碰撞
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,20,RED,GRAY0,"碰撞:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,20,BLUE,GRAY0,"碰撞:PASS");
			}
			break;
		case CODE :          //码盘       
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,40,RED,GRAY0,"码盘:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,40,BLUE,GRAY0,"码盘:PASS");
			}
			break;
		case LEAVELAND :    //离地
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,60,RED,GRAY0,"离地:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,60,BLUE,GRAY0,"离地:PASS");
			}
			break;
		case ALL_TEST :
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"下视:FAIL");
				Gui_DrawFont_GBK16(16,20,RED,GRAY0,"碰撞:FAIL");
				Gui_DrawFont_GBK16(16,40,RED,GRAY0,"码盘:FAIL");
				Gui_DrawFont_GBK16(16,60,RED,GRAY0,"离地:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"下视:PASS");
				Gui_DrawFont_GBK16(16,20,BLUE,GRAY0,"碰撞:PASS");
				Gui_DrawFont_GBK16(16,40,BLUE,GRAY0,"码盘:PASS");
				Gui_DrawFont_GBK16(16,60,BLUE,GRAY0,"离地:PASS");
			}
			break;
		default :
			Gui_DrawPoint(0,0,GRAY0);
			break;
	}
}

/*****************************************************************************
 * @name       :void Display_Post_Combination(u8 item,u8 status)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :显示后组合线束
 * @parameters :item:测试项
 *				status:状态
 * @retvalue   :None
******************************************************************************/
void Display_Post_Combination(u8 item,u8 status)    //    后组合线束
{
	switch(item)
	{
		case DROP :          //下视
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"下视:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"下视:PASS");
			}
			break;
		case GUIDE :         //引导
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,20,RED,GRAY0,"引导:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,20,BLUE,GRAY0,"引导:PASS");
			}
			break;
		case POWERKEY :     //电源按键
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,40,RED,GRAY0,"按键:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,40,BLUE,GRAY0,"按键:PASS");
			}
			break;
		case ALL_TEST :
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"下视:FAIL");
				Gui_DrawFont_GBK16(16,20,RED,GRAY0,"引导:FAIL");
				Gui_DrawFont_GBK16(16,40,RED,GRAY0,"按键:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"下视:PASS");
				Gui_DrawFont_GBK16(16,20,BLUE,GRAY0,"引导:PASS");
				Gui_DrawFont_GBK16(16,40,BLUE,GRAY0,"按键:PASS");
			}
			break;
		default :
			Gui_DrawPoint(0,0,GRAY0);
			break;
	}
}

/*****************************************************************************
 * @name       :Display_Dust_Combination(u8 item,u8 status)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :显示灰尘组合
 * @parameters :item:测试项
 *				status:状态
 * @retvalue   :None
******************************************************************************/
void Display_Dust_Combination(u8 item,u8 status)     //    灰尘组合
{
	switch(item)
	{
		case DUST :                  //灰尘
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"灰尘:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"灰尘:PASS");
			}
			break;
		case DUSTBOX :               //尘盒霍尔
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,20,RED,GRAY0,"霍尔:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,20,BLUE,GRAY0,"霍尔:PASS");
			}
			break;
		case ALL_TEST :
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"灰尘:FAIL");
				Gui_DrawFont_GBK16(16,20,RED,GRAY0,"霍尔:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"灰尘:PASS");
				Gui_DrawFont_GBK16(16,20,BLUE,GRAY0,"霍尔:PASS");
			}
			break;
		default :
			Gui_DrawPoint(0,0,GRAY0);
			break;
	}
}

/*****************************************************************************
 * @name       :Display_ChargingBase_Combination(u8 item,u8 status)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :显示充电座组件
 * @parameters :item:测试项
 *				status:状态
 * @retvalue   :None
******************************************************************************/
void Display_ChargingBase_Combination(u8 item,u8 status)     //    充电座组件
{
	switch(item)
	{
		case WORKLED :           //工作指示灯
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"指示灯:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"指示灯:PASS");
			}
			break;
		case GUIDE_SENDA :       //充电座A信号
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,20,RED,GRAY0,"引导A:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,20,BLUE,GRAY0,"引导A:PASS");
			}
			break;
		case GUIDE_SENDB :       //充电座B信号
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,40,RED,GRAY0,"引导B:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,40,BLUE,GRAY0,"引导B:PASS");
			}
			break;
		case GUIDE_SENDTOP :     //防撞信号
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,60,RED,GRAY0,"防撞:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,60,BLUE,GRAY0,"防撞:PASS");
			}
			break;
		case ALL_TEST :
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"指示灯:FAIL");
				Gui_DrawFont_GBK16(16,20,RED,GRAY0,"引导A:FAIL");
				Gui_DrawFont_GBK16(16,40,RED,GRAY0,"引导B:FAIL");
				Gui_DrawFont_GBK16(16,60,RED,GRAY0,"防撞:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"指示灯:PASS");
				Gui_DrawFont_GBK16(16,20,BLUE,GRAY0,"引导A:PASS");
				Gui_DrawFont_GBK16(16,40,BLUE,GRAY0,"引导B:PASS");
				Gui_DrawFont_GBK16(16,60,BLUE,GRAY0,"防撞:PASS");
			}
			break;
		default :
			Gui_DrawPoint(0,0,GRAY0);
			break;
	}
}

/*****************************************************************************
 * @name       :Display_Alongside_Combination(u8 item,u8 status)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :显示沿边组合
 * @parameters :item:测试项
 *				status:状态
 * @retvalue   :None
******************************************************************************/
void Display_Alongside_Combination(u8 item,u8 status)    //   沿边组件
{
	switch(item)
	{
		case PREVENT_BUMP45 :         //45度小板
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"45度:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"45度:PASS");
			}
			break;
		case ALONG_SIDEA :           //沿边A
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,20,RED,GRAY0,"沿边A:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,20,BLUE,GRAY0,"沿边A:PASS");
			}
			break;
		case ALONG_SIDEB :           //沿边B
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,40,RED,GRAY0,"沿边B:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,40,BLUE,GRAY0,"沿边B:PASS");
			}
			break;
		case GUIDE :
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,60,RED,GRAY0,"引导:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,60,BLUE,GRAY0,"引导:PASS");
			}
			break;
		case ALL_TEST :
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"45度:FAIL");
				Gui_DrawFont_GBK16(16,20,RED,GRAY0,"沿边A:FAIL");
				Gui_DrawFont_GBK16(16,40,RED,GRAY0,"沿边B:FAIL");
				Gui_DrawFont_GBK16(16,60,RED,GRAY0,"引导:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"45度:PASS");
				Gui_DrawFont_GBK16(16,20,BLUE,GRAY0,"沿边A:PASS");
				Gui_DrawFont_GBK16(16,40,BLUE,GRAY0,"沿边B:PASS");
				Gui_DrawFont_GBK16(16,60,BLUE,GRAY0,"引导:PASS");
			}
			break;
		default :
			Gui_DrawPoint(0,0,GRAY0);
			break;
	}
}

/*****************************************************************************
 * @name       :void Display_Test(u8 test_mode,u8 test_cmd,u8 test_status)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :display test
 * @parameters :test_cmd 读取到的命令码
 * @retvalue   :None
******************************************************************************/
void Display_Test(u8 test_mode,u8 test_cmd,u8 test_status)
{
	Gui_DrawFont_GBK16(16,56,GRAY0,GRAY0,"检测中...");
	
	switch(test_mode)
	{
		case TEST_FRONTDROP :               //前下视组件
			Display_FontDrop(test_cmd,test_status);
			break;
		case TEST_WHEEL :                   //大轮线束组件
			Display_Wheel(test_cmd,test_status);
			break;
		case TEST_KEY :                     //后下视-后引导-电源按键
			Display_Post_Combination(test_cmd,test_status);
			break;
		case TEST_DUST :                    //灰尘检测-尘盒霍尔组件
			Display_Dust_Combination(test_cmd,test_status);
			break;
		case TEST_GUIDE :                   //充电座引导组件
			Display_ChargingBase_Combination(test_cmd,test_status);
			break;
		case TEST_SIDE :                    //沿边-引导组件
			Display_Alongside_Combination(test_cmd,test_status);
			break;
		default :
			break;
	}
	//Lcd_Init();
// 	LCD_LED_SET;//通过IO控制背光亮		
	
//	LCD_LED_CLR;//IO控制背光灭	
}




















