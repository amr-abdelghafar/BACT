/*
* BTN.c
*
* Created: 28/02/2016 02:23:43 م
*  Author: hossam & Amr Tarek
*/
#include "../Config/BasicTypes.h"
#include "DIO.h"
#include "BTN.h"
#include "../Config/BTN_CFG.h"

#define STATE_OPEN 0x00
#define STATE_CLOSED 0x01

static u8 State[NUM_OF_BTNS];
void BTN_Init(void)
{
	u8 LoopIndex;
	u8 PortName;
	u8 PortMask;
	
	for(LoopIndex = 0; LoopIndex < NUM_OF_BTNS; LoopIndex ++)
	{
		PortName = BTN_COnfigParam[LoopIndex].BtnPort;
		PortMask = BTN_COnfigParam[LoopIndex].BtnMask;
		DIO_InitPortDirection(PortName,0x00,PortMask);
		State[LoopIndex] = STATE_OPEN;
	}
	
}
u8 BTN_GetState(u8 BtnId)
{
	u8 PinValue;
	u8 PortName;
	u8 PortMask;
	u8 RetVar;
	PortName = BTN_COnfigParam[BtnId].BtnPort;
	PortMask = BTN_COnfigParam[BtnId].BtnMask;
	PinValue = DIO_ReadPort(PortName,PortMask);
	
	if(State[BtnId] == STATE_OPEN)
	{
		
		if(PinValue != 0x00)
		{
			
			State[BtnId]= STATE_OPEN;
			RetVar = BTN_INACTIVE;
		}
		else
		{
			
			State[BtnId] = STATE_CLOSED;
			RetVar = BTN_ACTIVE;
		}
	}
	else
	{
		if(PinValue != 0x00)
		{
			State[BtnId]= STATE_OPEN;
			RetVar = BTN_INACTIVE;
		}
		else
		{
			State[BtnId] = STATE_CLOSED;
			RetVar = BTN_INACTIVE;
		}
	}
	return RetVar;
}