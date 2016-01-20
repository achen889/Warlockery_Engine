
//==============================================================================================================
//Time.hpp
//by Albert Chen Jan-27-2015.
//==============================================================================================================
#pragma once

#ifndef _included_Time__
#define _included_Time__

#include <vector>
#include "SystemClockWin32.hpp"
	
	void InitializeTime();
	double GetCurrentSeconds();
	double GetCurrentMilliSeconds();
	double GetCurrentMicroSeconds();
	double GetDeltaSeconds();
	float GetFramesPerSecond();
	std::string GetSystemClockWin32Time();

#endif

