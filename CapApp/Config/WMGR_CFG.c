/*
 * WMGR_CFG.c
 *
 * Created: 5/12/2016 12:02:32 AM
 *  Author: hossam
 */ 
#include "BasicTypes.h"
#include "WMGR_Cfg.h"
#include "BLTD.h"
#include "BLTD_Cfg.h"
#if(DEVICE_ROLE == DEVICE_ROLE_SLAVE)
const u8 WMGR_StartScriptCommands[WMGR_START_SCRIPT_SIZE] = 
{
	SELF_TEST_CMD_ID,
	GET_MAC_ADDRESS_ID,
	SET_DEVICE_NAME_ID,
	SET_ROLE_SLAVE_ID,
	SET_PASS_KEY_ID,
	#if(MODULE_TYPE == MODULE_TYPE_M10)
	SET_DEVICE_TYPE,
	#endif
	
	
	CANCEL_NOTIF_ID
	
	
	
};
#else
const u8 WMGR_StartScriptCommands[WMGR_START_SCRIPT_SIZE] =
{
	SET_ROLE_SLAVE_ID,
	SELF_TEST_CMD_ID,
	GET_MAC_ADDRESS_ID,
	SET_DEVICE_NAME_ID,
	
	SET_PASS_KEY_ID,
	#if(MODULE_TYPE == MODULE_TYPE_M10)
	SET_DEVICE_TYPE,
	#else
	CLR_LAST_CONN_ID,
	#endif
	SET_ROLE_MASTER_ID,
	
	
	
	
	
};
#endif

#if (DEVICE_ROLE == DEVICE_ROLE_SLAVE)
const u8 WMGR_DeviceName[]   = "B-ACT";
const u8 WMGR_DeviceNameSize  = 5;
#else
const u8 WMGR_DeviceName[]   = "B-ACT";
const u8 WMGR_DeviceNameSize  = 5;
#endif
const u8 WMGR_PassKey[]      = "123456";
const u8 WMGR_PassKeySize     = 6;