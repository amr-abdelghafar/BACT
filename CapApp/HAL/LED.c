/*
 * LED.c
 *
 * Created: 4/16/2016 3:04:05 PM
 *  Author: hossam
 */ 
#include "BasicTypes.h"
#include "DIO.h"
#include "LED_Cfg.h"
/*Is a function to initialize direction of all connected LEDs and turn them all off*/
void LED_Init(void)
{
	u8 LoopIndex;

	for(LoopIndex = 0; LoopIndex < LED_MAX_NUM; LoopIndex++)
	{

	  DIO_InitPortDirection(LED_ConfigParamaters[LoopIndex].LedPortName,0XFF,1<< LED_ConfigParamaters[LoopIndex].LedPinNum); 
	  DIO_WritePort(LED_ConfigParamaters[LoopIndex].LedPortName,0X00,1<< LED_ConfigParamaters[LoopIndex].LedPinNum); // this will make them all on -- by Amr
	}
	
}
/* Is A function to Write a data LED_DATA_OFF or LED_DATA_ON to a specific LED bu LedId*/
void LED_SetData(u8 LedId,u8 LedData)
{

DIO_WritePort(LED_ConfigParamaters[LedId].LedPortName,LedData,1<< LED_ConfigParamaters[LedId].LedPinNum);

}