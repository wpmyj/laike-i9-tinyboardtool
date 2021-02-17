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

        ʱ����ؿ⺯��

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

        ����UARTͨѶ

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
				
				mMaininf.mUart0.mReceiveFlag = 1;       //���յ�����
				
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
 * @parameters :test_mode : ���԰�   mState �� ʹ��״̬
 * @retvalue   :None
******************************************************************************/
void Test_Item_Check_Enable(u16 mPre,ControlStatus mState)
{
	u8 iCont;
	
	for(iCont = 0;iCont < 13;iCont ++)
	{
		switch(mPre & (1 << iCont))
		{
			case ST_DROP_FRONT :             //ǰ����
				if(mState == ENABLE)
				{
					mMaininf.mWork.mFrontDropCheckEnable = 1;
				}
				else
				{
					mMaininf.mWork.mFrontDropCheckEnable = 0;
				}
				break;
			case ST_DROP_LEFT_RIGHT :         //��������
				if(mState == ENABLE)
				{
					mMaininf.mWork.mLeftRightDropCheckEnable = 1;
				}
				else
				{
					mMaininf.mWork.mLeftRightDropCheckEnable = 0;
				}
				break;
			case ST_DROP_BACK :           //������
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
 * @parameters :type : ST_AVOID_BUMP45        ����ײ45�ȷ���    1
					   ST_ALONG_SIDE_A        �ر�A����         2
					   ST_ALONG_SIDE_B        �ر�B����         3
					   ST_DUST                �ҳ�����          4
					   ST_CODE                ���̷���          5
					   ST_FRONT_DROP          ǰ���ӷ���        6
					   ST_LEFT_RIGHT_DROP     �������ӷ���    7
					   ST_BACK_DROP           �����ӷ���        8
					   ST_CHARGE_A            ���������A����   9
					   ST_CHARGE_B            ���������B����   10
					   ST_CHARGE_TOP          ���������TOP���� 11
					   ST_BUMP_DUSTBOX        ��ײ���з���      12
				status : 0:�ر�   1:��
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
//  * @parameters :position : ST_FRONT_DROP,               //ǰ���ӷ���
// 						   ST_LEFT_RIGHT_DROP,          //�������ӷ���
// 						   ST_BACK_DROP                 //�����ӷ���
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
// 				api_Uart0_SendCMDData(01,01,1);          //���ɹ�
// 			}
// 			else
// 			{
// 				api_Uart0_SendCMDData(01,01,0);          //���ʧ��
// 			}
// 			mMaininf.mWork.mCheckStatusFlag = 0;         //δ���ɹ�
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
// 				api_Uart0_SendCMDData(02,01,1);          //���ɹ�
// 			}
// 			else
// 			{
// 				api_Uart0_SendCMDData(02,01,0);          //���ʧ��
// 			}
// 			mMaininf.mWork.mCheckStatusFlag = 0;         //δ���ɹ�
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
// 				api_Uart0_SendCMDData(03,01,1);          //���ɹ�
// 			}
// 			else
// 			{
// 				api_Uart0_SendCMDData(03,01,0);          //���ʧ��
// 			}
// 			mMaininf.mWork.mCheckStatusFlag = 0;         //δ���ɹ�
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
	//��������
	if(mMaininf.mWork.mWorkKey == 1)
	{
		mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_2) == 1)         //�����ն˳�ʼ״̬   ǰ����
// 		{
// 			Signal_Send(ST_FRONT_DROP,TRUE);                 //����ǰ�����ź�
// 		
// //			Test_Item_Check_Enable(ST_DROP_FRONT,ENABLE);   //��ǰ���Ӽ��
// 			
// 			mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
// 			
// 			mMaininf.mWork.mReceiveStatus = 1;               //��ɼ���һ��
// 		}
// 		else
// 		{
// 			mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime() + 60000;
// 		}
		
		mMaininf.mWork.mWorkKey = 0;                     //�������״̬
		mMaininf.mWork.mWorkChecking = 1;                //���ڽ��м��
	}
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mReceiveStatus == 0))
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_2) == 1)
		{
			mMaininf.mWork.mDropFrontStatusCont ++;
			
			if(mMaininf.mWork.mDropFrontStatusCont >= 10)
			{
				Signal_Send(ST_FRONT_DROP,TRUE);                 //����ǰ�����ź�
				
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
	
	
	//��⵽ǰ�����ź�
	if(mMaininf.mWork.mFrontDropFlag == 1)
	{
		Signal_Send(ST_FRONT_DROP,FALSE);                //�������ӷ���
//		Test_Item_Check_Enable(TEST_FRONTDROP,DISABLE);  //�ر�ǰ���Ӽ��
		api_Uart0_SendCMDData(0x01,0x01,0x01);           //��ʾǰ���Ӽ��ɹ�
		mMaininf.mWork.mFrontDropFlag = 0;               //�������״̬
		mMaininf.mWork.mReceiveStatus = 0;               //�����һ����־λ
		mMaininf.mWork.mWorkChecking = 0;                //�˳����ģʽ
		
		mMaininf.mWork.mDropFrontStatusCont = 0;
		mMaininf.mWork.mFrontDropFlagCont = 0;
	}
	
	//��ⳬʱ
	if((mMaininf.mWork.mWorkChecking == 1) && (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) >= 500))    //0.5s  ��ʱ
	{
		Signal_Send(ST_FRONT_DROP,FALSE);                //�������ӷ���
//		Test_Item_Check_Enable(TEST_FRONTDROP,DISABLE);  //�ر�ǰ���Ӽ��
		api_Uart0_SendCMDData(0x01,0x01,0x00);           //��ʾǰ���Ӽ��ʧ��
		mMaininf.mWork.mFrontDropFlag = 0;               //�������״̬
		mMaininf.mWork.mReceiveStatus = 0;               //�����һ����־λ
		mMaininf.mWork.mWorkChecking = 0;                //�˳����ģʽ
		
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
//	zbqt = GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_5);      //��ײ
//	zbqt = GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_12);     //����
	//��������
	if(mMaininf.mWork.mWorkKey == 1)
	{
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_9) == 1)    //��������
// 		{
// 			Test_Item_Check_Enable(ST_DROP_LEFT_RIGHT,ENABLE);
// 			
// 			Signal_Send(ST_LEFT_RIGHT_DROP,TRUE);        //�������������źŷ���
// 			
// 			mMaininf.mWork.mReceiveStatus = 1;           //��ɼ���һ��  ����
// 		}
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_12) == 1)   //��������
// 		{
// 			Test_Item_Check_Enable(ST_CODE_LEFT_RIGHT,ENABLE);
// 			
// 			Signal_Send(ST_CODE,TRUE);                  //�������������ź�
// 			
// 			mMaininf.mWork.mCodeReceiveStatus = 1;       //��ɼ���һ��  ����
// 			
// //			zbqt = GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_12);
// 		}
		
		Test_Item_Check_Enable(ST_BUMP_LEFT_RIGHT | ST_LEAVE_LEFT_RIGHT,ENABLE);
		
		mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
		
		
// 		if((GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_9) == 1) && (GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_12) == 1))
// 		{
// 			Signal_Send(ST_LEFT_RIGHT_DROP | ST_CODE,TRUE);        //�������������źŷ���
// 			
// 			Test_Item_Check_Enable(ST_DROP_LEFT_RIGHT | ST_BUMP_LEFT_RIGHT | ST_CODE_LEFT_RIGHT | ST_LEAVE_LEFT_RIGHT,ENABLE);             //�򿪴����������
// 			
// 			mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
// 			
// 			mMaininf.mWork.mReceiveStatus = 1;           //��ɼ���һ��  ����
// 			mMaininf.mWork.mCodeReceiveStatus = 1;       //��ɼ���һ��  ����
// 		}
// 		else
// 		{
// 			if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_9) == 1)
// 			{
// 				
// 			}
// 			mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime() + 60000;
// 		}
		
		
		mMaininf.mWork.mWorkKey = 0;                     //�������״̬
		mMaininf.mWork.mWorkChecking = 1;                //���ڽ��м��
	}
	
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mReceiveStatus == 0))    //����������ӳ�ʼ״̬
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_9) == 1)    //��������
		{
			mMaininf.mWork.mDropLeftRightStatusCont ++;
			
			if(mMaininf.mWork.mDropLeftRightStatusCont >= 10)
			{
				Test_Item_Check_Enable(ST_DROP_LEFT_RIGHT,ENABLE);
			
				Signal_Send(ST_LEFT_RIGHT_DROP,TRUE);        //�������������źŷ���
				
				mMaininf.mWork.mReceiveStatus = 1;           //��ɼ���һ��  ����
			}
		}
		else
		{
			mMaininf.mWork.mDropLeftRightStatusCont = 0;
		}
	}
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mCodeReceiveStatus == 0))    //����������̳�ʼ״̬
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_12) == 1)   //��������
		{
			mMaininf.mWork.mCodeLeftRightStatusCont ++;
			
			if(mMaininf.mWork.mCodeLeftRightStatusCont >= 10)
			{
				Test_Item_Check_Enable(ST_CODE_LEFT_RIGHT,ENABLE);
			
				Signal_Send(ST_CODE,TRUE);                  //�������������ź�
				
				mMaininf.mWork.mCodeReceiveStatus = 1;       //��ɼ���һ��  ����
			}
		}
		else
		{
			mMaininf.mWork.mCodeLeftRightStatusCont = 0;
		}
	}
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mBumpStatus == 0))    //���������ײ
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_5) == 1)   //��ײ
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
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mBumpStatus == 1))    //���������ײ
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_5) == 0)   //��ײ
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
	
// 	if((mMaininf.mWork.mWorkChecking == 1) && (GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_5) == 1))     //��ײ
// 	{
// 		mMaininf.mWork.mLeftRightBumpFlag = 1;
// 	}
	
// 	if((mMaininf.mWork.mWorkChecking == 1) && (GPIO_ReadInBit(HT_GPIOA,GPIO_PIN_8) == 0))     //���
// 	{
// 		mMaininf.mWork.mLeftRightLeaveFlag = 1;
// 	}
	
	//���ȫ�����ɹ�
	if((mMaininf.mWork.mLeftRightDropFlag == 1) && (mMaininf.mWork.mLeftRightBumpFlag == 1) && 
	   (mMaininf.mWork.mLeftRightCodeFlag == 1) && (mMaininf.mWork.mLeftRightLeaveFlag == 1))
	{
		Signal_Send(ST_LEFT_RIGHT_DROP | ST_CODE | ST_BUMP_DUSTBOX,FALSE); //������������  ���̼��
		Test_Item_Check_Enable(ST_DROP_LEFT_RIGHT | ST_BUMP_LEFT_RIGHT | ST_CODE_LEFT_RIGHT | ST_LEAVE_LEFT_RIGHT,DISABLE);      //�رմ����������
		api_Uart0_SendCMDData(0x02,0x10,0x01);           //��ʾ���ɹ���
		mMaininf.mWork.mLeftRightDropFlag = 0;           //�������״̬
		mMaininf.mWork.mLeftRightBumpFlag = 0;
		mMaininf.mWork.mLeftRightCodeFlag = 0;
		mMaininf.mWork.mLeftRightLeaveFlag = 0;
		
		mMaininf.mWork.mReceiveStatus = 0;               //�����һ����־λ
		mMaininf.mWork.mCodeReceiveStatus = 0;           //������̵�һ����־λ
		mMaininf.mWork.mBumpStatus = 0;
		mMaininf.mWork.mWorkChecking = 0;                //�˳����ģʽ
		
		mMaininf.mWork.mDropLeftRightStatusCont = 0;
		mMaininf.mWork.mCodeLeftRightStatusCont = 0;
		mMaininf.mWork.mBumpStatusCont = 0;
		
		mMaininf.mWork.mLeftRightDropFlagCont = 0;
		mMaininf.mWork.mLeftRightCodeFlagCont = 0;
		mMaininf.mWork.mLeftRightLeaveFlagCont = 0;
		mMaininf.mWork.mLeftRightBumpFlagCont = 0;
	}
	
	//��ⳬʱ
	if((mMaininf.mWork.mWorkChecking == 1) && (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) >= 5000))    //5s  ��ʱ
	{
		Signal_Send(ST_LEFT_RIGHT_DROP | ST_CODE | ST_BUMP_DUSTBOX,FALSE); //������������  ���̼��
		Test_Item_Check_Enable(ST_DROP_LEFT_RIGHT | ST_BUMP_LEFT_RIGHT | ST_CODE_LEFT_RIGHT | ST_LEAVE_LEFT_RIGHT,DISABLE);  //�رմ����������
		
		//��⵽���������ź�
		if(mMaininf.mWork.mLeftRightDropFlag == 1)
		{
			api_Uart0_SendCMDData(0x02,0x01,0x01);       //����������ӳɹ�
		}
		else
		{
			api_Uart0_SendCMDData(0x02,0x01,0x00);       //�����������ʧ��
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//��⵽������ײ�ź�
		if(mMaininf.mWork.mLeftRightBumpFlag == 1)
		{
			api_Uart0_SendCMDData(0x02,0x02,0x01);       //���������ײ�ɹ�
		}
		else
		{
			api_Uart0_SendCMDData(0x02,0x02,0x00);       //���������ײʧ��
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//��⵽���������ź�
		if(mMaininf.mWork.mLeftRightCodeFlag == 1)
		{
			api_Uart0_SendCMDData(0x02,0x03,0x01);       //����������̳ɹ�
		}
		else
		{
			api_Uart0_SendCMDData(0x02,0x03,0x00);       //�����������ʧ��
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//��⵽��������ź�
		if(mMaininf.mWork.mLeftRightLeaveFlag == 1)
		{
			api_Uart0_SendCMDData(0x02,0x04,0x01);       //���������سɹ�
		}
		else
		{
			api_Uart0_SendCMDData(0x02,0x04,0x00);       //����������ʧ��
		}
		
		mMaininf.mWork.mLeftRightDropFlag = 0;           //�������״̬
		mMaininf.mWork.mLeftRightBumpFlag = 0;
		mMaininf.mWork.mLeftRightCodeFlag = 0;
		mMaininf.mWork.mLeftRightLeaveFlag = 0;
		
		mMaininf.mWork.mReceiveStatus = 0;               //�����һ����־λ
		mMaininf.mWork.mCodeReceiveStatus = 0;           //������̵�һ����־λ
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
	//��������
	if(mMaininf.mWork.mWorkKey == 1)
	{
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_15) == 1)    //������
// 		{
// 			Test_Item_Check_Enable(ST_DROP_BACK,ENABLE);
// 			
// 			Signal_Send(ST_BACK_DROP,TRUE);              //�������������źŷ���
// 			
// 			mMaininf.mWork.mReceiveStatus = 1;           //��ɼ���һ��  ����
// 		}
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_6) == 1)     //������
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
// 			Signal_Send(ST_BACK_DROP,TRUE);                        //���������
// 		
// 			Test_Item_Check_Enable(TEST_KEY,ENABLE);               //�򿪺��������
// 			
// 			mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
// 		}
// 		else
// 		{
// 			mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime() + 60000;
// 		}
		
		mMaininf.mWork.mWorkKey = 0;                     //�������״̬
		mMaininf.mWork.mWorkChecking = 1;                //���ڽ��м��
	}
	
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mReceiveStatus == 0))
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_15) == 1)    //������
		{
			mMaininf.mWork.mDropBackStatusCont ++;
			
			if(mMaininf.mWork.mDropBackStatusCont >= 10)
			{
				Test_Item_Check_Enable(ST_DROP_BACK,ENABLE);
			
				Signal_Send(ST_BACK_DROP,TRUE);              //�������������źŷ���
				
				mMaininf.mWork.mReceiveStatus = 1;           //��ɼ���һ��  ����
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
// 			mMaininf.mWork.mPowerKeyFlag = 0;      //�������ʧ��
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
	
	
	//���ȫ�����ɹ�
	if((mMaininf.mWork.mBackDropFlag == 1) && (mMaininf.mWork.mBackGuideFlag == 1) && 
	   (mMaininf.mWork.mPowerKeyFlag == 1))
	{
		mMaininf.mWork.mGuideSendFlag = 0;
		mMaininf.mWork.mGuideSendChangeFlag = 0;
		
		Signal_Send(ST_BACK_DROP,FALSE);                 //������������  ���̼��
		PWM_Configuration(FALSE);
		api_Send_PWM(FALSE);
		Test_Item_Check_Enable(ST_DROP_BACK | ST_POWER_KEY | ST_GUIDE_BACK,DISABLE);        //�رմ����������
		api_Uart0_SendCMDData(0x03,0x10,0x01);           //��ʾ���ɹ���
		mMaininf.mWork.mBackDropFlag = 0;                //�������״̬
		mMaininf.mWork.mBackGuideFlag = 0;
		mMaininf.mWork.mPowerKeyFlag = 0;
		
		mMaininf.mWork.mReceiveStatus = 0;               //�����һ����־λ
		mMaininf.mWork.mWorkChecking = 0; 
		mMaininf.mWork.mGuideBackStatus = 0;             //���������һ�δ���
		mMaininf.mWork.mKeyCont = 0;
		mMaininf.mWork.mGuideCont = 0;
		
		mMaininf.mSystem.mGuideStartTimeT1 = 0;
		mMaininf.mSystem.mGuideStartTimeT2 = 0;
		mMaininf.mSystem.mGuideStartTimeT3 = 0;
		
		mMaininf.mWork.mDropBackStatusCont = 0;
		mMaininf.mWork.mBackDropFlagCont = 0;
	}
	
	//��ⳬʱ
	if((mMaininf.mWork.mWorkChecking == 1) && (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) >= 1000))    //1s  ��ʱ
	{
		mMaininf.mWork.mGuideSendFlag = 0;
		mMaininf.mWork.mGuideSendChangeFlag = 0;
		
		Signal_Send(ST_BACK_DROP,FALSE);                 //������������  ���̼��
		PWM_Configuration(FALSE);
		api_Send_PWM(FALSE);
		Test_Item_Check_Enable(ST_DROP_BACK | ST_POWER_KEY | ST_GUIDE_BACK,DISABLE);        //�رմ����������
		
		//��������
		if(mMaininf.mWork.mBackDropFlag == 1)
		{
			api_Uart0_SendCMDData(0x03,0x01,0x01);       //�������ӳɹ�
		}
		else
		{
			api_Uart0_SendCMDData(0x03,0x01,0x00);       //��������ʧ��
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//��������
		if(mMaininf.mWork.mBackGuideFlag == 1)
		{
			api_Uart0_SendCMDData(0x03,0x05,0x01);       //���������ɹ�
		}
		else
		{
			api_Uart0_SendCMDData(0x03,0x05,0x00);       //��������ʧ��
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//��ⰴ������
		if(mMaininf.mWork.mPowerKeyFlag == 1)
		{
			api_Uart0_SendCMDData(0x03,0x06,0x01);       //���������ɹ�
		}
		else
		{
			api_Uart0_SendCMDData(0x03,0x06,0x00);       //��������ʧ��
		}
		
		mMaininf.mWork.mBackDropFlag = 0;                //�������״̬
		mMaininf.mWork.mBackGuideFlag = 0;
		mMaininf.mWork.mPowerKeyFlag = 0;
		
		mMaininf.mWork.mReceiveStatus = 0;               //�����һ����־λ
		mMaininf.mWork.mWorkChecking = 0; 
		mMaininf.mWork.mGuideBackStatus = 0;             //���������һ�δ���
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
// 	//��������
// 	if(mMaininf.mWork.mWorkKey == 1)
// 	{
// 		
// 		Test_Item_Check_Enable(ST_DUST_BOX,ENABLE);
// 		
// 		mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
// 		
// 		mMaininf.mWork.mWorkKey = 0;                     //�������״̬
// 		mMaininf.mWork.mWorkChecking = 1;                //���ڽ��м��
// 	}
	//���д���������ʼ���
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
					mMaininf.mWork.mWorkChecking = 1;                //���ڽ��м��
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
				Signal_Send(ST_DUST,TRUE);                             //����ҳ�
			
				mMaininf.mWork.mDust3V3Flag = 1;                       //�ҳ�3.3V����
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
		Signal_Send(ST_DUST | ST_BUMP_DUSTBOX,FALSE);                      //�رջҳ�����
		Test_Item_Check_Enable(ST_DUST_CHECK | ST_DUST_BOX,DISABLE);       //�رճ������
		api_Uart0_SendCMDData(0x04,0x10,0x01);           //��ʾ���ɹ���
		mMaininf.mWork.mDustBoxFlag = 0;                 //�������״̬
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
	
	//��ⳬʱ
	if((mMaininf.mWork.mWorkChecking == 1) && (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) >= 3000))    //3s  ��ʱ
	{
		Signal_Send(ST_DUST | ST_BUMP_DUSTBOX,FALSE);                      //�رջҳ�����
		Test_Item_Check_Enable(ST_DUST_CHECK | ST_DUST_BOX,DISABLE);       //�رճ������
		
		//�ҳ�������
		if(mMaininf.mWork.mDustFlag == 1)
		{
			api_Uart0_SendCMDData(0x04,0x07,0x01);           //��ʾ���ɹ���
		}
		else
		{
			api_Uart0_SendCMDData(0x04,0x07,0x00);           //��ʾ���ʧ����
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//���л���
		if(mMaininf.mWork.mDustBoxFlag == 1)
		{
			api_Uart0_SendCMDData(0x04,0x08,0x01);           //��ʾ���ɹ���
		}
		else
		{
			api_Uart0_SendCMDData(0x04,0x08,0x00);           //��ʾ���ɹ���
		}
		
		mMaininf.mWork.mDustBoxFlag = 0;                //�������״̬
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
	
	//��������
	if(mMaininf.mWork.mWorkKey == 1)
	{
		Test_Item_Check_Enable(ST_CHARGE_BASE,ENABLE);
		
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 30);
		
// 		if(mMaininf.mWork.mSensorADC[0] <= 372)          //�����A��ѹС��0.3V
// 		{
// //			Signal_Send(ST_CHARGE_A,TRUE);
// 			
// 			mMaininf.mWork.mChargeBaseGuideAStatus = 1;    //��ɼ���һ��  ���������A
// 		}
// 		if(mMaininf.mWork.mSensorADC[1] <= 372)          //�����B��ѹС��0.3V
// 		{
// //			Signal_Send(ST_CHARGE_B,TRUE);
// 			
// 			mMaininf.mWork.mChargeBaseGuideBStatus = 1;    //��ɼ���һ��  ���������B
// 		}
// 		if(mMaininf.mWork.mSensorADC[2] <= 372)          //�����TOP��ѹС��0.3V
// 		{
// //			Signal_Send(ST_CHARGE_TOP,TRUE);
// 			
// 			mMaininf.mWork.mChargeBaseGuideTOPStatus = 1;    //��ɼ���һ��  ���������B
// 		}
		
		mMaininf.mSystem.mCheckStartTime = api_GetCurrentSystemTime();
		
		mMaininf.mWork.mWorkKey = 0;                     //�������״̬
		mMaininf.mWork.mWorkChecking = 1;                //���ڽ��м��
	}
	
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mChargeBaseGuideAStatus == 0) && 
	   (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) <= 100))
	{
		if(mMaininf.mWork.mSensorADC[0] <= 372)          //�����A��ѹС��0.3V
		{
			mMaininf.mWork.mChargeBaseGuideAStatusCont ++;
			
			if(mMaininf.mWork.mChargeBaseGuideAStatusCont >= 10)
			{
				mMaininf.mWork.mChargeBaseGuideAStatus = 1;    //��ɼ���һ��  ���������A
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
		if(mMaininf.mWork.mSensorADC[1] <= 372)          //�����B��ѹС��0.3V
		{
			mMaininf.mWork.mChargeBaseGuideBStatusCont ++;
			
			if(mMaininf.mWork.mChargeBaseGuideBStatusCont >= 10)
			{
				mMaininf.mWork.mChargeBaseGuideBStatus = 1;    //��ɼ���һ��  ���������B
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
		if(mMaininf.mWork.mSensorADC[2] <= 372)          //�����TOP��ѹС��0.3V
		{
			mMaininf.mWork.mChargeBaseGuideTOPStatusCont ++;
			
			if(mMaininf.mWork.mChargeBaseGuideTOPStatusCont >= 10)
			{
				mMaininf.mWork.mChargeBaseGuideTOPStatus = 1;    //��ɼ���һ��  ���������TOP
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
		api_Uart0_SendCMDData(0x05,0x10,0x01);           //��ʾ���ɹ���
		mMaininf.mWork.mWorkLedFlag = 0;                //�������״̬
		mMaininf.mWork.mChargeBaseGuideAFlag = 0;
		mMaininf.mWork.mChargeBaseGuideBFlag = 0;
		mMaininf.mWork.mChargeBaseGuideTopFlag = 0;
		
		mMaininf.mWork.mChargeBaseGuideAStatus = 0;               //�����һ����־λ
		mMaininf.mWork.mChargeBaseGuideBStatus = 0; 
		mMaininf.mWork.mChargeBaseGuideTOPStatus = 0;             //���������һ�δ���
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
	
	//��ⳬʱ
	if((mMaininf.mWork.mWorkChecking == 1) && (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) >= 1000))    //1s  ��ʱ
	{
		Test_Item_Check_Enable(ST_CHARGE_BASE,DISABLE);
		Signal_Send(ST_CHARGE_A | ST_CHARGE_B | ST_CHARGE_TOP,FALSE);
		
		//�����������
		if(mMaininf.mWork.mWorkLedFlag == 1)
		{
			api_Uart0_SendCMDData(0x05,0x09,0x01);       //����������Ƴɹ�
		}
		else
		{
			api_Uart0_SendCMDData(0x05,0x09,0x00);       
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//���������A
		if(mMaininf.mWork.mChargeBaseGuideAFlag == 1)
		{
			api_Uart0_SendCMDData(0x05,0x0a,0x01);       //���������A�ɹ�
		}
		else
		{
			api_Uart0_SendCMDData(0x05,0x0a,0x00);       
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//���������B
		if(mMaininf.mWork.mChargeBaseGuideBFlag == 1)
		{
			api_Uart0_SendCMDData(0x05,0x0b,0x01);       //���������B�ɹ�
		}
		else
		{
			api_Uart0_SendCMDData(0x05,0x0b,0x00);       
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//���������TOP
		if(mMaininf.mWork.mChargeBaseGuideTopFlag == 1)
		{
			api_Uart0_SendCMDData(0x05,0x0c,0x01);       //���������TOP�ɹ�
		}
		else
		{
			api_Uart0_SendCMDData(0x05,0x0c,0x00);       
		}
		
		
		mMaininf.mWork.mWorkLedFlag = 0;                //�������״̬
		mMaininf.mWork.mChargeBaseGuideAFlag = 0;
		mMaininf.mWork.mChargeBaseGuideBFlag = 0;
		mMaininf.mWork.mChargeBaseGuideTopFlag = 0;
		
		mMaininf.mWork.mChargeBaseGuideAStatus = 0;               //�����һ����־λ
		mMaininf.mWork.mChargeBaseGuideBStatus = 0; 
		mMaininf.mWork.mChargeBaseGuideTOPStatus = 0;             //���������һ�δ���
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
	//��������
	if(mMaininf.mWork.mWorkKey == 1)
	{
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_8) == 1)    //45��С��
// 		{
// //			Test_Item_Check_Enable(ST_DROP_BACK,ENABLE);
// 			
// 			Signal_Send(ST_AVOID_BUMP45,TRUE);           //����ײ45��С�巢��
// 			
// 			mMaininf.mWork.mAvoidBumpStatus = 1;         //��ɼ���һ��  ����
// 		}
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_7) == 1)    //�ر�
// 		{
// 			Signal_Send(ST_ALONG_SIDE_A,TRUE);           //����ײ45��С�巢��
// 			
// 			mMaininf.mWork.mAlongSideStatus = 1;         //��ɼ���һ��  ����
// 		}
// 		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_13) == 1)    //��������
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
		
		mMaininf.mWork.mWorkKey = 0;                     //�������״̬
		mMaininf.mWork.mWorkChecking = 1;                //���ڽ��м��
	}
	
	
	if((mMaininf.mWork.mWorkChecking == 1) && (mMaininf.mWork.mAvoidBumpStatus == 0) && 
	   (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) <= 100))
	{
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_8) == 1)    //45��С��
		{
			mMaininf.mWork.mAvoidBumpStatusCont ++;
			
			if(mMaininf.mWork.mAvoidBumpStatusCont >= 10)
			{
				Signal_Send(ST_AVOID_BUMP45,TRUE);           //����ײ45��С�巢��
			
				mMaininf.mWork.mAvoidBumpStatus = 1;         //��ɼ���һ��  ����
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
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_7) == 1)    //�ر�
		{
			mMaininf.mWork.mAlongSideStatusCont ++;
			
			if(mMaininf.mWork.mAlongSideStatusCont >= 10)
			{
				Signal_Send(ST_ALONG_SIDE_A,TRUE);           //����ײ45��С�巢��
			
				mMaininf.mWork.mAlongSideStatus = 1;         //��ɼ���һ��  ����
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
		
		Signal_Send(ST_AVOID_BUMP45 | ST_ALONG_SIDE_A | ST_ALONG_SIDE_B,FALSE);                 //����45��С����ر߷���
		PWM_Configuration(FALSE);
		api_Send_PWM(FALSE);
		Test_Item_Check_Enable(ST_GUIDE_LEFT_RIGHT,DISABLE);
		
		api_Uart0_SendCMDData(0x06,0x10,0x01);           //��ʾ���ɹ���
		mMaininf.mWork.m45BoardFlag = 0;                //�������״̬
		mMaininf.mWork.mAlongSideAFlag = 0;
		mMaininf.mWork.mAlongSideBFlag = 0;
		mMaininf.mWork.mLeftRightGuideFlag = 0;
		
		mMaininf.mWork.mAvoidBumpStatus = 0;               //�����һ����־λ
		mMaininf.mWork.mWorkChecking = 0; 
		mMaininf.mWork.mAlongSideStatus = 0;             //���������һ�δ���
		mMaininf.mWork.mGuideRightLeftStatus = 0;
		
		mMaininf.mWork.mWorkCont = 0;
		
		mMaininf.mSystem.mGuideStartTimeT1 = 0;
		mMaininf.mSystem.mGuideStartTimeT2 = 0;
		mMaininf.mSystem.mGuideStartTimeT3 = 0;
		
		mMaininf.mWork.mAvoidBumpStatusCont = 0;
		mMaininf.mWork.mAlongSideStatusCont = 0;
		
		mMaininf.mWork.m45BoardFlagCont = 0;
	}
	
	//��ⳬʱ
	if((mMaininf.mWork.mWorkChecking == 1) && (api_GetSystemTimePeriod(mMaininf.mSystem.mCheckStartTime) >= 3000))    //3s  ��ʱ
	{
		mMaininf.mWork.mGuideSendFlag = 0;
		mMaininf.mWork.mGuideSendChangeFlag = 0;
		
		Signal_Send(ST_AVOID_BUMP45 | ST_ALONG_SIDE_A | ST_ALONG_SIDE_B,FALSE);                 //����45��С����ر߷���
		PWM_Configuration(FALSE);
		api_Send_PWM(FALSE);
		Test_Item_Check_Enable(ST_GUIDE_LEFT_RIGHT,DISABLE);
		
		//45��С��
		if(mMaininf.mWork.m45BoardFlag == 1)
		{
			api_Uart0_SendCMDData(0x06,0x0d,0x01);       //���45��С��ɹ�
		}
		else
		{
			api_Uart0_SendCMDData(0x06,0x0d,0x00);       
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//�ر�A
		if(mMaininf.mWork.mAlongSideAFlag == 1)
		{
			api_Uart0_SendCMDData(0x06,0x0e,0x01);       //���������ɹ�
		}
		else
		{
			api_Uart0_SendCMDData(0x06,0x0e,0x00);       //��������ʧ��
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//�ر�B
		if(mMaininf.mWork.mAlongSideBFlag == 1)
		{
			api_Uart0_SendCMDData(0x06,0x0f,0x01);       //���������ɹ�
		}
		else
		{
			api_Uart0_SendCMDData(0x06,0x0f,0x00);       //��������ʧ��
		}
		mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 150);
		
		//��������
		if(mMaininf.mWork.mLeftRightGuideFlag == 1)
		{
			api_Uart0_SendCMDData(0x06,0x05,0x01);       //���������ɹ�
		}
		else
		{
			api_Uart0_SendCMDData(0x06,0x05,0x00);       //��������ʧ��
		}
		
		mMaininf.mWork.m45BoardFlag = 0;                //�������״̬
		mMaininf.mWork.mAlongSideAFlag = 0;
		mMaininf.mWork.mAlongSideBFlag = 0;
		mMaininf.mWork.mLeftRightGuideFlag = 0;
		
		mMaininf.mWork.mAvoidBumpStatus = 0;               //�����һ����־λ
		mMaininf.mWork.mWorkChecking = 0; 
		mMaininf.mWork.mAlongSideStatus = 0;             //���������һ�δ���
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
		case TEST_FRONTDROP :         //ǰ�������
			api_Test_Front_Drop();
			break;
		case TEST_WHEEL :             //�����������
			api_Test_Wheel_Wire();
			break;
		case TEST_KEY :               //������-������-��Դ����
			api_Test_Key_Wire();
			break;
		case TEST_DUST :              //�ҳ����-���л������
			api_Test_Dust_Component();
			break;
		case TEST_GUIDE :             //������������
			api_Test_Charge_Guide();
			break;
		case TEST_SIDE :              //�ر�-�������
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
		
		if(mMaininf.mUart0.mReceiveFlag == 1)            //    ���յ�����
		{
			mMaininf.mUart0.mReceiveFlag = 0;
			
			if((mMaininf.mUart0.ReceiveBuf[0] == 0) && (mMaininf.mUart0.ReceiveBuf[1] == 1))
			{
				if((mMaininf.mUart0.ReceiveBuf[2] & 0x07) == 0x00)          //�����
				{
					mMaininf.mWork.mWorkMODE = TEST_GUIDE;
					api_Uart0_SendCMDData(0,0,1);
					break;
				}
				else if((mMaininf.mUart0.ReceiveBuf[2] & 0x07) == 0x07)     //�����
				{
					mMaininf.mWork.mWorkMODE = TEST_KEY;
					api_Uart0_SendCMDData(0,0,1);
					break;
				}
				else if((mMaininf.mUart0.ReceiveBuf[2] & 0x07) == 0x03)     //�������
				{
					mMaininf.mWork.mWorkMODE = TEST_WHEEL;
					api_Uart0_SendCMDData(0,0,1);
					break;
				}
				else if((mMaininf.mUart0.ReceiveBuf[2] & 0x07) == 0x02)     //�ҳ�0x01
				{
					mMaininf.mWork.mWorkMODE = TEST_DUST;
					api_Uart0_SendCMDData(0,0,1);
					break;
				}
				else if((mMaininf.mUart0.ReceiveBuf[2] & 0x07) == 0x06)     //ǰ����
				{
					mMaininf.mWork.mWorkMODE = TEST_FRONTDROP;
					api_Uart0_SendCMDData(0,0,1);
					break;
				}
				else if((mMaininf.mUart0.ReceiveBuf[2] & 0x07) == 0x05)     //�ر�����
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
		
		if(mMaininf.mUart0.ReceiveBuf[2] & 0x08)     //��������
		{
			api_Uart0_SendCMDData(0,0,1);
			mMaininf.mSystem.mDelayStartTime = api_GetCurrentSystemTime();
			while(api_GetSystemTimePeriod(mMaininf.mSystem.mDelayStartTime) < 3);
			mMaininf.mWork.mWorkKey = 1; 
		}
	}
}


























