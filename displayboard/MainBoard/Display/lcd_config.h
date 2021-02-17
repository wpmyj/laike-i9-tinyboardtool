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

#define USE_SCREEN_WHIRL  	 3 //定义液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转

#define X_MAX_PIXEL	         128   //定义LCD宽
#define Y_MAX_PIXEL	         160   //定义LCD长
	
	
#ifdef _cplusplus
extern }
#endif

#endif /*__LCD_CONFIG_H__*/	





