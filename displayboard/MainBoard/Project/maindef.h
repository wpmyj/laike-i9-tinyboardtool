/****************************************************************************************************/
/*			@FileName		:	MAINDEF.h																																	  	*/
/*			@Author			:	zhengbaiqiu																																		*/
/*			@Version		:	v1.0																																					*/
/*			@Date				:	2019.11.12  																																	*/
/****************************************************************************************************/

#ifndef	__MAINDEF_H__
#define	__MAINDEF_H__

#include <stdlib.h>
#include "stm32f10x.h"


#ifdef __cplusplus
extern "C" {
#endif




/*--------------------------------------------------------------------------

        自定义结构体

---------------------------------------------------------------------------*/	


/*   	 自定义			*/
#define SYSTEMTIMEMAX                86400000               /*   System timing maximum 24 hours  */
	
#define FALSE                        (0)                   /*   假  */
#define TRUE                         (1)                   /*   真  */

/* timeout time define */

#define TIMEOUT_0ms           (0)               /*   0ms超时      */
#define TIMEOUT_5ms           (5)               /*   5ms超时      */
#define TIMEOUT_10ms          (10)              /*   10ms超时     */
#define TIMEOUT_50ms          (50)              /*   50ms超时     */
#define TIMEOUT_100ms         (100)             /*   100ms超时    */
#define TIMEOUT_300ms         (300)             /*   300ms超时    */
#define TIMEOUT_500ms         (500)             /*   500ms超时    */
#define TIMEOUT_1000ms        (1000)            /*   1000ms超时   */
#define TIMEOUT_3500ms        (3500)            /*   3500ms超时   */
#define TIMEOUT_5000ms        (5000)            /*   5000ms超时   */

/*   	 结构体声明			*/

typedef enum {
	ST_0x55 = 0x01,
	ST_0xAA,
	ST_MODE,
	ST_CMD,
	ST_STA,
	ST_BCC
} UART2_RECEIVE_STEP;

typedef enum {
	TEST_FRONTDROP = 0x01,       //前下视组件
	TEST_WHEEL,                  //大轮线束组件
	TEST_KEY,                    //后下视-后引导-电源按键
	TEST_DUST,                   //灰尘检测-尘盒霍尔组件
	TEST_GUIDE,                  //充电座引导组件
	TEST_SIDE                    //沿边-引导组件
} TEST_MODE;

typedef struct _ST_SYSTEM_MANAGER{
	u32 mSystemTick ;                /*  1ms 模拟滴答计数器   */
	u32 mPowerOnStartTime ;
	
	u32 mScreenUpdateStartTime;
}_ST_SYSTEM_MANAGER;

typedef struct _ST_WORK_MANAGER{
	u8 mWorkMODE ;           //工作模式
	u8 mWorkCMD ;            //工作命令字
	u8 mWorkSTATUS ;         //传感器状态
	
	u8 mWorkCheckMODE;       //显示板检测到的模式
	
	u8 mWorkKeyPress : 1;    //按键按下标志位     0：未按下   1：按下
	u8 mKeyPressFlag : 1;    //按下抬起为一次     0：未触发   1：触发
	u8 mKeyPressTime;
	
	u8 mPowerOnFlag : 1;     //初始化标志位       0:初始化中  1:初始化成功
}_ST_WORK_MANAGER;

/*		struct congig		*/
typedef struct _ST_USART_MANAGER{
	u8 TxBuffer[64];
	u8 RxBuffer[64];
	u8 SendBuf[64];
	u8 ReceiveBuf[64];
	
	u16 mSendSize;
	u16 mSendCont;
	
	u8 mReceiveStep : 3;
	u8 mReceiveBCC;
	u16 mReceiveSize;
	u16 mReceiveCont;
	u16 mReceiveTimeOut;
	
	u8 mReceiveFlag : 1;
	
}_ST_USART_MANAGER;

typedef struct _ST_MAINMANAGER{
	/*        文件内定义        */
	_ST_SYSTEM_MANAGER         mSystem;
	_ST_USART_MANAGER          mUart2;
	_ST_WORK_MANAGER           mWork;
}_ST_MAINMANAGER;
		
extern _ST_MAINMANAGER mMaininf;
	
void prvSetupHardware(void);



	
	
#ifdef _cplusplus
extern }
#endif

#endif /*__MAINDEF_H__*/	

