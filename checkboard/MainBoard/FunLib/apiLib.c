/****************************************************************************************************/
/*			@FileName		:	apiLib.c															  	*/
/*			@Author			:	zhengbaiqiu															*/
/*			@Version		:	v1.0																*/
/*			@Date			:	2019.11.12  														*/
/****************************************************************************************************/


/* Includes --------------------------------------------------------------------*/
#include "ht32.h"

/* Project ---------------------------------------------------------------------*/
#include "maindef.h"
#include "apiLib.h"
#include "bsp.h"

/* Variable --------------------------------------------------------------------*/
/* Function --------------------------------------------------------------------*/
u32 api_GetSystemTimePeriod(u32 StartTime);
u32 api_GetCurrentSystemTime(void);
void api_Uart0_SendCMDData(u8 mode,u8 cmd,u8 status);
void api_UART0_ReceiveData(u8 vData);
void Test_Item_Check_Enable(u16 mPre,ControlStatus mState);
void api_Test_PCB(u8 item);
void api_MachinePowerOn(void);
void Scan_Key(void);
void api_Send_PWM(u8 mFlag);



/*------------------------------------------------------------------------------

        时间相关库函数

--------------------------------------------------------------------------------*/

/*****************************************************************************
 * @name       :u32 api_GetSystemTimePeriod(u32 StartTime)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Get time difference
 * @parameters :start time
 * @retvalue   :time difference
******************************************************************************/
u32 api_GetSystemTimePeriod(u32 StartTime)
{
	u32 res;
	
	if(mMaininf.mSystem.mSystemTick >= StartTime)
	{
		res = mMaininf.mSystem.mSystemTick - StartTime;
	}
	else
	{
		res = SYSTEMTIMEMAX - StartTime + mMaininf.mSystem.mSystemTick;
	}
	
	return res;
}

/*****************************************************************************
 * @name       :u32 api_GetCurrentSystemTime(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Get clock ticks
 * @parameters :None
 * @retvalue   :time tick
******************************************************************************/
u32 api_GetCurrentSystemTime(void)
{
	return mMaininf.mSystem.mSystemTick;
}

/*--------------------------------------------------------------------------------

        主板UART通讯

---------------------------------------------------------------------------------*/


/*****************************************************************************
 * @name       :void api_UART0_SendData(u16 vCont, u8 *pData)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :send data
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void api_UART0_SendData(u16 vCont, u8 *pData)
{
	u16 iCont ;
	mMaininf.mUart0.mSendSize = vCont;
	for (iCont=0;iCont < vCont;iCont ++)
	{
		mMaininf.mUart0.TxBuffer[iCont] = *(pData + iCont);
	}
	USART_IntConfig(HT_UART0, USART_INT_TXDE, ENABLE);
}

/*****************************************************************************
 * @name       :void api_Uart0_SendCMDData(u8 mode,u8 cmd,u8 status)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :send data
 * @parameters :cmd
 * @retvalue   :None
******************************************************************************/
void api_Uart0_SendCMDData(u8 mode,u8 cmd,u8 status)
{
	mMaininf.mUart0.SendBuf[0] = 0x55;
	mMaininf.mUart0.SendBuf[1] = 0xAA;
	mMaininf.mUart0.SendBuf[2] = mode;
	mMaininf.mUart0.SendBuf[3] = cmd;
	mMaininf.mUart0.SendBuf[4] = status;
	mMaininf.mUart0.SendBuf[5] = mMaininf.mUart0.SendBuf[2] ^ mMaininf.mUart0.SendBuf[3] ^
								 mMaininf.mUart0.SendBuf[4];
	
	api_UART0_SendData(6,mMaininf.mUart0.SendBuf);
}

/*****************************************************************************
 * @name       :void api_ClearUART0ReceiveFlag(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :clear uart0 receive flag
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void api_ClearUART0ReceiveFlag(void)
{
	mMaininf.mUart0.mReceiveStep = ST_0xAA;
	mMaininf.mUart0.mReceiveBCC = 0;
	mMaininf.mUart0.mReceiveSize = 0;
}

/*****************************************************************************
 * @name       :void api_UART0_ReceiveData(u8 vData)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :receive data
 * @parameters :data
 * @retvalue   :None
******************************************************************************/
void api_UART0_ReceiveData(u8 vData)
{
	if(mMaininf.mUart0.mReceiveTimeOut == 0)
	{
		api_ClearUART0ReceiveFlag();
	}
	switch(mMaininf.mUart0.mReceiveStep)
	{
		case ST_0x55 :
			if(vData == 0x55)
			{
				mMaininf.mUart0.mReceiveStep = ST_0xAA;
				mMaininf.mUart0.mReceiveTimeOut = 50;
			}
			else mMaininf.mUart0.mReceiveTimeOut = 0;
			break;
		case ST_0xAA :
			if(vData == 0xAA)
			{
				mMaininf.mUart0.mReceiveStep = ST_MODE;
				mMaininf.mUart0.mReceiveTimeOut = 50;
			}
			else mMaininf.mUart0.mReceiveTimeOut = 0;
			break;
		case ST_MODE :
			mMaininf.mUart0.mReceiveStep = ST_CMD;
			mMaininf.mUart0.mReceiveTimeOut = 50;
			mMaininf.mUart0.RxBuffer[mMaininf.mUart0.mReceiveSize ++] = vData;
			mMaininf.mUart0.mReceiveBCC = mMaininf.mUart0.mReceiveBCC ^ vData;
			break;
		case ST_CMD :
			mMaininf.mUart0.mReceiveStep = ST_STA;
			mMaininf.mUart0.mReceiveTimeOut = 50;
			mMaininf.mUart0.RxBuffer[mMaininf.mUart0.mReceiveSize ++] = vData;
			mMaininf.mUart0.mReceiveBCC = mMaininf.mUart0.mReceiveBCC ^ vData;
			break;
		case ST_STA :
			mMaininf.mUart0.mReceiveStep = ST_BCC;
			mMaininf.mUart0.mReceiveTimeOut = 50;
			mMaininf.mUart0.RxBuffer[mMaininf.mUart0.mReceiveSize ++] = vData;
			mMaininf.mUart0.mReceiveBCC = mMaininf.mUart0.mReceiveBCC ^ vData;
			break;
		case ST_BCC :
			if(mMaininf.mUart0.mReceiveBCC == vData)
			{
				for(mMaininf.mUart0.mReceiveCont = 0;mMaininf.mUart0.mReceiveCont < 3;mMaininf.mUart0.mReceiveCont ++)
				{
					mMaininf.mUart0.ReceiveBuf[mMaininf.mUart0.mReceiveCont] = mMaininf.mUart0.RxBuffer[mMaininf.mUart0.mReceiveCont];
				}
				
				mMaininf.mUart0.mReceiveFlag = 1;       //接收到数据
				
			}
			else mMaininf.mUart0.mReceiveTimeOut = 0;
			break;
		default :
			break;
	}
}

/*****************************************************************************
 * @name       :void Test_Item_Check_Enable(enum_TEST_MODE test_mode,
											ControlStatus mState)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :check enable
 * @parameters :test_mode : 测试板   mState ： 使能状态
 * @retvalue   :None
******************************************************************************/
void Test_Item_Check_Enable(u16 mPre,ControlStatus mState)
{
	u8 iCont;
	
	for(iCont = 0;iCont < 13;iCont ++)
	{
		switch(mPre & (1 << iCont))
		{
			case ST_DROP_FRONT :             //前下视
				if(mState == ENABLE)
				{
					mMaininf.mWork.mFrontDropCheckEnable = 1;
				}
				else
				{
					mMaininf.mWork.mFrontDropCheckEnable = 0;
				}
				break;
			case ST_DROP_LEFT_RIGHT :         //左右下视
				if(mState == ENABLE)
				{
					mMaininf.mWork.mLeftRightDropCheckEnable = 1;
				}
				else
				{
					mMaininf.mWork.mLeftRightDropCheckEnable = 0;
				}
				break;
			case ST_DROP_BACK :           //后下视
				if(mState == ENABLE)
				{
					mMaininf.mWork.mBackDropCheckEnable = 1;
				}
				else
				{
					mMaininf.mWork.mBackDropCheckEnable = 0;
				}
				break;
			case ST_BUMP_LEFT_RIGHT :
				if(mState == ENABLE)
				{
					mMaininf.mWork.mLeftRightBumpCheckEnable = 1;
				}
				else
				{
					mMaininf.mWork.mLeftRightBumpCheckEnable = 0;
				}
				break;
			case ST_LEAVE_LEFT_RIGHT :
				if(mState == ENABLE)
				{
					mMaininf.mWork.mLeftRightLeaveCheckEnable = 1;
				}
				else
				{
					mMaininf.mWork.mLeftRightLeaveCheckEnable = 0;
				}
				break;
			case ST_DUST_BOX :
				if(mState == ENABLE)
				{
					mMaininf.mWork.mDustBoxCheckEnable = 1;
				}
				else
				{
					mMaininf.mWork.mDustBoxCheckEnable = 0;
				}
				break;
			case ST_CODE_LEFT_RIGHT :
				if(mState == ENABLE)
				{
					mMaininf.mWork.mLeftRightCodeCheckEnable = 1;
				}
				else
				{
					mMaininf.mWork.mLeftRightCodeCheckEnable = 0;
				}
				break;
			case ST_POWER_KEY :
				if(mState == ENABLE)
				{
					mMaininf.mWork.mPowerKeyCheckEnable = 1;
				}
				else
				{
					mMaininf.mWork.mPowerKeyCheckEnable = 0;
				}
				break;
			case ST_GUIDE_LEFT_RIGHT :
				if(mState == ENABLE)
				{
					mMaininf.mWork.mLeftRightGuideCheckEnable = 1;
				}
				else
				{
					mMaininf.mWork.mLeftRightGuideCheckEnable = 0;
				}
				break;
			case ST_GUIDE_BACK :
				if(mState == ENABLE)
				{
					mMaininf.mWork.mBackGuideCheckEnable = 1;
				}
				else
				{
					mMaininf.mWork.mBackGuideCheckEnable = 0;
				}
				break;
			case ST_DUST_CHECK :
				if(mState == ENABLE)
				{
					mMaininf.mWork.mDustCheckEnable = 1;
				}
				else
				{
					mMaininf.mWork.mDustCheckEnable = 0;
				}
				break;
			case ST_CHARGE_BASE :
				if(mState == ENABLE)
				{
					mMaininf.mWork.mChargeBaseCheckEnable = 1;
				}
				else
				{
					mMaininf.mWork.mChargeBaseCheckEnable = 0;
				}
				break;
			case ST_ALONG_SIDE :
				if(mState == ENABLE)
				{
					mMaininf.mWork.mAlongSideCheckEnable = 1;
				}
				else
				{
					mMaininf.mWork.mAlongSideCheckEnable = 0;
				}
				break;
			default :
				break;
		}
	}
}

/*****************************************************************************
 * @name       :void Signal_Send(u8 type,u8 status)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :signal send
 * @parameters :type : ST_AVOID_BUMP45        免碰撞45度发射    1
					   ST_ALONG_SIDE_A        沿边A发射         2
					   ST_ALONG_SIDE_B        沿边B发射         3
					   ST_DUST                灰尘发射          4
					   ST_CODE                码盘发射          5
					   ST_FRONT_DROP          前下视发射        6
					   ST_LEFT_RIGHT_DROP     左、右下视发射    7
					   ST_BACK_DROP           后下视发射        8
					   ST_CHARGE_A            充电座引导A发射   9
					   ST_CHARGE_B            充电座引导B发射   10
					   ST_CHARGE_TOP          充电座引导TOP发射 11
					   ST_BUMP_DUSTBOX        碰撞尘盒发射      12
				status : 0:关闭   1:打开
 * @retvalue   :None
******************************************************************************/
void Signal_Send(u16 type,u8 status)
{
	u8 iCont;
	
	for(iCont = 0;iCont < 12;iCont ++)
	{
		if(type & (0x01 << iCont))
		{
			switch(iCont)
			{
				case 0 :
					if(status == TRUE)
					{
						GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_2, SET);
					}
					else
					{
						GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_2, RESET);
					}
					break;
				case 1 :
					if(status == TRUE)
					{
						GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_5, SET);
					}
					else
					{
						GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_5, RESET);
					}
					break;
				case 2 :
					if(status == TRUE)
					{
						GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_14, SET);
					}
					else
					{
						GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_14, RESET);
					}
					break;
				case 3 :
					if(status == TRUE)
					{
						GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_13, SET);
					}
					else
					{
						GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_13, RESET);
					}
					break;
				case 4 :
					if(status == TRUE)
					{
						GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_4, SET);
					}
					else
					{
						GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_4, RESET);
					}
					break;
				case 5 :
					if(status == TRUE)
					{
						GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_10, SET);
					}
					else
					{
						GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_10, RESET);
					}
					break;
				case 6 :
					if(status == TRUE)
					{
						GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_0, SET);
					}
					else
					{
						GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_0, RESET);
					}
					break;
				case 7 :
					if(status == TRUE)
					{
						GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_14, SET);
					}
					else
					{
						GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_14, RESET);
					}
					break;
				case 8 :
					if(status == TRUE)
					{
						GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_1, SET);
					}
					else
					{
						GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_1, RESET);
					}
					break;
				case 9 :
					if(status == TRUE)
					{
						GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_2, SET);
					}
					else
					{
						GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_2, RESET);
					}
					break;
				case 10 :
					if(status == TRUE)
					{
						GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3, SET);
					}
					else
					{
						GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3, RESET);
					}
					break;
				case 11 :
					if(status == TRUE)
					{
						GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_15, RESET);
					}
					else
					{
						GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_15, SET);
					}
					break;
				default :
					break;
			}
		}
	}
	
}

void api_Send_PWM(u8 mFlag)
{
	if(mFlag == TRUE)
	{
		TM_SetCaptureCompare0(HT_SCTM3, 13);
	}
	else
	{
		TM_SetCaptureCompare0(HT_SCTM3, 0);
	}
}

// /*****************************************************************************
//  * @name       :void Check_Drop(u8 position)
//  * @date       :2020-09-19 
//  * @author     :zhengbaiqiu 
//  * @function   :check drop
//  * @parameters :position : ST_FRONT_DROP,               //前下视发射
// 						   ST_LEFT_RIGHT_DROP,          //左、右下视发射
// 						   ST_BACK_DROP                 //后下视发射
//  * @retvalue   :None
// ******************************************************************************/
// void Check_Drop(u8 position)
// {
// 	switch(position)
// 	{
// 		case ST_FRONT_DROP :
// 			if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_2) == 1)
// 			{
// 				mMaininf.mWork.mCheckStatusFlag = 1;
// 				mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
// 				while(api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) < 200)
// 				{
// 					if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_2) == 0)
// 					{
// 						mMaininf.mWork.mCheckStatusFlag = 0;
// 						break;
// 					}
// 				}
// 			}
// 			
// 			if(mMaininf.mWork.mCheckStatusFlag == 1)
// 			{
// 				api_Uart0_SendCMDData(01,01,1);          //检测成功
// 			}
// 			else
// 			{
// 				api_Uart0_SendCMDData(01,01,0);          //检测失败
// 			}
// 			mMaininf.mWork.mCheckStatusFlag = 0;         //未检测成功
// 			break;
// 		case ST_LEFT_RIGHT_DROP :
// 			if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_9) == 1)
// 			{
// 				mMaininf.mWork.mCheckStatusFlag = 1;
// 				mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
// 				while(api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) < 200)
// 				{
// 					if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_9) == 0)
// 					{
// 						mMaininf.mWork.mCheckStatusFlag = 0;
// 						break;
// 					}
// 				}
// 			}
// 			
// 			if(mMaininf.mWork.mCheckStatusFlag == 1)
// 			{
// 				api_Uart0_SendCMDData(02,01,1);          //检测成功
// 			}
// 			else
// 			{
// 				api_Uart0_SendCMDData(02,01,0);          //检测失败
// 			}
// 			mMaininf.mWork.mCheckStatusFlag = 0;         //未检测成功
// 			break;
// 		case ST_BACK_DROP :
// 			if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_15) == 1)
// 			{
// 				mMaininf.mWork.mCheckStatusFlag = 1;
// 				mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
// 				while(api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) < 200)
// 				{
// 					if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_15) == 0)
// 					{
// 						mMaininf.mWork.mCheckStatusFlag = 0;
// 						break;
// 					}
// 				}
// 			}
// 			
// 			if(mMaininf.mWork.mCheckStatusFlag == 1)
// 			{
// 				api_Uart0_SendCMDData(03,01,1);          //检测成功
// 			}
// 			else
// 			{
// 				api_Uart0_SendCMDData(03,01,0);          //检测失败
// 			}
// 			mMaininf.mWork.mCheckStatusFlag = 0;         //未检测成功
// 			break;
// 		default :
// 			break;
// 	}
// 	
// }

/*****************************************************************************
 * @name       :void api_Test_Front_Drop(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :test front drop
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void api_Test_Front_Drop(void)
{
	//按键按下
	if(mMaininf.mWork.mWorkKey == 1)
	{
		mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_2) == 1)         //检测接收端初始状态   前下视
// 		{
// 			Signal_Send(ST_FRONT_DROP,TRUE);                 //发射前下视信号
// 		
// //			Test_Item_Check_Enable(ST_DROP_FRONT,ENABLE);   //打开前下视检测
// 			
// 			mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
// 			
// 			mMaininf.mWork.mReceiveStatus = 1;               //完成检测第一步
// 		}
// 		else
// 		{
// 			mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime() + 60000;
// 		}
		
		mMaininf.mWork.mWorkKey = 0;                     //清除按键状态
		mMaininf.mWork.mWorkChecking = 1;                //正在进行检测
	}
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mReceiveStatus == 0))
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_2) == 1)
		{
			mMaininf.mWork.mDropFrontStatusCont ++;
			
			if(mMaininf.mWork.mDropFrontStatusCont >= 10)
			{
				Signal_Send(ST_FRONT_DROP,TRUE);                 //发射前下视信号
				
//				mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
				
				mMaininf.mWork.mReceiveStatus = 1;
			}
		}
		else
		{
			mMaininf.mWork.mDropFrontStatusCont = 0;
		}
	}
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mReceiveStatus == 1))
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_2) == 0)
		{
			mMaininf.mWork.mFrontDropFlagCont ++;
			
			if(mMaininf.mWork.mFrontDropFlagCont >= 10)
			{
				mMaininf.mWork.mFrontDropFlag = 1;
			}
		}
		else
		{
			mMaininf.mWork.mFrontDropFlagCont = 0;
		}
	}
	
	
	//检测到前下视信号
	if(mMaininf.mWork.mFrontDropFlag == 1)
	{
		Signal_Send(ST_FRONT_DROP,FALSE);                //结束下视发射
//		Test_Item_Check_Enable(TEST_FRONTDROP,DISABLE);  //关闭前下视检测
		api_Uart0_SendCMDData(0x01,0x01,0x01);           //显示前下视检测成功
		mMaininf.mWork.mFrontDropFlag = 0;               //清除触发状态
		mMaininf.mWork.mReceiveStatus = 0;               //清除第一步标志位
		mMaininf.mWork.mWorkChecking = 0;                //退出检测模式
		
		mMaininf.mWork.mDropFrontStatusCont = 0;
		mMaininf.mWork.mFrontDropFlagCont = 0;
	}
	
	//检测超时
	if((mMaininf.mWork.mWorkChecking == 1) && (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) >= 500))    //0.5s  超时
	{
		Signal_Send(ST_FRONT_DROP,FALSE);                //结束下视发射
//		Test_Item_Check_Enable(TEST_FRONTDROP,DISABLE);  //关闭前下视检测
		api_Uart0_SendCMDData(0x01,0x01,0x00);           //显示前下视检测失败
		mMaininf.mWork.mFrontDropFlag = 0;               //清除触发状态
		mMaininf.mWork.mReceiveStatus = 0;               //清除第一步标志位
		mMaininf.mWork.mWorkChecking = 0;                //退出检测模式
		
		mMaininf.mWork.mDropFrontStatusCont = 0;
		mMaininf.mWork.mFrontDropFlagCont = 0;
	}
	
}

/*****************************************************************************
 * @name       :void api_Test_Wheel_Wire(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :test wheel wire
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void api_Test_Wheel_Wire(void)
{
//	u8 zbqt = 0;
//	zbqt = GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_5);      //碰撞
//	zbqt = GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_12);     //码盘
	//按键按下
	if(mMaininf.mWork.mWorkKey == 1)
	{
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_9) == 1)    //左右下视
// 		{
// 			Test_Item_Check_Enable(ST_DROP_LEFT_RIGHT,ENABLE);
// 			
// 			Signal_Send(ST_LEFT_RIGHT_DROP,TRUE);        //发射左、右下视信号发射
// 			
// 			mMaininf.mWork.mReceiveStatus = 1;           //完成检测第一步  下视
// 		}
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_12) == 1)   //左右码盘
// 		{
// 			Test_Item_Check_Enable(ST_CODE_LEFT_RIGHT,ENABLE);
// 			
// 			Signal_Send(ST_CODE,TRUE);                  //发射左右码盘信号
// 			
// 			mMaininf.mWork.mCodeReceiveStatus = 1;       //完成检测第一步  码盘
// 			
// //			zbqt = GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_12);
// 		}
		
		Test_Item_Check_Enable(ST_BUMP_LEFT_RIGHT | ST_LEAVE_LEFT_RIGHT,ENABLE);
		
		mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
		
		
// 		if((GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_9) == 1) && (GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_12) == 1))
// 		{
// 			Signal_Send(ST_LEFT_RIGHT_DROP | ST_CODE,TRUE);        //发射左、右下视信号发射
// 			
// 			Test_Item_Check_Enable(ST_DROP_LEFT_RIGHT | ST_BUMP_LEFT_RIGHT | ST_CODE_LEFT_RIGHT | ST_LEAVE_LEFT_RIGHT,ENABLE);             //打开大轮线束检测
// 			
// 			mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
// 			
// 			mMaininf.mWork.mReceiveStatus = 1;           //完成检测第一步  下视
// 			mMaininf.mWork.mCodeReceiveStatus = 1;       //完成检测第一步  码盘
// 		}
// 		else
// 		{
// 			if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_9) == 1)
// 			{
// 				
// 			}
// 			mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime() + 60000;
// 		}
		
		
		mMaininf.mWork.mWorkKey = 0;                     //清除按键状态
		mMaininf.mWork.mWorkChecking = 1;                //正在进行检测
	}
	
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mReceiveStatus == 0))    //检测左右下视初始状态
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_9) == 1)    //左右下视
		{
			mMaininf.mWork.mDropLeftRightStatusCont ++;
			
			if(mMaininf.mWork.mDropLeftRightStatusCont >= 10)
			{
				Test_Item_Check_Enable(ST_DROP_LEFT_RIGHT,ENABLE);
			
				Signal_Send(ST_LEFT_RIGHT_DROP,TRUE);        //发射左、右下视信号发射
				
				mMaininf.mWork.mReceiveStatus = 1;           //完成检测第一步  下视
			}
		}
		else
		{
			mMaininf.mWork.mDropLeftRightStatusCont = 0;
		}
	}
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mCodeReceiveStatus == 0))    //检测左右码盘初始状态
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_12) == 1)   //左右码盘
		{
			mMaininf.mWork.mCodeLeftRightStatusCont ++;
			
			if(mMaininf.mWork.mCodeLeftRightStatusCont >= 10)
			{
				Test_Item_Check_Enable(ST_CODE_LEFT_RIGHT,ENABLE);
			
				Signal_Send(ST_CODE,TRUE);                  //发射左右码盘信号
				
				mMaininf.mWork.mCodeReceiveStatus = 1;       //完成检测第一步  码盘
			}
		}
		else
		{
			mMaininf.mWork.mCodeLeftRightStatusCont = 0;
		}
	}
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mBumpStatus == 0))    //检测左右碰撞
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_5) == 1)   //碰撞
		{
			mMaininf.mWork.mBumpStatusCont ++;
			
			if(mMaininf.mWork.mBumpStatusCont >= 10)
			{
				Signal_Send(ST_BUMP_DUSTBOX,TRUE);      

				mMaininf.mWork.mBumpStatus = 1;
			}
		}
		else
		{
			mMaininf.mWork.mBumpStatusCont = 0;
		}
	}
	
	
	
	
	
	
	
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mReceiveStatus == 1))
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_9) == 0)
		{
			mMaininf.mWork.mLeftRightDropFlagCont ++;
			
			if(mMaininf.mWork.mLeftRightDropFlagCont >= 10)
			{
				mMaininf.mWork.mLeftRightDropFlag = 1;
			}
		}
		else
		{
			mMaininf.mWork.mLeftRightDropFlagCont = 0;
		}
	}
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mCodeReceiveStatus == 1))
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_12) == 0)
		{
			mMaininf.mWork.mLeftRightCodeFlagCont ++;
			
			if(mMaininf.mWork.mLeftRightCodeFlagCont >= 10)
			{
				mMaininf.mWork.mLeftRightCodeFlag = 1;
			}
		}
		else
		{
			mMaininf.mWork.mLeftRightCodeFlagCont = 0;
		}
	}
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mBumpStatus == 1))    //检测左右碰撞
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_5) == 0)   //碰撞
		{
			mMaininf.mWork.mLeftRightBumpFlagCont ++;
			
			if(mMaininf.mWork.mLeftRightBumpFlagCont >= 10)
			{
				mMaininf.mWork.mLeftRightBumpFlag = 1;
			}
		}
		else
		{
			mMaininf.mWork.mLeftRightBumpFlagCont = 0;
		}
	}
	
	if(mMaininf.mWork.mWorkChecking == 1)
	{
		if(GPIO_ReadInBit(HT_GPIOA,GPIO_PIN_8) == 0)
		{
			mMaininf.mWork.mLeftRightLeaveFlagCont ++;
			
			if(mMaininf.mWork.mLeftRightLeaveFlagCont >= 10)
			{
				mMaininf.mWork.mLeftRightLeaveFlag = 1;
			}
		}
		else
		{
			mMaininf.mWork.mLeftRightLeaveFlagCont = 0;
		}
	}
	
	
	
// 	if((mMaininf.mWork.mReceiveStatus == 1) && (GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_9) == 0))
// 	{
// 		mMaininf.mWork.mLeftRightDropFlag = 1;
// 	}
// 	if((mMaininf.mWork.mCodeReceiveStatus == 1) && (GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_12) == 0))
// 	{
// 		mMaininf.mWork.mLeftRightCodeFlag = 1;
// 	}
	
// 	if((mMaininf.mWork.mWorkChecking == 1) && (GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_5) == 1))     //碰撞
// 	{
// 		mMaininf.mWork.mLeftRightBumpFlag = 1;
// 	}
	
// 	if((mMaininf.mWork.mWorkChecking == 1) && (GPIO_ReadInBit(HT_GPIOA,GPIO_PIN_8) == 0))     //离地
// 	{
// 		mMaininf.mWork.mLeftRightLeaveFlag = 1;
// 	}
	
	//如果全部检测成功
	if((mMaininf.mWork.mLeftRightDropFlag == 1) && (mMaininf.mWork.mLeftRightBumpFlag == 1) && 
	   (mMaininf.mWork.mLeftRightCodeFlag == 1) && (mMaininf.mWork.mLeftRightLeaveFlag == 1))
	{
		Signal_Send(ST_LEFT_RIGHT_DROP | ST_CODE | ST_BUMP_DUSTBOX,FALSE); //结束左、右下视  码盘检测
		Test_Item_Check_Enable(ST_DROP_LEFT_RIGHT | ST_BUMP_LEFT_RIGHT | ST_CODE_LEFT_RIGHT | ST_LEAVE_LEFT_RIGHT,DISABLE);      //关闭大轮线束检测
		api_Uart0_SendCMDData(0x02,0x10,0x01);           //显示检测成功项
		mMaininf.mWork.mLeftRightDropFlag = 0;           //清除触发状态
		mMaininf.mWork.mLeftRightBumpFlag = 0;
		mMaininf.mWork.mLeftRightCodeFlag = 0;
		mMaininf.mWork.mLeftRightLeaveFlag = 0;
		
		mMaininf.mWork.mReceiveStatus = 0;               //清除第一步标志位
		mMaininf.mWork.mCodeReceiveStatus = 0;           //清除码盘第一步标志位
		mMaininf.mWork.mBumpStatus = 0;
		mMaininf.mWork.mWorkChecking = 0;                //退出检测模式
		
		mMaininf.mWork.mDropLeftRightStatusCont = 0;
		mMaininf.mWork.mCodeLeftRightStatusCont = 0;
		mMaininf.mWork.mBumpStatusCont = 0;
		
		mMaininf.mWork.mLeftRightDropFlagCont = 0;
		mMaininf.mWork.mLeftRightCodeFlagCont = 0;
		mMaininf.mWork.mLeftRightLeaveFlagCont = 0;
		mMaininf.mWork.mLeftRightBumpFlagCont = 0;
	}
	
	//检测超时
	if((mMaininf.mWork.mWorkChecking == 1) && (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) >= 5000))    //5s  超时
	{
		Signal_Send(ST_LEFT_RIGHT_DROP | ST_CODE | ST_BUMP_DUSTBOX,FALSE); //结束左、右下视  码盘检测
		Test_Item_Check_Enable(ST_DROP_LEFT_RIGHT | ST_BUMP_LEFT_RIGHT | ST_CODE_LEFT_RIGHT | ST_LEAVE_LEFT_RIGHT,DISABLE);  //关闭大轮线束检测
		
		//检测到左、右下视信号
		if(mMaininf.mWork.mLeftRightDropFlag == 1)
		{
			api_Uart0_SendCMDData(0x02,0x01,0x01);       //检测左、右下视成功
		}
		else
		{
			api_Uart0_SendCMDData(0x02,0x01,0x00);       //检测左、右下视失败
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//检测到左、右碰撞信号
		if(mMaininf.mWork.mLeftRightBumpFlag == 1)
		{
			api_Uart0_SendCMDData(0x02,0x02,0x01);       //检测左、右碰撞成功
		}
		else
		{
			api_Uart0_SendCMDData(0x02,0x02,0x00);       //检测左、右碰撞失败
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//检测到左、右码盘信号
		if(mMaininf.mWork.mLeftRightCodeFlag == 1)
		{
			api_Uart0_SendCMDData(0x02,0x03,0x01);       //检测左、右码盘成功
		}
		else
		{
			api_Uart0_SendCMDData(0x02,0x03,0x00);       //检测左、右码盘失败
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//检测到左、右离地信号
		if(mMaininf.mWork.mLeftRightLeaveFlag == 1)
		{
			api_Uart0_SendCMDData(0x02,0x04,0x01);       //检测左、右离地成功
		}
		else
		{
			api_Uart0_SendCMDData(0x02,0x04,0x00);       //检测左、右离地失败
		}
		
		mMaininf.mWork.mLeftRightDropFlag = 0;           //清除触发状态
		mMaininf.mWork.mLeftRightBumpFlag = 0;
		mMaininf.mWork.mLeftRightCodeFlag = 0;
		mMaininf.mWork.mLeftRightLeaveFlag = 0;
		
		mMaininf.mWork.mReceiveStatus = 0;               //清除第一步标志位
		mMaininf.mWork.mCodeReceiveStatus = 0;           //清除码盘第一步标志位
		mMaininf.mWork.mBumpStatus = 0;
		mMaininf.mWork.mWorkChecking = 0; 
		
		mMaininf.mWork.mDropLeftRightStatusCont = 0;
		mMaininf.mWork.mCodeLeftRightStatusCont = 0;
		mMaininf.mWork.mBumpStatusCont = 0;
		
		mMaininf.mWork.mLeftRightDropFlagCont = 0;
		mMaininf.mWork.mLeftRightCodeFlagCont = 0;
		mMaininf.mWork.mLeftRightLeaveFlagCont = 0;
		mMaininf.mWork.mLeftRightBumpFlagCont = 0;
	}
}

/*****************************************************************************
 * @name       :void api_Test_Key_Wire(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Test key wire
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void api_Test_Key_Wire(void)
{
	//按键按下
	if(mMaininf.mWork.mWorkKey == 1)
	{
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_15) == 1)    //后下视
// 		{
// 			Test_Item_Check_Enable(ST_DROP_BACK,ENABLE);
// 			
// 			Signal_Send(ST_BACK_DROP,TRUE);              //发射左、右下视信号发射
// 			
// 			mMaininf.mWork.mReceiveStatus = 1;           //完成检测第一步  下视
// 		}
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_6) == 1)     //后引导
// 		{
// 			PWM_Configuration(TRUE);
// 			api_Send_PWM(TRUE);
// 			
// 			mMaininf.mWork.mGuideBackStatus = 1;
// 		}
		
		Test_Item_Check_Enable(ST_POWER_KEY | ST_GUIDE_BACK,ENABLE);
		mMaininf.mWork.mGuideSendFlag = 1;
		
		
// 		PWM_Configuration(TRUE);
// 		api_Send_PWM(TRUE);
		
		mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
		
		
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_15) == 1)
// 		{
// 			Signal_Send(ST_BACK_DROP,TRUE);                        //发射后下视
// 		
// 			Test_Item_Check_Enable(TEST_KEY,ENABLE);               //打开后组合线束
// 			
// 			mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
// 		}
// 		else
// 		{
// 			mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime() + 60000;
// 		}
		
		mMaininf.mWork.mWorkKey = 0;                     //清除按键状态
		mMaininf.mWork.mWorkChecking = 1;                //正在进行检测
	}
	
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mReceiveStatus == 0))
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_15) == 1)    //后下视
		{
			mMaininf.mWork.mDropBackStatusCont ++;
			
			if(mMaininf.mWork.mDropBackStatusCont >= 10)
			{
				Test_Item_Check_Enable(ST_DROP_BACK,ENABLE);
			
				Signal_Send(ST_BACK_DROP,TRUE);              //发射左、右下视信号发射
				
				mMaininf.mWork.mReceiveStatus = 1;           //完成检测第一步  下视
			}
		}
		else
		{
			mMaininf.mWork.mDropBackStatusCont = 0;
		}
	}
	
	
	
	
	
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mReceiveStatus == 1))
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_15) == 0)
		{
			mMaininf.mWork.mBackDropFlagCont ++;
			
			if(mMaininf.mWork.mBackDropFlagCont >= 10)
			{
				mMaininf.mWork.mBackDropFlag = 1;
			}
		}
		else
		{
			mMaininf.mWork.mBackDropFlagCont = 0;
		}
	}
	
// 	if((mMaininf.mWork.mReceiveStatus == 1) && (GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_15) == 0))
// 	{
// 		mMaininf.mWork.mBackDropFlag = 1;
// 	}
	
// 	if(mMaininf.mWork.mGuideBackStatus == 1)
// 	{
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_6) == 0)
// 		{
// 			if(mMaininf.mWork.mWorkChecking == 1)
// 			{
// 				mMaininf.mWork.mGuideCont ++;
// 			}
// 			if(mMaininf.mWork.mGuideCont > 10)
// 			{
// 				mMaininf.mWork.mBackGuideFlag = 1;
// 			}
// 		}
// 		else
// 		{
// 			mMaininf.mWork.mGuideCont = 0;
// 		}
// 	}
	
// 	mMaininf.mSystem.mKeyStartTime = api_GetCurrentSystemTime();
// 	mMaininf.mWork.mPowerKeyFlag = 1;
// 	while((mMaininf.mWork.mPowerKeyFlag == 0) && (api_GetSystemTimePeriod(mMaininf.mSystem.mKeyStartTime) < 20))
// 	{
// 		if(GPIO_ReadInBit(HT_GPIOA,GPIO_PIN_11) == 1)
// 		{
// 			mMaininf.mWork.mPowerKeyFlag = 0;      //按键检测失败
// 		}
// 	}

	if(mMaininf.mWork.mWorkChecking == 1)
	{
		if(GPIO_ReadInBit(HT_GPIOA,GPIO_PIN_11) == 0)
		{
			mMaininf.mWork.mKeyCont ++;
			
			if(mMaininf.mWork.mKeyCont >= 10)
			{
				mMaininf.mWork.mPowerKeyFlag = 1;
			}
		}
		else
		{
			mMaininf.mWork.mKeyCont = 0;
		}
	}
	
	
// 	if(GPIO_ReadInBit(HT_GPIOA,GPIO_PIN_11) == 0)
// 	{
// 		if(mMaininf.mWork.mWorkChecking == 1)
// 		{
// 			mMaininf.mWork.mKeyCont ++;
// 		}
// 		if(mMaininf.mWork.mKeyCont > 10)
// 		{
// 			mMaininf.mWork.mPowerKeyFlag = 1;
// 		}
// 	}
// 	else
// 	{
// 		mMaininf.mWork.mKeyCont = 0;
// 	}
	
	
	//如果全部检测成功
	if((mMaininf.mWork.mBackDropFlag == 1) && (mMaininf.mWork.mBackGuideFlag == 1) && 
	   (mMaininf.mWork.mPowerKeyFlag == 1))
	{
		mMaininf.mWork.mGuideSendFlag = 0;
		mMaininf.mWork.mGuideSendChangeFlag = 0;
		
		Signal_Send(ST_BACK_DROP,FALSE);                 //结束左、右下视  码盘检测
		PWM_Configuration(FALSE);
		api_Send_PWM(FALSE);
		Test_Item_Check_Enable(ST_DROP_BACK | ST_POWER_KEY | ST_GUIDE_BACK,DISABLE);        //关闭大轮线束检测
		api_Uart0_SendCMDData(0x03,0x10,0x01);           //显示检测成功项
		mMaininf.mWork.mBackDropFlag = 0;                //清除触发状态
		mMaininf.mWork.mBackGuideFlag = 0;
		mMaininf.mWork.mPowerKeyFlag = 0;
		
		mMaininf.mWork.mReceiveStatus = 0;               //清除第一步标志位
		mMaininf.mWork.mWorkChecking = 0; 
		mMaininf.mWork.mGuideBackStatus = 0;             //检查引导第一次触发
		mMaininf.mWork.mKeyCont = 0;
		mMaininf.mWork.mGuideCont = 0;
		
		mMaininf.mSystem.mGuideStartTimeT1 = 0;
		mMaininf.mSystem.mGuideStartTimeT2 = 0;
		mMaininf.mSystem.mGuideStartTimeT3 = 0;
		
		mMaininf.mWork.mDropBackStatusCont = 0;
		mMaininf.mWork.mBackDropFlagCont = 0;
	}
	
	//检测超时
	if((mMaininf.mWork.mWorkChecking == 1) && (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) >= 1000))    //1s  超时
	{
		mMaininf.mWork.mGuideSendFlag = 0;
		mMaininf.mWork.mGuideSendChangeFlag = 0;
		
		Signal_Send(ST_BACK_DROP,FALSE);                 //结束左、右下视  码盘检测
		PWM_Configuration(FALSE);
		api_Send_PWM(FALSE);
		Test_Item_Check_Enable(ST_DROP_BACK | ST_POWER_KEY | ST_GUIDE_BACK,DISABLE);        //关闭大轮线束检测
		
		//检测后下视
		if(mMaininf.mWork.mBackDropFlag == 1)
		{
			api_Uart0_SendCMDData(0x03,0x01,0x01);       //检测后下视成功
		}
		else
		{
			api_Uart0_SendCMDData(0x03,0x01,0x00);       //检测后下视失败
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//检测后引导
		if(mMaininf.mWork.mBackGuideFlag == 1)
		{
			api_Uart0_SendCMDData(0x03,0x05,0x01);       //检测后引导成功
		}
		else
		{
			api_Uart0_SendCMDData(0x03,0x05,0x00);       //检测后引导失败
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//检测按键开关
		if(mMaininf.mWork.mPowerKeyFlag == 1)
		{
			api_Uart0_SendCMDData(0x03,0x06,0x01);       //检测后引导成功
		}
		else
		{
			api_Uart0_SendCMDData(0x03,0x06,0x00);       //检测后引导失败
		}
		
		mMaininf.mWork.mBackDropFlag = 0;                //清除触发状态
		mMaininf.mWork.mBackGuideFlag = 0;
		mMaininf.mWork.mPowerKeyFlag = 0;
		
		mMaininf.mWork.mReceiveStatus = 0;               //清除第一步标志位
		mMaininf.mWork.mWorkChecking = 0; 
		mMaininf.mWork.mGuideBackStatus = 0;             //检查引导第一次触发
		mMaininf.mWork.mKeyCont = 0;
		mMaininf.mWork.mGuideCont = 0;
		
		mMaininf.mSystem.mGuideStartTimeT1 = 0;
		mMaininf.mSystem.mGuideStartTimeT2 = 0;
		mMaininf.mSystem.mGuideStartTimeT3 = 0;
		
		mMaininf.mWork.mDropBackStatusCont = 0;
		mMaininf.mWork.mBackDropFlagCont = 0;
	}
}

/*****************************************************************************
 * @name       :void api_Test_Dust_Component(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Test dust
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void api_Test_Dust_Component(void)
{
// 	//按键按下
// 	if(mMaininf.mWork.mWorkKey == 1)
// 	{
// 		
// 		Test_Item_Check_Enable(ST_DUST_BOX,ENABLE);
// 		
// 		mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
// 		
// 		mMaininf.mWork.mWorkKey = 0;                     //清除按键状态
// 		mMaininf.mWork.mWorkChecking = 1;                //正在进行检测
// 	}
	//尘盒磁铁靠近开始检测
	if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_10) == 1)
	{
		mMaininf.mWork.mDustBoxFlagCont ++;
		
		if(mMaininf.mWork.mDustBoxFlagCont >= 10)
		{
			mMaininf.mWork.mDustBoxFlagCont = 10;
			if(mMaininf.mWork.mDustBoxHightFlag == 1)
			{
				if(mMaininf.mWork.mDustBoxUartSendFlag == 0)
				{
					api_Uart0_SendCMDData(0,0,1);
					mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
					while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 3);
					mMaininf.mWork.mDustBoxUartSendFlag = 1;
					mMaininf.mWork.mDustBoxFlag = 1;
					mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
					mMaininf.mWork.mWorkChecking = 1;                //正在进行检测
				}
			}
		}
		
		mMaininf.mWork.mDustBoxHightFlagCont = 0;
	}
	else
	{
		mMaininf.mWork.mDustBoxHightFlagCont ++;
		
		if(mMaininf.mWork.mDustBoxHightFlagCont >= 10)
		{
			mMaininf.mWork.mDustBoxHightFlagCont = 10;
			mMaininf.mWork.mDustBoxHightFlag = 1;
		}
		
		mMaininf.mWork.mDustBoxFlagCont = 0;
	}
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mDust3V3Flag == 0))
	{
		if(GPIO_ReadInBit(HT_GPIOA,GPIO_PIN_6) == 1)
		{
			mMaininf.mWork.mDustStatusCont ++;
			
			if(mMaininf.mWork.mDustStatusCont >= 10)
			{
				Signal_Send(ST_DUST,TRUE);                             //发射灰尘
			
				mMaininf.mWork.mDust3V3Flag = 1;                       //灰尘3.3V触发
			}
		}
		else
		{
			mMaininf.mWork.mDustStatusCont = 0;
		}
	}
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mDust3V3Flag == 1))
	{
		if(GPIO_ReadInBit(HT_GPIOA,GPIO_PIN_6) == 0)
		{
			mMaininf.mWork.mDustFlagCont ++;
			
			if(mMaininf.mWork.mDustFlagCont >= 10)
			{
				mMaininf.mWork.mDustFlag = 1;
			}
		}
		else
		{
			mMaininf.mWork.mDustFlagCont = 0;
		}
	}
	
// //	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mDustBoxStatus == 1))
// 	if(mMaininf.mWork.mWorkChecking == 1)
// 	{
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_10) == 0)
// 		{
// 			mMaininf.mWork.mDustBoxFlagCont ++;
// 			
// 			if(mMaininf.mWork.mDustBoxFlagCont >= 10)
// 			{
// 				mMaininf.mWork.mDustBoxFlag = 1;
// 			}
// 		}
// 		else
// 		{
// 			mMaininf.mWork.mDustBoxFlagCont = 0;
// 		}
// 	}
	
	if((mMaininf.mWork.mDustFlag == 1) && (mMaininf.mWork.mDustBoxFlag == 1))
	{
		Signal_Send(ST_DUST | ST_BUMP_DUSTBOX,FALSE);                      //关闭灰尘发射
		Test_Item_Check_Enable(ST_DUST_CHECK | ST_DUST_BOX,DISABLE);       //关闭尘盒组件
		api_Uart0_SendCMDData(0x04,0x10,0x01);           //显示检测成功项
		mMaininf.mWork.mDustBoxFlag = 0;                 //清除触发状态
		mMaininf.mWork.mDust3V3Flag = 0;
		mMaininf.mWork.mDustFlag = 0;
		
		mMaininf.mWork.mDustBoxHightFlag = 0;
		
		mMaininf.mWork.mDustBoxStatus = 0;
		
		mMaininf.mWork.mWorkChecking = 0; 
		
		mMaininf.mWork.mDustStatusCont = 0;
		mMaininf.mWork.mDustBoxStatusCont = 0;
		mMaininf.mWork.mDustFlagCont = 0;
		mMaininf.mWork.mDustBoxFlagCont = 0;
		
		mMaininf.mWork.mDustBoxUartSendFlag = 0;
	}
	
	//检测超时
	if((mMaininf.mWork.mWorkChecking == 1) && (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) >= 3000))    //3s  超时
	{
		Signal_Send(ST_DUST | ST_BUMP_DUSTBOX,FALSE);                      //关闭灰尘发射
		Test_Item_Check_Enable(ST_DUST_CHECK | ST_DUST_BOX,DISABLE);       //关闭尘盒组件
		
		//灰尘传感器
		if(mMaininf.mWork.mDustFlag == 1)
		{
			api_Uart0_SendCMDData(0x04,0x07,0x01);           //显示检测成功项
		}
		else
		{
			api_Uart0_SendCMDData(0x04,0x07,0x00);           //显示检测失败项
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//尘盒霍尔
		if(mMaininf.mWork.mDustBoxFlag == 1)
		{
			api_Uart0_SendCMDData(0x04,0x08,0x01);           //显示检测成功项
		}
		else
		{
			api_Uart0_SendCMDData(0x04,0x08,0x00);           //显示检测成功项
		}
		
		mMaininf.mWork.mDustBoxFlag = 0;                //清除触发状态
		mMaininf.mWork.mDust3V3Flag = 0;
		mMaininf.mWork.mDustFlag = 0;
		
		mMaininf.mWork.mDustBoxHightFlag = 0;
		
		mMaininf.mWork.mDustBoxStatus = 0;
		
		mMaininf.mWork.mWorkChecking = 0; 
		
		mMaininf.mWork.mDustStatusCont = 0;
		mMaininf.mWork.mDustBoxStatusCont = 0;
		mMaininf.mWork.mDustFlagCont = 0;
		mMaininf.mWork.mDustBoxFlagCont = 0;
		
		mMaininf.mWork.mDustBoxUartSendFlag = 0;
	}
}

/*****************************************************************************
 * @name       :void api_Test_Charge_Guide(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Test charge guide
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void api_Test_Charge_Guide(void)
{
// 	Signal_Send(ST_CHARGE_A,TRUE);
// 	Signal_Send(ST_CHARGE_B,TRUE);
// 	Signal_Send(ST_CHARGE_TOP,TRUE);
	
	//按键按下
	if(mMaininf.mWork.mWorkKey == 1)
	{
		Test_Item_Check_Enable(ST_CHARGE_BASE,ENABLE);
		
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 30);
		
// 		if(mMaininf.mWork.mSensorADC[0] <= 372)          //充电座A电压小于0.3V
// 		{
// //			Signal_Send(ST_CHARGE_A,TRUE);
// 			
// 			mMaininf.mWork.mChargeBaseGuideAStatus = 1;    //完成检测第一步  充电座引导A
// 		}
// 		if(mMaininf.mWork.mSensorADC[1] <= 372)          //充电座B电压小于0.3V
// 		{
// //			Signal_Send(ST_CHARGE_B,TRUE);
// 			
// 			mMaininf.mWork.mChargeBaseGuideBStatus = 1;    //完成检测第一步  充电座引导B
// 		}
// 		if(mMaininf.mWork.mSensorADC[2] <= 372)          //充电座TOP电压小于0.3V
// 		{
// //			Signal_Send(ST_CHARGE_TOP,TRUE);
// 			
// 			mMaininf.mWork.mChargeBaseGuideTOPStatus = 1;    //完成检测第一步  充电座引导B
// 		}
		
		mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
		
		mMaininf.mWork.mWorkKey = 0;                     //清除按键状态
		mMaininf.mWork.mWorkChecking = 1;                //正在进行检测
	}
	
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mChargeBaseGuideAStatus == 0) && 
	   (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) <= 100))
	{
		if(mMaininf.mWork.mSensorADC[0] <= 372)          //充电座A电压小于0.3V
		{
			mMaininf.mWork.mChargeBaseGuideAStatusCont ++;
			
			if(mMaininf.mWork.mChargeBaseGuideAStatusCont >= 10)
			{
				mMaininf.mWork.mChargeBaseGuideAStatus = 1;    //完成检测第一步  充电座引导A
			}
		}
		else
		{
			mMaininf.mWork.mChargeBaseGuideAStatusCont = 0;
		}
	}
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mChargeBaseGuideBStatus == 0) && 
	   (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) <= 100))
	{
		if(mMaininf.mWork.mSensorADC[1] <= 372)          //充电座B电压小于0.3V
		{
			mMaininf.mWork.mChargeBaseGuideBStatusCont ++;
			
			if(mMaininf.mWork.mChargeBaseGuideBStatusCont >= 10)
			{
				mMaininf.mWork.mChargeBaseGuideBStatus = 1;    //完成检测第一步  充电座引导B
			}
		}
		else
		{
			mMaininf.mWork.mChargeBaseGuideBStatusCont = 0;
		}
	}
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mChargeBaseGuideTOPStatus == 0) && 
	   (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) <= 100))
	{
		if(mMaininf.mWork.mSensorADC[2] <= 372)          //充电座TOP电压小于0.3V
		{
			mMaininf.mWork.mChargeBaseGuideTOPStatusCont ++;
			
			if(mMaininf.mWork.mChargeBaseGuideTOPStatusCont >= 10)
			{
				mMaininf.mWork.mChargeBaseGuideTOPStatus = 1;    //完成检测第一步  充电座引导TOP
			}
		}
		else
		{
			mMaininf.mWork.mChargeBaseGuideTOPStatusCont = 0;
		}
	}
	
	
	
	
	
	
	
	
	if(mMaininf.mWork.mWorkChecking == 1)
	{
		if((mMaininf.mWork.mSensorADC[3] > 1861) && (mMaininf.mWork.mSensorADC[3] < 2481))
		{
			mMaininf.mWork.mWorkLedFlagCont ++;
			
			if(mMaininf.mWork.mWorkLedFlagCont >= 10)
			{
				mMaininf.mWork.mWorkLedFlag = 1;
			}
		}
		else
		{
			mMaininf.mWork.mWorkLedFlagCont = 0;
		}
	}
	
	
// 	if((mMaininf.mWork.mSensorADC[3] > 1861) && (mMaininf.mWork.mSensorADC[3] < 2481))
// 	{
// 		mMaininf.mWork.mWorkLedFlag = 1;
// 	}
	
	if((mMaininf.mWork.mWorkChecking == 1) && (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) > 100) && 
	   (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) <= 200))
	{
		Signal_Send(ST_CHARGE_B | ST_CHARGE_TOP,FALSE);
		Signal_Send(ST_CHARGE_A,TRUE);
		mMaininf.mWork.mChargeBaseGuideFlag = 1;
	}
	else if((mMaininf.mWork.mWorkChecking == 1) && (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) > 200) && 
			(api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) <= 300))
	{
		Signal_Send(ST_CHARGE_A | ST_CHARGE_TOP,FALSE);
		Signal_Send(ST_CHARGE_B,TRUE);
		mMaininf.mWork.mChargeBaseGuideFlag = 2;
	}
	if((mMaininf.mWork.mWorkChecking == 1) && (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) > 300) && 
	   (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) <= 400))
	{
		Signal_Send(ST_CHARGE_A | ST_CHARGE_B,FALSE);
		Signal_Send(ST_CHARGE_TOP,TRUE);
		mMaininf.mWork.mChargeBaseGuideFlag = 3;
	}
	
	
	
	
	
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mChargeBaseGuideAStatus == 1))
	{
		if((mMaininf.mWork.mChargeBaseGuideFlag == 1) && (mMaininf.mWork.mSensorADC[0] > 1116) && 
		   (mMaininf.mWork.mSensorADC[0] < 1861) && (mMaininf.mWork.mSensorADC[1] <= 372) && 
		   (mMaininf.mWork.mSensorADC[2] <= 372))
		{
			mMaininf.mWork.mChargeBaseGuideAFlagCont ++;
			
			if(mMaininf.mWork.mChargeBaseGuideAFlagCont >= 10)
			{
				mMaininf.mWork.mChargeBaseGuideAFlag = 1;
			}
		}
		else
		{
			mMaininf.mWork.mChargeBaseGuideAFlagCont = 0;
		}
	}
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mChargeBaseGuideBStatus == 1))
	{
		if((mMaininf.mWork.mChargeBaseGuideFlag == 2) && (mMaininf.mWork.mSensorADC[1] > 1116) && 
		   (mMaininf.mWork.mSensorADC[1] < 1861) && (mMaininf.mWork.mSensorADC[0] <= 372) && 
		   (mMaininf.mWork.mSensorADC[2] <= 372))
		{
			mMaininf.mWork.mChargeBaseGuideBFlagCont ++;
			
			if(mMaininf.mWork.mChargeBaseGuideBFlagCont >= 10)
			{
				mMaininf.mWork.mChargeBaseGuideBFlag = 1;
			}
		}
		else
		{
			mMaininf.mWork.mChargeBaseGuideBFlagCont = 0;
		}
	}
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mChargeBaseGuideTOPStatus == 1))
	{
		if((mMaininf.mWork.mChargeBaseGuideFlag == 3) && (mMaininf.mWork.mSensorADC[2] > 1116) && 
		   (mMaininf.mWork.mSensorADC[2] < 1861) && (mMaininf.mWork.mSensorADC[0] <= 372) &&
		   (mMaininf.mWork.mSensorADC[1] <= 372))
		{
			mMaininf.mWork.mChargeBaseGuideTopFlagCont ++;
			
			if(mMaininf.mWork.mChargeBaseGuideTopFlagCont >= 10)
			{
				mMaininf.mWork.mChargeBaseGuideTopFlag = 1;
			}
		}
		else
		{
			mMaininf.mWork.mChargeBaseGuideTopFlagCont = 0;
		}
	}
	
// 	if((mMaininf.mWork.mChargeBaseGuideAStatus == 1) && (mMaininf.mWork.mSensorADC[0] > 1116) && (mMaininf.mWork.mSensorADC[0] < 1861))
// 	{
// 		mMaininf.mWork.mChargeBaseGuideAFlag = 1;
// 	}
	
// 	if((mMaininf.mWork.mChargeBaseGuideBStatus == 1) && (mMaininf.mWork.mSensorADC[1] > 1116) && (mMaininf.mWork.mSensorADC[1] < 1861))
// 	{
// 		mMaininf.mWork.mChargeBaseGuideBFlag = 1;
// 	}
	
// 	if((mMaininf.mWork.mChargeBaseGuideTOPStatus == 1) && (mMaininf.mWork.mSensorADC[2] > 1116) && (mMaininf.mWork.mSensorADC[2] < 1861))
// 	{
// 		mMaininf.mWork.mChargeBaseGuideTopFlag = 1;
// 	}
	
	if((mMaininf.mWork.mWorkLedFlag == 1) && (mMaininf.mWork.mChargeBaseGuideAFlag == 1) && 
	   (mMaininf.mWork.mChargeBaseGuideBFlag == 1) && (mMaininf.mWork.mChargeBaseGuideTopFlag == 1))
	{
		Test_Item_Check_Enable(ST_CHARGE_BASE,DISABLE);
		Signal_Send(ST_CHARGE_A | ST_CHARGE_B | ST_CHARGE_TOP,FALSE);
		api_Uart0_SendCMDData(0x05,0x10,0x01);           //显示检测成功项
		mMaininf.mWork.mWorkLedFlag = 0;                //清除触发状态
		mMaininf.mWork.mChargeBaseGuideAFlag = 0;
		mMaininf.mWork.mChargeBaseGuideBFlag = 0;
		mMaininf.mWork.mChargeBaseGuideTopFlag = 0;
		
		mMaininf.mWork.mChargeBaseGuideAStatus = 0;               //清除第一步标志位
		mMaininf.mWork.mChargeBaseGuideBStatus = 0; 
		mMaininf.mWork.mChargeBaseGuideTOPStatus = 0;             //检查引导第一次触发
		mMaininf.mWork.mWorkChecking = 0;
		
		mMaininf.mWork.mChargeBaseGuideAStatusCont = 0;
		mMaininf.mWork.mChargeBaseGuideBStatusCont = 0;
		mMaininf.mWork.mChargeBaseGuideTOPStatusCont = 0;
		
		mMaininf.mWork.mWorkLedFlagCont = 0;
		mMaininf.mWork.mChargeBaseGuideAFlagCont = 0;
		mMaininf.mWork.mChargeBaseGuideBFlagCont = 0;
		mMaininf.mWork.mChargeBaseGuideTopFlagCont = 0;
		mMaininf.mWork.mChargeBaseGuideFlag = 0;
	}
	
	//检测超时
	if((mMaininf.mWork.mWorkChecking == 1) && (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) >= 1000))    //1s  超时
	{
		Test_Item_Check_Enable(ST_CHARGE_BASE,DISABLE);
		Signal_Send(ST_CHARGE_A | ST_CHARGE_B | ST_CHARGE_TOP,FALSE);
		
		//充电座工作灯
		if(mMaininf.mWork.mWorkLedFlag == 1)
		{
			api_Uart0_SendCMDData(0x05,0x09,0x01);       //充电座工作灯成功
		}
		else
		{
			api_Uart0_SendCMDData(0x05,0x09,0x00);       
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//充电座引导A
		if(mMaininf.mWork.mChargeBaseGuideAFlag == 1)
		{
			api_Uart0_SendCMDData(0x05,0x0a,0x01);       //充电座引导A成功
		}
		else
		{
			api_Uart0_SendCMDData(0x05,0x0a,0x00);       
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//充电座引导B
		if(mMaininf.mWork.mChargeBaseGuideBFlag == 1)
		{
			api_Uart0_SendCMDData(0x05,0x0b,0x01);       //充电座引导B成功
		}
		else
		{
			api_Uart0_SendCMDData(0x05,0x0b,0x00);       
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//充电座引导TOP
		if(mMaininf.mWork.mChargeBaseGuideTopFlag == 1)
		{
			api_Uart0_SendCMDData(0x05,0x0c,0x01);       //充电座引导TOP成功
		}
		else
		{
			api_Uart0_SendCMDData(0x05,0x0c,0x00);       
		}
		
		
		mMaininf.mWork.mWorkLedFlag = 0;                //清除触发状态
		mMaininf.mWork.mChargeBaseGuideAFlag = 0;
		mMaininf.mWork.mChargeBaseGuideBFlag = 0;
		mMaininf.mWork.mChargeBaseGuideTopFlag = 0;
		
		mMaininf.mWork.mChargeBaseGuideAStatus = 0;               //清除第一步标志位
		mMaininf.mWork.mChargeBaseGuideBStatus = 0; 
		mMaininf.mWork.mChargeBaseGuideTOPStatus = 0;             //检查引导第一次触发
		mMaininf.mWork.mWorkChecking = 0;
		
		mMaininf.mWork.mChargeBaseGuideAStatusCont = 0;
		mMaininf.mWork.mChargeBaseGuideBStatusCont = 0;
		mMaininf.mWork.mChargeBaseGuideTOPStatusCont = 0;
		
		mMaininf.mWork.mWorkLedFlagCont = 0;
		mMaininf.mWork.mChargeBaseGuideAFlagCont = 0;
		mMaininf.mWork.mChargeBaseGuideBFlagCont = 0;
		mMaininf.mWork.mChargeBaseGuideTopFlagCont = 0;
		mMaininf.mWork.mChargeBaseGuideFlag = 0;
	}
}

/*****************************************************************************
 * @name       :void api_Test_Along_Side(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Test along side
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void api_Test_Along_Side(void)
{
	//按键按下
	if(mMaininf.mWork.mWorkKey == 1)
	{
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_8) == 1)    //45度小板
// 		{
// //			Test_Item_Check_Enable(ST_DROP_BACK,ENABLE);
// 			
// 			Signal_Send(ST_AVOID_BUMP45,TRUE);           //免碰撞45度小板发射
// 			
// 			mMaininf.mWork.mAvoidBumpStatus = 1;         //完成检测第一步  下视
// 		}
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_7) == 1)    //沿边
// 		{
// 			Signal_Send(ST_ALONG_SIDE_A,TRUE);           //免碰撞45度小板发射
// 			
// 			mMaininf.mWork.mAlongSideStatus = 1;         //完成检测第一步  下视
// 		}
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_13) == 1)    //左右引导
// 		{
// 			PWM_Configuration(TRUE);
// 			api_Send_PWM(TRUE);
// 			
// 			mMaininf.mWork.mGuideRightLeftStatus = 1;
// 		}
		
		Test_Item_Check_Enable(ST_GUIDE_LEFT_RIGHT,ENABLE);
		mMaininf.mWork.mGuideSendFlag = 1;
		
// 		PWM_Configuration(TRUE);
// 		api_Send_PWM(TRUE);
		
		mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
		
		mMaininf.mWork.mWorkKey = 0;                     //清除按键状态
		mMaininf.mWork.mWorkChecking = 1;                //正在进行检测
	}
	
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mAvoidBumpStatus == 0) && 
	   (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) <= 100))
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_8) == 1)    //45度小板
		{
			mMaininf.mWork.mAvoidBumpStatusCont ++;
			
			if(mMaininf.mWork.mAvoidBumpStatusCont >= 10)
			{
				Signal_Send(ST_AVOID_BUMP45,TRUE);           //免碰撞45度小板发射
			
				mMaininf.mWork.mAvoidBumpStatus = 1;         //完成检测第一步  下视
			}
		}
		else
		{
			mMaininf.mWork.mAvoidBumpStatusCont = 0;
		}
	}
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mAlongSideStatus == 0) && 
	   (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) <= 100))
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_7) == 1)    //沿边
		{
			mMaininf.mWork.mAlongSideStatusCont ++;
			
			if(mMaininf.mWork.mAlongSideStatusCont >= 10)
			{
				Signal_Send(ST_ALONG_SIDE_A,TRUE);           //免碰撞45度小板发射
			
				mMaininf.mWork.mAlongSideStatus = 1;         //完成检测第一步  下视
			}
		}
		else
		{
			mMaininf.mWork.mAlongSideStatusCont = 0;
		}
	}
	
	
	
	
	
	
	
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mAvoidBumpStatus == 1))
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_8) == 0)
		{
			mMaininf.mWork.m45BoardFlagCont ++;
			
			if(mMaininf.mWork.m45BoardFlagCont >= 10)
			{
				mMaininf.mWork.m45BoardFlag = 1;
			}
		}
		else
		{
			mMaininf.mWork.m45BoardFlagCont = 0;
		}
	}
	
// 	if((mMaininf.mWork.mAvoidBumpStatus == 1) && (GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_8) == 0))
// 	{
// 		mMaininf.mWork.m45BoardFlag = 1;
// 	}
	
// 	if(mMaininf.mWork.mGuideRightLeftStatus == 1)
// 	{
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_13) == 0)
// 		{
// 			if(mMaininf.mWork.mWorkChecking == 1)
// 			{
// 				if(mMaininf.mWork.GuideFirstFlag == 0)
// 				{
// 					mMaininf.mWork.GuideFirstFlag = 1;
// 					mMaininf.mSystem.mGuideStartTime = api_GetCurrentSystemTime();
// 				}
// 				else
// 				{
// 					if(api_GetSystemTimePeriod(mMaininf.mSystem.mGuideStartTime) >= 20)
// 					{
// 						mMaininf.mWork.mLeftRightGuideFlag = 1;
// 					}
// 				}
// 			}
// 		}
// 		else
// 		{
// 			mMaininf.mWork.GuideFirstFlag = 0;
// 		}
// 	}
// 	if(mMaininf.mWork.mGuideRightLeftStatus == 1)
// 	{
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_13) == 0)
// 		{
// 			if(mMaininf.mWork.mWorkChecking == 1)
// 			{
// 				mMaininf.mWork.mGuideCont++ ;
// 			}
// 			if(mMaininf.mWork.mGuideCont > 50)
// 			{
// 				mMaininf.mWork.mLeftRightGuideFlag = 1;
// 			}
// 		}
// 		else
// 		{
// 			mMaininf.mWork.mGuideCont = 0;
// 		}
// 	}
	
	if((mMaininf.mWork.mAlongSideStatus == 1) && (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) >= 1000))
	{
		mMaininf.mWork.mAlongSideStatus = 2;
		Signal_Send(ST_ALONG_SIDE_A,FALSE);
		Signal_Send(ST_ALONG_SIDE_B,TRUE);
		
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 20);
	}
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mAlongSideStatus == 1))
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_7) == 0)
		{
			if(mMaininf.mWork.mWorkCont < 10)
			{
				mMaininf.mWork.mWorkCont ++;
			}
			else
			{
				mMaininf.mWork.mAlongSideAFlag = 1;
				mMaininf.mWork.mAlongSideStatus = 2;
				Signal_Send(ST_ALONG_SIDE_A,FALSE);
				Signal_Send(ST_ALONG_SIDE_B,TRUE);
				
				mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
				while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 20);
			}
		}
		else
		{
			mMaininf.mWork.mWorkCont = 0;
		}
	}
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mAlongSideStatus == 2))
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_7) == 0)
		{
			if(mMaininf.mWork.mWorkCont < 10)
			{
				mMaininf.mWork.mWorkCont ++;
			}
			else
			{
				mMaininf.mWork.mAlongSideBFlag = 1;
				Signal_Send(ST_ALONG_SIDE_B,FALSE);
			}
		}
		else
		{
			mMaininf.mWork.mWorkCont = 0;
		}
	}
	
	if((mMaininf.mWork.m45BoardFlag == 1) && (mMaininf.mWork.mAlongSideAFlag == 1) &&
	   (mMaininf.mWork.mAlongSideBFlag == 1) && (mMaininf.mWork.mLeftRightGuideFlag == 1))
	{
		mMaininf.mWork.mGuideSendFlag = 0;
		mMaininf.mWork.mGuideSendChangeFlag = 0;
		
		Signal_Send(ST_AVOID_BUMP45 | ST_ALONG_SIDE_A | ST_ALONG_SIDE_B,FALSE);                 //结束45度小板和沿边发射
		PWM_Configuration(FALSE);
		api_Send_PWM(FALSE);
		Test_Item_Check_Enable(ST_GUIDE_LEFT_RIGHT,DISABLE);
		
		api_Uart0_SendCMDData(0x06,0x10,0x01);           //显示检测成功项
		mMaininf.mWork.m45BoardFlag = 0;                //清除触发状态
		mMaininf.mWork.mAlongSideAFlag = 0;
		mMaininf.mWork.mAlongSideBFlag = 0;
		mMaininf.mWork.mLeftRightGuideFlag = 0;
		
		mMaininf.mWork.mAvoidBumpStatus = 0;               //清除第一步标志位
		mMaininf.mWork.mWorkChecking = 0; 
		mMaininf.mWork.mAlongSideStatus = 0;             //检查引导第一次触发
		mMaininf.mWork.mGuideRightLeftStatus = 0;
		
		mMaininf.mWork.mWorkCont = 0;
		
		mMaininf.mSystem.mGuideStartTimeT1 = 0;
		mMaininf.mSystem.mGuideStartTimeT2 = 0;
		mMaininf.mSystem.mGuideStartTimeT3 = 0;
		
		mMaininf.mWork.mAvoidBumpStatusCont = 0;
		mMaininf.mWork.mAlongSideStatusCont = 0;
		
		mMaininf.mWork.m45BoardFlagCont = 0;
	}
	
	//检测超时
	if((mMaininf.mWork.mWorkChecking == 1) && (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) >= 3000))    //3s  超时
	{
		mMaininf.mWork.mGuideSendFlag = 0;
		mMaininf.mWork.mGuideSendChangeFlag = 0;
		
		Signal_Send(ST_AVOID_BUMP45 | ST_ALONG_SIDE_A | ST_ALONG_SIDE_B,FALSE);                 //结束45度小板和沿边发射
		PWM_Configuration(FALSE);
		api_Send_PWM(FALSE);
		Test_Item_Check_Enable(ST_GUIDE_LEFT_RIGHT,DISABLE);
		
		//45度小板
		if(mMaininf.mWork.m45BoardFlag == 1)
		{
			api_Uart0_SendCMDData(0x06,0x0d,0x01);       //检测45度小板成功
		}
		else
		{
			api_Uart0_SendCMDData(0x06,0x0d,0x00);       
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//沿边A
		if(mMaininf.mWork.mAlongSideAFlag == 1)
		{
			api_Uart0_SendCMDData(0x06,0x0e,0x01);       //检测后引导成功
		}
		else
		{
			api_Uart0_SendCMDData(0x06,0x0e,0x00);       //检测后引导失败
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//沿边B
		if(mMaininf.mWork.mAlongSideBFlag == 1)
		{
			api_Uart0_SendCMDData(0x06,0x0f,0x01);       //检测后引导成功
		}
		else
		{
			api_Uart0_SendCMDData(0x06,0x0f,0x00);       //检测后引导失败
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//左右引导
		if(mMaininf.mWork.mLeftRightGuideFlag == 1)
		{
			api_Uart0_SendCMDData(0x06,0x05,0x01);       //检测后引导成功
		}
		else
		{
			api_Uart0_SendCMDData(0x06,0x05,0x00);       //检测后引导失败
		}
		
		mMaininf.mWork.m45BoardFlag = 0;                //清除触发状态
		mMaininf.mWork.mAlongSideAFlag = 0;
		mMaininf.mWork.mAlongSideBFlag = 0;
		mMaininf.mWork.mLeftRightGuideFlag = 0;
		
		mMaininf.mWork.mAvoidBumpStatus = 0;               //清除第一步标志位
		mMaininf.mWork.mWorkChecking = 0; 
		mMaininf.mWork.mAlongSideStatus = 0;             //检查引导第一次触发
		mMaininf.mWork.mGuideRightLeftStatus = 0;
		
		mMaininf.mWork.mWorkCont = 0;
		
		mMaininf.mSystem.mGuideStartTimeT1 = 0;
		mMaininf.mSystem.mGuideStartTimeT2 = 0;
		mMaininf.mSystem.mGuideStartTimeT3 = 0;
		
		mMaininf.mWork.mAvoidBumpStatusCont = 0;
		mMaininf.mWork.mAlongSideStatusCont = 0;
		
		mMaininf.mWork.m45BoardFlagCont = 0;
	}
}

/*****************************************************************************
 * @name       :void api_Test(u8 item)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Test items
 * @parameters :item
 * @retvalue   :None
******************************************************************************/
void api_Test_PCB(u8 item)
{
	switch(item)
	{
		case TEST_FRONTDROP :         //前下视组件
			api_Test_Front_Drop();
			break;
		case TEST_WHEEL :             //大轮线束组件
			api_Test_Wheel_Wire();
			break;
		case TEST_KEY :               //后下视-后引导-电源按键
			api_Test_Key_Wire();
			break;
		case TEST_DUST :              //灰尘检测-尘盒霍尔组件
			api_Test_Dust_Component();
			break;
		case TEST_GUIDE :             //充电座引导组件
			api_Test_Charge_Guide();
			break;
		case TEST_SIDE :              //沿边-引导组件
			api_Test_Along_Side();
			break;
		default :
			break;
	}
}

/*****************************************************************************
 * @name       :void api_MachinePowerOn(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :machine power on
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void api_MachinePowerOn(void)
{
// 	mMaininf.mSystem.mPowerOnTime = api_GetCurrentSystemTime();
// 	while(api_GetSystemTimePeriod(mMaininf.mSystem.mPowerOnTime) < 3000);
		
	mMaininf.mSystem.mPowerOnTime = api_GetCurrentSystemTime();
	mMaininf.mSystem.mReadModeStartTime = api_GetCurrentSystemTime();
	api_Uart0_SendCMDData(0,0,0);
	
	while(api_GetSystemTimePeriod(mMaininf.mSystem.mPowerOnTime) < 10000)
	{
		if(api_GetSystemTimePeriod(mMaininf.mSystem.mReadModeStartTime) > 500)
		{
			api_Uart0_SendCMDData(0,0,0);
			mMaininf.mSystem.mReadModeStartTime = api_GetCurrentSystemTime();
			
		}
		
		if(mMaininf.mUart0.mReceiveFlag == 1)            //    接收到反馈
		{
			mMaininf.mUart0.mReceiveFlag = 0;
			
			if((mMaininf.mUart0.ReceiveBuf[0] == 0) && (mMaininf.mUart0.ReceiveBuf[1] == 1))
			{
				if((mMaininf.mUart0.ReceiveBuf[2] & 0x07) == 0x00)          //充电座
				{
					mMaininf.mWork.mWorkMODE = TEST_GUIDE;
					api_Uart0_SendCMDData(0,0,1);
					break;
				}
				else if((mMaininf.mUart0.ReceiveBuf[2] & 0x07) == 0x07)     //后组合
				{
					mMaininf.mWork.mWorkMODE = TEST_KEY;
					api_Uart0_SendCMDData(0,0,1);
					break;
				}
				else if((mMaininf.mUart0.ReceiveBuf[2] & 0x07) == 0x03)     //大轮组合
				{
					mMaininf.mWork.mWorkMODE = TEST_WHEEL;
					api_Uart0_SendCMDData(0,0,1);
					break;
				}
				else if((mMaininf.mUart0.ReceiveBuf[2] & 0x07) == 0x02)     //灰尘0x01
				{
					mMaininf.mWork.mWorkMODE = TEST_DUST;
					api_Uart0_SendCMDData(0,0,1);
					break;
				}
				else if((mMaininf.mUart0.ReceiveBuf[2] & 0x07) == 0x06)     //前下视
				{
					mMaininf.mWork.mWorkMODE = TEST_FRONTDROP;
					api_Uart0_SendCMDData(0,0,1);
					break;
				}
				else if((mMaininf.mUart0.ReceiveBuf[2] & 0x07) == 0x05)     //沿边引导
				{
					mMaininf.mWork.mWorkMODE = TEST_SIDE;
					api_Uart0_SendCMDData(0,0,1);
					break;
				}
			}
		}
	}
}

/*****************************************************************************
 * @name       :void Scan_Key(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :scan key
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Scan_Key(void)
{
	if((mMaininf.mUart0.mReceiveFlag == 1) && (mMaininf.mUart0.ReceiveBuf[0] == 0) && (mMaininf.mUart0.ReceiveBuf[1] == 0))
	{
		mMaininf.mUart0.mReceiveFlag = 0;
		
		if(mMaininf.mUart0.ReceiveBuf[2] & 0x08)     //按键按下
		{
			api_Uart0_SendCMDData(0,0,1);
			mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
			while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 3);
			mMaininf.mWork.mWorkKey = 1; 
		}
	}
}


























