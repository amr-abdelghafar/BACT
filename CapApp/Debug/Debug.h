/*
 * Debug.h
 *
 * Created: 5/17/2016 2:36:32 PM
 *  Author: hossam
 */ 


#ifndef DEBUG_H_
#define DEBUG_H_
#include "WMGR_Cfg.h"
#include <util/delay.h>
#if(DEVICE_ROLE == DEVICE_ROLE_SLAVE)
#define BUZZER_PORT PC
#define BUZZER_PIN_NUM 4
#else
#define BUZZER_PORT PC		/* From PD to PC by Amr */
#define BUZZER_PIN_NUM 3	/* from 5 to 3 by Amr */
#endif
#define BUZZER_ON() DIO_WritePort(BUZZER_PORT,0xff,1<<BUZZER_PIN_NUM)
#define BUZZER_OFF() DIO_WritePort(BUZZER_PORT,0x00,1<<BUZZER_PIN_NUM)
void DBG_InsertBreakPoint1(void);
void DBG_InsertBreakPoint2(void);
void DBG_InsertBreakPoint3(void);




#endif /* DEBUG_H_ */