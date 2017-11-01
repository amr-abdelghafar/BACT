/*
 * LED_Cfg.h
 *
 * Created: 4/16/2016 3:04:28 PM
 *  Author: hossam
 */ 


#ifndef LED_CFG_H_
#define LED_CFG_H_
#include "BasicTypes.h"

#define LED_MAX_NUM 2

typedef struct
{
	u8 LedPortName;
	u8 LedPinNum;
	}LED_ConfigType;
	extern LED_ConfigType LED_ConfigParamaters[LED_MAX_NUM];




#endif /* LED_CFG_H_ */