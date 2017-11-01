/*
 * LED.h
 *
 * Created: 4/16/2016 3:04:16 PM
 *  Author: hossam & Amr Tarek
 */ 


#ifndef LED_H_
#define LED_H_
#include "../Config/BasicTypes.h"
#define LED_DATA_OFF 0xff   /* From 0x00 to 0xff by Amr */
#define LED_DATA_ON 0x00    /* From 0xff to 0x00 by Amr */

/*Is a function to initialize direction of all connected LEDs and turn them all off*/
void LED_Init(void);
/* Is A function to Write a data LED_DATA_OFF or LED_DATA_ON to a specific LED bu LedId*/
void LED_SetData(u8 LedId,u8 LedData);


#endif /* LED_H_ */