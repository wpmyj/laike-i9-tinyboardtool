/****************************************************************************************************/
/*			@FileName		:	MAINDEF.h																																	  	*/
/*			@Author			:	zhengbaiqiu																																		*/
/*			@Version		:	v1.0																																					*/
/*			@Date			:	2019.11.12  																																	*/
/****************************************************************************************************/

#ifndef	__MAINDEF_H__
#define	__MAINDEF_H__

#include <stdlib.h>
#include "ht32.h"


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
} enum_UART2_RECEIVE_STEP;

typedef enum {
	TEST_FRONTDROP = 0x01,       //ǰ�������
	TEST_WHEEL,                  //�����������
	TEST_KEY,                    //������-������-��Դ����
	TEST_DUST,                   //�ҳ����-���л������
	TEST_GUIDE,                  //������������
	TEST_SIDE                    //�ر�-�������
} enum_TEST_MODE;

typedef enum {
	ST_AVOID_BUMP45    = ((uint16_t)0x0001),       //����ײ45�ȷ���
	ST_ALONG_SIDE_A    = ((uint16_t)0x0002),       //�ر�A�ź�
	ST_ALONG_SIDE_B    = ((uint16_t)0x0004),       //�ر�B�ź�
	ST_DUST            = ((uint16_t)0x0008),       //�ҳ�����
	ST_CODE            = ((uint16_t)0x0010),       //���̷���
	ST_FRONT_DROP      = ((uint16_t)0x0020),       //ǰ���ӷ���
	ST_LEFT_RIGHT_DROP = ((uint16_t)0x0040),       //�������ӷ���
	ST_BACK_DROP       = ((uint16_t)0x0080),       //�����ӷ���
	ST_CHARGE_A        = ((uint16_t)0x0100),       //���������A����
	ST_CHARGE_B        = ((uint16_t)0x0200),       //���������B����
	ST_CHARGE_TOP      = ((uint16_t)0x0400),       //���������TOP����
	ST_BUMP_DUSTBOX    = ((uint16_t)0x0800),       //��ײ����
} enum_SEND_SIGNAL;

typedef enum {
	ST_DROP_FRONT               = ((uint16_t)0x0001),       //ǰ���ӽ���
	ST_DROP_LEFT_RIGHT          = ((uint16_t)0x0002),       //�������ӽ���
	ST_DROP_BACK                = ((uint16_t)0x0004),       //�����ӽ���
	ST_BUMP_LEFT_RIGHT          = ((uint16_t)0x0008),       //������ײ����
	ST_LEAVE_LEFT_RIGHT         = ((uint16_t)0x0010),       //������ؽ���
	ST_DUST_BOX                 = ((uint16_t)0x0020),       //���н���
	ST_CODE_LEFT_RIGHT          = ((uint16_t)0x0040),       //�������̽���
	ST_POWER_KEY                = ((uint16_t)0x0080),       //��Դ�������
	ST_GUIDE_LEFT_RIGHT         = ((uint16_t)0x0100),       //������������
	ST_GUIDE_BACK               = ((uint16_t)0x0200),       //����������
	ST_DUST_CHECK               = ((uint16_t)0x0400),       //�ҳ����
	ST_CHARGE_BASE              = ((uint16_t)0x0800),       //��������
	ST_ALONG_SIDE               = ((uint16_t)0x1000)        //�ر߼��
} enum_RECEIVE_SIGNAL;

// typedef enum {
// 	ST_DROP_FRONT = 0x01,
// 	ST_DROP_LEFT_RIGHT,
// 	ST_DROP_BACK
// } enum_DROP_POSITION;

typedef struct _ST_SYSTEM_MANAGER{
	u32 mSystemTick ;                /*  1ms ģ��δ������   */
	u32 mPowerOnStartTime ;
	
	u32 mCheckStartTime;
	u32 mPowerOnTime;            //������ʼ��
	u32 mReadModeStartTime;      //��ȡģʽʱ��
	u32 mDelayStartTime;         
	u32 mGuideStartTime;         //�������ش���
	u32 mChargeStartTime;
	u32 mKeyStartTime;           //������ʼʱ��
	u32 mGuideStartTimeT1;
	u32 mGuideStartTimeT2;
	u32 mGuideStartTimeT3;
}_ST_SYSTEM_MANAGER;

typedef struct _ST_WORK_MANAGER{
 	u8 mWorkMODE ;           //����ģʽ
// 	u8 mWorkCMD ;            //����������
// 	u8 mWorkSTATUS ;         //������״̬
// 	
// 	u8 mWorkCheckMODE;       //��ʾ���⵽��ģʽ
	
	u8 mReceiveStatus : 1;   //����û�յ��źŸߵ�ƽ  �յ��źŵ͵�ƽ   0��û��ɵ�һ��  1����ɵ�һ��
	u8 mWorkChecking : 1;    //0:û�м��     1�����ڼ����
	
	u8 mCodeReceiveStatus : 1;  //����û�յ��źŸߵ�ƽ  �յ��źŵ͵�ƽ   0��û��ɵ�һ��  1����ɵ�һ��
//	u8 mKeyReceiveStatus : 1;         //�������յ���һ��
	u8 mGuideBackStatus : 1;          //���������ν���  0:��һ�α��ش���ʱ���¼   1���ڶ��α��ش����¼���¼
	u8 mAvoidBumpStatus : 1;          //����ײ����      0:û����ɵ�һ��           1����ɵ�һ��
	u8 mAlongSideStatus : 2;          //�ر߽���        0��û����ɵ�һ��  1����ɵ�һ��   2����ɵڶ���    3����ɵ�����
	u8 mGuideRightLeftStatus : 1;     //�����������ν���    0��û����ɵ�һ��
	u8 mChargeBaseGuideAStatus : 1;       //���������A�ź�        0:û����ɵ�һ��           1����ɵ�һ��
	u8 mChargeBaseGuideBStatus : 1;       //���������B�ź�        0:û����ɵ�һ��           1����ɵ�һ��
	u8 mChargeBaseGuideTOPStatus : 1;     //���������TOP�ź�      0:û����ɵ�һ��           1����ɵ�һ��
	u8 mBumpStatus : 1;
	u8 mDustBoxStatus : 1;
	
	u8 GuideFirstFlag : 1;
	
	u8 mGuideSendFlag : 1;
	u8 mGuideSendChangeFlag : 1;
	u8 mGuideSendTime;
	
	u8 mWorkCont;                         //��������
	u8 mGuideCont;                        //��������
	
	u8 mWorkKey : 1;         //0:δ����   1:����
	u8 mCheckStatusFlag : 1; //0:fail     1:ok
	u8 mKeyStatus : 1;
	u8 mKeyCont;
	
	u8 mFrontDropFlag : 1;             //ǰ����             0��δ����   1������
	u8 mLeftRightDropFlag : 1;         //��������
	u8 mBackDropFlag : 1;              //������
	u8 mLeftRightBumpFlag : 1;         //������ײ
	u8 mLeftRightLeaveFlag : 1;        //�������
	u8 mDustBoxFlag : 1;               //����
	u8 mLeftRightCodeFlag : 1;         //��������
	u8 mPowerKeyFlag : 1;              //��Դ����
	u8 mLeftRightGuideFlag : 1;        //��������
	u8 mBackGuideFlag : 1;             //������
	u8 mWorkLedFlag : 1;               //�������Դָʾ��
	u8 mChargeBaseGuideAFlag : 1;      //���������A�ź�
	u8 mChargeBaseGuideBFlag : 1;      //���������B�ź�
	u8 mChargeBaseGuideTopFlag : 1;    //�������ײ�ź�
	u8 m45BoardFlag : 1;               //45��С��
	u8 mAlongSideAFlag : 1;            //�ر�A
	u8 mAlongSideBFlag : 1;            //�ر�B
	
	u8 mDust3V3Flag : 1;               //�ҳ�3.3V������־λ
	u8 mDustFlag : 1;                  //�ҳ�������־λ
	
// 	u8 mFrontDropModeleEnable : 1;     //ǰ���ӽ���ʹ��      0��DISABLE   1��ENABLE
// 	u8 mWheelWireModeleEnable : 1;     //������������ʹ��
// 	u8 mPowerKeyModeleEnable : 1;      //�������������ʹ��
// 	u8 mDustBoxModeleEnable : 1;       //�ҳ���������ʹ��
// 	u8 mChargeBaseModeleEnable : 1;    //����������������ʹ��
// 	u8 mAlongSideModeleEnable : 1;     //�ر��������ʹ��
	
	u8 mFrontDropCheckEnable : 1;                  //ǰ���ӽ���         0��DISABLE   1��ENABLE
	u8 mLeftRightDropCheckEnable : 1;              //�������ӽ���
	u8 mBackDropCheckEnable : 1;                   //�����ӽ���
	u8 mLeftRightBumpCheckEnable : 1;              //������ײ����
	u8 mLeftRightLeaveCheckEnable : 1;             //������ؽ���
	u8 mDustBoxCheckEnable : 1;                    //���м��
	u8 mLeftRightCodeCheckEnable : 1;              //�������̽���
	u8 mPowerKeyCheckEnable : 1;                   //��Դ��������
	u8 mLeftRightGuideCheckEnable : 1;             //������������
	u8 mBackGuideCheckEnable : 1;                  //����������
	u8 mDustCheckEnable : 1;                       //�ҳ�����
	u8 mChargeBaseCheckEnable : 1;                 //���������
	u8 mAlongSideCheckEnable : 1;                  //�ر߽���
	
	
	u16 mSensorADC[6];                 //������ADC
	
	u8 mDropFrontStatusCont;
	u8 mDropLeftRightStatusCont;
	u8 mDropBackStatusCont;
	u8 mCodeLeftRightStatusCont;
	u8 mDustStatusCont;
	u8 mChargeBaseGuideAStatusCont;
	u8 mChargeBaseGuideBStatusCont;
	u8 mChargeBaseGuideTOPStatusCont;
	u8 mAvoidBumpStatusCont;
	u8 mAlongSideStatusCont;
	u8 mBumpStatusCont;
	u8 mDustBoxStatusCont;
	
	u8 mFrontDropFlagCont;
	u8 mLeftRightDropFlagCont;
	u8 mBackDropFlagCont;
	u8 mLeftRightCodeFlagCont;
	u8 mLeftRightLeaveFlagCont;
	u8 mDustFlagCont;
	u8 mWorkLedFlagCont;
	u8 mChargeBaseGuideAFlagCont;
	u8 mChargeBaseGuideBFlagCont;
	u8 mChargeBaseGuideTopFlagCont;
	u8 m45BoardFlagCont;
	u8 mLeftRightBumpFlagCont;
	u8 mDustBoxFlagCont;
	u8 mDustBoxHightFlag : 1;
	u8 mDustBoxHightFlagCont;
	u8 mDustBoxUartSendFlag : 1;
	
	u8 mChargeBaseGuideFlag : 2;
	u8 mChargeBaseGuideCont;
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
	_ST_USART_MANAGER          mUart0;
	_ST_USART_MANAGER          mUart1;
	_ST_WORK_MANAGER           mWork;
}_ST_MAINMANAGER;
		
extern _ST_MAINMANAGER mMaininf;
	
void prvSetupHardware(void);



	
	
#ifdef _cplusplus
extern }
#endif

#endif /*__MAINDEF_H__*/	

