//==============================================================================================================
//Time.cpp
//by Albert Chen Jan-27-2015.
//==============================================================================================================
#include <windows.h>
#include "Engine/Core/Time.hpp"
#include "Engine/Console/DevConsole.hpp"

double g_secondsPerTick;

CONSOLE_COMMAND(time){
	UNUSED_COMMAND_ARGS
	const unsigned int timeTextLineSkipValue = 500;
	std::string appTimeText = "Seconds since Application Start up = " + FloatToString((float)GetCurrentSeconds()) + "";
	
	OUTPUT_STRING_TO_CONSOLE(appTimeText, timeTextLineSkipValue);
}

//===========================================================================================================

void InitializeTime(){
	LARGE_INTEGER ticksPerSecond;
	QueryPerformanceFrequency(&ticksPerSecond );
	g_secondsPerTick = 1.0 / ticksPerSecond.QuadPart;

	REGISTER_CONSOLE_COMMAND(time, "Display GetCurrentSeconds value.");
}

//===========================================================================================================

double GetCurrentSeconds(){
	LARGE_INTEGER ticksPerSecond;
	QueryPerformanceCounter(&ticksPerSecond );
	return g_secondsPerTick * ticksPerSecond.QuadPart;
}

//===========================================================================================================

double GetCurrentMilliSeconds(){
	return GetCurrentSeconds() * 1000.0;
}

double GetCurrentMicroSeconds(){
	return GetCurrentSeconds() * 1000.0 * 1000.0;
}

//===========================================================================================================

double GetDeltaSeconds(){
	static double lastTime = GetCurrentSeconds();
	double currentTime = GetCurrentSeconds();
	double deltaSeconds = currentTime - lastTime;
	lastTime = currentTime;

	return deltaSeconds;
}

//===========================================================================================================

float GetFramesPerSecond(){
	return 1.0f / (float)GetDeltaSeconds();
}

//===========================================================================================================

std::string GetSystemClockWin32Time(){
	return GetSystem32TimeString();
}

//===========================================================================================================