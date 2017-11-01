/*
 * GYRO.h
 *
 * Created: 5/2/2016 7:30:33 PM
 *  Author: hossam & Amr Tarek
 */ 


#ifndef GYRO_H_
#define GYRO_H_
#define STP 0x00
#define FWD 0x01
#define BWD 0x02

#define RGHT 0x03
#define LEFT 0x04

u8 GetGyroState(void);


#endif /* GYRO_H_ */