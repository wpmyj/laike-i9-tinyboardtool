/****************************************************************************************************/
/*			@FileName		:	DELAY.h																																		   	*/
/*			@Author			:	zhengbaiqiu																																		*/
/*			@Version		:	v1.0																																					*/
/*			@Date			:	2019.11.12  																																	*/
/****************************************************************************************************/
#ifndef __DELAY_H
#define __DELAY_H 			   
#include "stm32f10x.h"


#ifdef __cplusplus
extern "C" {
#endif

void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);
	
	
#ifdef _cplusplus
extern }
#endif

#endif





























