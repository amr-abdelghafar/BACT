/*
* CFile1.c
*
* Created: 08/03/2016 08:48:00 م
*  Author: hossam & Amr Tarek
*/

#include "../Config/BasicTypes.h"
#include <avr/io.h>

void PWM_Init(void)
{
	TCCR1A = 0x00;
	TCCR1A |= (1<<COM1A1) | (1<<COM1B1) | (1<<WGM11) | (1<<WGM10);
	TCCR1B = 0x00;
	TCCR1B |= (1<<WGM12);
	DDRB |= (1<<PB2) | (1<<PB1);
	
}

void PWM_SetSpeed(float SpeedPercent)
{
	u32 Duty;
	if(SpeedPercent == 0)
	{
		TCCR1B &= 0xf8;
		TCCR1A &= ~((1<<COM1A1) | (1<<COM1B1));
		PORTB &= ~((1<<PB1) | (1 << PB2));
		OCR1A = 0x00;
		OCR1B = 0x00;
		
		
	}
	else
	{
		TCCR1A |= ((1<<COM1A1) | (1<<COM1B1));

		Duty = SpeedPercent * 1023.0;
		Duty = Duty /100;
		OCR1A= Duty;
		//OCR1AH= (u8)(Duty >> 8u);
		OCR1B= Duty;
		//OCR1BH= (u8)(Duty >> 8u);
		TCCR1B|=0x01;	
	}
}