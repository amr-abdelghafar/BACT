/*
 * BLTD.h
 *
 * Created: 5/11/2016 9:05:02 AM
 *  Author: hossam
 */ 


#ifndef BLTD_H_
#define BLTD_H_

#include "BasicTypes.h"
#define BLTD_NUM_OF_COMMANDS 0x0d

#define SELF_TEST_CMD_ID     0x00
#define SET_DEVICE_NAME_ID   0x01
#define SET_ROLE_MASTER_ID   0x02
#define SET_ROLE_SLAVE_ID    0x03
#define SET_PASS_KEY_ID      0x04
#define GET_MAC_ADDRESS_ID   0x05
#define MAKE_DISCOVERABLE_ID 0x06
#define WAIT_FOR_CONNECTION_ID  0x07
#define SET_DEVICE_TYPE      0x08
#define CANCEL_NOTIF_ID      0x09
#define CLR_LAST_CONN_ID     0x0a
#define START_CONN_ID        0x0b
#define RENEW_CMD_ID         0x0c
#define INVALID_CMD          0xff

#define BLTD_RESP_STATE_VALID   0x00
#define BLTD_RESP_STATE_INVALID 0x01

typedef void (*BLTD_RespCbkFnPtrType)(u8 CmdId,u8 CmdResp);

void BLTD_Init(BLTD_RespCbkFnPtrType RespCallBackPtr);
void BLTD_SendCmd(u8 CommandId);
void BLTD_SetDeviceName(const u8* DeviceNamePtr,u8 DeviceNameLength);
void BLTD_SetPassKey(const u8* PassKey,u8 PassKeyLength);
void BLTD_SetToConnectAddress( u8* AddressPtr,u8 AddressLength);
void BLTD_GetMacAddress(u8** MacAddressPtr);
void BLTD_PowerOn(void);
void BLTD_PowerOff(void);



#endif /* BLTD_H_ */