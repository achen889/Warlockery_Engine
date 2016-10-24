//==============================================================================================================
//NamedProperties.hpp
//by Albert Chen Mar-23-2016.
//==============================================================================================================

#pragma once

#ifndef _included_NamedProperties__
#define _included_NamedProperties__

#include "TypedProperty.hpp"

//===========================================================================================================

enum PropertyGetResult {
	RESULT_SUCCESS,
	RESULT_FAILED_NOT_FOUND,
	RESULT_FAILED_WRONG_TYPE,
	RESULT_PROPERTIES_WAS_EMPTY,

	NUM_RESULTS
};

inline const std::string ToString(const PropertyGetResult& propertyGetResult) {
	switch (propertyGetResult) {
	case RESULT_SUCCESS: 
		return "PropertyGetResult: Success!";
	case RESULT_FAILED_NOT_FOUND: 
		return "PropertyGetResult: Fail Not Found!";
	case RESULT_FAILED_WRONG_TYPE: 
		return "PropertyGetResult: Fail Wrong Type!";
	default:
		return "PropertyGetResult: Unknown!";
	}
	return "";
}

//===========================================================================================================
//will work using bizzaro magic
class NamedProperties {

public:
	static NamedProperties EMPTY;

	NamedProperties(const NamedProperties& copy);

	const NamedProperties& operator=(const NamedProperties& copy);

	NamedProperties() {
		//do nothing
	}

	~NamedProperties(){
		//do nothing
		for (auto& propertyIt : m_properties) {
			BaseTypedProperty* property = propertyIt.second;
			if (property) {
				delete property;
				property = NULL;
			}
		}
		m_properties.clear();
	}

	bool HasProperty(const std::string& propertyName) {
		TypedPropertiesMapIterator it = m_properties.find(propertyName);
		if (it != m_properties.end()) {
			return true;
		}
		return false;
	}
	
	void Set(const std::string& propertyName, const char* propertyValue);
	template<typename T >
	void Set(const std::string& propertyName, const T& propertyValue);
	
	PropertyGetResult Get(const std::string& propertyName, std::string& out_propertyValue) const;
	template<typename T>
	PropertyGetResult Get(const std::string& propertyName, T& out_propertyValue) const;

	template<typename T>
	T* Get(const std::string& propertyName) const;


	const TypedPropertiesMap& GetProperties() const {
		return m_properties;
	}

	bool IsEmpty() {
		return m_properties.empty();
	}
	
private:
	TypedPropertiesMap m_properties;
	
};


///----------------------------------------------------------------------------------------------------------
///inline methods

inline NamedProperties::NamedProperties(const NamedProperties& copy) {

	*this = copy;

}



inline void NamedProperties::Set(const std::string& propertyName, const char* propertyValue) {
	
	//check if it exists, replace or delete
	TypedPropertiesMapIterator it = m_properties.find(propertyName);
	if (it != m_properties.end()) {
		BaseTypedProperty* propertyBase = it->second;
		//check if types are the same
		if (propertyBase->IsOfType(typeid(const char*))) {
			//if same type do stuff
			//typecast it to the right <T>
			TypedProperty<const char*>* TProperty = (TypedProperty<const char*>*)propertyBase;
			//directly set the value
			TProperty->value = propertyValue;
			return;
		}
		else {
			//if exists but different type
			delete propertyBase;
		}//end of if/else

	}

	//create a new one
	BaseTypedProperty* newProperty = new TypedProperty<const char*>(propertyValue);
	m_properties[propertyName] = newProperty;
}

template<typename T>
inline void NamedProperties::Set(const std::string& propertyName, const T& propertyValue) {
	
	//check if it exists, replace or delete
	TypedPropertiesMapIterator it = m_properties.find(propertyName);
	if(it != m_properties.end()) {
		BaseTypedProperty* propertyBase = it->second;
		//check if types are the same
		if (propertyBase->IsOfType(typeid(T))) {
			//if same type do stuff
			//typecast it to the right <T>
			TypedProperty<T>* TProperty = (TypedProperty<T>*)propertyBase;
			//directly set the value
			TProperty->value = propertyValue;
			return;
		}
		else {
			//if exists but different type
			delete propertyBase;
		}//end of if/else

	}

	//create a new one
	BaseTypedProperty* newProperty = new TypedProperty<T>(propertyValue);
	m_properties[propertyName] = newProperty;
	
}

//-----------------------------------------------------------------------------------------------------------

inline PropertyGetResult NamedProperties::Get(const std::string& propertyName, std::string& out_propertyValue) const {

	if (m_properties.empty())
		return RESULT_PROPERTIES_WAS_EMPTY;

	TypedPropertiesMapConstIterator it = m_properties.find(propertyName);

	if (it == m_properties.end())
		return RESULT_FAILED_NOT_FOUND;

	BaseTypedProperty* propertyBase = it->second;

	if (propertyBase->IsOfType(typeid(const char*))) {
		TypedProperty<const char*>* TProperty = (TypedProperty<const char*>*)propertyBase;
		//now set by ref
		out_propertyValue = std::string(TProperty->value);

		return RESULT_SUCCESS;
	}

	if (propertyBase->IsOfType(typeid(std::string))) {
		TypedProperty<std::string>* TProperty = (TypedProperty<std::string>*)propertyBase;
		//now set by ref
		out_propertyValue = TProperty->value;

		return RESULT_SUCCESS;
	}

	return RESULT_FAILED_WRONG_TYPE;

}

//-----------------------------------------------------------------------------------------------------------

template<typename T>
inline PropertyGetResult NamedProperties::Get(const std::string& propertyName, T& out_propertyValue) const {
	
	if (m_properties.empty())
		return RESULT_PROPERTIES_WAS_EMPTY;

	TypedPropertiesMapConstIterator it = m_properties.find(propertyName);

	if (it == m_properties.end()) 
		return RESULT_FAILED_NOT_FOUND;

	BaseTypedProperty* propertyBase = it->second;
	//ASSERT_PTR_VALID(propertyBase);
	if (propertyBase->IsOfType(typeid(T))) {
		TypedProperty<T>* TProperty = (TypedProperty<T>*)propertyBase;
		//now set by ref
		out_propertyValue = TProperty->value;
		return RESULT_SUCCESS;
	}

	return RESULT_FAILED_WRONG_TYPE;
}

//-----------------------------------------------------------------------------------------------------------

template<typename T>
inline T* NamedProperties::Get(const std::string& propertyName) const {

	if (m_properties.empty())
		return NULL;

	TypedPropertiesMapConstIterator it = m_properties.find(propertyName);
	if (it == m_properties.end())
		return NULL;

	BaseTypedProperty* propertyBase = it->second;
	//ASSERT_PTR_VALID(propertyBase);
	if (propertyBase->IsOfType(typeid(T))) {
		TypedProperty<T>* TProperty = (TypedProperty<T>*)propertyBase;
		//now set by ref
		return &TProperty->value;
	}
	return NULL;
}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================

#endif //__includedNamedProperties__

