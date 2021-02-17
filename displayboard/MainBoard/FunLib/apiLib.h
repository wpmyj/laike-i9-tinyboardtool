/****************************************************************************************************/
/*			@FileName		:	apiLib.h																																    	*/
/*			@Author			:	zhengbaiqiu																																		*/
/*			@Version		:	v1.0																																					*/
/*			@Date				:	2019.11.12  																																	*/
/****************************************************************************************************/

#ifndef	__APILIB_H__
#define	__APILIB_H__
#include "stm32f10x.h"
#include "maindef.h"
#include "gpioredef.h"

#ifdef __cplusplus
extern "C" {
#endif



	
u32 api_GetSystemTimePeriod(u32 StartTime);
u32 api_GetCurrentSystemTime(void);
void api_UART2_SendData(u16 vCont, u8 *pData);
void api_SendCMDData(u8 mode,u8 cmd,u8 status);
void api_UART2_ReceiveData(u8 vData);
void Scan_Uart_Receive(void);
void Scan_Key(void);
void Key_Process(void);
	

	
	
#ifdef _cplusplus
extern }
#endif

#endif /*__APILIB_H__*/	

