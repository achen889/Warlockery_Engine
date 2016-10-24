//==============================================================================================================
//WidgetStyle.hpp
//by Albert Chen May-7-2016.
//==============================================================================================================

#pragma once


#ifndef _included_WidgetStyle__
#define _included_WidgetStyle__

#include "Widgets\BaseWidget.hpp"

//===========================================================================================================

class WidgetStyle{
public:
	//methods

	WidgetStyle();
	WidgetStyle(const std::string& name);
	WidgetStyle(const XMLNode& node);

	//xml helpers

	void SetStyleDefaultsFromXML(const XMLNode& node);

	NamedProperties& GetStyleDefaultProperties() {
		return m_widgetDefaultProperties;
	}

	const std::string GetColorSchemeName(){
		return m_colorSchemeName;
	}

protected:
	//vars
	char* m_name;

	std::string m_colorSchemeName = "MenuColors";

	XMLNode m_xmlNode;

	WidgetStateProperties m_properties[NUM_WIDGET_STATES];
	NamedProperties m_widgetDefaultProperties; //defaults for widgets

};


//===========================================================================================================

#endif //__includedWidgetStyle__

