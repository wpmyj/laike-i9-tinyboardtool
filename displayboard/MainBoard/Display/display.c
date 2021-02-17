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
 * @function   :��ʾǰ����
 * @parameters :item:������  
 *				status:״̬
 * @retvalue   :None
******************************************************************************/
void Display_FontDrop(u8 item,u8 status)     //   ǰ����
{
	switch(item)
	{
		case DROP :
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"����:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"����:PASS");
			}
			break;
		case ALL_TEST :
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"����:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"����:PASS");
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
 * @function   :��ʾ��������
 * @parameters :item:������
 *				status:״̬
 * @retvalue   :None
******************************************************************************/
void Display_Wheel(u8 item,u8 status)     //   ��������
{
	switch(item)
	{
		case DROP :            //����
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"����:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"����:PASS");
			}
			break;
		case BUMP :           //��ײ
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,20,RED,GRAY0,"��ײ:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,20,BLUE,GRAY0,"��ײ:PASS");
			}
			break;
		case CODE :          //����       
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,40,RED,GRAY0,"����:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,40,BLUE,GRAY0,"����:PASS");
			}
			break;
		case LEAVELAND :    //���
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,60,RED,GRAY0,"���:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,60,BLUE,GRAY0,"���:PASS");
			}
			break;
		case ALL_TEST :
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"����:FAIL");
				Gui_DrawFont_GBK16(16,20,RED,GRAY0,"��ײ:FAIL");
				Gui_DrawFont_GBK16(16,40,RED,GRAY0,"����:FAIL");
				Gui_DrawFont_GBK16(16,60,RED,GRAY0,"���:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"����:PASS");
				Gui_DrawFont_GBK16(16,20,BLUE,GRAY0,"��ײ:PASS");
				Gui_DrawFont_GBK16(16,40,BLUE,GRAY0,"����:PASS");
				Gui_DrawFont_GBK16(16,60,BLUE,GRAY0,"���:PASS");
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
 * @function   :��ʾ���������
 * @parameters :item:������
 *				status:״̬
 * @retvalue   :None
******************************************************************************/
void Display_Post_Combination(u8 item,u8 status)    //    ���������
{
	switch(item)
	{
		case DROP :          //����
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"����:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"����:PASS");
			}
			break;
		case GUIDE :         //����
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,20,RED,GRAY0,"����:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,20,BLUE,GRAY0,"����:PASS");
			}
			break;
		case POWERKEY :     //��Դ����
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,40,RED,GRAY0,"����:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,40,BLUE,GRAY0,"����:PASS");
			}
			break;
		case ALL_TEST :
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"����:FAIL");
				Gui_DrawFont_GBK16(16,20,RED,GRAY0,"����:FAIL");
				Gui_DrawFont_GBK16(16,40,RED,GRAY0,"����:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"����:PASS");
				Gui_DrawFont_GBK16(16,20,BLUE,GRAY0,"����:PASS");
				Gui_DrawFont_GBK16(16,40,BLUE,GRAY0,"����:PASS");
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
 * @function   :��ʾ�ҳ����
 * @parameters :item:������
 *				status:״̬
 * @retvalue   :None
******************************************************************************/
void Display_Dust_Combination(u8 item,u8 status)     //    �ҳ����
{
	switch(item)
	{
		case DUST :                  //�ҳ�
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"�ҳ�:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"�ҳ�:PASS");
			}
			break;
		case DUSTBOX :               //���л���
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,20,RED,GRAY0,"����:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,20,BLUE,GRAY0,"����:PASS");
			}
			break;
		case ALL_TEST :
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"�ҳ�:FAIL");
				Gui_DrawFont_GBK16(16,20,RED,GRAY0,"����:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"�ҳ�:PASS");
				Gui_DrawFont_GBK16(16,20,BLUE,GRAY0,"����:PASS");
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
 * @function   :��ʾ��������
 * @parameters :item:������
 *				status:״̬
 * @retvalue   :None
******************************************************************************/
void Display_ChargingBase_Combination(u8 item,u8 status)     //    ��������
{
	switch(item)
	{
		case WORKLED :           //����ָʾ��
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"ָʾ��:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"ָʾ��:PASS");
			}
			break;
		case GUIDE_SENDA :       //�����A�ź�
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,20,RED,GRAY0,"����A:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,20,BLUE,GRAY0,"����A:PASS");
			}
			break;
		case GUIDE_SENDB :       //�����B�ź�
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,40,RED,GRAY0,"����B:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,40,BLUE,GRAY0,"����B:PASS");
			}
			break;
		case GUIDE_SENDTOP :     //��ײ�ź�
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,60,RED,GRAY0,"��ײ:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,60,BLUE,GRAY0,"��ײ:PASS");
			}
			break;
		case ALL_TEST :
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"ָʾ��:FAIL");
				Gui_DrawFont_GBK16(16,20,RED,GRAY0,"����A:FAIL");
				Gui_DrawFont_GBK16(16,40,RED,GRAY0,"����B:FAIL");
				Gui_DrawFont_GBK16(16,60,RED,GRAY0,"��ײ:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"ָʾ��:PASS");
				Gui_DrawFont_GBK16(16,20,BLUE,GRAY0,"����A:PASS");
				Gui_DrawFont_GBK16(16,40,BLUE,GRAY0,"����B:PASS");
				Gui_DrawFont_GBK16(16,60,BLUE,GRAY0,"��ײ:PASS");
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
 * @function   :��ʾ�ر����
 * @parameters :item:������
 *				status:״̬
 * @retvalue   :None
******************************************************************************/
void Display_Alongside_Combination(u8 item,u8 status)    //   �ر����
{
	switch(item)
	{
		case PREVENT_BUMP45 :         //45��С��
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"45��:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"45��:PASS");
			}
			break;
		case ALONG_SIDEA :           //�ر�A
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,20,RED,GRAY0,"�ر�A:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,20,BLUE,GRAY0,"�ر�A:PASS");
			}
			break;
		case ALONG_SIDEB :           //�ر�B
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,40,RED,GRAY0,"�ر�B:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,40,BLUE,GRAY0,"�ر�B:PASS");
			}
			break;
		case GUIDE :
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,60,RED,GRAY0,"����:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,60,BLUE,GRAY0,"����:PASS");
			}
			break;
		case ALL_TEST :
			if(status == 0)
			{
				Gui_DrawFont_GBK16(16,0,RED,GRAY0,"45��:FAIL");
				Gui_DrawFont_GBK16(16,20,RED,GRAY0,"�ر�A:FAIL");
				Gui_DrawFont_GBK16(16,40,RED,GRAY0,"�ر�B:FAIL");
				Gui_DrawFont_GBK16(16,60,RED,GRAY0,"����:FAIL");
			}
			else if(status == 1)
			{
				Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"45��:PASS");
				Gui_DrawFont_GBK16(16,20,BLUE,GRAY0,"�ر�A:PASS");
				Gui_DrawFont_GBK16(16,40,BLUE,GRAY0,"�ر�B:PASS");
				Gui_DrawFont_GBK16(16,60,BLUE,GRAY0,"����:PASS");
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
 * @parameters :test_cmd ��ȡ����������
 * @retvalue   :None
******************************************************************************/
void Display_Test(u8 test_mode,u8 test_cmd,u8 test_status)
{
	Gui_DrawFont_GBK16(16,56,GRAY0,GRAY0,"�����...");
	
	switch(test_mode)
	{
		case TEST_FRONTDROP :               //ǰ�������
			Display_FontDrop(test_cmd,test_status);
			break;
		case TEST_WHEEL :                   //�����������
			Display_Wheel(test_cmd,test_status);
			break;
		case TEST_KEY :                     //������-������-��Դ����
			Display_Post_Combination(test_cmd,test_status);
			break;
		case TEST_DUST :                    //�ҳ����-���л������
			Display_Dust_Combination(test_cmd,test_status);
			break;
		case TEST_GUIDE :                   //������������
			Display_ChargingBase_Combination(test_cmd,test_status);
			break;
		case TEST_SIDE :                    //�ر�-�������
			Display_Alongside_Combination(test_cmd,test_status);
			break;
		default :
			break;
	}
	//Lcd_Init();
// 	LCD_LED_SET;//ͨ��IO���Ʊ�����		
	
//	LCD_LED_CLR;//IO���Ʊ�����	
}




















