//==============================================================================================================
//Performance.hpp
//by Albert Chen Oct-21-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Performance__
#define _included_Performance__

#include "Time.hpp"
#include <map>
#include <string>
#include "Engine/Multithreading/CriticalSection.hpp"

//===========================================================================================================

struct ProfileReport;

typedef std::map<std::string, ProfileReport> ProfileMap;
typedef std::map<std::string, ProfileReport>::iterator ProfileMapIterator;
typedef std::pair<std::string, ProfileReport> ProfileEntry;

extern ProfileMap s_profileMap;

extern bool doDebugProfile;

#define PROFILE_SECTION() ProfileSection p(FUNCTION_NAME);
#define PROFILE_START(name) { ProfileSection p(STRINGIFY(name), false); //causes a warning in 2015



#define PROFILE_STOP() p.ProfileEnd(); }
//===========================================================================================================

struct ProfileSection{
public:
	 ProfileSection(); 
	 ProfileSection(const std::string& profileName, bool isAutoDestruct = true); //set start time

	 void ProfileStart(const std::string& profileName);
	 ~ProfileSection(); //set end time
	 void ProfileEnd();

	 void SetStartTime();
	 void SetEndTime();
	
	 void SetElapsedTime(){ 
		 elapsedTime = endTime - startTime; totalElapsedTime += elapsedTime; 
	 }

	 void AddProfileToMap(const std::string& profileName);

	 friend void RenderProfileMapToScreen();

	 friend std::string OutputProfileMapToScreen();

	

	 //vars
	std::string m_name;
	double startTime;

	bool autoDestruct;
	double endTime;
	double elapsedTime;
	double totalElapsedTime = 0;
	unsigned int numCalls = 1;
	double averageElapsedTime = 0;

	static bool s_doDebugProfiling;

	
	//static ProfileMap s_profileMap;
};

//===========================================================================================================

struct ProfileReport{
	
	//methods
	ProfileReport(const std::string& profileName, const double& newTotalElapsedTime, const unsigned int& newNumCalls);
	
	//helpers
	void CalcAverageElapsedTime();
	void CalcPercentFrameTime();

	std::string BuildProfileReport();
	std::string  OutputProfileReportToScreen();
	
	//vars
	std::string m_name;

	double elapsedTime;

	double totalElapsedTime;
	unsigned int numCalls;
	double averageElapsedTime = 0;
	double percentFrameTime;
};

///----------------------------------------------------------------------------------------------------------
///inline methods

inline void ProfileReport::CalcAverageElapsedTime(){ 
	averageElapsedTime = totalElapsedTime / numCalls;
}



//===========================================================================================================

#endif //__includedPerformance__

