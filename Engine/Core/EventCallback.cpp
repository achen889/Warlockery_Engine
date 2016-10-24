//==============================================================================================================
//EventCallback.cpp
//by Albert Chen Oct-29-2015.
//==============================================================================================================

#include "EventCallback.hpp"
#include "Engine\Console\Console.hpp"

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///constructors

EventCallback::EventCallback(){
	//do nothing
}

EventCallback::EventCallback(const std::string& event_name, EventCallbackFunc* func, void* data):
	eventName(event_name),
	m_function(func),
	args(data){

}

//-----------------------------------------------------------------------------------------------------------

EventCallback::EventCallback(const std::string& event_name, NamedProperties& params) :
	m_eventProperties(&params),
	eventName(event_name){

}

//-----------------------------------------------------------------------------------------------------------


EventCallback::~EventCallback(){
	//do nothing

}

//-----------------------------------------------------------------------------------------------------------


void EventCallback::Execute(){

	//ConsolePrintString("Calling Event: " + eventName);
	
	if (m_function){
		(*m_function)(eventName.c_str(), args);
	}
	else if(m_eventProperties){
		EventSystem::GetInstance().FireEvent(eventName, *m_eventProperties);
	}


}


//===========================================================================================================

