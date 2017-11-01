/*
 * UTL.h
 *
 * Created: 5/23/2016 9:04:33 PM
 *  Author: hossam
 */ 


#ifndef UTL_H_
#define UTL_H_

#include "BasicTypes.h"
 void UTL_MemCpy(u8 *Des,const u8 *Src,u16 Length);
 u8 UTL_MemCmp(const u8 * Src1,const u8 *Src2,u16 CmpLength);
 u8 UTL_MemSearch(const u8* SrcPtr, u8 SearchedItem,u8 SearchSize,u8* FoundIndexPtr);
 u16 UTL_StringToInteger(const u8* StrPtr,u8 Length);
 void UTL_MemReplace(u8* DataPtr,u8 SearchedChar,u8 ReplacedChar,u8 BufferLength);


#endif /* UTL_H_ */