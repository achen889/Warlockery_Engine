//==============================================================================================================
//TypedProperty.hpp
//by Albert Chen Mar-30-2016.
//==============================================================================================================

#pragma once

#ifndef _included_TypedProperty__
#define _included_TypedProperty__

#include <string>
#include <stdio.h>
#include <iosfwd>
#include <vector>
#include <map>

#include <typeinfo> //must include for type info

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///global helpers

//maybe have stuff here

//===========================================================================================================

//bizzaro magic

struct BaseTypedProperty {
	//do nothing for now

	virtual const std::string GetTypeName() const = 0;
	virtual bool IsOfType(const type_info& typeToCompare) = 0;
	virtual BaseTypedProperty* Clone() = 0;

};

template<typename T>
struct TypedProperty : public BaseTypedProperty {

	//vars
	T value;

	//methods
	TypedProperty(const T& propertyValue);

	const std::string GetTypeName() const { return typeid(T).name(); }

	virtual bool IsOfType(const type_info& typeToCompare) override;

	virtual BaseTypedProperty* Clone() override;

	//const char* GetTypeName() const;
	//const std::string ToString() const; //gets typename and value
};

typedef std::map<std::string, BaseTypedProperty* > TypedPropertiesMap;
typedef std::map<std::string, BaseTypedProperty* >::iterator TypedPropertiesMapIterator;
typedef std::map<std::string, BaseTypedProperty* >::const_iterator TypedPropertiesMapConstIterator;
typedef std::pair<std::string, BaseTypedProperty* > TypedPropertiesMapEntry;

///----------------------------------------------------------------------------------------------------------
///inline methods
template<typename T>
inline BaseTypedProperty* TypedProperty<T>::Clone(){

	return new TypedProperty<T>(value);

}

template<typename T>
inline TypedProperty<T>::TypedProperty(const T& propertyValue) :
	value(propertyValue) {
	//do stuff here
}

//-----------------------------------------------------------------------------------------------------------

template<typename T>
inline bool TypedProperty<T>::IsOfType(const type_info& typeToCompare) {
	const type_info& myType = typeid(T);

	return (myType == typeToCompare);
}


//===========================================================================================================

#endif //__includedTypedProperty__

