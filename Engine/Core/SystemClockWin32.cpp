//==============================================================================================================
//Clock.cpp
//by Albert Chen Oct-13-2015.
//==============================================================================================================

#include "SystemClockWin32.hpp"
#include "Utilities.hpp"


#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//===========================================================================================================

//I think this is okay, I mean the compile seems happy
SYSTEMTIME* g_systemTimeWin32 = new SYSTEMTIME;

//===========================================================================================================

SystemClockWin32::SystemClockWin32(SYSTEMTIME* systemTimeWin32){

	SetClockFromSystemTimeWin32(systemTimeWin32);
	
}

//-----------------------------------------------------------------------------------------------------------

void SystemClockWin32::SetClockFromSystemTimeWin32(SYSTEMTIME* systemTimeWin32){
	Year = (int)systemTimeWin32->wYear;
	//ClampInt(clock.Year, 1601, 30827);
	Month = (unsigned int)systemTimeWin32->wMonth;
	SetNameOfMonth();
	SetDayOfWeek(systemTimeWin32->wDayOfWeek);
	DayOfMonth = (int)systemTimeWin32->wDay;

	Hour = (int)systemTimeWin32->wHour;
	Minute = (int)systemTimeWin32->wMinute;
	Second = (int)systemTimeWin32->wSecond;
	Milliseconds = (int)systemTimeWin32->wMilliseconds;
}

void SystemClockWin32::SetNameOfMonth(){
	if (Month == 1){
		nameOfMonth = "January";
	}
	else if (Month == 2){
		nameOfMonth = "February";
	}
	else if (Month == 3){
		nameOfMonth = "March";
	}
	else if (Month == 4){
		nameOfMonth = "April";
	}
	else if (Month == 5){
		nameOfMonth = "May";
	}
	else if (Month == 6){
		nameOfMonth = "June";
	}
	else if (Month == 7){
		nameOfMonth = "July";
	}
	else if (Month == 8){
		nameOfMonth = "August";
	}
	else if (Month == 9){
		nameOfMonth = "September";
	}
	else if (Month == 10){
		nameOfMonth = "October";
	}
	else if (Month == 11){
		nameOfMonth = "November";
	}
	else if (Month == 12){
		nameOfMonth = "December";
	}
}

//-----------------------------------------------------------------------------------------------------------

void SystemClockWin32::SetDayOfWeek(WORD WdayOfWeek){
	if (WdayOfWeek == 0){
		DayOfWeek = "Sunday";
	}
	else if (WdayOfWeek == 1){
		DayOfWeek = "Monday";
	}
	else if (WdayOfWeek == 2){
		DayOfWeek = "Tuesday";
	}
	else if (WdayOfWeek == 3){
		DayOfWeek = "Wednesday";
	}
	else if (WdayOfWeek == 4){
		DayOfWeek = "Thursday";
	}
	else if (WdayOfWeek == 5){
		DayOfWeek = "Friday";
	}
	else if (WdayOfWeek == 6){
		DayOfWeek = "Saturday";
	}
}

//-----------------------------------------------------------------------------------------------------------

std::string SystemClockWin32::GetDateString(bool isNumerical){
	if (!isNumerical){
		return "" + DayOfWeek + ", " + nameOfMonth + " " + IntToString(DayOfMonth) + ", " + IntToString(Year);
	}
	return "" + IntToString(Month) + "/" + IntToString(DayOfMonth) + "/" + IntToString(Year);
}

//-----------------------------------------------------------------------------------------------------------

std::string SystemClockWin32::GetTimeString(){
	std::string timeString = "" + IntToString(Hour);
	std::string minuteString;
	if (Minute < 10)
		minuteString += "0";
	minuteString += IntToString(Minute);

	std::string secondsString;
	if (Second < 10)
		secondsString += "0";
	secondsString += IntToString(Second);

	timeString += ":" + minuteString + ":" + secondsString;

	return timeString;
}

//-----------------------------------------------------------------------------------------------------------

void SetEngineClock(SystemClockWin32* clock, const bool& isLocalTime){
	//PROFILE_SECTION();
	//#ifdef(WIN32) //put this here later for platform stuff
	if (!isLocalTime)
		GetSystemTime(g_systemTimeWin32);
	else
		GetLocalTime(g_systemTimeWin32);

	clock->SetClockFromSystemTimeWin32(g_systemTimeWin32);
}

//===========================================================================================================

