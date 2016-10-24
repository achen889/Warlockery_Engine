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

extern bool g_isGameClockPaused;

//===========================================================================================================

class Clock {
public:
	Clock();
	Clock(const std::string& name, Clock* parent = NULL);
	~Clock();

	//helpers
	void AddChild(Clock* childClock);
	void RemoveChild(Clock* childClock);
	Clock* GetChild(const std::string& name);

	bool IsPaused() { return m_isPaused; }

	void SetName(const std::string& name) {
		m_name = name; 
		m_id = GetStringID(name);
	}

	void AdvanceTime(double deltaSeconds);

	//getters and setters
	double GetDeltaSeconds(){ return m_deltaSeconds; }
	void TogglePause(){ 
		m_isPaused = !m_isPaused;
	}
	void Pause() {
		m_isPaused = true;
	}
	void UnPause() {
		m_isPaused =false;
	}

	void SetTimeScale(const float& newTimeScale){ m_timeScale = newTimeScale; }

	//timer helpers
	void SetElapsedTimer(); //plz only use this once per clock
	Timer* GetElapsedTimer() { return m_elapsedTimerHandle;}

	void AddTimer(Timer* timer) {
		m_timers.push_back(timer);
	}

	void SetCountdownTimer(const std::string& name, const float& countdownFromSeconds, EventCallbackFunc* callback, void* data);
	void SetPeriodicTimer(const std::string& name, const float& countdownFromSeconds, EventCallbackFunc* callback, void* data);
	void KillTimer(const std::string& name);

	Timer* GetTimerByName(const std::string name){
		Timer* timer = FindTimerByName(name, m_timers);

		return timer;
	}

	double GetCurrentSeconds() {
		if (m_elapsedTimerHandle) {
			return m_elapsedTimerHandle->GetSecondsElapsed();
		}
		return 0.0;
	}

	//friend
	friend void InitializeClockSystem();
	static void ShutDown();
	
	static Clock* s_SystemClock; //master clock
	static Clock* s_GameClock;

	///----------------------------------------------------------------------------------------------------------
	///friend methods

	//system clock
	friend Clock& GetSystemClock();

	//game clock
	friend Clock& GetGameClock();

	friend double GetGameClockCurrentSeconds();

	friend bool ToggleGameClockPause();
	
	friend bool PauseGameClock();
	friend bool UnPauseGameClock();

private:
	//vars
	StringID m_id;
	double m_currentTime = 0;
	double m_deltaSeconds = 0;

	float m_timeScale = 1.0f;
	float m_maxDelta = 0.5f;
	bool m_isPaused = false;

	Clocks m_children;
	Clock* m_parent = NULL;

	std::string m_name;

	Timers m_timers;
	Timer* m_elapsedTimerHandle = NULL; //generally you only want one elapsed timer per clock
	
};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline void Clock::AddChild(Clock* childClock){
	m_children.push_back(childClock);
	childClock->m_parent = this;
}

//-----------------------------------------------------------------------------------------------------------

inline void Clock::RemoveChild(Clock* childClock) {

	for (ClocksIterator it = m_children.begin(); it != m_children.end(); ++ it ){
		Clock* clock = (*it);
		if (clock && clock == childClock) {
			clock->m_parent = NULL;
			it = m_children.erase(it);
			break;
		}
	}

// 	for (ClocksIterator it = m_children.begin(); it != m_children.end();) {
// 		//call delete on the children
// 		Clock* clock = (*it);
// 		if (clock != NULL) {
// 			if (clock == childClock) {
// 				it = m_children.erase(it);
// 			}
// 		}
// 		++it;
// 	}//end of for
}

//-----------------------------------------------------------------------------------------------------------

inline Clock& GetSystemClock(){ 
	return (*Clock::s_SystemClock); 
}

inline Clock& GetGameClock(){
	return (*GetSystemClock().GetChild("GameClock"));
}

inline double GetGameClockCurrentSeconds() {
	return GetGameClock().GetCurrentSeconds();
}

inline bool ToggleGameClockPause() {
	//pause game clock
	GetGameClock().TogglePause();
	//toggle bool
	g_isGameClockPaused = !g_isGameClockPaused;

	return g_isGameClockPaused;
}

inline bool PauseGameClock() {
	GetGameClock().Pause();
	g_isGameClockPaused = true;
	return g_isGameClockPaused;
}

inline bool UnPauseGameClock() {
	GetGameClock().UnPause();
	g_isGameClockPaused = false;
	return g_isGameClockPaused;
}



//===========================================================================================================


#endif //__includedClock__

