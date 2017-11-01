/*
* GYRO.c
*
* Created: 5/2/2016 7:29:58 PM
*  Author: hossam
*/

#include "BasicTypes.h"
#include "GYRO.h"
#include "L3G4200D.h"
u8 GetxDiff(void)
{
	u8 Diff;
	int Datax;
	GYHD_ReadYDirection(&Datax,0);
	if(Datax > 2000)
	{
		Diff = 1;
		
	}
	else if(Datax < -2000)
	{
		Diff = 2;

	}
	else
	{
		Diff = 0;
	}
	return Diff;
	
	
}
u8 GetyDiff(void)
{
	u8 Diff;
	int Datax;
	GYHD_ReadXDirection(&Datax,0);
	if(Datax > 2000)
	{
		Diff = 1;
		
	}
	else if(Datax < -2000)
	{
		Diff = 2;

	}
	else
	{
		Diff = 0;
	}
	return Diff;
	
	
}


u8 GetGyroState(void)
{
	u8 Diff;
	static u8 State = STP;
	static u8 lastDiff ;
	static u8 ReadingStarted = 0;


	static u8 YawCheckAllowed  =0;
	static u8 PichCheckAllowed = 1;
	
	if(PichCheckAllowed == 1)
	{
		Diff = GetxDiff();
		switch(State)
		{
			case STP:
			{
				
				//LCD_DispString("STP");
				if(ReadingStarted == 0)
				{
					if((Diff != 0))
					{
						ReadingStarted = 1;
						lastDiff = Diff;
						YawCheckAllowed =0;
					}
					else
					{
						YawCheckAllowed = 1;
					}
				}
				else
				{
					if(Diff == 0)
					{
						ReadingStarted = 0;
						if(lastDiff == 2)
						{
							State = FWD;
							YawCheckAllowed = 0;
						}
						else if(lastDiff == 1)
						{
							State = BWD;
							YawCheckAllowed = 0;
						}
					}
				}
			}
			break;
			case FWD:
			{
				//LCD_DispString("FWD");
				if(ReadingStarted == 0)
				{
					if(Diff != 0)
					{
						ReadingStarted = 1;
						lastDiff = Diff;
					}
				}
				else
				{
					if(Diff == 0)
					{
						ReadingStarted = 0;
						if(lastDiff == 2)
						{
							State = FWD;
						}
						else if(lastDiff == 1)
						{
							State = STP;
							
						}
					}
				}
			}
			break;
			
			case BWD:
			{
				//LCD_DispString("BWD");
				if(ReadingStarted == 0)
				{
					if(Diff != 0)
					{
						ReadingStarted = 1;
						lastDiff = Diff;
					}
				}
				else
				{
					if(Diff == 0)
					{
						ReadingStarted = 0;
						if(lastDiff == 2)
						{
							State = STP;
							
						}
						else if(lastDiff == 1)
						{
							State = BWD;
						}
					}
				}
			}

		}
	}

	if(YawCheckAllowed == 1)
	{
		Diff = GetyDiff();
		switch(State)
		{
			case STP:
			{
				//LCD_DispString("STP");
				if(ReadingStarted == 0)
				{
					if((Diff != 0))
					{
						ReadingStarted = 1;
						lastDiff = Diff;
						PichCheckAllowed =0;
					}
					else
					{
						PichCheckAllowed = 1;
					}
				}
				else
				{
					if(Diff == 0)
					{
						ReadingStarted = 0;
						if(lastDiff == 2)
						{
							State = RGHT;
							PichCheckAllowed = 0 ;
						}
						else if(lastDiff == 1)
						{
							State = LEFT;
							PichCheckAllowed = 0 ;
						}
					}
				}
			}
			break;
			case RGHT:
			{
				//LCD_DispString("RIGHT");
				if(ReadingStarted == 0)
				{
					if(Diff != 0)
					{
						ReadingStarted = 1;
						lastDiff = Diff;
					}
				}
				else
				{
					if(Diff == 0)
					{
						ReadingStarted = 0;
						if(lastDiff == 2)
						{
							State = RGHT;
						}
						else if(lastDiff == 1)
						{
							State = STP;
							
						}
					}
				}
			}
			break;
			
			case LEFT:
			{
				//LCD_DispString("LEFT");
				if(ReadingStarted == 0)
				{
					if(Diff != 0)
					{
						ReadingStarted = 1;
						lastDiff = Diff;
					}
				}
				else
				{
					if(Diff == 0)
					{
						ReadingStarted = 0;
						if(lastDiff == 2)
						{
							State = STP;
							
						}
						else if(lastDiff == 1)
						{
							State = LEFT;
						}
					}
				}
			}

		}
	}

	return State;
}