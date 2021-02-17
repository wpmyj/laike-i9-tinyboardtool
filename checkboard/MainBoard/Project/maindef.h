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
} enum_UART2_RECEIVE_STEP;

typedef enum {
	TEST_FRONTDROP = 0x01,       //前下视组件
	TEST_WHEEL,                  //大轮线束组件
	TEST_KEY,                    //后下视-后引导-电源按键
	TEST_DUST,                   //灰尘检测-尘盒霍尔组件
	TEST_GUIDE,                  //充电座引导组件
	TEST_SIDE                    //沿边-引导组件
} enum_TEST_MODE;

typedef enum {
	ST_AVOID_BUMP45    = ((uint16_t)0x0001),       //免碰撞45度发射
	ST_ALONG_SIDE_A    = ((uint16_t)0x0002),       //沿边A信号
	ST_ALONG_SIDE_B    = ((uint16_t)0x0004),       //沿边B信号
	ST_DUST            = ((uint16_t)0x0008),       //灰尘发射
	ST_CODE            = ((uint16_t)0x0010),       //码盘发射
	ST_FRONT_DROP      = ((uint16_t)0x0020),       //前下视发射
	ST_LEFT_RIGHT_DROP = ((uint16_t)0x0040),       //左、右下视发射
	ST_BACK_DROP       = ((uint16_t)0x0080),       //后下视发射
	ST_CHARGE_A        = ((uint16_t)0x0100),       //充电座引导A发射
	ST_CHARGE_B        = ((uint16_t)0x0200),       //充电座引导B发射
	ST_CHARGE_TOP      = ((uint16_t)0x0400),       //充电座引导TOP发射
	ST_BUMP_DUSTBOX    = ((uint16_t)0x0800),       //碰撞尘盒
} enum_SEND_SIGNAL;

typedef enum {
	ST_DROP_FRONT               = ((uint16_t)0x0001),       //前下视接收
	ST_DROP_LEFT_RIGHT          = ((uint16_t)0x0002),       //左右下视接收
	ST_DROP_BACK                = ((uint16_t)0x0004),       //后下视接收
	ST_BUMP_LEFT_RIGHT          = ((uint16_t)0x0008),       //左右碰撞接收
	ST_LEAVE_LEFT_RIGHT         = ((uint16_t)0x0010),       //左右离地接收
	ST_DUST_BOX                 = ((uint16_t)0x0020),       //尘盒接收
	ST_CODE_LEFT_RIGHT          = ((uint16_t)0x0040),       //左右码盘接收
	ST_POWER_KEY                = ((uint16_t)0x0080),       //电源按键检测
	ST_GUIDE_LEFT_RIGHT         = ((uint16_t)0x0100),       //左右引导接收
	ST_GUIDE_BACK               = ((uint16_t)0x0200),       //后引导接收
	ST_DUST_CHECK               = ((uint16_t)0x0400),       //灰尘检测
	ST_CHARGE_BASE              = ((uint16_t)0x0800),       //充电座检测
	ST_ALONG_SIDE               = ((uint16_t)0x1000)        //沿边检测
} enum_RECEIVE_SIGNAL;

// typedef enum {
// 	ST_DROP_FRONT = 0x01,
// 	ST_DROP_LEFT_RIGHT,
// 	ST_DROP_BACK
// } enum_DROP_POSITION;

typedef struct _ST_SYSTEM_MANAGER{
	u32 mSystemTick ;                /*  1ms 模拟滴答计数器   */
	u32 mPowerOnStartTime ;
	
	u32 mCheckStartTime;
	u32 mPowerOnTime;            //开机初始化
	u32 mReadModeStartTime;      //读取模式时间
	u32 mDelayStartTime;         
	u32 mGuideStartTime;         //引导边沿触发
	u32 mChargeStartTime;
	u32 mKeyStartTime;           //按键开始时间
	u32 mGuideStartTimeT1;
	u32 mGuideStartTimeT2;
	u32 mGuideStartTimeT3;
}_ST_SYSTEM_MANAGER;

typedef struct _ST_WORK_MANAGER{
 	u8 mWorkMODE ;           //工作模式
// 	u8 mWorkCMD ;            //工作命令字
// 	u8 mWorkSTATUS ;         //传感器状态
// 	
// 	u8 mWorkCheckMODE;       //显示板检测到的模式
	
	u8 mReceiveStatus : 1;   //下视没收到信号高电平  收到信号低电平   0：没完成第一步  1：完成第一步
	u8 mWorkChecking : 1;    //0:没有检测     1：正在检测中
	
	u8 mCodeReceiveStatus : 1;  //码盘没收到信号高电平  收到信号低电平   0：没完成第一步  1：完成第一步
//	u8 mKeyReceiveStatus : 1;         //按键接收到第一步
	u8 mGuideBackStatus : 1;          //后引导波形接收  0:第一次边沿触发时间记录   1：第二次边沿触发事件记录
	u8 mAvoidBumpStatus : 1;          //免碰撞接收      0:没有完成第一步           1：完成第一步
	u8 mAlongSideStatus : 2;          //沿边接收        0：没有完成第一步  1：完成第一步   2：完成第二步    3：完成第三步
	u8 mGuideRightLeftStatus : 1;     //左右引导波形接收    0：没有完成第一步
	u8 mChargeBaseGuideAStatus : 1;       //充电座引导A信号        0:没有完成第一步           1：完成第一步
	u8 mChargeBaseGuideBStatus : 1;       //充电座引导B信号        0:没有完成第一步           1：完成第一步
	u8 mChargeBaseGuideTOPStatus : 1;     //充电座引导TOP信号      0:没有完成第一步           1：完成第一步
	u8 mBumpStatus : 1;
	u8 mDustBoxStatus : 1;
	
	u8 GuideFirstFlag : 1;
	
	u8 mGuideSendFlag : 1;
	u8 mGuideSendChangeFlag : 1;
	u8 mGuideSendTime;
	
	u8 mWorkCont;                         //工作计数
	u8 mGuideCont;                        //引导计数
	
	u8 mWorkKey : 1;         //0:未按下   1:按下
	u8 mCheckStatusFlag : 1; //0:fail     1:ok
	u8 mKeyStatus : 1;
	u8 mKeyCont;
	
	u8 mFrontDropFlag : 1;             //前下视             0：未触发   1：触发
	u8 mLeftRightDropFlag : 1;         //左、右下视
	u8 mBackDropFlag : 1;              //后下视
	u8 mLeftRightBumpFlag : 1;         //左、右碰撞
	u8 mLeftRightLeaveFlag : 1;        //左、右离地
	u8 mDustBoxFlag : 1;               //尘盒
	u8 mLeftRightCodeFlag : 1;         //左、右码盘
	u8 mPowerKeyFlag : 1;              //电源按键
	u8 mLeftRightGuideFlag : 1;        //左、右引导
	u8 mBackGuideFlag : 1;             //后引导
	u8 mWorkLedFlag : 1;               //充电座电源指示灯
	u8 mChargeBaseGuideAFlag : 1;      //充电座引导A信号
	u8 mChargeBaseGuideBFlag : 1;      //充电座引导B信号
	u8 mChargeBaseGuideTopFlag : 1;    //充电座防撞信号
	u8 m45BoardFlag : 1;               //45度小板
	u8 mAlongSideAFlag : 1;            //沿边A
	u8 mAlongSideBFlag : 1;            //沿边B
	
	u8 mDust3V3Flag : 1;               //灰尘3.3V触发标志位
	u8 mDustFlag : 1;                  //灰尘触发标志位
	
// 	u8 mFrontDropModeleEnable : 1;     //前下视接收使能      0：DISABLE   1：ENABLE
// 	u8 mWheelWireModeleEnable : 1;     //大轮线束接收使能
// 	u8 mPowerKeyModeleEnable : 1;      //后组合线束接收使能
// 	u8 mDustBoxModeleEnable : 1;       //灰尘线束接收使能
// 	u8 mChargeBaseModeleEnable : 1;    //充电座引导组件接收使能
// 	u8 mAlongSideModeleEnable : 1;     //沿边组件接收使能
	
	u8 mFrontDropCheckEnable : 1;                  //前下视接收         0：DISABLE   1：ENABLE
	u8 mLeftRightDropCheckEnable : 1;              //左右下视接收
	u8 mBackDropCheckEnable : 1;                   //后下视接收
	u8 mLeftRightBumpCheckEnable : 1;              //左右碰撞接收
	u8 mLeftRightLeaveCheckEnable : 1;             //左右离地接收
	u8 mDustBoxCheckEnable : 1;                    //尘盒检测
	u8 mLeftRightCodeCheckEnable : 1;              //左右码盘接收
	u8 mPowerKeyCheckEnable : 1;                   //电源按键接收
	u8 mLeftRightGuideCheckEnable : 1;             //左右引导接收
	u8 mBackGuideCheckEnable : 1;                  //后引导接收
	u8 mDustCheckEnable : 1;                       //灰尘接收
	u8 mChargeBaseCheckEnable : 1;                 //充电座接收
	u8 mAlongSideCheckEnable : 1;                  //沿边接收
	
	
	u16 mSensorADC[6];                 //传感器ADC
	
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
	/*        文件内定义        */
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

