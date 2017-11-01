/*
* GYROTemp.c
*
* Created: 5/25/2016 2:17:17 AM
*  Author: hossam & Amr Tarek
*/
#if 0
#include "BasicTypes.h"
#include "UART_Drv.h"
#include "DIO.h"
#include <util/delay.h>
#define BUZZER_PORT PC
#define BUZZER_PIN_NUM 4
#define  PWR_PORT PC
#define PWR_PIN_NUM 5
#define KEY_PORT PD
#define KEY_PIN 2
#define BUZZER_INIT()  DIO_InitPortDirection(BUZZER_PORT,0xff,1<<BUZZER_PIN_NUM)
#define BUZZER_ON() DIO_WritePort(BUZZER_PORT,0xff,1<<BUZZER_PIN_NUM)
#define BUZZER_OFF() DIO_WritePort(BUZZER_PORT,0x00,1<<BUZZER_PIN_NUM)

#define INIT_PWR_PIN() DIO_InitPortDirection(PWR_PORT,0xff,1<<PWR_PIN_NUM);\
DIO_WritePort(PWR_PORT,0x00,1<<PWR_PIN_NUM)

#define SET_PWR_PIN()  DIO_WritePort(PWR_PORT,0xff,1<<PWR_PIN_NUM)

#define CLR_PWR_PIN()  DIO_WritePort(PWR_PORT,0x00,1<<PWR_PIN_NUM)

#define INIT_KEY_PIN() DIO_InitPortDirection(KEY_PORT,0xff,1<<KEY_PIN);\
DIO_WritePort(KEY_PORT,0x00,1<<KEY_PIN)

#define SET_KEY_PIN()  DIO_WritePort(KEY_PORT,0xff,1<<KEY_PIN)

#define CLR_KEY_PIN()  DIO_WritePort(KEY_PORT,0x00,1<<KEY_PIN)
int main (void )
{
	BUZZER_INIT();
	INIT_KEY_PIN();
	INIT_PWR_PIN();
	SET_PWR_PIN();
	SET_KEY_PIN();
	_delay_ms(1000);
	UART_Init();
	BUZZER_ON();
		UART_TxBuffer("AT+INIT\r\n",9);
		_delay_ms(500);
		
	UART_TxBuffer("AT+ADDR?\r\n",10);
	_delay_ms(500);
		UART_TxBuffer("AT+NAME=B-ACT-CH\r\n",18);
		_delay_ms(500);
		UART_TxBuffer("AT+PSWD=1234\r\n",14);
		_delay_ms(500);
		
		UART_TxBuffer("AT+ROLE=0\r\n",11);
		_delay_ms(500);
		
		UART_TxBuffer("AT+INQ\r\n",8);
		_delay_ms(500);
	
	BUZZER_OFF();
	
	
}
#endif