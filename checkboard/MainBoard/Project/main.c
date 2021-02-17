/****************************************************************************************************/
/*			@FileName		:	MAIN.c															  	*/
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
#include "string.h"

/* Project ---------------------------------------------------------------------*/
#include "maindef.h"
#include "bsp.h"
#include "apiLib.h"

/* Variable --------------------------------------------------------------------*/
_ST_MAINMANAGER mMaininf;

/* Function --------------------------------------------------------------------*/




/*****************************************************************************
 * @name       :int main(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :main
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
int main(void)
{
	prvSetupHardware();
	
	api_MachinePowerOn();
	while(1)
	{
		Scan_Key();
		api_Test_PCB(mMaininf.mWork.mWorkMODE);
	}
}
















































