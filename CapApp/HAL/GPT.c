/*
 * GPT.c
 *
 * Created: 06/02/2016 10:44:10 ص
 *  Author: hossam
 */ 
#include "BasicTypes.h"
#include "GPT.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static PfType Timer5msCallBackPtr;
static PfType Timer10msCallBackPtr;
void GPT_Timer30msInit(PfType CallBackPtr)
{
	Timer5msCallBackPtr = CallBackPtr;

	TIMSK |= 1<<TOIE0;
	TCCR0 = 0x00;
	TCCR0 |=  0x05;
}
void GPT_Timer10msInit(PfType CallBackPtr)
{
		Timer10msCallBackPtr = CallBackPtr;
		OCR2 = 78;
		TIMSK |= 1<<OCIE2;
		TCCR2 = 0x00;
		TCCR2 |= (1<<WGM21) | 0x07;
		
	
}

ISR(TIMER0_OVF_vect)
{
	Timer5msCallBackPtr();
}

ISR(TIMER2_COMP_vect)
{
	
	Timer10msCallBackPtr();
}

