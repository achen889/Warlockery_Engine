//==============================================================================================================
//EventSystem.hpp
//by Albert Chen Mar-21-2016.
//==============================================================================================================

#pragma once

#ifndef _included_EventSystem__
#define _included_EventSystem__

#include "EventUtilities.hpp"

//===========================================================================================================

static bool s_doEventSystemDebug = false;

class EventSystem {
public:

	static void CreateInstance() { s_theEventSystem = new EventSystem(); }
	static EventSystem& GetInstance();
	static void DestroyInstance() { 
		if (s_theEventSystem) {
			delete s_theEventSystem;
			s_theEventSystem = NULL;
		}
	}

	void ShutDown() {
		ClearEventSubscriberListRegistry();
	}

	int FireEvent(const std::string& event_name, NamedProperties& args = NamedProperties::EMPTY );

	void ExecuteEventSubscriptionRegistry(const std::string& event_name, NamedProperties& args, 
					int& numSubscribers, std::string& fireEventErrorStr);

	void ExecuteEventMethodSubscriptionRegistry(const std::string& event_name, NamedProperties& args, 
					int& numSubscribers, std::string& fireEventErrorStr);

	void RegisterEventCallback(const std::string& event_name, EventCallbackFunction* callbackMethod);

	void UnregisterEventCallback(const std::string& event_name, EventCallbackFunction* callbackFunc);

	template<typename T_METHOD_PTR_TYPE, typename T_OBJECT_TYPE>
	void RegisterEventMethodCallback(const std::string& event_name, T_METHOD_PTR_TYPE callbackMethod, T_OBJECT_TYPE& objectInstance);

	template<typename T_METHOD_PTR_TYPE, typename T_OBJECT_TYPE>
	void UnregisterEventMethodCallback(const std::string& event_name, T_METHOD_PTR_TYPE callbackMethod, T_OBJECT_TYPE& objectInstance);
	
	template<typename T_OBJECT_TYPE>
	void UnregisterEventObjectInstance(const std::string& event_name, T_OBJECT_TYPE& objectInstance);

	template<typename T_OBJECT_TYPE> 
	void UnregisterObjectInstanceFromAllEvents( T_OBJECT_TYPE& objectInstance);
	//int EventSystem::ExecuteMethodSubscriberList(EventMethodSubscriberListBase* baseMethodSubscriberList, NamedProperties& args);

	void ClearEventSubscriberListRegistry() {
		m_eventSubscriptionRegistry.clear();
	}

	//friend
	friend bool IsEventSystemCreated();

private:
	static EventSystem* s_theEventSystem;

	EventSubscriberRegistry m_eventSubscriptionRegistry;

	//EventMethodSubscriberRegistry m_eventMethodSubscriptionRegistry;

};

///----------------------------------------------------------------------------------------------------------
///inline methods

inline EventSystem& EventSystem::GetInstance() {
	if (!s_theEventSystem) {
		ERROR_AND_DIE("The Event System is NULL!!!");
	}
	return *s_theEventSystem;
}

//-----------------------------------------------------------------------------------------------------------

template<typename T_METHOD_PTR_TYPE, typename T_OBJECT_TYPE>
inline void EventSystem::RegisterEventMethodCallback(const std::string& event_name, T_METHOD_PTR_TYPE callbackMethod, T_OBJECT_TYPE& objectInstance) {

	EventSubscriberList& subscriberList = m_eventSubscriptionRegistry[event_name];
	subscriberList.m_eventName = event_name;

	subscriberList.AddEventCallbackMethod(callbackMethod, objectInstance);

}

//-----------------------------------------------------------------------------------------------------------

template<typename T_METHOD_PTR_TYPE, typename T_OBJECT_TYPE>
inline void EventSystem::UnregisterEventMethodCallback(const std::string& event_name, T_METHOD_PTR_TYPE callbackMethod, T_OBJECT_TYPE& objectInstance) {
	
	EventSubscriberList& subscriberList = m_eventSubscriptionRegistry[event_name];

	subscriberList.FindAndRemoveFromSubscriberList(callbackMethod, objectInstance);

}

//-----------------------------------------------------------------------------------------------------------

template<typename T_OBJECT_TYPE>
inline void EventSystem::UnregisterEventObjectInstance(const std::string& event_name, T_OBJECT_TYPE& objectInstance) {

	EventSubscriberList& subscriberList = m_eventSubscriptionRegistry[event_name];

	subscriberList.FindAndRemoveObjectInstanceFromSubscriberList(&objectInstance);
}

template<typename T_OBJECT_TYPE>
inline void EventSystem::UnregisterObjectInstanceFromAllEvents(T_OBJECT_TYPE& objectInstance) {

	for (EventSubscriberRegistryIterator it = m_eventSubscriptionRegistry.begin(); it != m_eventSubscriptionRegistry.end(); ++ it ){
		EventSubscriberList& subscriberList = (it->second);

		subscriberList.FindAndRemoveObjectInstanceFromSubscriberList(objectInstance);
		
	}
	
}

inline bool IsEventSystemCreated() {
	return (EventSystem::s_theEventSystem != NULL);
}

//===========================================================================================================


//===========================================================================================================

//generic base object to clean up events
struct EventSubscriberObject {

	virtual ~EventSubscriberObject() {
		//ConsolePrintString("\ndeleting this from all events!");
		EventSystem::GetInstance().UnregisterObjectInstanceFromAllEvents(*this);
	}

};


//===========================================================================================================

#endif //__includedEventSystem__

