/*
 * LED_Cfg.c
 *
 * Created: 4/16/2016 3:04:40 PM
 *  Author: hossam & Amr Tarek
 */ 
#include "BasicTypes.h"
#include "../HAL/DIO.h"
#include "LED_Cfg.h"
LED_ConfigType LED_ConfigParamaters[LED_MAX_NUM] = 
{
	{
		PD,6 /* From 5 to 6 by Amr */
	},
	{
		PD,7 /* From 6 to 7 by Amr */
	}
};