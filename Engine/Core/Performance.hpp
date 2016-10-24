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

	 friend std::string ProfileMapToString();

	 //vars
	std::string m_name;
	double startTime;

	bool autoDestruct;
	double endTime;
	double elapsedTime;
	double totalElapsedTime = 0;
	unsigned int numCalls = 1;
	double averageElapsedTime = 0;

	//static ProfileMap s_profileMap;
};

//===========================================================================================================

struct ProfileReport{
	
	//methods
	ProfileReport(const std::string& profileName, const double& newTotalElapsedTime, const unsigned int& newNumCalls);
	
	//overload <= operator
	bool operator<=(const ProfileReport& other) { 
		double avgElapsedTime = this->CalcAverageElapsedTime();
		double otherAvgElapsedTime = other.CalcAverageElapsedTime();

		return (!(avgElapsedTime > otherAvgElapsedTime));
	}

	//helpers
	const double CalcAverageElapsedTime() const;
	const double CalcPercentFrameTime() const;

	void SetAverageElapsedTime() {
		m_averageElapsedTime = CalcAverageElapsedTime();
	}
	void SetPercentFrameTime() {
		m_percentFrameTime = CalcPercentFrameTime();
	}


	std::string BuildProfileReport();
	std::string  ProfileReportToString();
	
	//vars
	std::string m_name;

	double elapsedTime;

	double m_totalElapsedTime;
	unsigned int m_numCalls;
	double m_averageElapsedTime = 0;
	double m_percentFrameTime;
};

typedef std::vector<ProfileReport> ProfileReports;

///----------------------------------------------------------------------------------------------------------
///inline methods

inline const double ProfileReport::CalcAverageElapsedTime() const { 
	
	static const double percentWeightOfNewAverage = 0.5;
	static const double percentWeightOfOldAverage = (1.0 - percentWeightOfNewAverage);

	double averageElapsedTime = 0.0f;

	if (averageElapsedTime == 0.0) {
		averageElapsedTime = m_totalElapsedTime / m_numCalls;
	}
	else {
		//change this to a weighted average
		double newAverageElapsedTime = m_totalElapsedTime / m_numCalls;

		double weightedOldAverage = averageElapsedTime * percentWeightOfOldAverage;

		double weightedNewAverage = newAverageElapsedTime * (percentWeightOfNewAverage);

		averageElapsedTime = weightedOldAverage + weightedNewAverage;
	}
	return averageElapsedTime;
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///global helpers

bool CreateProfileReportsFromProfileMap(ProfileReports& outProfileReports, bool sortReports = true);


//===========================================================================================================

#endif //__includedPerformance__

