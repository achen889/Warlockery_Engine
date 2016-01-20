//==============================================================================================================
//EventCallback.hpp
//by Albert Chen Oct-29-2015.
//==============================================================================================================

#pragma once
#include <string>

#ifndef _included_EventCallback__
#define _included_EventCallback__

//===========================================================================================================

//callback event
typedef void(EventCallbackFunc)(const char* event_name, void* args);

//===========================================================================================================

struct EventCallback{
	//timer callback func
	EventCallback();
	EventCallback(const std::string& event_name, EventCallbackFunc* func, void* data);
	~EventCallback();

	//methods
	void CallEventFunction();

	//vars
	std::string eventName;
	EventCallbackFunc* m_function;
	void* args;

	//static std::map<std::string name, EventCallback*> s_eventCallbackMap;

};

//===========================================================================================================

#endif //__includedEventCallback__

