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

        �Զ���ṹ��

---------------------------------------------------------------------------*/	


/*   	 �Զ���			*/
#define SYSTEMTIMEMAX                86400000               /*   System timing maximum 24 hours  */
	
#define FALSE                        (0)                   /*   ��  */
#define TRUE                         (1)                   /*   ��  */

/* timeout time define */

#define TIMEOUT_0ms           (0)               /*   0ms��ʱ      */
#define TIMEOUT_5ms           (5)               /*   5ms��ʱ      */
#define TIMEOUT_10ms          (10)              /*   10ms��ʱ     */
#define TIMEOUT_50ms          (50)              /*   50ms��ʱ     */
#define TIMEOUT_100ms         (100)             /*   100ms��ʱ    */
#define TIMEOUT_300ms         (300)             /*   300ms��ʱ    */
#define TIMEOUT_500ms         (500)             /*   500ms��ʱ    */
#define TIMEOUT_1000ms        (1000)            /*   1000ms��ʱ   */
#define TIMEOUT_3500ms        (3500)            /*   3500ms��ʱ   */
#define TIMEOUT_5000ms        (5000)            /*   5000ms��ʱ   */

/*   	 �ṹ������			*/

typedef enum {
	ST_0x55 = 0x01,
	ST_0xAA,
	ST_MODE,
	ST_CMD,
	ST_STA,
	ST_BCC
} UART2_RECEIVE_STEP;

typedef enum {
	TEST_FRONTDROP = 0x01,       //ǰ�������
	TEST_WHEEL,                  //�����������
	TEST_KEY,                    //������-������-��Դ����
	TEST_DUST,                   //�ҳ����-���л������
	TEST_GUIDE,                  //������������
	TEST_SIDE                    //�ر�-�������
} TEST_MODE;

typedef struct _ST_SYSTEM_MANAGER{
	u32 mSystemTick ;                /*  1ms ģ��δ������   */
	u32 mPowerOnStartTime ;
	
	u32 mScreenUpdateStartTime;
}_ST_SYSTEM_MANAGER;

typedef struct _ST_WORK_MANAGER{
	u8 mWorkMODE ;           //����ģʽ
	u8 mWorkCMD ;            //����������
	u8 mWorkSTATUS ;         //������״̬
	
	u8 mWorkCheckMODE;       //��ʾ���⵽��ģʽ
	
	u8 mWorkKeyPress : 1;    //�������±�־λ     0��δ����   1������
	u8 mKeyPressFlag : 1;    //����̧��Ϊһ��     0��δ����   1������
	u8 mKeyPressTime;
	
	u8 mPowerOnFlag : 1;     //��ʼ����־λ       0:��ʼ����  1:��ʼ���ɹ�
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
	/*        �ļ��ڶ���        */
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

