/****************************************************************************************************/
/*			@FileName		:	DISPLAY.h																																		   	*/
/*			@Author			:	zhengbaiqiu																																		*/
/*			@Version		:	v1.0																																					*/
/*			@Date			:	2019.11.12  																																	*/
/****************************************************************************************************/

#ifndef	__DISPLAY_H__
#define	__DISPLAY_H__
#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	DROP = 0x01,                 //下视             //1
	
	BUMP,                        //碰撞             //2
	CODE,                        //码盘             //3
	LEAVELAND,                   //离地             //4
	
	GUIDE,                       //引导             //5
	POWERKEY,                    //电源键           //6
	
	DUST,                        //灰尘             //7
	DUSTBOX,                     //尘盒             //8
	
	WORKLED,                     //工作指示灯       //9
	GUIDE_SENDA,                 //充电座A信号      //10
	GUIDE_SENDB,                 //充电座B信号      //11
	GUIDE_SENDTOP,               //防撞             //12
	
	PREVENT_BUMP45,              //45度小板         //13
	ALONG_SIDEA,                 //沿边A            //14
	ALONG_SIDEB,                 //沿边B            //15
	
	ALL_TEST                     //全部测试项       //16
} TEST_MODE_PART;


void Display_Test(u8 test_mode,u8 test_cmd,u8 test_status);
	
	
#ifdef _cplusplus
extern }
#endif

#endif /*__DISPLAY_H__*/	


