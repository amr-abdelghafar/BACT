/*
 * MOTOR.h
 *
 * Created: 4/22/2016 12:20:28 AM
 *  Author: hossam
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include "BasicTypes.h"
#include "DIO.h"

#define INIT_DIRCTION_PINS() DIO_InitPortDirection(PD,0xff,0xC0)
#define SET_DIRECTION_FWD() DIO_WritePort(PD,0x00,0xc0)
#define SET_DIRECTION_BWD() DIO_WritePort(PD,0xC0,0xc0)
#define SET_DIRECTION_RHT() DIO_WritePort(PD,0x40,0xc0)
#define SET_DIRECTION_LFT() DIO_WritePort(PD,0x80,0xc0)
#define INIT_REFRESH_PIN() DIO_InitPortDirection(PB,0xff,0x01)
#define SET_REFRESH_PIN_DATA(DATA) DIO_WritePort(PB,DATA,0x01)
#define SET_REFRESH_PIN() DIO_WritePort(PB,0xff,0x01)
#define CLR_REFRESH_PIN() DIO_WritePort(PB,0x00,0x01)
#define INIT_BREAK_PIN() DIO_InitPortDirection(PD,0xff,1<<4)
#define SET_BREAK_PIN() DIO_WritePort(PD,0xff,1<<4)
#define CLR_BREAK_PIN() DIO_WritePort(PD,0x00,1<<4)

void MOTOR_Init(void);
void MOTOR_SetSpeed(u8 Speed,u8 Direction);
void MOTOR_SetDirection(u8 Direction);
void MOTOR_StateMachine(void);


#endif /* MOTOR_H_ */