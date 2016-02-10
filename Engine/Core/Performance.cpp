//==============================================================================================================
//Performance.cpp
//by Albert Chen Oct-21-2015.
//==============================================================================================================

#include "Performance.hpp"
#include "Utilities.hpp"
#include "..\Renderer\Text\TextSystem.hpp"

//===========================================================================================================

ProfileMap s_profileMap = ProfileMap();

bool doDebugProfile = true;

bool ProfileSection::s_doDebugProfiling = false;

//===========================================================================================================

CriticalSection profileCritSec;

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///constructors

ProfileSection::ProfileSection(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

ProfileSection::ProfileSection(const std::string& profileName, bool isAutoDestruct):
autoDestruct(isAutoDestruct)
{
	ProfileStart(profileName);
}

//-----------------------------------------------------------------------------------------------------------

void ProfileSection::ProfileStart(const std::string& profileName){
	if (doDebugProfile){
		SetStartTime();
		m_name = profileName;
	}
}

//-----------------------------------------------------------------------------------------------------------

ProfileSection::~ProfileSection(){
	if (autoDestruct){
		ProfileEnd();
	}
}

//-----------------------------------------------------------------------------------------------------------

void ProfileSection::ProfileEnd(){
	if (doDebugProfile){
		SetEndTime();

		SetElapsedTime();

		AddProfileToMap(m_name);
	}
}

//-----------------------------------------------------------------------------------------------------------

void ProfileSection::SetStartTime(){ 
	startTime = GetCurrentMilliSeconds(); 
}

//-----------------------------------------------------------------------------------------------------------

void ProfileSection::SetEndTime(){ 
	endTime = GetCurrentMilliSeconds(); 
}

//-----------------------------------------------------------------------------------------------------------

void ProfileSection::AddProfileToMap(const std::string& profileName){
	profileCritSec.Enter();
	ProfileMapIterator profileEntry = s_profileMap.find(profileName);
	//if entry exists, increment existing entry
	if (profileEntry != s_profileMap.end()){
		ProfileReport& existingProfile = (profileEntry->second);

		//assumes elapsed time already set
		existingProfile.totalElapsedTime += elapsedTime;
		existingProfile.numCalls++;//= numCalls;
		
		existingProfile.CalcAverageElapsedTime();
	}
	else{
		//numCalls++;
		//add entry
		s_profileMap.insert(ProfileEntry(profileName, ProfileReport(profileName, totalElapsedTime, numCalls)));
		//Access violation reading location 0xBAADF00D. serious memory corruption here O.o
	}
	profileCritSec.Exit();
}

//-----------------------------------------------------------------------------------------------------------

void RenderProfileMapToScreen(){

	if (ProfileSection::s_doDebugProfiling) {
		//std::string profileResults = GetProfileMapString();
		std::string profileResults = OutputProfileMapToScreen();
		//ConsolePrintString(profileResults);
		OUTPUT_STRING_TO_SCREEN(profileResults, theOGLRenderer->GetDisplayWidth() * 0.65f, 850);
	}

}

//-----------------------------------------------------------------------------------------------------------

//printing the report
std::string OutputProfileMapToScreen(){
	//PROFILE_SECTION();
	profileCritSec.Enter();
	//IntVec2 startReportPos = IntVec2(50, 800);

	//double frameTimeMilliSeconds = GetSystemClock().GetDeltaSeconds() * 1000.0;
	//something...
	std::string outputProfileMap ="\n//===========================================================================================================\n";
	for (ProfileMapIterator it = s_profileMap.begin(); it != s_profileMap.end(); ++it ){
		 ProfileReport& report = (it->second);
		 report.CalcAverageElapsedTime();
		 report.CalcPercentFrameTime();
		 
		 outputProfileMap += report.OutputProfileReportToScreen();
		 //startReportPos.y -= (int)TEXT_LINE_SPACING_FACTOR * 5;

	}

	//clear profile map
	s_profileMap.clear();

	outputProfileMap += "\n//===========================================================================================================\n";
	
	profileCritSec.Exit();

	return outputProfileMap;
}

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///profile stats

ProfileReport::ProfileReport(const std::string& profileName, const double& newTotalElapsedTime, const unsigned int& newNumCalls) :
m_name(profileName),
totalElapsedTime(newTotalElapsedTime),
numCalls(newNumCalls)
{

	//CalcAverageElapsedTime();

}

//-----------------------------------------------------------------------------------------------------------

void ProfileReport::CalcPercentFrameTime(){
	double totalFrameTime = GetDeltaSeconds() * 1000.0;
	percentFrameTime = totalElapsedTime / totalFrameTime;
	percentFrameTime *= 100.0;
}

//-----------------------------------------------------------------------------------------------------------

std::string ProfileReport::BuildProfileReport(){

	std::string myReport = "==="+ m_name +" ===";
	myReport += "\nTotal  : " + FloatToString((float)totalElapsedTime) +"ms";
	myReport += "\nAverage: " + FloatToString((float)averageElapsedTime) + "ms";
	myReport += "\nNum Calls: " + IntToString(numCalls) + "\n";
	//myReport += "\n Frame Time: " + IntToString((int)percentFrameTime) + "\n";

	return myReport;
}

//-----------------------------------------------------------------------------------------------------------

std::string ProfileReport::OutputProfileReportToScreen(){
	std::string profileReport = BuildProfileReport();

	return profileReport;
	//OUTPUT_STRING_TO_SCREEN(profileReport, screenPos.x, screenPos.y);

}


//===========================================================================================================
