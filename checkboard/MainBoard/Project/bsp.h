/****************************************************************************************************/
/*			@FileName		:	BSP.h																																	  	*/
/*			@Author			:	zhengbaiqiu																																		*/
/*			@Version		:	v1.0																																					*/
/*			@Date			:	2019.11.12  																																	*/
/****************************************************************************************************/

#ifndef	__BSP_H__
#define	__BSP_H__

#include <stdlib.h>
#include "ht32.h"


#ifdef __cplusplus
extern "C" {
#endif

	
void prvSetupHardware(void);
void PWM_Configuration(u8 mFlag);
	
	
#ifdef _cplusplus
extern }
#endif

#endif /*__BSP_H__*/	

