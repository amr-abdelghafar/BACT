/*
* MOTOR.c
*
* Created: 4/22/2016 12:21:52 AM
*  Author: hossam & Amr Tarek
*/
#include "PWM.h"
#include "MOTOR.h"
#include <util/delay.h>

#define STATE_BREAK 0x00
#define STATE_DISABLE_BREAK 0x01
#define STATE_SET_SPEED 0x02
#define STATE_INC_SPEED 0x03
#define START_BREAK 0x04
#define WAIT_BREAK 0x05
#define STATE_GIVE_REFRESH 0x06

#define IS_DIRECTION_SET_1_PORT PC
#define IS_DIRECTION_SET_1_PIN  3
#define IS_DIRECTION_SET_2_PORT PB
#define IS_DIRECTION_SET_2_PIN  7

#define INIT_IS_DIRECTION_PINS() DIO_InitPortDirection(IS_DIRECTION_SET_1_PORT,0x00,1<<IS_DIRECTION_SET_1_PIN);\
                                 DIO_InitPortDirection(IS_DIRECTION_SET_2_PORT,0x00,1<<IS_DIRECTION_SET_2_PIN) 

#define IS_DIRECTION_SET() ((DIO_ReadPort(IS_DIRECTION_SET_1_PORT,1<<IS_DIRECTION_SET_1_PIN) != 0) && (DIO_ReadPort(IS_DIRECTION_SET_1_PORT,1<<IS_DIRECTION_SET_1_PIN) != 0)) ? 1 : 0

static volatile  u8 MotorState;
static u8 TimeCounter;
static float CurrentSpeed;
static u8 TargetSpeed;
void MOTOR_Init(void)
{
	PWM_Init();
	INIT_DIRCTION_PINS();
	INIT_REFRESH_PIN();
	INIT_BREAK_PIN();
	INIT_IS_DIRECTION_PINS();
	MotorState = STATE_BREAK;
	TimeCounter = 0;
	CurrentSpeed = 0;
	TargetSpeed = 0;
}
void MOTOR_SetSpeed(u8 Speed,u8 Direction)
{
	u8 LoopIndex;
	static u8 RefreshCount = 0;
	static u8 RefreshData = 0x00;
	if((Direction == 0x03) || (Direction == 0x04))
	{
		Speed = 0.5*Speed;
	}
	switch(MotorState)
	{
		case START_BREAK:
		{
			
						if(CurrentSpeed > 0 )
						{
							PWM_SetSpeed(CurrentSpeed*(1/16.0));
							TimeCounter ++;
							if(TimeCounter > 0)
							{
								TimeCounter = 0;
								CurrentSpeed /= 4.0;
							}
							
						}
						else
						{
							MotorState = WAIT_BREAK;
							TimeCounter = 0;
						}
		}
		break;
		
		case WAIT_BREAK:
		{
			PWM_SetSpeed(0x00);
			TimeCounter ++; 
			if(TimeCounter > 0)
			{
				TimeCounter = 0;
				MotorState = STATE_BREAK;
			}
		
			
		}
		break;
		case STATE_BREAK:
		{
			
			PWM_SetSpeed(0x00);

			CLR_BREAK_PIN();	
			
			if(Speed != 0x00)
			{
				MotorState = STATE_DISABLE_BREAK;
				
			}
			else
			{
				MotorState = STATE_BREAK;
				TimeCounter = 0;
			}
		}
		break;
		
		case STATE_DISABLE_BREAK:
		{
			
			SET_BREAK_PIN();

			TimeCounter ++;
			if(TimeCounter > 2)
			{
				TimeCounter = 0;
				RefreshData ^=0xff;
				RefreshCount ++;
				if(RefreshCount > 20)
				{
					RefreshCount = 0;
					RefreshData = 0x00;
					if(Speed != 0x00)
					{

						TimeCounter = 0;
						MotorState = /*STATE_INC_SPEED*/ STATE_GIVE_REFRESH;
					//	CurrentSpeed = 0;
					RefreshData = 0;
						TargetSpeed = Speed;
						

						
					}
					else
					{
						TimeCounter  = 0;
						//CurrentSpeed = TargetSpeed;
						MotorState = START_BREAK;
					}
				}
				
				
			}
			SET_REFRESH_PIN_DATA(RefreshData);
			

			
		}
		break;
		
		case STATE_GIVE_REFRESH:
		{
			TimeCounter ++;
			RefreshData ^=0xff;
			
			if(TimeCounter > 2)
			{
				TimeCounter = 0;
				if(IS_DIRECTION_SET() == 1)
				{
					//DBG_InsertBreakPoint2();
					MotorState = STATE_INC_SPEED;
					RefreshData = 0;
					TargetSpeed = Speed;
				}
				else
				{
					SET_REFRESH_PIN_DATA(RefreshData);
				}
			}
			
		}
		break;
		
		case STATE_INC_SPEED:
		{
			if(Speed != 0)
			{
				if(CurrentSpeed < TargetSpeed)
				{
					PWM_SetSpeed(CurrentSpeed*(1/4.0));
					TimeCounter ++;
					if(TimeCounter > 1)
					{
						TimeCounter = 0;
						CurrentSpeed += 0.5;
					}
					
				}
				else
				{
					MotorState = STATE_SET_SPEED;
					TimeCounter = 0;
				}
				
			}
			else
			{
				TimeCounter = 0;
				//CurrentSpeed = TargetSpeed;
				
					MotorState = START_BREAK;
			}
		}
		break;
		case STATE_SET_SPEED:
		{
			if(Speed != 0x00)
			{
				PWM_SetSpeed(Speed*(1/4.0));
			}
			else
			{
				TimeCounter = 0;
				CurrentSpeed = TargetSpeed;
				
				MotorState = START_BREAK;
			}
		}
		break;
	}
	
	
}

void MOTOR_SetDirection(u8 Direction)
{
	switch(Direction)
	{
		case 0x01:
		{
			SET_DIRECTION_FWD();
		}
		break;
		
		case 0x02:
		{
			SET_DIRECTION_BWD();
		}
		break;
		
		case 0x03:
		{
			SET_DIRECTION_RHT();
		}
		break;
		
		case 0x04:
		{
			SET_DIRECTION_LFT();
		}
		break;
	}
	
}
void MOTOR_StateMachine(void)
{
	
}
