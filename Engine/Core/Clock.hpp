//==============================================================================================================
//Clock.hpp
//by Albert Chen Oct-20-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Clock__
#define _included_Clock__

#include "Engine/Core/Timer.hpp"
//#include "Engine/Components/Component.hpp"

//===========================================================================================================

class Clock;

typedef std::vector<Clock*> Clocks;
typedef std::vector<Clock*>::iterator ClocksIterator;

//===========================================================================================================

class Clock{
public:
	 Clock();
	 Clock(const std::string& name, Clock* parent = NULL);
	~Clock();
	
	//helpers
	void AddChild(Clock* childClock);
	Clock* GetChild(const std::string& name);

	void AdvanceTime(double deltaSeconds);
	
	//getters and setters
	double GetDeltaSeconds(){ return m_deltaSeconds; }
	void TogglePause(){ m_isPaused = !m_isPaused; }

	void SetTimeScale(const float& newTimeScale){ m_timeScale = newTimeScale; }

	//timer helpers
	void SetCountdownTimer(const std::string& name, const float& countdownFromSeconds, EventCallbackFunc* callback, void* data);
	void SetPeriodicTimer(const std::string& name, const float& countdownFromSeconds, EventCallbackFunc* callback, void* data);
	void KillTimer(const std::string& name);

	Timer* GetTimerByName(const std::string name){
		Timer* timer = FindTimerByName(name, m_timers);

		return timer;
	}

	//friend
	friend void InitializeClockSystem();
	static void ShutDown();
	
	static Clock* s_SystemClock; //master clock
	static Clock* s_GameClock;

	friend Clock& GetSystemClock();
	friend Clock& GetGameClock();
	
private:
	//vars
	StringID m_id;
	double m_currentTime = 0;
	double m_deltaSeconds;

	float m_timeScale = 1.0f;
	float m_maxDelta = 0.5f;
	bool m_isPaused = false;

	Clocks m_children;
	Clock* m_parent = NULL;

	Timers m_timers;
	
};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline void Clock::AddChild(Clock* childClock){
	m_children.push_back(childClock);
}

//-----------------------------------------------------------------------------------------------------------

inline Clock& GetSystemClock(){ 
	return (*Clock::s_SystemClock); 
}

inline Clock& GetGameClock(){
	return (*GetSystemClock().GetChild("GameClock"));
}


//===========================================================================================================


#endif //__includedClock__

