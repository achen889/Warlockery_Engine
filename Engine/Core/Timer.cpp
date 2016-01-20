//==============================================================================================================
//Timer.cpp
//by Albert Chen May-6-2015.
//==============================================================================================================

#include "Timer.hpp"

//===========================================================================================================

static unsigned int newID = 0;

//===========================================================================================================

Timer::Timer(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

Timer::Timer(const float& countdownFromTime, bool isTimerPeriodic):
countdownTime(countdownFromTime),
elapsedTime(countdownTime),
timerComplete(false),
isActive(true),
isPeriodic(isTimerPeriodic),
timerEventCallback(EventCallback("", NULL, NULL))
{
	if(countdownTime == 0.0f)
		isElapsedTimer = true;
	else
		isElapsedTimer = false;
	isPeriodic = isTimerPeriodic;
}

//-----------------------------------------------------------------------------------------------------------

Timer::Timer(const float& countdownFromTime, const std::string& eventName, 
	         EventCallbackFunc* func, void* data, bool isTimerPeriodic):
timerEventCallback(EventCallback(eventName, func, data)),
countdownTime(countdownFromTime),
elapsedTime(countdownTime),
timerComplete(false),
isActive(true),
isPeriodic(isTimerPeriodic),
id(newID)
{
	if (countdownTime == 0.0f)
		isElapsedTimer = true;
	else
		isElapsedTimer = false;
	isPeriodic = isTimerPeriodic;
	
	newID++;
}


//===========================================================================================================
void Timer::Update(double deltaSeconds){

	//alarm updating
	if(isActive){
		if(!isElapsedTimer){
	
			if(elapsedTime> 0.0f){
				elapsedTime -= (float)deltaSeconds;
			}else if( !timerComplete && elapsedTime < 0.0f){

				TriggerAlarm();

			
			}//end of countdown timer block
		}
		else if(isElapsedTimer){
			UpdateElapsedTimer(deltaSeconds);
		}

	}//end of alarm updating
}

//-----------------------------------------------------------------------------------------------------------

void Timer::TriggerAlarm(){
	timerComplete = true;
	
	timerEventCallback.CallEventFunction();

	//reset if periodic timer
	ResetIfPeriodic();
}



//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///friend methods

void UpdateTimers( double deltaSeconds, Timers& alarmsToUpdate){
	for(TimersIterator alarmIter = alarmsToUpdate.begin(); alarmIter != alarmsToUpdate.end(); ++alarmIter ){
		Timer* alarm = (*alarmIter);

		alarm->Update(deltaSeconds);
	}
}

//-----------------------------------------------------------------------------------------------------------

Timer* FindTimerByName(const std::string& name, Timers& timersToCheck){
	StringID nameID = GetStringID(name);
	for (TimersIterator timerIter = timersToCheck.begin(); timerIter != timersToCheck.end(); ++timerIter){
		Timer* timer = (*timerIter);

		if (timer->id == nameID){
			return timer;
		}
	}

	return NULL;
}
