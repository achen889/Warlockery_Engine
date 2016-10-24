//==============================================================================================================
//EventSystem.cpp
//by Albert Chen Mar-23-2016.
//==============================================================================================================

#include "EventSystem.hpp"

//===========================================================================================================

EventSystem* EventSystem::s_theEventSystem = NULL;

//===========================================================================================================

int EventSystem::FireEvent(const std::string& event_name, NamedProperties& args) {

	if (s_doEventSystemDebug) {
		ConsolePrintString("\nFiring Event: " + event_name + "!");
	}

	int numSubscribers = 0;
	std::string fireEventErrorStr = "";
	
	//ExecuteAllGlobalCallbacks
	ExecuteEventSubscriptionRegistry(event_name, args, numSubscribers, fireEventErrorStr);

	//ExecuteAllMethodCallbacks
	ExecuteEventMethodSubscriptionRegistry(event_name, args, numSubscribers, fireEventErrorStr);

	return numSubscribers;
}

//-----------------------------------------------------------------------------------------------------------

void EventSystem::ExecuteEventSubscriptionRegistry(const std::string& event_name, NamedProperties& args, 
	int& numSubscribers, std::string& fireEventErrorStr) {

	EventSubscriberRegistryIterator it = m_eventSubscriptionRegistry.find(event_name);

	if (it != m_eventSubscriptionRegistry.end()) {
		EventSubscriberList& subscriberList = (it->second);

		//get num of subscribers
		numSubscribers = subscriberList.GetNumSubscribers();

		if (numSubscribers == 0) {
			//event is not registered
			if (s_doEventSystemDebug) {
				fireEventErrorStr += "\nEvent: " + event_name + " has no subscribers!!";
				ConsolePrintString(fireEventErrorStr);
			}
			//ERROR_RECOVERABLE(fireEventErrorStr);
		}

		subscriberList.ExecuteAllFunctionSubscriberCallbacks(args);

	}//end of if
	else {
		//event is not registered
		if (s_doEventSystemDebug) {
			fireEventErrorStr = "";
			fireEventErrorStr += "\nEvent: " + event_name + " is not registered!!";
			ConsolePrintString(fireEventErrorStr);
		}
		//ERROR_RECOVERABLE(fireEventErrorStr);
	}
}

//-----------------------------------------------------------------------------------------------------------

void EventSystem::ExecuteEventMethodSubscriptionRegistry(const std::string& event_name, NamedProperties& args, 
	int& numSubscribers, std::string& fireEventErrorStr){

	UNUSED(numSubscribers);

	UNUSED(fireEventErrorStr);
	EventSubscriberRegistryIterator it = m_eventSubscriptionRegistry.find(event_name);

	if (it != m_eventSubscriptionRegistry.end()) {
		EventSubscriberList& subscriberList = (it->second);
		
		subscriberList.ExecuteAllMethodSubscriberCallbacks(args);

	}

}

//-----------------------------------------------------------------------------------------------------------

void EventSystem::RegisterEventCallback(const std::string& event_name, EventCallbackFunction* callbackFunction) {

	EventSubscriberList& subscriberList = m_eventSubscriptionRegistry[event_name];
	
	subscriberList.m_eventName = event_name;

	subscriberList.AddEventCallbackFunction(callbackFunction);

}

//-----------------------------------------------------------------------------------------------------------

void EventSystem::UnregisterEventCallback(const std::string& event_name, EventCallbackFunction* callbackFunction) {

	EventSubscriberList& subscriberList = m_eventSubscriptionRegistry[event_name];

	subscriberList.FindAndRemoveFromSubscriberList(callbackFunction);
}


//===========================================================================================================

