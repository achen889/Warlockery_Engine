//==============================================================================================================
//Clock.cpp
//by Albert Chen Oct-20-2015.
//==============================================================================================================

#include "Clock.hpp"

//===========================================================================================================

Clock* Clock::s_SystemClock = NULL;
Clock* Clock::s_GameClock = NULL;

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///constructors

Clock::Clock(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

Clock::Clock(const std::string& name, Clock* parent) :
m_id(GetStringID(name)),
m_parent(parent)
{
	
}

//-----------------------------------------------------------------------------------------------------------

Clock::~Clock(){
	//do nothing

	if (!m_children.empty()){
		//remove clocks one by one
		for (ClocksIterator it = m_children.begin(); it != m_children.end();){
			 //call delete on the children
			 Clock* clock = (*it);
			 delete clock;
			 clock = NULL;

			 it = m_children.erase(it);
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
		deltaSeconds = 0.0f;
	//advance time
	m_currentTime += deltaSeconds;
	m_deltaSeconds = deltaSeconds;

	UpdateTimers(m_deltaSeconds, m_timers);
	
	//iterate through children, will become recursive
	for (ClocksIterator it = m_children.begin(); it != m_children.end(); ++it){
		Clock* clock = (*it);
		//advance time on children
		clock->AdvanceTime(m_deltaSeconds);
	}
}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///timer map helpers

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

	Clock::s_SystemClock->AddChild(Clock::s_GameClock);
	
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