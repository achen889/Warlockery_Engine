//==============================================================================================================
//EventUtilities.hpp
//by Albert Chen Mar-31-2016.
//==============================================================================================================

#pragma once

#ifndef _included_EventUtilities__
#define _included_EventUtilities__

#include "NamedProperties.hpp" //data container for events

#include <typeinfo> //must include for type info

#include "Engine/Console/Console.hpp"

//===========================================================================================================

typedef void(EventCallbackFunction)(NamedProperties& args);

typedef std::vector<EventCallbackFunction*> EventCallbackFunctions;
typedef std::vector<EventCallbackFunction*>::iterator EventCallbackFunctionsIterator;

//===========================================================================================================

struct EventCallbackMethodBase {

	//methods
	virtual void Execute(NamedProperties& args) = 0;
	virtual bool IsOfMethodType(const type_info& t) = 0;
	virtual bool IsOfObjectType(const type_info& t) = 0;

};

typedef std::vector<EventCallbackMethodBase*> EventCallbackMethods;
typedef std::vector<EventCallbackMethodBase*>::iterator EventCallbackMethodsIterator;

template<typename T_OBJECT_TYPE>
struct EventCallbackMethod : public EventCallbackMethodBase {

	//vars
	typedef void(T_OBJECT_TYPE::*MethodEventCallbackPtr)(NamedProperties& args);

	std::string m_eventName;
	T_OBJECT_TYPE& m_objectInstance; //by reference
	MethodEventCallbackPtr m_objectMethod;

	//constructor
	EventCallbackMethod(const std::string& eventName, MethodEventCallbackPtr method, T_OBJECT_TYPE& objectInstance);

	//methods

	void Execute(NamedProperties& args);

	bool IsOfMethodType(const type_info& t) {
		return (typeid(m_objectMethod) == t);
	}
	bool IsOfObjectType(const type_info& t) {
		return (typeid(T_OBJECT_TYPE) == t);
	}

};

typedef std::vector<EventCallbackMethodBase*> EventObjectMethods;
typedef std::vector<EventCallbackMethodBase*>::iterator EventObjectMethodsIterator;

///----------------------------------------------------------------------------------------------------------
///inline methods

template<typename T_OBJECT_TYPE>
inline EventCallbackMethod<T_OBJECT_TYPE>::EventCallbackMethod(const std::string& eventName, 
	MethodEventCallbackPtr method, T_OBJECT_TYPE& objectInstance) :
	m_eventName(eventName),
	m_objectInstance(objectInstance),
	m_objectMethod(method){



}

//-----------------------------------------------------------------------------------------------------------

template<typename T_OBJECT_TYPE>
inline void EventCallbackMethod<T_OBJECT_TYPE>::Execute(NamedProperties& args) {
	//call that function on that object with those params
	if (m_objectMethod) {
		//must call it this way
		(m_objectInstance.*m_objectMethod)(args); //m_objectMethod is not a member of X
	}
}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================

struct EventSubscriberList {

	EventCallbackFunctions m_subscriberCallbackFunctions;
	EventCallbackMethods m_subscriberCallbackMethods;

	std::string m_eventName;

	~EventSubscriberList() {
		ClearAllEventSubscriberMethods();
		ClearAllEventSubscriberFunctions();
	}

	int GetNumSubscribers() { return m_subscriberCallbackFunctions.size() + m_subscriberCallbackMethods.size(); }

	void AddEventCallbackFunction(EventCallbackFunction* func);

	void FindAndRemoveFromSubscriberList(EventCallbackFunction* funcToCheck);

	template<typename T_METHOD_PTR_TYPE, typename T_OBJECT_TYPE >
	void AddEventCallbackMethod(T_METHOD_PTR_TYPE method, T_OBJECT_TYPE& objectInstance);

	template<typename T_METHOD_PTR_TYPE, typename T_OBJECT_TYPE >
	void FindAndRemoveFromSubscriberList(T_METHOD_PTR_TYPE method, T_OBJECT_TYPE& objectInstance);

	template<typename T_OBJECT_TYPE>
	void FindAndRemoveObjectInstanceFromSubscriberList(T_OBJECT_TYPE& objectInstance);
	
	void ExecuteAllFunctionSubscriberCallbacks(NamedProperties& args);

	void ExecuteAllMethodSubscriberCallbacks(NamedProperties& args);

	void ClearAllEventSubscriberMethods() {
		for (EventCallbackMethodsIterator it = m_subscriberCallbackMethods.begin(); 
		it != m_subscriberCallbackMethods.end(); ){
			EventCallbackMethodBase* method = (*it);
			if (method) {
				delete method;
				method = NULL;
			}
			it = m_subscriberCallbackMethods.erase(it);
		}
	}
	void ClearAllEventSubscriberFunctions() {
		m_subscriberCallbackFunctions.clear();
	}
	
};

typedef std::map<std::string, EventSubscriberList > EventSubscriberRegistry;
typedef std::map<std::string, EventSubscriberList >::iterator EventSubscriberRegistryIterator;
typedef std::pair<std::string, EventSubscriberList > EventSubscriberRegistryEntry;

///----------------------------------------------------------------------------------------------------------
///inline methods

inline void EventSubscriberList::AddEventCallbackFunction(EventCallbackFunction* func) {
	m_subscriberCallbackFunctions.push_back(func);
}

inline void EventSubscriberList::FindAndRemoveFromSubscriberList(EventCallbackFunction* funcToCheck) {

	for (EventCallbackFunctionsIterator it = m_subscriberCallbackFunctions.begin(); it != m_subscriberCallbackFunctions.end(); ++it) {
		EventCallbackFunction* func = (*it);
		if (func && func == funcToCheck) {
			it = m_subscriberCallbackFunctions.erase(it);
			break;
		}

	}
}

//-----------------------------------------------------------------------------------------------------------

template<typename T_METHOD_PTR_TYPE, typename T_OBJECT_TYPE >
inline void EventSubscriberList::AddEventCallbackMethod(T_METHOD_PTR_TYPE method, T_OBJECT_TYPE& objectInstance) {
	EventCallbackMethodBase* newMethodCallback = new EventCallbackMethod<T_OBJECT_TYPE>(m_eventName, method, objectInstance);

	m_subscriberCallbackMethods.push_back(newMethodCallback);
}

//-----------------------------------------------------------------------------------------------------------

template<typename T_METHOD_PTR_TYPE, typename T_OBJECT_TYPE >
inline void EventSubscriberList::FindAndRemoveFromSubscriberList(T_METHOD_PTR_TYPE method, T_OBJECT_TYPE& objectInstance) {
	for (EventObjectMethodsIterator it = m_subscriberCallbackMethods.begin(); it != m_subscriberCallbackMethods.end(); ++it) {
		EventCallbackMethodBase* objectMethodBase = (*it);
		//check if method and object type match

		bool methodsMatch = objectMethodBase->IsOfMethodType(typeid(T_METHOD_PTR_TYPE));
		if (methodsMatch) {

			EventCallbackMethod<T_OBJECT_TYPE>* TypedObjectMethod = (EventCallbackMethod<T_OBJECT_TYPE>*)objectMethodBase;
			
			//check method ptrs and object ptrs
			if (TypedObjectMethod->m_objectMethod == method && 
				(&TypedObjectMethod->m_objectInstance) == (&objectInstance)) {
				//just need to delete it
				it = m_subscriberCallbackMethods.erase(it);

				break;
			}
		}//if methods match

	}//end of for
}

//-----------------------------------------------------------------------------------------------------------

template<typename T_OBJECT_TYPE>
inline void EventSubscriberList::FindAndRemoveObjectInstanceFromSubscriberList(T_OBJECT_TYPE& objectInstance) {

	for (EventObjectMethodsIterator it = m_subscriberCallbackMethods.begin(); it != m_subscriberCallbackMethods.end(); ) {
		EventCallbackMethodBase* objectMethodBase = (*it);
		//check if method and object type match
		EventCallbackMethod<T_OBJECT_TYPE>* TypedObjectMethod = (EventCallbackMethod<T_OBJECT_TYPE>*)objectMethodBase;

		//check object ptrs
		if ( (&TypedObjectMethod->m_objectInstance) == (&objectInstance) ) {
			//just need to delete it
			it = m_subscriberCallbackMethods.erase(it);
		}
		else {
			++it;
		}

	}//end of for

}

//-----------------------------------------------------------------------------------------------------------

inline void EventSubscriberList::ExecuteAllFunctionSubscriberCallbacks(NamedProperties& args) {
	//ranged for, fancy way of doing a quick for loop
	for (EventCallbackFunction* func : m_subscriberCallbackFunctions) {
		if (func)
			(*func)(args);	//executes the function with the args
	}//shortest for loop ever
}

//-----------------------------------------------------------------------------------------------------------

inline void EventSubscriberList::ExecuteAllMethodSubscriberCallbacks(NamedProperties& args) {

	for (EventCallbackMethodBase* baseMethodCallback : m_subscriberCallbackMethods) {

		baseMethodCallback->Execute(args);

	}

}

//===========================================================================================================


//===========================================================================================================

#endif //__includedEventUtilities__

