/*
 * BTN_CFG.h
 *
 * Created: 28/02/2016 02:24:08 م
 *  Author: hossam
 */ 


#ifndef BTN_CFG_H_
#define BTN_CFG_H_

#include "BasicTypes.h"
#define NUM_OF_BTNS 0x01
typedef struct  
{
	u8 BtnPort;
	u8 BtnMask;
}BTN_ConfigType;

extern BTN_ConfigType BTN_COnfigParam[NUM_OF_BTNS];

#endif /* BTN_CFG_H_ */