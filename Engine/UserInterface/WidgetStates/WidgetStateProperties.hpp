//==============================================================================================================
//WidgetStateProperties.hpp
//by Albert Chen Apr-25-2016.
//==============================================================================================================

#pragma once
#include "Engine/Core/Event/NamedProperties.hpp"
#include "Engine/Core/XML/XMLUtils.hpp"
#include "../KeyFrameAnimation/AnimatedProperty.hpp"

#ifndef _included_WidgetStateProperties__
#define _included_WidgetStateProperties__

//===========================================================================================================

enum WidgetStateID{
	WIDGET_STATE_DISABLED,
	WIDGET_STATE_IDLE,
	WIDGET_STATE_HIGHLIGHTED,
	WIDGET_STATE_ACTIVATED,
	NUM_WIDGET_STATES
};

//===========================================================================================================

struct WidgetStateProperties {
public:
	//methods

	WidgetStateProperties() {
		//do nothing
	}
	WidgetStateProperties(const std::string& widgetType, const XMLNode& node, const std::string& colorScheme);

	~WidgetStateProperties();

	const NamedProperties& GetStateProperties() {
		return m_stateProperties;
	}
	
	template<typename T> 
	PropertyGetResult GetProperty(const std::string& propertyName, T& outPropertyValue);

	template<typename T>
	PropertyGetResult GetStateProperty(const std::string& propertyName, T& outPropertyValue);
	
	template<typename T> 
	PropertyGetResult GetStyleProperty(const std::string& propertyName, T& outPropertyValue);

	template<typename T>
	PropertyGetResult GetDefaultProperty(const std::string& propertyName, T& outPropertyValue);

	template<typename T>
	PropertyGetResult GetCurrentProperty(NamedProperties* properties, const std::string& propertyName, T& outPropertyValue);

	template<typename T> 
	void CreateProperty(const std::string& name, const T& val);

	void SetMyDefaultStateProperties(const NamedProperties& npPtr) {
		m_stateProperties = npPtr;
	}

	void SetMyDefaultProperties(NamedProperties* npPtr) {
// 		if (!m_defaultProperties) {
// 			m_defaultProperties = new NamedProperties();
// 		}
		m_defaultProperties = npPtr;
	}

	void SetMyStyleDefaultProperties(NamedProperties* npPtr) {
// 		if (!m_styleDefaultProperties) {
// 			m_styleDefaultProperties = new NamedProperties();
// 		}
		m_styleDefaultProperties = npPtr;
	}
	
	//upkeep
	void Update(double deltaSeconds);

	//additional helpers
	template<typename T> 
	void AddAnimatedProperty(const T& val);
	//actually used helpers
	template<typename T>
	void CreateAnimatedProperty(const char* name, const XMLNode& node);

	template<typename T>
	void LoadAnimFromXML(const char* name, const XMLNode& node);

	//vars
	NamedProperties m_stateProperties; //state properties
	NamedProperties* m_styleStateProperties = NULL; //style state properties
	NamedProperties* m_styleDefaultProperties = NULL; //style def properties
	NamedProperties* m_defaultProperties = NULL; //widget default properties
	WidgetStateID m_widgetStateID;
	std::string m_colorScheme;

	AnimatedProperties m_animProperties; //figure out how to stuff the data into animated properties
};

///----------------------------------------------------------------------------------------------------------
///inline

template<typename T>
PropertyGetResult WidgetStateProperties::GetProperty(const std::string& propertyName, T& outPropertyValue) {
	//this will go all the way down
	return GetStateProperty(propertyName, outPropertyValue);
}

//-----------------------------------------------------------------------------------------------------------

template<typename T>
PropertyGetResult WidgetStateProperties::GetStateProperty(const std::string& propertyName, T& outPropertyValue) {
	if (m_stateProperties.IsEmpty()) {
		return GetStyleProperty(propertyName, outPropertyValue);
	}
	PropertyGetResult result = GetCurrentProperty(&m_stateProperties, propertyName, outPropertyValue);
	if (result == RESULT_FAILED_NOT_FOUND) {

		return GetStyleProperty(propertyName, outPropertyValue);

	}
	return result;
}

//-----------------------------------------------------------------------------------------------------------

template<typename T>
PropertyGetResult WidgetStateProperties::GetStyleProperty(const std::string& propertyName, T& outPropertyValue) {
	if (!m_styleDefaultProperties) {
		return GetDefaultProperty(propertyName, outPropertyValue);
	}

	PropertyGetResult result = RESULT_PROPERTIES_WAS_EMPTY;

	result = GetCurrentProperty(m_styleDefaultProperties, propertyName, outPropertyValue);
	if (result == RESULT_FAILED_NOT_FOUND) {
		return GetDefaultProperty(propertyName, outPropertyValue);
	}

	return result;
}

//-----------------------------------------------------------------------------------------------------------

template<typename T>
PropertyGetResult WidgetStateProperties::GetDefaultProperty(const std::string& propertyName, T& outPropertyValue) {
	
	PropertyGetResult result = RESULT_PROPERTIES_WAS_EMPTY;
	if (m_defaultProperties) {
		result = GetCurrentProperty(m_defaultProperties, propertyName, outPropertyValue);
	}
	return result;	
}

//-----------------------------------------------------------------------------------------------------------

template<typename T>
inline PropertyGetResult WidgetStateProperties::GetCurrentProperty(NamedProperties* properties, const std::string& propertyName, T& outPropertyValue){
	PropertyGetResult result = RESULT_PROPERTIES_WAS_EMPTY;
	if (properties) {
		AnimatedProperty<T>* anim;
		result = properties->Get(propertyName, anim);
		if (result == RESULT_SUCCESS) {
			outPropertyValue = anim->GetCurrentValue();
		}
		else if (result == RESULT_FAILED_WRONG_TYPE) {
			result = properties->Get(propertyName, outPropertyValue);
		}
	}

	return result;

}

//-----------------------------------------------------------------------------------------------------------

template<typename T>
void WidgetStateProperties::CreateProperty(const std::string& name, const T& val) {
	m_stateProperties.Set(name, val);

	//if the property has an anim, then add anim property
	AddAnimatedProperty(val);
}

//-----------------------------------------------------------------------------------------------------------

template<typename T>
void WidgetStateProperties::AddAnimatedProperty(const T& val) {
	AnimatedProperty<T>* newAnimProperty = new AnimatedProperty<T>(val);

	m_animProperties.push_back(newAnimProperty);

}

//-----------------------------------------------------------------------------------------------------------

template<typename T>
void WidgetStateProperties::CreateAnimatedProperty(const char* name, const XMLNode& node ) {
	AnimatedProperty<T>* newAnimProperty = new AnimatedProperty<T>(node);

	//figure out how to load it from xml and stuff it in here
	m_stateProperties.Set(name, newAnimProperty);

	m_animProperties.push_back(newAnimProperty);

}

//-----------------------------------------------------------------------------------------------------------

template<typename T>
void WidgetStateProperties::LoadAnimFromXML(const char* name, const XMLNode& node) {

	CreateAnimatedProperty<T>(name, node);

}

//===========================================================================================================

#endif //__includedWidgetState__

