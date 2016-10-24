//==============================================================================================================
//Clock.hpp
//by Albert Chen Oct-13-2015.
//==============================================================================================================

#pragma once


#ifndef _included_SystemClockWin32__
#define _included_SystemClockWin32__

#include <stdio.h>
#include <string>

//===========================================================================================================

struct _SYSTEMTIME;

typedef _SYSTEMTIME SYSTEMTIME;
typedef unsigned short WORD;

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
	SystemClockWin32(SYSTEMTIME* systemTimeWin32);

	~SystemClockWin32() {
		//do nothing
	}

	//setters
	void SetClockFromSystemTimeWin32(SYSTEMTIME* systemTimeWin32);
	void SetNameOfMonth();
	void SetDayOfWeek(WORD WdayOfWeek);
	
	//getters

	std::string ToString(bool isNumerical = false, bool addDateString = false ){
		std::string timeStr;
		if (addDateString) {
			timeStr += GetDateString(isNumerical) + " ";
		}
		timeStr += GetTimeString();
		return timeStr;
	}

	std::string GetDateString(bool isNumerical = false);

	std::string GetTimeString();


};

extern SYSTEMTIME* g_systemTimeWin32;

//===========================================================================================================

//win32 clock functions
void SetEngineClock(SystemClockWin32* clock, const bool& isLocalTime = false);

std::string GetSystem32TimeString();


///----------------------------------------------------------------------------------------------------------
///inline global helpers

inline std::string GetSystem32TimeString(){
	//create temp sys clock win32
	SystemClockWin32 tempSysClockWin32(g_systemTimeWin32);

	SetEngineClock(&tempSysClockWin32, true);

	std::string timeStr = tempSysClockWin32.ToString();

	return timeStr;
}

//===========================================================================================================

#endif //__includedClock__

