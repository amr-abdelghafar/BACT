/*
 * BTN_CFG.c
 *
 * Created: 28/02/2016 02:24:19 م
 *  Author: hossam & Amr Tarek
 */ 
#include "BasicTypes.h"
#include "BTN_CFG.h"
#include "../HAL/DIO.h"
BTN_ConfigType BTN_COnfigParam[NUM_OF_BTNS] = 
{
	{
		PB,0x02    // by Amr 0x02 = 0000 0010 = pin 1
	}
};