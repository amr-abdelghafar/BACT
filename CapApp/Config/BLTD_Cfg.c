#include "BasicTypes.h"
#include "BLTD.h"
#include "BLTD_Cfg.h"
#include "WMGR.h"
#include "WMGR_Cfg.h"
#if (MODULE_TYPE == MODULE_TYPE_M10)
 BLTD_CmdType BLTD_CmdList[BLTD_NUM_OF_COMMANDS] = 

{
	{
		(u8*)"AT",2,NULL,0,(u8*)"OK\r\n",4,NULL,0
	},
	{
	    (u8*)"AT+NAME=",7,NULL,0,(u8*)"+NAME=",6,NULL,0	
	},
	{
		(u8*)"AT+ROLE1",8,NULL,0,(u8*)"+ROLE=1\r\nOK\r\n",13,NULL,0
	},
	{
	    (u8*)"AT+ROLE0",8,NULL,0,(u8*)"+ROLE=0\r\nOK\r\n",13,NULL,0	
	},
	{
		(u8*)"AT+PASS",7,NULL,0,(u8*)"+PASS=",6,NULL,12
	},
	{
		(u8*)"AT+ADDR",7,NULL,0,(u8*)"+ADDR=",6,&WMGR_MacAddress[0],19
	},
	{
		NULL,0,NULL,0,NULL,0,NULL,0
	},
	{
		NULL,0,NULL,0,(u8*)"OK+CONN",7,NULL,0
	},
	{
		(u8*)"AT+TYPE0",8,NULL,0,(u8*)"+TYPE=0\r\nOK\r\n",13,NULL,0
	},
	{
		(u8*)"AT+NOTI0",8,NULL,0,(u8*)"+NOTI=0\r\nOK\r\n",13,NULL,0
	},
	{
		(u8*)"AT+CLEAR",8,NULL,0,(u8*)"+ADDR=",6,&WMGR_MacAddress[0],20
	},
	{
		(u8*)"AT+CONN",7,NULL,0,(u8*)"OK+CONN",7,NULL,0
	},
	{
		(u8*)"AT+RENEW",8,NULL,0,(u8*)"+RENEW\r\nOK\r\n",12,NULL,0
	}
} ;
#else
BLTD_CmdType BLTD_CmdList[BLTD_NUM_OF_COMMANDS] = 
{
	{
		(u8*)"AT+INIT",7,NULL,0,(u8*)"OK\r\n",4,NULL,0
	},
	{
		(u8*)"AT+NAME=",8,NULL,0,(u8*)"OK\r\n",4,NULL,0
	},
	{
		(u8*)"AT+ROLE=1",9,NULL,0,(u8*)"OK\r\n",4,NULL,0
	},
	{
		(u8*)"AT+ROLE=0",9,NULL,0,(u8*)"OK\r\n",4,NULL,0
	},
	{
		(u8*)"AT+PSWD=",8,NULL,0,(u8*)"OK\r\n",4,NULL,0
	},
	{
		(u8*)"AT+ADDR?",8,NULL,0,(u8*)"+ADDR:",6,&WMGR_MacAddress[0],20
	},
	{
		NULL,0,NULL,0,NULL,0,NULL,0
	},
	{
		NULL,0,NULL,0,(u8*)"OK+CONN",7,NULL,0
	},
	{
		(u8*)"AT+TYPE1",8,NULL,0,(u8*)"+TYPE=1\r\nOK\r\n",13,NULL,0
	},
	{
		(u8*)"AT+INQ",6,NULL,0,(u8*)"OK\r\n",4,NULL,0
	},
	{
		(u8*)"AT+INQC",7,NULL,0,(u8*)"OK\r\n",4,NULL,0
	},
	{
		(u8*)"AT+LINK=",8,NULL,0,(u8*)"OK\r\n",4,NULL,0
	},
	{
		(u8*)"AT+RENEW",8,NULL,0,(u8*)"+RENEW\r\nOK\r\n",12,NULL,0
	}
} ;
#endif
