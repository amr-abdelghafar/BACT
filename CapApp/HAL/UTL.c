/*
 * UTL.c
 *
 * Created: 5/23/2016 9:03:01 PM
 *  Author: hossam & Amr Tarek
 */ 
#include "../Config/BasicTypes.h"
 void UTL_MemCpy(u8 *Des,const u8 *Src,u16 Length)
{
	u16 i;
	for(i = 0 ; i<Length ; i++)
	{
		*(Des+i) = *(Src+i);
	}
}

u8 UTL_MemCmp(const u8 * Src1,const u8 *Src2,u16 CmpLength)
{
	u8 RetVal = 0;
	u16 i;
	for(i = 0 ;(i < CmpLength); i++)
	{
		if(Src1[i] != Src2[i])
		{
			RetVal = 1;
			break;
		}
	}
	return RetVal;
}

u8 UTL_MemSearch(const u8* SrcPtr, u8 SearchedItem,u8 SearchSize,u8* FoundIndexPtr)
{
	u8 ItemFound = 1;
	u8 LoopIndex;
	for(LoopIndex = 0; LoopIndex <SearchSize; LoopIndex ++)
	{
		if(SrcPtr[LoopIndex] == SearchedItem)
		{
			ItemFound = 0;
			*FoundIndexPtr = LoopIndex;
			LoopIndex = SearchSize;
		}
	}
	return ItemFound;
}
u16 UTL_StringToInteger(const u8* StrPtr,u8 Length)
{
	u8 LoopIndex;
	u8 Base = 1;
	u16 Integer = 0;
	for(LoopIndex = 0; LoopIndex < Length; LoopIndex ++)
	{
	   Integer += (StrPtr[Length - 1 - LoopIndex] - '0') * Base;
	   Base *=10;
	}
	return Integer;
}
void UTL_MemReplace(u8* DataPtr,u8 SearchedChar,u8 ReplacedChar,u8 BufferLength)
{
	u8 LoopIndex;
	for(LoopIndex = 0; LoopIndex < BufferLength; LoopIndex ++)
	{
		if(DataPtr[LoopIndex] == SearchedChar)
		{
			DataPtr[LoopIndex] = ReplacedChar;
		}
	}
}