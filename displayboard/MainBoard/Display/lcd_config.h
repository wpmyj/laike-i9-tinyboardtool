/****************************************************************************************************/
/*			@FileName		:	LCD_CONFIG.h																																		   	*/
/*			@Author			:	zhengbaiqiu																																		*/
/*			@Version		:	v1.0																																					*/
/*			@Date			:	2019.11.12  																																	*/
/****************************************************************************************************/

#ifndef	__LCD_CONFIG_H__
#define	__LCD_CONFIG_H__
#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

#define USE_SCREEN_WHIRL  	 3 //����Һ����˳ʱ����ת���� 	0-0����ת��1-90����ת��2-180����ת��3-270����ת

#define X_MAX_PIXEL	         128   //����LCD��
#define Y_MAX_PIXEL	         160   //����LCD��
	
	
#ifdef _cplusplus
extern }
#endif

#endif /*__LCD_CONFIG_H__*/	





