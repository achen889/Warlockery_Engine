//==============================================================================================================
//Performance.cpp
//by Albert Chen Oct-21-2015.
//==============================================================================================================

#include "Performance.hpp"
#include "Utilities.hpp"
#include "..\Renderer\Text\TextSystem.hpp"

#include "Engine/Multithreading/CriticalSection.hpp"
#include "SortingUtils.hpp"

//===========================================================================================================

ProfileMap s_profileMap = ProfileMap();

bool doDebugProfile = false;

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
		existingProfile.m_totalElapsedTime += elapsedTime;
		existingProfile.m_numCalls++;//= numCalls;
		
		existingProfile.SetAverageElapsedTime();
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

///----------------------------------------------------------------------------------------------------------
///friend methods

void RenderProfileMapToScreen(){
	//PROFILE_SECTION();
	if (doDebugProfile) {
		//std::string profileResults = GetProfileMapString();
		std::string profileResults = ProfileMapToString();
		//ConsolePrintString(profileResults);
		theOGLRenderer->RenderTextString(profileResults, Vector2(50.0f, 850.0f), Rgba::WHITE, 0.5f);
		//OUTPUT_STRING_TO_SCREEN(profileResults, theOGLRenderer->GetDisplayWidth() * 0.65f, 850);
	}

}

//-----------------------------------------------------------------------------------------------------------

//printing the report
std::string ProfileMapToString(){
	//PROFILE_SECTION();
	profileCritSec.Enter();
	//IntVec2 startReportPos = IntVec2(50, 800);

	std::string outputProfileMap ="\n//===========================================================================================================\n";
	
	ProfileReports profileReportsInMap;
	if (CreateProfileReportsFromProfileMap(profileReportsInMap, true)) {
		for (ProfileReport& report : profileReportsInMap) {
			outputProfileMap += report.ProfileReportToString();
		}
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
m_totalElapsedTime(newTotalElapsedTime),
m_numCalls(newNumCalls)
{

	SetPercentFrameTime();

}

//-----------------------------------------------------------------------------------------------------------

const double ProfileReport::CalcPercentFrameTime() const {
	double totalFrameTime = GetDeltaSeconds() * 1000.0;
	
	double percentFrameTime;
	percentFrameTime = m_totalElapsedTime / (1.0 + totalFrameTime);
	percentFrameTime *= 100.0;

	return percentFrameTime;
}

//-----------------------------------------------------------------------------------------------------------

std::string ProfileReport::BuildProfileReport(){

	std::string myReport = "==="+ m_name +" ===";
	myReport += "\nTotal  : " + FloatToString((float)m_totalElapsedTime, "%.03f") +"ms";
	myReport += "\nAverage: " + FloatToString((float)m_averageElapsedTime, "%.03f") + "ms";
	myReport += "\nNum Calls: " + IntToString(m_numCalls) +"\n";
	//myReport += "\n% Frame Time: " + DoubleToString(m_percentFrameTime) + "\n";

	return myReport;
}

//-----------------------------------------------------------------------------------------------------------

std::string ProfileReport::ProfileReportToString(){
	std::string profileReport = BuildProfileReport();

	return profileReport;

}

//===========================================================================================================

//creates a sorted list of profile reports to work with
bool CreateProfileReportsFromProfileMap(ProfileReports& outProfileReports, bool sortReports) {
	outProfileReports.clear();
	for (ProfileMapIterator it = s_profileMap.begin(); it != s_profileMap.end(); ++it) {
		ProfileReport& report = (it->second);
		outProfileReports.push_back(report);
	}

	if (sortReports) {
		QuickSort(outProfileReports, outProfileReports.size(), 0);
	}

	return (!outProfileReports.empty());
}

//===========================================================================================================