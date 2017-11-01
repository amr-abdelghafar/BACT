#ifndef _BLTD_CFG_
#define _BLTD_CFG_
#include "BasicTypes.h"
#include "UART_Drv.h"
#include "BLTD.h"
#include "DIO.h"
#include "WMGR_Cfg.h"
#define MAX_CMD_SIZE 40
#define MAX_RESP_SIZE 40

typedef struct
{
	const u8* ComdPtr;
	const u8 CmdSize;
	u8* CmdParamPtr;
	u8  CmdParamSize;
	const u8* RespPtr;
	const u8 RespSize;
	u8* RespParamPtr;
	u8  RespParamSize;
}BLTD_CmdType;

extern BLTD_CmdType BLTD_CmdList[BLTD_NUM_OF_COMMANDS];


#endif
