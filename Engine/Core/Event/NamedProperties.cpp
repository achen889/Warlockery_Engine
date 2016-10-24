//==============================================================================================================
//NamedProperties.cpp
//by Albert Chen Mar-23-2016.
//==============================================================================================================

#include "NamedProperties.hpp"

//===========================================================================================================

NamedProperties NamedProperties::EMPTY = NamedProperties();

//===========================================================================================================

//assignment operator

const NamedProperties& NamedProperties::operator=(const NamedProperties& copy) {
	
	for (TypedPropertiesMapEntry copyMapEntry : copy.GetProperties()) {

		BaseTypedProperty* cloneProperty = copyMapEntry.second->Clone();

		m_properties.insert(TypedPropertiesMapEntry(copyMapEntry.first, cloneProperty));

	}

	return *this;
}

//===========================================================================================================

