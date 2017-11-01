/*
 * WMGR_Cfg.h
 *
 * Created: 5/12/2016 12:02:15 AM
 *  Author: hossam & Amr Tarek
 */ 


#ifndef WMGR_CFG_H_
#define WMGR_CFG_H_

#include "BasicTypes.h"
#include "../HAL/BLTD.h"
#include "../HAL/WMGR.h"
#include "../HAL/DIO.h"
#include "BLTD_Cfg.h"

 
#define DEVICE_ROLE DEVICE_ROLE_MASTER       /* Master for Cap, Slave for Chair -- Amr */
#define  MODULE_TYPE_M10 0x01
#define MODULE_TYPE_C05 0x02
#define MODULE_TYPE MODULE_TYPE_M10
#if(MODULE_TYPE == MODULE_TYPE_M10)
#define WMGR_START_SCRIPT_SIZE 7
#else
#if (DEVICE_ROLE == DEVICE_ROLE_SLAVE)
#define WMGR_START_SCRIPT_SIZE 6
#else
#define WMGR_START_SCRIPT_SIZE 7
#endif
#endif

#if (DEVICE_ROLE == DEVICE_ROLE_SLAVE)
#define  PWR_PORT PC
#define PWR_PIN_NUM 5
#define KEY_PORT PD							
#define KEY_PIN 2							
#else
#define  PWR_PORT PC						/* From PD to PC --Edit by Amr */
#define PWR_PIN_NUM 7						/* From 3 to 7   --Edit by Amr */
#define KEY_PORT PB							/* From PD to PB --Edit by Amr */
#define KEY_PIN 5							/* From 2 to 3   --Edit by Amr */

#endif




#define ITERATION_TIME_MS 50

extern const u8 WMGR_StartScriptCommands[WMGR_START_SCRIPT_SIZE];
extern const u8 WMGR_DeviceName[9];
extern const u8 WMGR_DeviceNameSize;
extern const u8 WMGR_PassKey[7];
extern const u8 WMGR_PassKeySize;

#if (DEVICE_ROLE == DEVICE_ROLE_SLAVE)
#define BUZZER_PORT PC
#define BUZZER_PIN_NUM 4
#else
#define BUZZER_PORT PC
#define BUZZER_PIN_NUM 3					/* from 5 to 3    --Edit by Amr */
#endif

#define BUZZER_INIT()  DIO_InitPortDirection(BUZZER_PORT,0xff,1<<BUZZER_PIN_NUM)
#define BUZZER_ON() DIO_WritePort(BUZZER_PORT,0xff,1<<BUZZER_PIN_NUM)
#define BUZZER_OFF() DIO_WritePort(BUZZER_PORT,0x00,1<<BUZZER_PIN_NUM)




#endif /* WMGR_CFG_H_ */