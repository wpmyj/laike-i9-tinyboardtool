/****************************************************************************************************/
/*			@FileName		:	DISPLAY.h																																		   	*/
/*			@Author			:	zhengbaiqiu																																		*/
/*			@Version		:	v1.0																																					*/
/*			@Date			:	2019.11.12  																																	*/
/****************************************************************************************************/

#ifndef	__DISPLAY_H__
#define	__DISPLAY_H__
#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	DROP = 0x01,                 //����             //1
	
	BUMP,                        //��ײ             //2
	CODE,                        //����             //3
	LEAVELAND,                   //���             //4
	
	GUIDE,                       //����             //5
	POWERKEY,                    //��Դ��           //6
	
	DUST,                        //�ҳ�             //7
	DUSTBOX,                     //����             //8
	
	WORKLED,                     //����ָʾ��       //9
	GUIDE_SENDA,                 //�����A�ź�      //10
	GUIDE_SENDB,                 //�����B�ź�      //11
	GUIDE_SENDTOP,               //��ײ             //12
	
	PREVENT_BUMP45,              //45��С��         //13
	ALONG_SIDEA,                 //�ر�A            //14
	ALONG_SIDEB,                 //�ر�B            //15
	
	ALL_TEST                     //ȫ��������       //16
} TEST_MODE_PART;


void Display_Test(u8 test_mode,u8 test_cmd,u8 test_status);
	
	
#ifdef _cplusplus
extern }
#endif

#endif /*__DISPLAY_H__*/	


