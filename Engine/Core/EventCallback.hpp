//==============================================================================================================
//EventCallback.hpp
//by Albert Chen Oct-29-2015.
//==============================================================================================================

#pragma once


#ifndef _included_EventCallback__
#define _included_EventCallback__

#include <string>

#include "Event/EventSystem.hpp"

//===========================================================================================================

//callback event
typedef void(EventCallbackFunc)(const char* event_name, void* args);

//===========================================================================================================

//this struct is used to wrap an event call
struct EventCallback{
	//timer callback func
	EventCallback();
	EventCallback(const std::string& event_name, EventCallbackFunc* func, void* data);
	EventCallback(const std::string& event_name, NamedProperties& params);
	~EventCallback();
	
	//methods
	void Execute();

	//vars
	std::string eventName;
	EventCallbackFunc* m_function = NULL;
	void* args = NULL;
	NamedProperties* m_eventProperties = NULL;

};

//-----------------------------------------------------------------------------------------------------------



//===========================================================================================================

#endif //__includedEventCallback__

