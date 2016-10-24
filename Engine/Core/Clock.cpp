//==============================================================================================================
//Clock.cpp
//by Albert Chen Oct-20-2015.
//==============================================================================================================

#include "Clock.hpp"

//===========================================================================================================

Clock* Clock::s_SystemClock = NULL;
Clock* Clock::s_GameClock = NULL;

bool g_isGameClockPaused = false;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors

Clock::Clock(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

Clock::Clock(const std::string& name, Clock* parent) :
m_name(name),
m_id(GetStringID(name)),
m_parent(parent)
{
// 	if (m_parent) {
// 		AddChild(m_parent);
// 	}
	m_timers.clear();
}

//-----------------------------------------------------------------------------------------------------------

Clock::~Clock(){
	//do nothing

	//Clean up clock hierarchy ptrs

	//redirect child pts
	if (m_parent) {
		m_parent->RemoveChild(this);
		for (Clock* child : m_children) {
			m_parent->AddChild(child);
		}
	}
}

//-----------------------------------------------------------------------------------------------------------

Clock* Clock::GetChild(const std::string& name){
	Clock* outClock = NULL;
	if (m_children.size() > 0){
		for (ClocksIterator it = m_children.begin(); it != m_children.end(); ++it){
			Clock* clock = (*it);

			if (clock->m_id == GetStringID(name)){
				outClock = clock;
				break;
			}

		}//end of for


	}

	return outClock;
}

//-----------------------------------------------------------------------------------------------------------

void Clock::AdvanceTime(double deltaSeconds){
	
	//timescale it first
	deltaSeconds *= m_timeScale;
	//set to 0
	if (m_isPaused)
		deltaSeconds = 0.0;

	m_deltaSeconds = deltaSeconds;

	//advance time
	m_currentTime += m_deltaSeconds;
	

	UpdateTimers(m_deltaSeconds, m_timers);

	for (ClocksIterator it = m_children.begin(); it != m_children.end(); ++it){
		Clock* clock = (*it);
		if (clock) {
			//advance time on children
			clock->AdvanceTime(m_deltaSeconds);
		}
	}

}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///timer map helpers

void Clock::SetElapsedTimer() {
	//adds a timer to the map
	Timer* newElapsedTimer = new Timer(0.0f);
	m_timers.push_back(newElapsedTimer);

	m_elapsedTimerHandle = newElapsedTimer;
}

void Clock::SetCountdownTimer(const std::string& name, const float& countdownFromSeconds, EventCallbackFunc* callback, void* data){
	//adds a timer to the map
	m_timers.push_back( new Timer(countdownFromSeconds, name, callback, data, false) );
}

void Clock::SetPeriodicTimer(const std::string& name, const float& countdownFromSeconds, EventCallbackFunc* callback, void* data){
	//adds a timer to the map
	m_timers.push_back( new Timer(countdownFromSeconds, name, callback, data, true) );
}

void Clock::KillTimer(const std::string& name){
	//kills a timer
	Timer* timerToKill = FindTimerByName(name, m_timers);
	if (timerToKill){
		timerToKill->Kill();
	}
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///friend methods

void InitializeClockSystem(){
	Clock::s_SystemClock = new Clock("MasterSystemClock", NULL);

	Clock::s_GameClock = new Clock("GameClock", Clock::s_SystemClock);
	Clock::s_GameClock->SetElapsedTimer();
	
	Clock::s_SystemClock->AddChild(Clock::s_GameClock);


	//kill the timer for now, revive it when the game starts
	Timer* gameElapsedTimer = GetGameClock().GetElapsedTimer();
	if (gameElapsedTimer) {
		gameElapsedTimer->Kill();
	}
	//InitializeEntitiesClock();
}

//delete all clocks and stuff
void Clock::ShutDown(){

	//ShutDownEntitiesClock();
// 	if (Clock::s_GameClock){
// 		delete Clock::s_GameClock;
// 		Clock::s_GameClock = NULL;
// 	}
	if (Clock::s_SystemClock){
		delete Clock::s_SystemClock;
		Clock::s_SystemClock = NULL;
	}
}

//===========================================================================================================


//===========================================================================================================