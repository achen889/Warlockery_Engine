//==============================================================================================================
//Clock.hpp
//by Albert Chen Oct-13-2015.
//==============================================================================================================

#pragma once


#ifndef _included_SystemClockWin32__
#define _included_SystemClockWin32__

#include <windows.h>
#include <stdio.h>
#include <string>

//===========================================================================================================

struct SystemClockWin32{
	int Year;
	unsigned int Month;
	std::string nameOfMonth;
	int DayOfMonth;
	std::string DayOfWeek;

	int Hour;
	int Minute;
	int Second;
	int Milliseconds;

	SystemClockWin32(){
		//do nothing
	}
	SystemClockWin32(const SYSTEMTIME& systemTimeWin32);
	//setters
	void SetClockFromSystemTimeWin32(const SYSTEMTIME& systemTimeWin32);
	void SetNameOfMonth();
	void SetDayOfWeek(WORD WdayOfWeek);

	
	//getters

	std::string ToString(bool isNumerical = false){
		return GetDateString(isNumerical)+" "+ GetTimeString();
	}

	std::string GetDateString(bool isNumerical = false);

	std::string GetTimeString();


};

static SYSTEMTIME g_systemTimeWin32;

//===========================================================================================================

//win32 clock functions
void SetEngineClock(SystemClockWin32& clock, const bool& isLocalTime = false);

inline std::string GetSystem32TimeString(){
	SystemClockWin32 tempSysClock;

	SetEngineClock(tempSysClock, true);

	return tempSysClock.ToString();
}

//===========================================================================================================

#endif //__includedClock__

