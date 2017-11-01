/*
 * UART_Drv.c
 *
 * Created: 26/07/2015 05:23:46 م
 *  Author: hossam & Amr Tarek
 */ 
#include "UART_Drv.h"
#include <avr/io.h>
#include "../Config/BasicTypes.h"
#include "../Config/UART_LCFG.h"
#include <avr/interrupt.h>
static const u8* TxBuffAdd = 0;
static u16 TxCounter = 0;
static u16 TxBuffLength = 0;
static u8* RxBuffAdd = 0;
static u16 RxCounter = 0;
static u16 RxBuffLength = 0;
static tCbkFn UartRxcCnkFnPtr;

void UART_Init(void)
{
	u16 Ubrr ;
	UCSRA = 0x00;
	UCSRC = 0x00;
	UCSRC |= (1<<URSEL) | (UartConfig.Parity << UPM0) | UartConfig.stopBitSetting << USBS | 0x03<<UCSZ0 ;
	Ubrr = F_CPU/16/UartConfig.BaudRate-1;
	UBRRL = (u8) Ubrr;
	UBRRH = (u8)(Ubrr >> 8);
	UCSRB = (1<<RXEN)|(1<<TXEN);
	TxBuffAdd = 0;
	TxCounter = 0;
	TxBuffLength = 0;
	UartRxcCnkFnPtr = 0;
	
}
void UART_TxBuffer(const u8* Buffer,u16 BufferLength)
{
u8 i;
for (i = 0 ;i<BufferLength;i++)
{
	USART_Transmit(Buffer[i]);
}

}
void UART_StartReception(u8* Buffer,u16 BufferLength,tCbkFn RxcCbkFnPtr)
{
	RxCounter =0;
 RxBuffAdd = Buffer;
 RxBuffLength = BufferLength;
 UartRxcCnkFnPtr = RxcCbkFnPtr; 
 //UCSRB |= 1<<RXEN;
 UCSRB |= 1<<RXCIE;

	
}



ISR (USART_RXC_vect)
{

	 *(RxBuffAdd + RxCounter)  = UDR; 
	RxCounter ++;
	
	if(RxCounter == RxBuffLength)
	{

 		RxBuffLength = 0;
		UCSRB &= ~(1<<RXCIE);
	//	UCSRB &= ~(1<<RXEN);
	    
		UartRxcCnkFnPtr();
  
	}
}
u16 UART_GetNumOfRxbytes(void)
{
	return RxCounter;
}
void UART_StopRception(void)
{
	UCSRB &= ~(1<<RXCIE);
	RxCounter = 0;
	UCSRB &= ~(1<<RXEN);
	UCSRB |=1<<RXEN;
}

 void USART_Transmit( unsigned char data )
{
/* Wait for empty transmit buffer */
while ( !( UCSRA & (1<<UDRE)) )
;
/* Put data into buffer, sends the data */
UDR = data;
}