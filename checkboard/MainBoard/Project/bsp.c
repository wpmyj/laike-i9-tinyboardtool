/****************************************************************************************************/
/*			@FileName		:	bsp.c															  	*/
/*			@Author			:	zhengbaiqiu															*/
/*			@Version		:	v1.0																*/
/*			@Date			:	2019.11.12  														*/
/****************************************************************************************************/


/* Includes --------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32f5xxxx_01.h"
#include "ht32_cm0plus_misc.h"
#include "ht32f5xxxx_ckcu.h"
#include "ht32f5xxxx_gpio.h"
#include "ht32f5xxxx_exti.h"
#include "string.h"

/* Project ---------------------------------------------------------------------*/
#include "maindef.h"
#include "bsp.h"

/* Variable --------------------------------------------------------------------*/
/* Function --------------------------------------------------------------------*/
void prvSetupHardware(void);
void PWM_Configuration(u8 mFlag);



/*****************************************************************************
 * @name       :void NVIC_Configuration(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :nvic configuration
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_SetVectorTable(NVIC_VECTTABLE_FLASH, 0x0);     /* Set the Vector Table base location at 0x00000000   */
}

/*****************************************************************************
 * @name       :void CKCU_Configuration(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Configure the system clocks.
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void CKCU_Configuration(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
// 	CKCUClock.Bit.PDMA       = 0;
// 	CKCUClock.Bit.USBD       = 0;
// 	CKCUClock.Bit.CKREF      = 0;
// 	CKCUClock.Bit.EBI        = 0;
// 	CKCUClock.Bit.CRC        = 0;
	CKCUClock.Bit.PA         = 1;
	CKCUClock.Bit.PB         = 1;
	CKCUClock.Bit.PC         = 1;
// 	CKCUClock.Bit.PD         = 0;
// 	CKCUClock.Bit.I2C0       = 0;
// 	CKCUClock.Bit.I2C1       = 0;
// 	CKCUClock.Bit.SPI0       = 0;
// 	CKCUClock.Bit.SPI1       = 0;
// 	CKCUClock.Bit.USART0     = 1;
// 	CKCUClock.Bit.USART1     = 1;
	CKCUClock.Bit.UART0      = 1;
	CKCUClock.Bit.UART1      = 1;
	CKCUClock.Bit.AFIO       = 1;
	CKCUClock.Bit.EXTI       = 1;
// 	CKCUClock.Bit.SCI0       = 0;
// 	CKCUClock.Bit.SCI1       = 0;
// 	CKCUClock.Bit.I2S        = 0;
// 	CKCUClock.Bit.MCTM0      = 0;
// 	CKCUClock.Bit.WDT        = 0;
// 	CKCUClock.Bit.BKP        = 0;
// 	CKCUClock.Bit.GPTM0      = 0;
// 	CKCUClock.Bit.GPTM1      = 0;
	CKCUClock.Bit.BFTM0      = 1;
	CKCUClock.Bit.BFTM1      = 1;
// 	CKCUClock.Bit.CMP        = 0;
	CKCUClock.Bit.ADC        = 1;
	CKCUClock.Bit.SCTM0      = 1;
	CKCUClock.Bit.SCTM1      = 1;
	CKCUClock.Bit.SCTM2      = 1;
	CKCUClock.Bit.SCTM3      = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
}

/*****************************************************************************
 * @name       :void TIME_Configuration(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Configure the Time.
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TIME_Configuration(void)
{
	//计时器 20ms
	NVIC_EnableIRQ(BFTM0_IRQn);
	
	BFTM_SetCompare(HT_BFTM0, SystemCoreClock / 50);
	BFTM_SetCounter(HT_BFTM0, 0);
	BFTM_IntConfig(HT_BFTM0, ENABLE);
	BFTM_EnaCmd(HT_BFTM0, ENABLE);
	
	//计时器 1ms
	NVIC_EnableIRQ(BFTM1_IRQn);
	
	BFTM_SetCompare(HT_BFTM1, SystemCoreClock / 1000);
	BFTM_SetCounter(HT_BFTM1, 0);
	BFTM_IntConfig(HT_BFTM1, ENABLE);
	BFTM_EnaCmd(HT_BFTM1, ENABLE);
}

/*****************************************************************************
 * @name       :void UART_Configuration(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Configure the Uart.
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void UART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	
	//UART0 显示板通讯
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_6, AFIO_FUN_USART_UART);
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_7, AFIO_FUN_USART_UART);
	
	/* HT_UART0 configured as follow:
				- BaudRate = 115200 baud
				- Word Length = 8 Bits
				- One Stop Bit
				- None parity bit
	*/
	USART_DeInit(HT_UART0);
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
	USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure.USART_Parity = USART_PARITY_NO;
	USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
	USART_Init(HT_UART0, &USART_InitStructure);
	
	/* Seting HT_UART0 interrupt-flag*/
	USART_IntConfig(HT_UART0, USART_INT_RXDR, ENABLE);
	/* Enable HT_UART0*/
	USART_TxCmd(HT_UART0, ENABLE);
	USART_RxCmd(HT_UART0, ENABLE);
	/* Configure HT_UART0 interrupt*/
	NVIC_EnableIRQ(UART0_IRQn);
	
	
// 	//UART1 调试串口
// 	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_3, AFIO_FUN_USART_UART);
// 	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_4, AFIO_FUN_USART_UART);
// 	
// 	/* HT_UART1 configured as follow:
// 				- BaudRate = 115200 baud
// 				- Word Length = 8 Bits
// 				- One Stop Bit
// 				- None parity bit
// 	*/
// 	USART_DeInit(HT_UART1);
// 	USART_InitStructure.USART_BaudRate = 115200;
// 	USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
// 	USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
// 	USART_InitStructure.USART_Parity = USART_PARITY_NO;
// 	USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
// 	USART_Init(HT_UART1, &USART_InitStructure);
// 	
// 	/* Seting HT_UART1 interrupt-flag*/
// 	USART_IntConfig(HT_UART1, USART_INT_RXDR, ENABLE);
// 	/* Enable HT_UART1*/
// 	USART_TxCmd(HT_UART1, ENABLE);
// 	USART_RxCmd(HT_UART1, ENABLE);
// 	/* Configure HT_UART1 interrupt*/
// 	NVIC_EnableIRQ(UART1_IRQn);
}

/*****************************************************************************
 * @name       :void PWM_Configuration(u8 mFlag)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Configure the pwm.
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void PWM_Configuration(u8 mFlag)
{
	TM_TimeBaseInitTypeDef TM_TimeBaseInitStructure;
	TM_OutputInitTypeDef TM_OutputInitStructure;
//	TM_CaptureInitTypeDef TM_CapInitStructure;
	
	if(mFlag == TRUE)
	{
		AFIO_GPxConfig(GPIO_PC, AFIO_PIN_0, AFIO_FUN_SCTM);
		
		TM_TimeBaseInitStructure.CounterReload = 26;
		TM_TimeBaseInitStructure.Prescaler = 39;
		TM_TimeBaseInitStructure.RepetitionCounter = 0;
		TM_TimeBaseInitStructure.CounterMode = TM_CNT_MODE_UP;
		TM_TimeBaseInitStructure.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
		TM_TimeBaseInit(HT_SCTM3, &TM_TimeBaseInitStructure);
		/* Force channel output reference to inactive level */
		TM_OutputModeConfig(HT_SCTM3, TM_CH_0, TM_OM_FORCED_INACTIVE);
		//Duty Set
		TM_OutputInitStructure.Channel = TM_CH_0;
		TM_OutputInitStructure.OutputMode = TM_OM_PWM1;
		TM_OutputInitStructure.Control = TM_CHCTL_ENABLE;
		TM_OutputInitStructure.ControlN = TM_CHCTL_DISABLE;//TM_CHCTL_ENABLE;
		TM_OutputInitStructure.Polarity = TM_CHP_NONINVERTED;
		TM_OutputInitStructure.PolarityN = TM_CHP_NONINVERTED;
		TM_OutputInitStructure.Compare = 0;
		TM_OutputInit(HT_SCTM3, &TM_OutputInitStructure);
		TM_Cmd(HT_SCTM3, ENABLE);
	}
	else
	{
		//关闭引导PWM
		TM_Cmd(HT_SCTM3, DISABLE);
		AFIO_GPxConfig(GPIO_PC, AFIO_PIN_0, AFIO_FUN_GPIO);
		GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_0, GPIO_DIR_OUT);
		GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_0, RESET);
	}
}

/*****************************************************************************
 * @name       :void UART_Configuration(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Configure the Uart.
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void ADC_Configuration(u8 mFlag)
{
	if(mFlag == TRUE)
	{
		//正常工作
		/* Enable the ADC Interrupts                                                                              */
		NVIC_EnableIRQ(ADC_IRQn);
		
		ADC_DeInit(HT_ADC);
		
		/* ADCLK frequency is set to CK_AHB/64                                                                    */
		CKCU_SetADCPrescaler(CKCU_ADCPRE_DIV64);

		/* Config AFIO mode as ADC function                                                                       */
		AFIO_GPxConfig(GPIO_PA, AFIO_PIN_0 | AFIO_PIN_1 | AFIO_PIN_3 | AFIO_PIN_4 /* | AFIO_PIN_6*/, AFIO_FUN_ADC);
//		AFIO_GPxConfig(GPIO_PB, AFIO_PIN_7, AFIO_FUN_ADC);
		
		/* Continuous Mode, Length 7, SubLength 7                                                                 */
		ADC_RegularGroupConfig(HT_ADC, CONTINUOUS_MODE, 4, 4);

		/* ADC Channel n, Rank 0, Sampling clock is (1.5 + 0) ADCLK
			 Conversion time = (sampling clock + 12.5) / ADCLK = 12.4 uS */
		ADC_RegularChannelConfig(HT_ADC, ADC_CH_3, 0);//充电座A信号
		ADC_RegularChannelConfig(HT_ADC, ADC_CH_2, 1);//充电座B信号
		ADC_RegularChannelConfig(HT_ADC, ADC_CH_6, 2);//充电座TOP信号
		ADC_RegularChannelConfig(HT_ADC, ADC_CH_5, 3);//充电座5V
//		ADC_RegularChannelConfig(HT_ADC, ADC_CH_8, 4);//灰尘检测传感器
		
//		ADC_RegularChannelConfig(HT_ADC, ADC_CH_0, 4);//沿边接收


		/* Use Software Trigger as ADC trigger source                                                             */
		ADC_RegularTrigConfig(HT_ADC, ADC_TRIG_SOFTWARE);

		/* Enable ADC single end of conversion interrupt,
			 The ADC ISR will store the ADC result into global variable gPotentiometerLevel. */
		ADC_IntConfig(HT_ADC, ADC_INT_SINGLE_EOC, ENABLE);

		ADC_Cmd(HT_ADC, ENABLE);

		/* Software trigger to start continuous mode                                                              */
		ADC_SoftwareStartConvCmd(HT_ADC, ENABLE);
	}
	else
	{
		//休眠
		/* Enable the ADC Interrupts                                                                              */
		NVIC_DisableIRQ(ADC_IRQn);
		ADC_IntConfig(HT_ADC, ADC_INT_SINGLE_EOC, DISABLE);
		ADC_Cmd(HT_ADC, DISABLE);
		ADC_SoftwareStartConvCmd(HT_ADC, DISABLE);
		/* Config AFIO mode as GPIO function                                                                       */
		AFIO_GPxConfig(GPIO_PA, AFIO_PIN_0 | AFIO_PIN_1 | AFIO_PIN_3 | AFIO_PIN_4/* | AFIO_PIN_6*/, AFIO_FUN_GPIO);
		GPIO_DirectionConfig(HT_GPIOA, AFIO_PIN_0 | AFIO_PIN_1 | AFIO_PIN_3 | AFIO_PIN_4/* | AFIO_PIN_6*/, GPIO_DIR_IN);
		GPIO_PullResistorConfig(HT_GPIOA, AFIO_PIN_0 | AFIO_PIN_1 | AFIO_PIN_3 | AFIO_PIN_4/* | AFIO_PIN_6*/, GPIO_PR_DOWN);
		GPIO_InputConfig(HT_GPIOA, AFIO_PIN_0 | AFIO_PIN_1 | AFIO_PIN_3 | AFIO_PIN_4/* | AFIO_PIN_6*/, ENABLE);
		
// 		AFIO_GPxConfig(GPIO_PB, AFIO_PIN_7, AFIO_FUN_GPIO);
// 		GPIO_DirectionConfig(HT_GPIOB, AFIO_PIN_7, GPIO_DIR_IN);
// 		GPIO_PullResistorConfig(HT_GPIOB, AFIO_PIN_7, GPIO_PR_DOWN);
// 		GPIO_InputConfig(HT_GPIOB, AFIO_PIN_7, ENABLE);
	}
}

/*****************************************************************************
 * @name       :void GPIO_Configuration(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Configure the Gpio.
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void GPIO_Configuration(void)
{
	//发射 PA2 PA5 PB14 PA13 PC4 PA10 PB0 PA14 PC0 PC1 PC2 PC3
	//接收 PB5 PA8 PB10 PB12 PA11 PB2 PB9 PB15 PB13 PB6     PA6 PB7
	EXTI_InitTypeDef EXTI_InitStruct = {0};
	
	//免碰撞45度发射
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_2, AFIO_FUN_GPIO);//PA2
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_2, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_2, RESET);

	//沿边A发射
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_5, AFIO_FUN_GPIO);//PA5
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_5, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_5, RESET);

	//沿边B发射
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_14, AFIO_FUN_GPIO);//PB14
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_14, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_14, RESET);

	#ifndef zbq_Debug
	//灰尘发射
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_13, AFIO_FUN_GPIO);//PA13
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_13, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_13, RESET);
	#endif

	//码盘发射
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_4, AFIO_FUN_GPIO);//PC4
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_4, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_4, RESET);
	
	//前下视发射
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_10, AFIO_FUN_GPIO);//PA10
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_10, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_10, RESET);
	
	//右下视、左下视发射
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_0, AFIO_FUN_GPIO);//PB0
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_0, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_0, RESET);
	
	//后下视发射
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_14, AFIO_FUN_GPIO);//PA14
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_14, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_14, RESET);
	
	//引导发射
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_0, AFIO_FUN_GPIO);//PC0
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_0, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_0, RESET);
	
	//充电座引导A发射
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_1, AFIO_FUN_GPIO);//PC1
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_1, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_1, RESET);
	
	//充电座引导B发射
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_2, AFIO_FUN_GPIO);//PC2
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_2, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_2, RESET);
	
	//充电座引导TOP发射
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_3, AFIO_FUN_GPIO);//PC3
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_3, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3, RESET);
	
	//碰撞加尘盒霍尔发射
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_15, AFIO_FUN_GPIO);//PA15
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_15, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_15, SET);      //初始化高电平
	
	


	//免碰撞45度检测
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_8, AFIO_FUN_GPIO);//PB8
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_8, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_8, GPIO_PR_DISABLE);
	GPIO_InputConfig(HT_GPIOB, GPIO_PIN_8, ENABLE);
	
	
	//碰撞检测
	//左、右
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_5, AFIO_FUN_GPIO);//PB5
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_5, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_5, GPIO_PR_DISABLE);  //为了区分引脚悬空与检测引脚低电平触发，改为内部上拉
	GPIO_InputConfig(HT_GPIOB, GPIO_PIN_5, ENABLE);

// 	AFIO_EXTISourceConfig(AFIO_EXTI_CH_5, AFIO_ESS_PB);
// 	/* Configure button EXTI Line on falling edge*/
// 	EXTI_InitStruct.EXTI_Channel     = EXTI_CHANNEL_5;
// 	EXTI_InitStruct.EXTI_Debounce    = EXTI_DEBOUNCE_ENABLE;
// 	EXTI_InitStruct.EXTI_DebounceCnt = 40;/* 16bits counter, 0x0 ~ 0xFFFF*/
// 	EXTI_InitStruct.EXTI_IntType     = EXTI_POSITIVE_EDGE;
// 	EXTI_Init(&EXTI_InitStruct);
// 	/* Enable Button EXTI Interrupt*/
// 	EXTI_IntConfig(EXTI_CHANNEL_5, ENABLE);
	
	
	//离地检测
	//左右
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_8, AFIO_FUN_GPIO);//PA8
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_8, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_8, GPIO_PR_DISABLE);
	GPIO_InputConfig(HT_GPIOA, GPIO_PIN_8, ENABLE);
	
// 	AFIO_EXTISourceConfig(AFIO_EXTI_CH_8, AFIO_ESS_PA);
// 	/* Configure button EXTI Line on falling edge*/
// 	EXTI_InitStruct.EXTI_Channel     = EXTI_CHANNEL_8;
// 	EXTI_InitStruct.EXTI_Debounce    = EXTI_DEBOUNCE_ENABLE;
// 	EXTI_InitStruct.EXTI_DebounceCnt = 40;/* 16bits counter, 0x0 ~ 0xFFFF*/
// 	EXTI_InitStruct.EXTI_IntType     = EXTI_NEGATIVE_EDGE;
// 	EXTI_Init(&EXTI_InitStruct);
// 	/* Enable Button EXTI Interrupt*/
// 	EXTI_IntConfig(EXTI_CHANNEL_8, ENABLE);


	//尘盒检测
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_10, AFIO_FUN_GPIO);//PB10
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_10, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_10, GPIO_PR_DISABLE);
	GPIO_InputConfig(HT_GPIOB, GPIO_PIN_10, ENABLE);
	
// 	AFIO_EXTISourceConfig(AFIO_EXTI_CH_10, AFIO_ESS_PB);
// 	/* Configure button EXTI Line on falling edge*/
// 	EXTI_InitStruct.EXTI_Channel     = EXTI_CHANNEL_10;
// 	EXTI_InitStruct.EXTI_Debounce    = EXTI_DEBOUNCE_ENABLE;
// 	EXTI_InitStruct.EXTI_DebounceCnt = 40;/* 16bits counter, 0x0 ~ 0xFFFF*/
// 	EXTI_InitStruct.EXTI_IntType     = EXTI_NEGATIVE_EDGE;
// 	EXTI_Init(&EXTI_InitStruct);
// 	/* Enable Button EXTI Interrupt*/
// 	EXTI_IntConfig(EXTI_CHANNEL_10, ENABLE);
	
	
	//码盘检测
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_12, AFIO_FUN_GPIO);//PB12
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_12, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_12, GPIO_PR_DISABLE);
	GPIO_InputConfig(HT_GPIOB, GPIO_PIN_12, ENABLE);
	
// 	AFIO_EXTISourceConfig(AFIO_EXTI_CH_12, AFIO_ESS_PB);
// 	/* Configure button EXTI Line on falling edge*/
// 	EXTI_InitStruct.EXTI_Channel     = EXTI_CHANNEL_12;
// 	EXTI_InitStruct.EXTI_Debounce    = EXTI_DEBOUNCE_ENABLE;
// 	EXTI_InitStruct.EXTI_DebounceCnt = 40;/* 16bits counter, 0x0 ~ 0xFFFF*/
// 	EXTI_InitStruct.EXTI_IntType     = EXTI_NEGATIVE_EDGE;
// 	EXTI_Init(&EXTI_InitStruct);
// 	/* Enable Button EXTI Interrupt*/
// 	EXTI_IntConfig(EXTI_CHANNEL_12, ENABLE);
	
	
	//电源按键检测
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_11, AFIO_FUN_GPIO);//PA11
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_11, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_11, GPIO_PR_DISABLE);
	GPIO_InputConfig(HT_GPIOA, GPIO_PIN_11, ENABLE);
	
// 	AFIO_EXTISourceConfig(AFIO_EXTI_CH_11, AFIO_ESS_PA);
// 	/* Configure button EXTI Line on falling edge*/
// 	EXTI_InitStruct.EXTI_Channel     = EXTI_CHANNEL_11;
// 	EXTI_InitStruct.EXTI_Debounce    = EXTI_DEBOUNCE_ENABLE;
// 	EXTI_InitStruct.EXTI_DebounceCnt = 40;/* 16bits counter, 0x0 ~ 0xFFFF*/
// 	EXTI_InitStruct.EXTI_IntType     = EXTI_NEGATIVE_EDGE;
// 	EXTI_Init(&EXTI_InitStruct);
// 	/* Enable Button EXTI Interrupt*/
// 	EXTI_IntConfig(EXTI_CHANNEL_11, ENABLE);
	
	
	
	
	
	
	/* Configure AFIO mode of input pins                                                                      */
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_2, AFIO_FUN_GPIO);//PB2

	/* Enable GPIO Input Function                                                                             */
	GPIO_InputConfig(HT_GPIOB, GPIO_PIN_2, ENABLE);

	/* Configure GPIO pull resistor of input pins                                                             */
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_2, GPIO_PR_DISABLE);

	/* Select Port as EXTI Trigger Source                                                                     */
	AFIO_EXTISourceConfig(AFIO_EXTI_CH_2, AFIO_ESS_PB);

	{ /* Configure EXTI Channel n as rising edge trigger                                                      */

		/* !!! NOTICE !!!
		   Notice that the local variable (structure) did not have an initial value.
		   Please confirm that there are no missing members in the parameter settings below in this function.
		*/
		EXTI_InitTypeDef EXTI_InitStruct;
		EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_2;
		EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;
		EXTI_InitStruct.EXTI_DebounceCnt = 0;/* 16bits counter, 0x0 ~ 0xFFFF*/
		EXTI_InitStruct.EXTI_IntType = EXTI_BOTH_EDGE;
		EXTI_Init(&EXTI_InitStruct);
	}

	/* Enable EXTI & NVIC line Interrupt                                                                      */
	EXTI_IntConfig(EXTI_CHANNEL_2, DISABLE);
//	NVIC_EnableIRQ(EXTI2_3_IRQn);
	
	//下视检测
// 	//前下视
// 	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_2, AFIO_FUN_GPIO);//PB2
// 	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_2, GPIO_DIR_IN);
// 	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_2, GPIO_PR_DISABLE);
// 	GPIO_InputConfig(HT_GPIOB, GPIO_PIN_2, ENABLE);
// 	
// 	AFIO_EXTISourceConfig(AFIO_EXTI_CH_2, AFIO_ESS_PB);
// 	/* Configure button EXTI Line on falling edge*/
// 	EXTI_InitStruct.EXTI_Channel     = EXTI_CHANNEL_2;
// 	EXTI_InitStruct.EXTI_Debounce    = EXTI_DEBOUNCE_ENABLE;
// 	EXTI_InitStruct.EXTI_DebounceCnt = 40;/* 16bits counter, 0x0 ~ 0xFFFF*/
// 	EXTI_InitStruct.EXTI_IntType     = EXTI_BOTH_EDGE;
// 	EXTI_Init(&EXTI_InitStruct);
// 	/* Enable Button EXTI Interrupt*/
// //	EXTI_IntConfig(EXTI_CHANNEL_2, DISABLE);
// 	EXTI_IntConfig(EXTI_CHANNEL_2, ENABLE);
	
	
	//左、右下视
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_9, AFIO_FUN_GPIO);//PB9
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_9, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_9, GPIO_PR_DISABLE);
	GPIO_InputConfig(HT_GPIOB, GPIO_PIN_9, ENABLE);
	
// 	AFIO_EXTISourceConfig(AFIO_EXTI_CH_9, AFIO_ESS_PB);
// 	/* Configure button EXTI Line on falling edge*/
// 	EXTI_InitStruct.EXTI_Channel     = EXTI_CHANNEL_9;
// 	EXTI_InitStruct.EXTI_Debounce    = EXTI_DEBOUNCE_ENABLE;
// 	EXTI_InitStruct.EXTI_DebounceCnt = 40;/* 16bits counter, 0x0 ~ 0xFFFF*/
// 	EXTI_InitStruct.EXTI_IntType     = EXTI_NEGATIVE_EDGE;
// 	EXTI_Init(&EXTI_InitStruct);
// 	/* Enable Button EXTI Interrupt*/
// 	EXTI_IntConfig(EXTI_CHANNEL_9, ENABLE);
	
	
	//后下视
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_15, AFIO_FUN_GPIO);//PB15
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_15, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_15, GPIO_PR_DISABLE);
	GPIO_InputConfig(HT_GPIOB, GPIO_PIN_15, ENABLE);
	
// 	AFIO_EXTISourceConfig(AFIO_EXTI_CH_15, AFIO_ESS_PB);
// 	/* Configure button EXTI Line on falling edge*/
// 	EXTI_InitStruct.EXTI_Channel     = EXTI_CHANNEL_15;
// 	EXTI_InitStruct.EXTI_Debounce    = EXTI_DEBOUNCE_ENABLE;
// 	EXTI_InitStruct.EXTI_DebounceCnt = 40;/* 16bits counter, 0x0 ~ 0xFFFF*/
// 	EXTI_InitStruct.EXTI_IntType     = EXTI_NEGATIVE_EDGE;
// 	EXTI_Init(&EXTI_InitStruct);
// 	/* Enable Button EXTI Interrupt*/
// 	EXTI_IntConfig(EXTI_CHANNEL_15, ENABLE);
	
	
	//引导检测
	//左、右引导
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_13, AFIO_FUN_GPIO);//PB13
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_13, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_13, GPIO_PR_DISABLE);
	GPIO_InputConfig(HT_GPIOB, GPIO_PIN_13, ENABLE);
	
	AFIO_EXTISourceConfig(AFIO_EXTI_CH_13, AFIO_ESS_PB);
	/* Configure button EXTI Line on falling edge*/
	EXTI_InitStruct.EXTI_Channel     = EXTI_CHANNEL_13;
	EXTI_InitStruct.EXTI_Debounce    = EXTI_DEBOUNCE_ENABLE;
	EXTI_InitStruct.EXTI_DebounceCnt = 40;/* 16bits counter, 0x0 ~ 0xFFFF*/
	EXTI_InitStruct.EXTI_IntType     = EXTI_BOTH_EDGE;
	EXTI_Init(&EXTI_InitStruct);
	/* Enable Button EXTI Interrupt*/
	EXTI_IntConfig(EXTI_CHANNEL_13, ENABLE);
	
	
	//后引导
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_6, AFIO_FUN_GPIO);//PB6
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_6, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_6, GPIO_PR_DISABLE);
	GPIO_InputConfig(HT_GPIOB, GPIO_PIN_6, ENABLE);
	
	AFIO_EXTISourceConfig(AFIO_EXTI_CH_6, AFIO_ESS_PB);
	/* Configure button EXTI Line on falling edge*/
	EXTI_InitStruct.EXTI_Channel     = EXTI_CHANNEL_6;
	EXTI_InitStruct.EXTI_Debounce    = EXTI_DEBOUNCE_ENABLE;
	EXTI_InitStruct.EXTI_DebounceCnt = 40;/* 16bits counter, 0x0 ~ 0xFFFF*/
	EXTI_InitStruct.EXTI_IntType     = EXTI_BOTH_EDGE;
	EXTI_Init(&EXTI_InitStruct);
	/* Enable Button EXTI Interrupt*/
	EXTI_IntConfig(EXTI_CHANNEL_6, ENABLE);
	
	NVIC_EnableIRQ(EXTI4_15_IRQn);
	
// 	//充电座信号检测
// 	//引导信号A
// 	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_1, AFIO_FUN_GPIO);//PA1
// 	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_1, GPIO_DIR_IN);
// 	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_1, GPIO_PR_DISABLE);
// 	GPIO_InputConfig(HT_GPIOA, GPIO_PIN_1, ENABLE);
// 	
// 	//引导信号B
// 	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_0, AFIO_FUN_GPIO);//PA0
// 	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_0, GPIO_DIR_IN);
// 	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_0, GPIO_PR_DISABLE);
// 	GPIO_InputConfig(HT_GPIOA, GPIO_PIN_0, ENABLE);
// 	
// 	//引导信号TOP
// 	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_4, AFIO_FUN_GPIO);//PA4
// 	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_4, GPIO_DIR_IN);
// 	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_4, GPIO_PR_DISABLE);
// 	GPIO_InputConfig(HT_GPIOA, GPIO_PIN_4, ENABLE);

	//灰尘检测
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_6, AFIO_FUN_GPIO);//PA6
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_6, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_6, GPIO_PR_DISABLE);
	GPIO_InputConfig(HT_GPIOA, GPIO_PIN_6, ENABLE);
	
	//沿边检测
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_7, AFIO_FUN_GPIO);//PA6
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_7, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_7, GPIO_PR_DISABLE);
	GPIO_InputConfig(HT_GPIOB, GPIO_PIN_7, ENABLE);
}

/*****************************************************************************
 * @name       :void BSP_Init(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Chip configuration
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void BSP_Init(void)
{
	NVIC_Configuration();
	CKCU_Configuration();
	TIME_Configuration();
	UART_Configuration();
	ADC_Configuration(TRUE);
	GPIO_Configuration();
}

/*****************************************************************************
 * @name       :void Param_Init(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Parameter initialization
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Param_Init(void)
{
	//    系统时间
	mMaininf.mSystem.mSystemTick = 0;
	mMaininf.mSystem.mPowerOnStartTime = 0;
	mMaininf.mSystem.mCheckStartTime = 0;
	mMaininf.mSystem.mPowerOnTime = 0;
	mMaininf.mSystem.mReadModeStartTime = 0;
	mMaininf.mSystem.mDelayStartTime = 0;
	mMaininf.mSystem.mGuideStartTime = 0;
	mMaininf.mSystem.mChargeStartTime = 0;
	mMaininf.mSystem.mKeyStartTime = 0;
	mMaininf.mSystem.mGuideStartTimeT1 = 0;
	mMaininf.mSystem.mGuideStartTimeT2 = 0;
	mMaininf.mSystem.mGuideStartTimeT3 = 0;
	
	//    串口参数
	memset(mMaininf.mUart0.TxBuffer, 0, sizeof(mMaininf.mUart0.TxBuffer));     //  int arr[10]={0};
	memset(mMaininf.mUart0.RxBuffer, 0, sizeof(mMaininf.mUart0.RxBuffer));
	memset(mMaininf.mUart0.SendBuf, 0, sizeof(mMaininf.mUart0.SendBuf));
	memset(mMaininf.mUart0.ReceiveBuf, 0, sizeof(mMaininf.mUart0.ReceiveBuf));
	
	mMaininf.mUart0.mSendSize = 0;
	mMaininf.mUart0.mSendCont = 0;
	mMaininf.mUart0.mReceiveStep = ST_0x55;
	mMaininf.mUart0.mReceiveBCC = 0;
	mMaininf.mUart0.mReceiveSize = 0;
	mMaininf.mUart0.mReceiveCont = 0;
	mMaininf.mUart0.mReceiveTimeOut = 0;
	mMaininf.mUart0.mReceiveFlag = 0;      //    没有接收导数据
	
	//    工作模式
	mMaininf.mWork.mWorkMODE = 0;
	
	mMaininf.mWork.mReceiveStatus = 0;
	mMaininf.mWork.mWorkChecking = 0;
	mMaininf.mWork.mCodeReceiveStatus = 0;
	
	mMaininf.mWork.mGuideBackStatus = 0;
	mMaininf.mWork.mAvoidBumpStatus = 0;
	mMaininf.mWork.mAlongSideStatus = 0;
	mMaininf.mWork.mGuideRightLeftStatus = 0;
	mMaininf.mWork.mChargeBaseGuideAStatus = 0;
	mMaininf.mWork.mChargeBaseGuideBStatus = 0;
	mMaininf.mWork.mChargeBaseGuideTOPStatus = 0;
	mMaininf.mWork.mBumpStatus = 0;
	mMaininf.mWork.mDustBoxStatus = 0;
	
	mMaininf.mWork.mKeyStatus = 0;
	mMaininf.mWork.mKeyCont = 0;
	
	mMaininf.mWork.mWorkCont = 0;
	mMaininf.mWork.mGuideCont = 0;
	
	mMaininf.mWork.GuideFirstFlag = 0;
	
	mMaininf.mWork.mGuideSendFlag = 0;
	mMaininf.mWork.mGuideSendChangeFlag = 0;
	mMaininf.mWork.mGuideSendTime = 0;
	
	mMaininf.mWork.mWorkKey = 0;
	mMaininf.mWork.mCheckStatusFlag = 0;
	
	mMaininf.mWork.mFrontDropFlag = 0;           //未触发
	mMaininf.mWork.mLeftRightDropFlag = 0;
	mMaininf.mWork.mBackDropFlag = 0;
	mMaininf.mWork.mLeftRightBumpFlag = 0;
	mMaininf.mWork.mLeftRightLeaveFlag = 0;
	mMaininf.mWork.mDustBoxFlag = 0;
	mMaininf.mWork.mLeftRightCodeFlag = 0;
	mMaininf.mWork.mPowerKeyFlag = 0;
	mMaininf.mWork.mLeftRightGuideFlag = 0;
	mMaininf.mWork.mBackGuideFlag = 0;
	mMaininf.mWork.mWorkLedFlag = 0;
	mMaininf.mWork.mChargeBaseGuideAFlag = 0;
	mMaininf.mWork.mChargeBaseGuideBFlag = 0;
	mMaininf.mWork.mChargeBaseGuideTopFlag = 0;
	mMaininf.mWork.m45BoardFlag = 0;
	mMaininf.mWork.mAlongSideAFlag = 0;
	mMaininf.mWork.mAlongSideBFlag = 0;
	
	mMaininf.mWork.mDust3V3Flag = 0;
	mMaininf.mWork.mDustFlag = 0;
	
// 	mMaininf.mWork.mFrontDropModeleEnable = 0;   //未使能
// 	mMaininf.mWork.mWheelWireModeleEnable = 0;
// 	mMaininf.mWork.mPowerKeyModeleEnable = 0;
// 	mMaininf.mWork.mDustBoxModeleEnable = 0;
// 	mMaininf.mWork.mChargeBaseModeleEnable = 0;
// 	mMaininf.mWork.mAlongSideModeleEnable = 0;
// 	
	mMaininf.mWork.mFrontDropCheckEnable = 0;            //未使能
	mMaininf.mWork.mLeftRightDropCheckEnable = 0;
	mMaininf.mWork.mBackDropCheckEnable = 0;
	mMaininf.mWork.mLeftRightBumpCheckEnable = 0;
	mMaininf.mWork.mLeftRightLeaveCheckEnable = 0;
	mMaininf.mWork.mDustBoxCheckEnable = 0;
	mMaininf.mWork.mLeftRightCodeCheckEnable = 0;
	mMaininf.mWork.mPowerKeyCheckEnable = 0;
	mMaininf.mWork.mLeftRightGuideCheckEnable = 0;
	mMaininf.mWork.mBackGuideCheckEnable = 0;
	mMaininf.mWork.mDustCheckEnable = 0;
	mMaininf.mWork.mChargeBaseCheckEnable = 0;
	mMaininf.mWork.mAlongSideCheckEnable = 0;
	
	
	mMaininf.mWork.mDropFrontStatusCont = 0;
	mMaininf.mWork.mDropLeftRightStatusCont = 0;
	mMaininf.mWork.mDropBackStatusCont = 0;
	mMaininf.mWork.mCodeLeftRightStatusCont = 0;
	mMaininf.mWork.mDustStatusCont = 0;
	mMaininf.mWork.mChargeBaseGuideAStatusCont = 0;
	mMaininf.mWork.mChargeBaseGuideBStatusCont = 0;
	mMaininf.mWork.mChargeBaseGuideTOPStatusCont = 0;
	mMaininf.mWork.mAvoidBumpStatusCont = 0;
	mMaininf.mWork.mAlongSideStatusCont = 0;
	mMaininf.mWork.mBumpStatusCont = 0;
	mMaininf.mWork.mDustBoxStatusCont = 0;
	
	mMaininf.mWork.mFrontDropFlagCont = 0;
	mMaininf.mWork.mLeftRightDropFlagCont = 0;
	mMaininf.mWork.mBackDropFlagCont = 0;
	mMaininf.mWork.mLeftRightCodeFlagCont = 0;
	mMaininf.mWork.mLeftRightLeaveFlagCont = 0;
	mMaininf.mWork.mDustFlagCont = 0;
	mMaininf.mWork.mWorkLedFlagCont = 0;
	mMaininf.mWork.mChargeBaseGuideAFlagCont = 0;
	mMaininf.mWork.mChargeBaseGuideBFlagCont = 0;
	mMaininf.mWork.mChargeBaseGuideTopFlagCont = 0;
	mMaininf.mWork.m45BoardFlagCont = 0;
	mMaininf.mWork.mLeftRightBumpFlagCont = 0;
	mMaininf.mWork.mDustBoxFlagCont = 0;
	mMaininf.mWork.mDustBoxHightFlag = 0;
	mMaininf.mWork.mDustBoxHightFlagCont = 0;
	mMaininf.mWork.mDustBoxUartSendFlag = 0;
	
	
	mMaininf.mWork.mChargeBaseGuideFlag = 0;
	
	mMaininf.mWork.mChargeBaseGuideCont = 0;
	
	
	mMaininf.mWork.mSensorADC[0] = 0;
	mMaininf.mWork.mSensorADC[1] = 0;
	mMaininf.mWork.mSensorADC[2] = 0;
	mMaininf.mWork.mSensorADC[3] = 0;
	mMaininf.mWork.mSensorADC[4] = 0;
	mMaininf.mWork.mSensorADC[5] = 0;
}

/*****************************************************************************
 * @name       :void prvSetupHardware(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Initialize the chip
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void prvSetupHardware(void)
{
	BSP_Init();
	Param_Init();
}








































