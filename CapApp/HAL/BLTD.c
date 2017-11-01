#include "../Config/BasicTypes.h"
#include "UART_Drv.h"
#include "DIO.h"
#include "../Debug/Debug.h"
#include "../Config/BLTD_Cfg.h"
#include "BLTD.h"
#include "UTL.h"

#define INIT_PWR_PIN() DIO_InitPortDirection(PWR_PORT,0xff,1<<PWR_PIN_NUM);\
DIO_WritePort(PWR_PORT,0x00,1<<PWR_PIN_NUM)

#define SET_PWR_PIN()  DIO_WritePort(PWR_PORT,0xff,1<<PWR_PIN_NUM)

#define CLR_PWR_PIN()  DIO_WritePort(PWR_PORT,0x00,1<<PWR_PIN_NUM)

#define INIT_KEY_PIN() DIO_InitPortDirection(KEY_PORT,0xff,1<<KEY_PIN);\
DIO_WritePort(KEY_PORT,0x00,1<<KEY_PIN)

#define SET_KEY_PIN()  DIO_WritePort(KEY_PORT,0xff,1<<KEY_PIN)

#define CLR_KEY_PIN()  DIO_WritePort(KEY_PORT,0x00,1<<KEY_PIN)


static u8 BTCommandBuffer[MAX_CMD_SIZE];
static u8 BTResPonseBuffer[MAX_RESP_SIZE] = "0";
static u8 BLTD_CmdInProcess;
static BLTD_RespCbkFnPtrType BLTD_RespCallBackPtr;
/****************Private Functions************************/

static void RxcCallBackFun(void);

/*******************Global Services***********************/
void BLTD_Init(BLTD_RespCbkFnPtrType RespCallBackPtr)
{
	/*Init Key Pin*/
	INIT_KEY_PIN();
	/*Init Pwr Pin*/
	INIT_PWR_PIN();
	BLTD_RespCallBackPtr = 	RespCallBackPtr;
	BLTD_CmdInProcess = 0xff;
	UART_Init();
}
/*********************************************************/
void BLTD_SendCmd(u8 CommandId)
{
	u8 AddedLength = 0;
	u8 LoopIndex;
	/*Prepare Cmd*/
	UTL_MemCpy(&BTCommandBuffer[0],BLTD_CmdList[CommandId].ComdPtr,BLTD_CmdList[CommandId].CmdSize);
	AddedLength += BLTD_CmdList[CommandId].CmdSize;
	UTL_MemCpy(&BTCommandBuffer[AddedLength],BLTD_CmdList[CommandId].CmdParamPtr,BLTD_CmdList[CommandId].CmdParamSize);
	AddedLength +=BLTD_CmdList[CommandId].CmdParamSize;
	#if (MODULE_TYPE == MODULE_TYPE_C05)
	if(CommandId == SET_DEVICE_NAME_ID)
	{
		for(LoopIndex = 0; LoopIndex < 4; LoopIndex ++)
		{
			
			
		BTCommandBuffer[AddedLength + LoopIndex] = *(BLTD_CmdList[GET_MAC_ADDRESS_ID].RespParamPtr + LoopIndex + 10);

				
		}
		AddedLength += LoopIndex;
	}
	#endif
	UTL_MemCpy(&BTCommandBuffer[AddedLength],(u8*)"\r\n",2);
	AddedLength+=2;
	UART_StartReception(BTResPonseBuffer,BLTD_CmdList[CommandId].RespSize + BLTD_CmdList[CommandId].RespParamSize,RxcCallBackFun);
	UART_TxBuffer(BTCommandBuffer,AddedLength);
	BLTD_CmdInProcess = CommandId;
}
/*********************************************************/
void BLTD_SetDeviceName( const u8* DeviceNamePtr,u8 DeviceNameLength)
{
	BLTD_CmdList[SET_DEVICE_NAME_ID].CmdParamPtr = (u8*)DeviceNamePtr;
	BLTD_CmdList[SET_DEVICE_NAME_ID].CmdParamSize = DeviceNameLength;
	#if(MODULE_TYPE == MODULE_TYPE_M10)
	BLTD_CmdList[SET_DEVICE_NAME_ID].RespParamSize = DeviceNameLength + 6;
	#endif
	
}
/*********************************************************/
void BLTD_SetPassKey(const u8* PassKey,u8 PassKeyLength)
{
	BLTD_CmdList[SET_PASS_KEY_ID].CmdParamPtr = (u8*)PassKey;
	BLTD_CmdList[SET_PASS_KEY_ID].CmdParamSize = PassKeyLength;
}
/*********************************************************/
//void BLTD_SetToConnectAddress(u8* AddressPtr,u8 AddressLength)
//{
	//BLTD_CmdList[START_CONNECTION_ID].CmdParamPtr = AddressPtr;
	//BLTD_CmdList[START_CONNECTION_ID].CmdParamSize = AddressLength;
//}
/*********************************************************/
void BLTD_GetMacAddress(u8** MacAddressPtr)
{
	
	*MacAddressPtr = BLTD_CmdList[GET_MAC_ADDRESS_ID].RespParamPtr;
	
}
/*********************************************************/
void BLTD_PowerOn(void)
{
	SET_PWR_PIN();
	SET_KEY_PIN();
}
/*********************************************************/
void BLTD_PowerOff(void)
{
	CLR_KEY_PIN();
	CLR_PWR_PIN();
	
}
/*********************************************************/
/****************Private Functions************************/

/*******************************************************************************************************/
static void RxcCallBackFun(void)
{
	u8 RespState = 0;

	RespState = UTL_MemCmp(BTResPonseBuffer,BLTD_CmdList[BLTD_CmdInProcess].RespPtr,BLTD_CmdList[BLTD_CmdInProcess].RespSize);
	if(RespState == 0)
	{
		if(BLTD_CmdInProcess == GET_MAC_ADDRESS_ID)
		{
			
			UTL_MemCpy(BLTD_CmdList[BLTD_CmdInProcess].RespParamPtr,&BTResPonseBuffer[BLTD_CmdList[BLTD_CmdInProcess].RespSize],BLTD_CmdList[BLTD_CmdInProcess].RespParamSize);

		}
		
		
		RespState = BLTD_RESP_STATE_VALID;
	}
	else
	{
		UART_TxBuffer(BTResPonseBuffer,BLTD_CmdList[BLTD_CmdInProcess].RespSize);
		RespState = BLTD_RESP_STATE_INVALID;
	}
	BLTD_RespCallBackPtr(BLTD_CmdInProcess,RespState);
	
}
/*******************************************************************************************************/
