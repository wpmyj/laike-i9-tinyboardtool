/****************************************************************************************************/
/*			@FileName		:	APILIB.h																																	  	*/
/*			@Author			:	zhengbaiqiu																																		*/
/*			@Version		:	v1.0																																					*/
/*			@Date			:	2019.11.12  																																	*/
/****************************************************************************************************/

#ifndef	__APILIB_H__
#define	__APILIB_H__

#include <stdlib.h>
#include "ht32.h"
#include "maindef.h"


#ifdef __cplusplus
extern "C" {
#endif


u32 api_GetSystemTimePeriod(u32 StartTime);
u32 api_GetCurrentSystemTime(void);
void api_Uart0_SendCMDData(u8 mode,u8 cmd,u8 status);
void api_UART0_ReceiveData(u8 vData);	
void Test_Item_Check_Enable(u16 mPre,ControlStatus mState);
void api_Test_PCB(u8 item);
void api_MachinePowerOn(void);
void Scan_Key(void);
void api_Send_PWM(u8 mFlag);

	
	
#ifdef _cplusplus
extern }
#endif

#endif /*__APILIB_H__*/	

