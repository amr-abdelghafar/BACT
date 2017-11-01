/*
 * WMGR.h
 *
 * Created: 5/12/2016 12:01:35 AM
 *  Author: hossam & Amr Tarek
 */ 


#ifndef WMGR_H_
#define WMGR_H_

#define DEVICE_ROLE_MASTER 0x01
#define DEVICE_ROLE_SLAVE 0x00

#define WMGR_SCRIPT_STATE_DONE        0x00
#define WMGR_SCRIPT_STATE_IN_PROGRESS 0x01
#define WMGR_SCRIPT_STATE_FAILED      0x02

#define WMGR_RX_STATUS_CONNECTED     0x00
#define WMGR_RX_STATUS_INPROGRESS    0x01
#define WMGR_RX_STATUS_DISCONN       0x02
#define MAC_ADDRESS_LENGTH 17u

typedef struct
{
	const u8* SpeedPtr;
	u8 SpeedOffset;
	u8 StringSize;
}SpeedBufferType;

extern u8 WMGR_MacAddress[MAC_ADDRESS_LENGTH];

void WMGR_Init(void);
u8 WMGR_RunInitScript(void);
void WMGR_GetMacAddress(u8** MacAddressPtr);
u8 WMGR_GetData(u8* Speed,u8* Direction);
void WMGR_StartConnection(u8* MacAddrPtr,u8 MacAddLength);
extern  SpeedBufferType SpeedBuffer[5];
void WMGR_SendData(u8 Speed,u8 Direction);
u8 WMGR_MasterStartConn(u8* MAcAddPtr,u8 MacAddLength);
#endif /* WMGR_H_ */
