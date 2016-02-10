
//==============================================================================================================
//Time.hpp
//by Albert Chen Jan-27-2015.
//==============================================================================================================
#pragma once

#ifndef _included_Time__
#define _included_Time__

#include <vector>

struct SystemClockWin32;
	
	void InitializeTime();
	double GetCurrentSeconds();
	double GetCurrentMilliSeconds();
	double GetCurrentMicroSeconds();
	double GetDeltaSeconds();
	double GetFramesPerSecond();
	std::string GetSystemClockWin32Time();

#endif

