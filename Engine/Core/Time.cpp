//==============================================================================================================
//Time.cpp
//by Albert Chen Jan-27-2015.
//==============================================================================================================

#include "Engine/Core/Time.hpp"
#include "Engine/Console/DevConsole.hpp"

#include "SystemClockWin32.hpp"

double g_secondsPerTick;

CONSOLE_COMMAND(time){
	UNUSED_COMMAND_ARGS
	const unsigned int timeTextLineSkipValue = 500;
	std::string appTimeText = "Seconds since Application Start Up = " + FloatToString((float)GetCurrentSeconds()) + "";
	
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

double GetFramesPerSecond(){

	static double lastFPS = 1.0 / GetDeltaSeconds();

	double curFPS = 1.0 / GetDeltaSeconds();

	double lastFPSBlend = 0.9;
	double curFPSBlend = 1.0 - lastFPSBlend;

	double blendFPS = (curFPS * curFPSBlend) + (lastFPS * lastFPSBlend);

	lastFPS = curFPS;

	return blendFPS;
}

//===========================================================================================================

std::string GetSystemClockWin32Time(){
	return GetSystem32TimeString();
}

//===========================================================================================================