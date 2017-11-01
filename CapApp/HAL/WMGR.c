/*
* WMGR.c
*
* Created: 5/12/2016 12:01:51 AM
*  Author: hossam
*/
#include "BasicTypes.h"
#include "BLTD.h"
#include "DIO.h"
#include "WMGR.h"
#include "WMGR_Cfg.h"
#include "UART_Drv.h"
#include "UTL.h"
#include "BLTD_Cfg.h"
#include "Debug.h"

#define POWER_STATE_OFF                 0x00
#define POWER_STATE_IN_PROGRESS         0x01
#define POWER_STATE_ON_SEND_CMD         0x02
#define POWER_STATE_ON_WAIT_RESP        0x03
#define POWER_STATE_FAILED              0x04
#define POWER_STATE_DONE                0x05

#define RCV_STATE_START_RECEPT    0x00
#define RCV_STATE_WAIT_FOR_DATA   0x01
#define RCV_STATE_DATA_VALID      0x02
#define RCV_STATE_DISC_DETECTED   0x03
#define RCV_STATE_WAIT_FIRST_TIME 0x04
#define RCV_STATE_START_FIRST_TIME 0x05
#define RCV_STATE_WAIT_PWR_OFF 0x06

#define MSTR_CONN_STATE_INQ 0x00
#define MSTR_CONN_STATE_WAIT_INQ_RESP 0x01
#define MSTR_CONN_STATE_LINK 0x02
#define MSTR_CONN_STATE_PWR_OFF_INPROGRESS 0x03


#define WMGR_AFTER_POWER_TIME_OUT_MS 200
#define WMGR_AFTER_POWER_COUNT (WMGR_AFTER_POWER_TIME_OUT_MS/ITERATION_TIME_MS)
#define WMGR_WAIT_RESPONSE_TIME_MS 30000
#define WMGR_WAIT_RESP_COUNT (WMGR_WAIT_RESPONSE_TIME_MS/ITERATION_TIME_MS)
#define WMGR_BEFORE_WAIT_CONNECTION_TIME_OUT_MS 0
#define WMGR_BEFORE_WAIT_CONNECTION_COUNT (WMGR_BEFORE_WAIT_CONNECTION_TIME_OUT_MS/ ITERATION_TIME_MS)

#define WMGR_WAIT_FOR_DATA_TIME_OUT_MS 10000
#define WMGR_WAIT_FOR_DATA_COUNT (WMGR_WAIT_FOR_DATA_TIME_OUT_MS / ITERATION_TIME_MS)

#define WMGR_WAIT_FOR_INQ_RESP_TIME_OUT_MS 30000
#define WMGR_WAIT_FOR_INQ_RESP_COUNT  (WMGR_WAIT_FOR_INQ_RESP_TIME_OUT_MS / ITERATION_TIME_MS)

#if(DEVICE_ROLE == DEVICE_ROLE_MASTER)
#define DATA_BUFFER_SIZE 80
#else
#define DATA_BUFFER_SIZE 15
#endif


u8 WMGR_MacAddress[MAC_ADDRESS_LENGTH];
SpeedBufferType SpeedBuffer[5] =
{
	{
		(const u8*)"STOP",0,4
	},
	{
		(const u8*)"UP",3,2
	},
	{
		(const u8*)"DOWN",5,4
	},
	{
		(const u8*)"RIGHT",6,5
	},
	{
		(const u8*)"LEFT",5,4
	}

};
static u32 WMGR_TimeoutCounter;
static u8 WMGR_InitScriptIndex;
static u8 WMGR_PowerState;
static u8 WMGR_IsResponseReceived;
static u8 WMGR_ResPonseStatus;
static u8* WMGR_DeviceMacAddresPtr;
static u8 WMGR_DataBuffer[DATA_BUFFER_SIZE];
static u8 WMGR_ReceivingState;
static u8 WMGR_IsDataReceived;

static void WMGR_ResponseReceivedCbk(u8 RespId,u8 RespStatus);
static void WMGR_DataReceivedCbk(void);


void WMGR_Init(void)
{
	WMGR_TimeoutCounter = 0;
	WMGR_InitScriptIndex = 0;
	WMGR_IsResponseReceived = 0;
	WMGR_IsDataReceived = 0;
	WMGR_ResPonseStatus = BLTD_RESP_STATE_INVALID;
	WMGR_PowerState = POWER_STATE_OFF;
	WMGR_ReceivingState = RCV_STATE_START_FIRST_TIME;
	BLTD_SetDeviceName(WMGR_DeviceName,WMGR_DeviceNameSize);
	BLTD_SetPassKey(WMGR_PassKey,WMGR_PassKeySize);
	BLTD_Init(WMGR_ResponseReceivedCbk);
	BUZZER_INIT();
	BLTD_PowerOn(); // commented by Amr because the BT_enable can't get 1 in --WMGR_RunInitScript--

}
u8 WMGR_RunInitScript(void)
{
	u8 ScriptState;
	
	switch(WMGR_PowerState)
	{
		case POWER_STATE_OFF:
		{
			
			BLTD_PowerOn();
			WMGR_PowerState = POWER_STATE_IN_PROGRESS;
			ScriptState = WMGR_SCRIPT_STATE_IN_PROGRESS;
			
			

		}
		break;
		case POWER_STATE_IN_PROGRESS:
		{
			WMGR_TimeoutCounter ++;
			if(WMGR_TimeoutCounter > WMGR_AFTER_POWER_COUNT)
			{
				
				WMGR_TimeoutCounter = 0;
				WMGR_PowerState = POWER_STATE_ON_SEND_CMD;
				

			}
			ScriptState = WMGR_SCRIPT_STATE_IN_PROGRESS;
		}
		break;
		case POWER_STATE_ON_SEND_CMD:
		{
			
			WMGR_IsResponseReceived = 0;
			BLTD_SendCmd(WMGR_StartScriptCommands[WMGR_InitScriptIndex]);
			ScriptState = WMGR_SCRIPT_STATE_IN_PROGRESS;
			WMGR_PowerState = POWER_STATE_ON_WAIT_RESP;
			
			
		}
		break;
		case POWER_STATE_ON_WAIT_RESP:
		{
			if(WMGR_IsResponseReceived == 1)
			{
				WMGR_IsResponseReceived = 0;
				
				if(WMGR_ResPonseStatus == BLTD_RESP_STATE_VALID)
				{
					
					WMGR_InitScriptIndex ++;
					if(WMGR_InitScriptIndex == WMGR_START_SCRIPT_SIZE)
					{
						
						BLTD_GetMacAddress(&WMGR_DeviceMacAddresPtr);
						UTL_MemReplace(WMGR_DeviceMacAddresPtr,':',',',14);
						ScriptState = WMGR_SCRIPT_STATE_DONE;
						WMGR_PowerState = POWER_STATE_OFF;
						WMGR_InitScriptIndex = 0;
						
						
						
					}
					else
					{
						ScriptState = WMGR_SCRIPT_STATE_IN_PROGRESS;
						WMGR_PowerState = POWER_STATE_ON_SEND_CMD;
					}
				}
				else
				{
					
					
					ScriptState = WMGR_SCRIPT_STATE_FAILED;
				}
			}
			else
			{
				WMGR_TimeoutCounter ++;
				if(WMGR_TimeoutCounter > WMGR_WAIT_RESP_COUNT)
				{
					WMGR_TimeoutCounter = 0;
					ScriptState = WMGR_SCRIPT_STATE_FAILED;
				}
				else
				{
					ScriptState = WMGR_SCRIPT_STATE_IN_PROGRESS;
				}
			}
			
		}
		break;
		default:
		{
			ScriptState = WMGR_SCRIPT_STATE_FAILED;
		}
		break;
		
	}
	return ScriptState;
}

void WMGR_GetMacAddress(u8** MacAddressPtr)
{
	*MacAddressPtr = WMGR_DeviceMacAddresPtr;
}
u8 WMGR_GetData(u8* SpeedPtr,u8* DirectionPtr)
{
	u8 RxStatus;
	u8 Status;
	u8 StartPosition;
	u8 LoopIndex;
	switch(WMGR_ReceivingState)
	{
		case RCV_STATE_START_FIRST_TIME:
		{
			UART_StartReception(WMGR_DataBuffer,DATA_BUFFER_SIZE,WMGR_DataReceivedCbk);
			WMGR_ReceivingState = RCV_STATE_WAIT_FIRST_TIME;
			RxStatus = WMGR_RX_STATUS_INPROGRESS;
		}
		break;
		case RCV_STATE_WAIT_FIRST_TIME:
		{
			if(WMGR_IsDataReceived == 1)
			{
				WMGR_IsDataReceived = 0;
				WMGR_ReceivingState = RCV_STATE_START_RECEPT;
				RxStatus = WMGR_RX_STATUS_CONNECTED;
				
			}
			else
			{
				RxStatus = WMGR_RX_STATUS_INPROGRESS;
			}
			
		}
		break;
		case RCV_STATE_START_RECEPT:
		{
			UART_StartReception(WMGR_DataBuffer,DATA_BUFFER_SIZE,WMGR_DataReceivedCbk);
			WMGR_ReceivingState = RCV_STATE_WAIT_FOR_DATA;
			WMGR_TimeoutCounter = 0;
			WMGR_IsDataReceived = 0;
			RxStatus = WMGR_RX_STATUS_INPROGRESS;
			
		}
		break;
		
		case RCV_STATE_WAIT_FOR_DATA:
		{
			
			if(WMGR_IsDataReceived == 1)
			{
				
				WMGR_IsDataReceived = 0;
				/*search for * */
				Status = UTL_MemSearch(&WMGR_DataBuffer[0],'*',DATA_BUFFER_SIZE,&StartPosition);
				if(Status == 0)
				{
					for ( LoopIndex = 0; LoopIndex < 5; LoopIndex ++)
					{
						Status = UTL_MemCmp(&WMGR_DataBuffer[StartPosition + 1],SpeedBuffer[LoopIndex].SpeedPtr,2);
						if(Status == 0)
						{
							break;
						}
					}
					/*Check for Stop*/
					if(LoopIndex == 0)
					{
						
						*SpeedPtr = 0x00;
					}
					else
					{
						
						*DirectionPtr = LoopIndex;
						*SpeedPtr = UTL_StringToInteger((const u8*) &WMGR_DataBuffer[StartPosition + SpeedBuffer[LoopIndex].SpeedOffset],2);
					}
				}
				else
				{
					
					*SpeedPtr = 0;
				}
				

				WMGR_ReceivingState = RCV_STATE_START_RECEPT;
				RxStatus = WMGR_RX_STATUS_INPROGRESS;
				
			}
			else
			{
				
				WMGR_TimeoutCounter ++;
				
				if(WMGR_TimeoutCounter > /*WMGR_WAIT_FOR_DATA_COUNT*/ 50)
				{
					
					/*Disconnection detected*/
					WMGR_TimeoutCounter = 0;
					WMGR_ReceivingState = RCV_STATE_DISC_DETECTED;
					RxStatus = WMGR_RX_STATUS_INPROGRESS;
					
				}
				else
				{
					/*Wait for Data frame*/
					RxStatus = WMGR_RX_STATUS_INPROGRESS;
				}
				
			}
			
		}
		break;
		case RCV_STATE_DISC_DETECTED:
		{
			BLTD_PowerOff();
			
			WMGR_ReceivingState = RCV_STATE_WAIT_PWR_OFF;
			RxStatus = WMGR_RX_STATUS_INPROGRESS;
			WMGR_TimeoutCounter = 0;
		}
		break;
		case RCV_STATE_WAIT_PWR_OFF:
		{
			WMGR_TimeoutCounter ++;
			if(WMGR_TimeoutCounter > WMGR_AFTER_POWER_COUNT)
			{
				
				WMGR_TimeoutCounter = 0;
				RxStatus = WMGR_RX_STATUS_DISCONN;
				WMGR_ReceivingState = RCV_STATE_START_FIRST_TIME;
			}
			else
			{
				RxStatus = WMGR_RX_STATUS_INPROGRESS;
			}
			
			
		}
		break;
		default:
		{
			RxStatus = WMGR_RX_STATUS_DISCONN;
		}
		break;
	}
	return RxStatus;
}
void WMGR_StartConnection(u8* MacAddrPtr,u8 MacAddLength)
{
	//UTL_MemCpy(WMGR_TargetMacAddress,MacAddrPtr,MacAddLength);
	BLTD_CmdList[START_CONN_ID].CmdParamPtr = MacAddrPtr;
	BLTD_CmdList[START_CONN_ID].CmdParamSize = MacAddLength;
	BLTD_SendCmd(START_CONN_ID);
}

static void WMGR_ResponseReceivedCbk(u8 RespId,u8 RespStatus)
{
	WMGR_ResPonseStatus = RespStatus;
	WMGR_IsResponseReceived = 1;
}

static void WMGR_DataReceivedCbk(void)
{
	WMGR_IsDataReceived = 1;
}
#if(DEVICE_ROLE == DEVICE_ROLE_MASTER)
u8 WMGR_MasterStartConn(u8* MAcAddPtr,u8 MacAddLength)
{
	static u8 MasterConnectionState = MSTR_CONN_STATE_INQ;
	u8 RxSize;
	u8 LoopIndex;
	u8 Status;
	u8 RxStatus;
	switch(MasterConnectionState)
	{
		case MSTR_CONN_STATE_INQ:
		{

			UART_StartReception(WMGR_DataBuffer,DATA_BUFFER_SIZE,WMGR_DataReceivedCbk);
			MasterConnectionState = MSTR_CONN_STATE_WAIT_INQ_RESP;
			WMGR_TimeoutCounter = 0;
			WMGR_IsDataReceived = 0;
			RxStatus = WMGR_RX_STATUS_INPROGRESS;
			UART_TxBuffer((u8*)"AT+INQ\r\n",8);
			
		}
		break;
		
		case MSTR_CONN_STATE_WAIT_INQ_RESP:
		{
			
			RxSize = UART_GetNumOfRxbytes();
			if(RxSize > 0)
			{
				
				/*Search for OK*/
				for(LoopIndex = 0; LoopIndex < (RxSize - 3)  ; LoopIndex ++)
				{
					Status = UTL_MemCmp(&WMGR_DataBuffer[LoopIndex],(u8*)"OK\r\n",4);
					if(Status == 0)
					{
						/*ResPonse Received*/
						UART_StopRception();
						WMGR_TimeoutCounter = 0;
						BLTD_CmdList[START_CONN_ID].CmdParamPtr = MAcAddPtr;
						BLTD_CmdList[START_CONN_ID].CmdParamSize = MacAddLength;
						WMGR_IsResponseReceived = 0;
						BLTD_SendCmd(START_CONN_ID);
						MasterConnectionState = MSTR_CONN_STATE_LINK;
						LoopIndex = RxSize - 3;
						
						
						
					}
					
					
				}
				RxStatus = WMGR_RX_STATUS_INPROGRESS;
			}
			else
			{
				WMGR_TimeoutCounter ++;
				if(WMGR_TimeoutCounter > WMGR_WAIT_FOR_INQ_RESP_COUNT)
				{
					WMGR_TimeoutCounter = 0;
					MasterConnectionState = MSTR_CONN_STATE_PWR_OFF_INPROGRESS;
					RxStatus = WMGR_RX_STATUS_INPROGRESS;
					BLTD_PowerOff();
					
					
				}
				else
				{
					RxStatus = WMGR_RX_STATUS_INPROGRESS;
					
				}
				
			}
			
		}
		break;
		
		case MSTR_CONN_STATE_PWR_OFF_INPROGRESS:
		{
			
			WMGR_TimeoutCounter ++;
			if(WMGR_TimeoutCounter > WMGR_AFTER_POWER_COUNT)
			{
				UART_StopRception();
				WMGR_Init();
				WMGR_TimeoutCounter = 0;
				RxStatus = WMGR_RX_STATUS_DISCONN;
				MasterConnectionState = MSTR_CONN_STATE_INQ;
			}
			else
			{
				RxStatus = WMGR_RX_STATUS_INPROGRESS;
			}
		}
		break;
		case MSTR_CONN_STATE_LINK:
		{
			
			if(WMGR_IsResponseReceived == 1)
			{
				
				
				WMGR_IsResponseReceived = 0;
				
				if(WMGR_ResPonseStatus == BLTD_RESP_STATE_VALID)
				{
					
					/*Connection OK*/
					RxStatus = WMGR_RX_STATUS_CONNECTED;
					MasterConnectionState = MSTR_CONN_STATE_INQ;
					WMGR_TimeoutCounter = 0;
					UART_StopRception();
				}
				else
				{
					/*Connection NOK*/
					UART_StopRception();
					RxStatus = WMGR_RX_STATUS_INPROGRESS;
					MasterConnectionState = MSTR_CONN_STATE_PWR_OFF_INPROGRESS;
					BLTD_PowerOff();
					WMGR_TimeoutCounter = 0;
					
					

				}
			}
			else
			{
				WMGR_TimeoutCounter ++;
				if(WMGR_TimeoutCounter > WMGR_WAIT_RESP_COUNT)
				{
					
					/*Connection NOK*/
					UART_StopRception();
		RxStatus = WMGR_RX_STATUS_INPROGRESS;
		MasterConnectionState = MSTR_CONN_STATE_PWR_OFF_INPROGRESS;
				}
				else
				{
					/*Wait for Response*/
					RxStatus = WMGR_RX_STATUS_INPROGRESS;
				}
			}
			
		}
		break;
		default:
		{
			RxStatus = WMGR_RX_STATUS_DISCONN;
		}
		break;
	}
	return RxStatus;
}
#endif
void WMGR_SendData(u8 Speed,u8 DirectionId)
{
	u8 BufferSize = 0;
	WMGR_DataBuffer [0] = '*';
	BufferSize ++;

if(Speed != 0)
{
	UTL_MemCpy(&WMGR_DataBuffer[1],SpeedBuffer[DirectionId].SpeedPtr,SpeedBuffer[DirectionId].StringSize);
	WMGR_DataBuffer[SpeedBuffer[DirectionId].StringSize + 1] = (Speed / 10) + '0';
	WMGR_DataBuffer[SpeedBuffer[DirectionId].StringSize + 2] = (Speed % 10) + '0';
	WMGR_DataBuffer[SpeedBuffer[DirectionId].StringSize + 3] = '%';
	WMGR_DataBuffer[SpeedBuffer[DirectionId].StringSize + 4] = '\n';
	BufferSize += SpeedBuffer[DirectionId].StringSize + 4;
	
}
else
{
	UTL_MemCpy(&WMGR_DataBuffer[1],SpeedBuffer[0].SpeedPtr,SpeedBuffer[DirectionId].StringSize);
	WMGR_DataBuffer[SpeedBuffer[0].StringSize + 1] = '\n';
	BufferSize += SpeedBuffer[0].StringSize + 1;
}
UART_TxBuffer(WMGR_DataBuffer, BufferSize);
}