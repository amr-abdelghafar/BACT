/*
 * Debug.c
 *
 * Created: 5/17/2016 2:36:44 PM
 *  Author: hossam
 */ 
#include "BasicTypes.h"
#include "DIO.h"
#include "Debug.h"
#include <util/delay.h>
void DBG_InsertBreakPoint1(void)
{
	u8 i;
	
	static int cout = 0;
	cout ++;
	for(i=0; i< 5; i++)
	{
			BUZZER_ON();
			_delay_us(1);
			BUZZER_OFF();
			_delay_us(1);
	}


}
void DBG_InsertBreakPoint2(void)
{
	u8 i;
	for(i=0; i< 5; i++)
	{
		BUZZER_ON();
		_delay_ms(100);
		BUZZER_OFF();
		_delay_ms(100);
	}
	while(1);
}
void DBG_InsertBreakPoint3(void)
{
	u8 i;
	for(i=0; i< 5; i++)
	{
		BUZZER_ON();
		_delay_ms(2);
		BUZZER_OFF();
		_delay_ms(2);
	}
	//while(1);
}