/*
 * BTN.h
 *
 * Created: 28/02/2016 02:23:53 م
 *  Author: hossam & Amr Tarek
 */ 


#ifndef BTN_H_
#define BTN_H_

#include "../Config/BasicTypes.h"

#define BTN_ACTIVE 0x00
#define BTN_INACTIVE 0x01

void BTN_Init(void);
u8 BTN_GetState(u8 BtnId);



#endif /* BTN_H_ */