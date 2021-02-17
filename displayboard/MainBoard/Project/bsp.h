/****************************************************************************************************/
/*			@FileName		:	BSP.h																																		    	*/
/*			@Author			:	zhengbaiqiu																																		*/
/*			@Version		:	v1.0																																					*/
/*			@Date				:	2019.11.12  																																	*/
/****************************************************************************************************/

#ifndef	__BSP_H__
#define	__BSP_H__


#include "stm32f10x.h"
#include <stm32f10x_conf.h>

#ifdef __cplusplus
extern "C" {
#endif

	
	
	
/*-----------------------------------------------------------------------*/
/*    中断优先级                                                         */
/*-----------------------------------------------------------------------*/
	
#define WIFI_UART_PRIORITY                        5
#define TIME_TIM2_PRIORITY                        2
#define WIFI_UART_DMA_PRIORITY                    6
	
	
/*-----------------------------------------------------------------------*/
/*    端口定义                                                           */
/*-----------------------------------------------------------------------*/

/* UART configuration ----------------------------------------------*/
	
#define DEBUG_UART_GPIO                GPIOA               //      uart1
#define DEBUG_UART_TX_PIN         	   GPIO_Pin_9
#define DEBUG_UART_RX_PIN              GPIO_Pin_10


#define MAIN_UART_GPIO             	   GPIOA               //      uart2
#define MAIN_UART_TX_PIN               GPIO_Pin_2
#define MAIN_UART_RX_PIN               GPIO_Pin_3	
	
	
// /* board configuration */
// // <o> SDCard Driver <1=>SDIO sdcard <0=>SPI MMC card
// // 	<i>Default: 1
// #define STM32_USE_SDIO			0

// /* whether use board external SRAM memory */
// // <e>Use external SRAM memory on the board
// // 	<i>Enable External SRAM memory
// #define STM32_EXT_SRAM          0
// //	<o>Begin Address of External SRAM
// //		<i>Default: 0x68000000
// #define STM32_EXT_SRAM_BEGIN    0x68000000 /* the begining address of external SRAM */
// //	<o>End Address of External SRAM
// //		<i>Default: 0x68080000
// #define STM32_EXT_SRAM_END      0x68080000 /* the end address of external SRAM */
// // </e>

// // <o> Internal SRAM memory size[Kbytes] <8-64>
// //	<i>Default: 64
// #define STM32_SRAM_SIZE         64
// #define STM32_SRAM_END          (SRAM_BASE + STM32_SRAM_SIZE * 1024)

// #define VECT_TAB_FLASH                                     /* use Flash to store vector table */

// #define LED_RUN_ON              GPIO_SetBits  (GPIOB,GPIO_Pin_5)  	   //RUN
// #define LED_RUN_OFF             GPIO_ResetBits(GPIOB,GPIO_Pin_5) 	   //RUN
	
	
	
	
	
	
	
static volatile ErrorStatus HSEStartUpStatus = SUCCESS;	

void prvSetupHardware(void);
void delay(uint32_t nCount);	


	
	
#ifdef _cplusplus
extern }
#endif

#endif /*__BSP_H__*/	

