/*
* GccApplication1.c
*
* Created: 5/11/2016 8:54:23 AM
*  Author: hossam & Amr Tarek
*/
#include "Config/WMGR_Cfg.h"
#include "Config/BasicTypes.h"
#include <util/delay.h>
#include "HAL/WMGR.h"
#include "HAL/DIO.h"
#include "HAL/UART_Drv.h"
#include <avr/interrupt.h>
#include "HAL/GPT.h"
#include "HAL/UART_Drv.h"
#include "Debug/Debug.h"
#include "HAL/L3G4200D.h"
#include "HAL/GYRO.h"
#include "HAL/LED.h"
#include "HAL/BTN.h"
#include "HAL/MOTOR.h"
//#define BUZZER_INIT()  DIO_InitPortDirection(BUZZER_PORT,0xff,1<<BUZZER_PIN_NUM)
//#define BUZZER_ON() DIO_WritePort(BUZZER_PORT,0xff,1<<BUZZER_PIN_NUM)
//#define BUZZER_OFF() DIO_WritePort(BUZZER_PORT,0x00,1<<BUZZER_PIN_NUM)

#define MGR_STATE_INIT      0x00
#define MGR_STATE_READY_INDICATION 0x01
#define MGR_STATE_READ_DATA 0x02
#define MGR_STATE_DISCONNECT 0x03
#define MGR_STATE_START_CONNECTION 0x04
#define MGR_UPDATE_MOTOR_DATA 0x05
#define MGR_STATE_SEND_DATA 0x06
#define MGR_STATE_WAIT_FOR_CONN_REQ 0x07
#define MGR_STATE_WAIT_PWR_OFF 0x08
#define SYS_STATE_IDLE 0x00
#define SYS_STATE_ACTIVE 0x01
#define SYS_STATE_DISCONNECT 0x02
#define SYS_STATE_DISCONNECT_REQUEST 0x03

static u8 RunInitScript(void);
#if(DEVICE_ROLE == DEVICE_ROLE_MASTER)
static u8 LedTimeOut;
static u8 SystemState;
u8 WakeUpState;
#endif
static u8 MgrState;

void Test(void)
{
	u8 ScriptState;
	u8 RxState;
	u8 BtnState;
	static u16 TimeOutCounter = 0;
	static u8 GyroTimeOutCounter = 0;
	//u8* MacAddressPtr;
	static u8 BuzzerState = 0;
	static u8 Speed = 0;
	static u8 Direction = 0;
	

	u8 Data;
	{
		
		switch(MgrState)
		{
			case MGR_STATE_INIT:
			{
							
				TimeOutCounter ++;
				if(TimeOutCounter > 20)
				{
					
					
					TimeOutCounter = 0;
					ScriptState = RunInitScript();
					if(ScriptState == WMGR_SCRIPT_STATE_DONE)
					{
						//DBG_InsertBreakPoint2();  /*added by Amr*/
						
						MgrState = MGR_STATE_READY_INDICATION;
						#if (DEVICE_ROLE == DEVICE_ROLE_MASTER)
						GYHD_WakeUpModule(&WakeUpState);
						#endif
						
						
					}
					
				}
			}
			break;
			case MGR_STATE_READY_INDICATION:
			{
				
				TimeOutCounter ++;
				if(TimeOutCounter > 4)
				{
					TimeOutCounter = 0;
					#if(DEVICE_ROLE == DEVICE_ROLE_SLAVE)
					MgrState = MGR_STATE_READ_DATA;
					
					
					#else
					
					MgrState = MGR_STATE_WAIT_FOR_CONN_REQ;
					#endif
					
					
				}
				else
				{
					if(BuzzerState == 0)
					{
						
						BUZZER_ON();
						BuzzerState = 1;
					}
					else
					{
						BUZZER_OFF();
						BuzzerState = 0;
					}
				}
				
			}
			break;
			#if(DEVICE_ROLE == DEVICE_ROLE_SLAVE)
			case MGR_STATE_READ_DATA:
			{
				
				RxState = WMGR_GetData(&Speed,&Direction);
				if(RxState == WMGR_RX_STATUS_CONNECTED)
				{
					
					TimeOutCounter = 0;
					BuzzerState = 0;
					MgrState = MGR_STATE_READY_INDICATION;
					
					Speed = 0x00;
					
				}
				else if(RxState == WMGR_RX_STATUS_DISCONN)
				{
					MgrState = MGR_STATE_DISCONNECT;
					
					Speed = 0x00;
					MOTOR_SetSpeed(Speed,Direction);
					
					
				}
				else
				{
					MOTOR_SetSpeed(Speed,Direction);
					MOTOR_SetDirection(Direction);
					

				}
				
				

			}
			break;
			#endif
			case MGR_STATE_DISCONNECT:
			{
				#if(DEVICE_ROLE == DEVICE_ROLE_SLAVE)
				MOTOR_SetSpeed(0,Direction);
				#endif
				TimeOutCounter ++;
				if(TimeOutCounter > 10)
				{
					
					TimeOutCounter = 0;
					WMGR_Init();
					MgrState = MGR_STATE_INIT;
					DBG_InsertBreakPoint1();
					
				}
				else
				{
					if(BuzzerState == 0)
					{
						
						BUZZER_ON();
						BuzzerState = 1;
					}
					else
					{
						BUZZER_OFF();
						BuzzerState = 0;
					}
				}
			}
			break;
			#if(DEVICE_ROLE == DEVICE_ROLE_MASTER)
			case MGR_STATE_WAIT_FOR_CONN_REQ:
			{
				BtnState = BTN_GetState(0);
				if(BtnState == BTN_ACTIVE)
				{
					MgrState = MGR_STATE_START_CONNECTION;
				}
				
			}
			break;
			case MGR_STATE_START_CONNECTION:
			{

				
				RxState = WMGR_MasterStartConn((u8*)"3014,11,181325",14);
				if(RxState == WMGR_RX_STATUS_CONNECTED)
				{
					MgrState = MGR_STATE_SEND_DATA;
					TimeOutCounter = 0;
				}
				else if(RxState == WMGR_RX_STATUS_DISCONN)
				{
					
					
					LED_SetData(1,0);
					TimeOutCounter = 0;
					MgrState = MGR_STATE_DISCONNECT;
					
					
					
				}
				else
				{
					/*Connection Porcess in progress*/
					
				}
				
			}
			break;
			#endif
			case MGR_STATE_SEND_DATA:
			{
				GyroTimeOutCounter ++;
				TimeOutCounter ++;
				if(GyroTimeOutCounter > 20)
				{
					GyroTimeOutCounter = 0;
					Data = GetGyroState();
					if(Data == STP)
					{
						Speed = 0;
						LED_SetData(0,0xff);
					}
					else if ((Data == RGHT) || (Data == LEFT))
					{
						Speed = 25;
						Direction = Data;
						LED_SetData(0,0x00);
					}
					else 
					{
						Speed = 15;
						Direction = Data;
						LED_SetData(0,0x00);
					}
					
					
				}
				if(TimeOutCounter > 10)
				{
					TimeOutCounter = 0;
					WMGR_SendData(Speed,Direction);
				}
				BtnState = BTN_GetState(0);
				if(BtnState == BTN_ACTIVE)
				{
					GyroTimeOutCounter = 0;
					TimeOutCounter = 0;
					
					BLTD_PowerOff();
					WMGR_Init();
					UART_StopRception();

					MgrState = MGR_STATE_WAIT_PWR_OFF;
				}
				
				
			}
			break;
			case MGR_STATE_WAIT_PWR_OFF:
			{
				TimeOutCounter ++;
				if(TimeOutCounter > 100)
				{
					TimeOutCounter = 0;
					MgrState = MGR_STATE_DISCONNECT;
										WMGR_Init();
										UART_StopRception();
				}
			}
			break;
		}
		
	}
}

int main(void)
{


	MgrState = MGR_STATE_INIT;
	BUZZER_INIT();

	WMGR_Init();
	GPT_Timer10msInit(Test);   /* disabled for using the bluetooth Enable by Amr */
	sei();
	#if (DEVICE_ROLE == DEVICE_ROLE_MASTER)
	//GYHD_Init();				 /* disabled for using the bluetooth Enable by Amr */
	LED_Init();
	SystemState = SYS_STATE_IDLE;
	#else
	MOTOR_Init();
	#endif
    
	while(1)
	{
		//Test();					/* added for using the bluetooth Enable by Amr */
		//_delay_ms(10);			/* added for using the bluetooth Enable by Amr */
	}
}
static u8 RunInitScript(void)
{
	u8 ScriptState;
	
	ScriptState = WMGR_RunInitScript();
	
	switch(ScriptState)
	{
		case WMGR_SCRIPT_STATE_IN_PROGRESS:
		{
			//_delay_ms(50);
		}
		break;
		
		case WMGR_SCRIPT_STATE_DONE:
		{
			DBG_InsertBreakPoint2();  /*added by Amr*/
		}
		break;
		
		case WMGR_SCRIPT_STATE_FAILED:
		{

		}
		break;
	}
	return ScriptState;
}
