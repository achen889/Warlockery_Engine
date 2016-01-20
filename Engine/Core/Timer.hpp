//==============================================================================================================
//Timer.hpp
//by Albert Chen May-6-2015.
//==============================================================================================================

#pragma once
#ifndef _included_Timer__
#define _included_Timer__

#include <vector>
#include "Time.hpp"
#include "StringTable.hpp"
#include "EventCallback.hpp"

//===========================================================================================================

struct Timer;

typedef std::vector<Timer*> Timers;
typedef std::vector<Timer*>::iterator TimersIterator;

typedef std::map<std::string,  Timer*> TimerMap;
typedef std::map<std::string,  Timer*>::iterator TimerMapIterator;
typedef std::pair<std::string, Timer*> TimerMapEntry;

//===========================================================================================================

struct Timer{
	//alarm data
	StringID id;
	double countdownTime;
	double elapsedTime;

	//alarm flags
	//consider changing to bit flags later
	bool  timerComplete;
	bool  isPeriodic;
	bool  isActive;
	bool  isElapsedTimer;
	//event stuff
	EventCallback timerEventCallback;

	//methods
	Timer();
	Timer(const float& countdownFromTime, bool isTimerPeriodic = false);
	Timer(const float& countdownFromTime, const std::string& eventName, 
		  EventCallbackFunc* func, void* data, bool isTimerPeriodic = false);
	
	//upkeep
	void Update(double deltaSeconds);
	
	void TriggerAlarm();

	void UpdateElapsedTimer(double deltaSeconds);
	void ResetIfPeriodic();

	void Kill(){ isActive = false; }

	//queries
	double GetSecondsElapsed(){ return elapsedTime; }
	double GetSecondsRemaining(){ return countdownTime - elapsedTime; }
	double GetPercentElapsed(){ return (elapsedTime / countdownTime) * 100.0; } //premultiplied by 100
	double GetPercentRemaining(){ return ( 1.0 - (elapsedTime / countdownTime) ) * 100.0; } //premultiplied by 100

	friend void UpdateTimers(double deltaSeconds, Timers& alarmsToUpdate);
	friend Timer* FindTimerByName(const std::string& name, Timers& timersToCheck);

};
///----------------------------------------------------------------------------------------------------------
///inline methods
//-----------------------------------------------------------------------------------------------------------
inline void Timer::UpdateElapsedTimer(double deltaSeconds){
	elapsedTime += (float)deltaSeconds;
}

//-----------------------------------------------------------------------------------------------------------
inline void Timer::ResetIfPeriodic(){
	if(isPeriodic){
		elapsedTime = countdownTime;
		if (timerComplete)
			timerComplete = false;
	}
}

//-----------------------------------------------------------------------------------------------------------



#endif