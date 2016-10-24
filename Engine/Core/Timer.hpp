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

#include "Utilities.hpp"

//===========================================================================================================

struct Timer;

typedef std::vector<Timer*> Timers;
typedef std::vector<Timer*>::iterator TimersIterator;

typedef std::map<std::string,  Timer*> TimerMap;
typedef std::map<std::string,  Timer*>::iterator TimerMapIterator;
typedef std::pair<std::string, Timer*> TimerMapEntry;

static unsigned int s_newTimerID = 0;

//===========================================================================================================

struct Timer{
	//alarm data
	StringID id = 0;
	double countdownTime = 0;
	double elapsedTime = 0;

	//alarm flags
	//consider changing to bit flags later
	bool  timerComplete = false;
	bool  isPeriodic = false;
	bool  isActive = false;
	bool  isElapsedTimer = false;

	//old event stuff
	EventCallback timerEventCallback;

	//new event stuff

	//methods
	Timer();
	Timer(const float& countdownFromTime, bool isTimerPeriodic = false);
	
	//using old event callback stuff
	Timer(const float& countdownFromTime, const std::string& eventName, 
		  EventCallbackFunc* func, void* data, bool isTimerPeriodic = false);

	//how to use new one?
	Timer(const float& countdownFromTime, const std::string& eventName, NamedProperties& args , bool isTimerPeriodic = false);
	
	//upkeep
	void Update(double deltaSeconds);
	
	void TriggerAlarm();

	void UpdateElapsedTimer(double deltaSeconds);
	void ResetIfPeriodic();
    void Reset();

	void Revive() { isActive = true; }
	void Kill(){ isActive = false; }

	void SetCountdownTime(double new_countdownTime) {
		countdownTime = new_countdownTime;
	}

	//queries
	double GetSecondsElapsed(){ return elapsedTime; }
	double GetSecondsRemaining(){ return countdownTime - elapsedTime; }
	double GetPercentRemaining(){ 
		return (elapsedTime / countdownTime); 
	}
	double GetPercentElapsed(){ 
		return (1.0 - (elapsedTime / countdownTime));
	}

	bool IsComplete() { return timerComplete; }

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
		Reset();
	}
}

//-----------------------------------------------------------------------------------------------------------

inline void Timer::Reset() {
	elapsedTime = countdownTime;
	if (timerComplete)
		timerComplete = false;
}

//-----------------------------------------------------------------------------------------------------------


//===========================================================================================================

#endif