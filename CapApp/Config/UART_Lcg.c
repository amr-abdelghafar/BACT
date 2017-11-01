/*
 * UART_Lcg.c
 *
 * Created: 26/07/2015 06:38:27 م
 *  Author: hossam & Amr Tarek
 */ 
#include "UART_LCFG.h"
#define NO_PARITY 0x00
#define EVEN_PARITY 0x02
#define ODD_PARITY 0x03

#define STOP_1 0x00
#define  STOP_2 0x01

const UART_tConfig UartConfig = {9600,NO_PARITY,STOP_1};