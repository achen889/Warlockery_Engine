//==============================================================================================================
//WidgetStyle.cpp
//by Albert Chen May-7-2016.
//==============================================================================================================

#include "WidgetStyle.hpp"
#include "UISystem.hpp"

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///constructors

WidgetStyle::WidgetStyle() {

}

WidgetStyle::WidgetStyle(const std::string& name) : 
	m_name(StringToWritableCStr(name)){

}

WidgetStyle::WidgetStyle(const XMLNode& node) :
	m_xmlNode(node){

	std::string name = ReadXMLAttributeAsString(m_xmlNode, "name");
	m_name = StringToWritableCStr(name);

	std::string colorSchemeName = ReadXMLAttributeAsString(m_xmlNode, "colorScheme");
	m_colorSchemeName = StringToWritableCStr(colorSchemeName);

	//load data using internal node
	SetStyleDefaultsFromXML(m_xmlNode);

}

///----------------------------------------------------------------------------------------------------------
///xml helpers

void WidgetStyle::SetStyleDefaultsFromXML(const XMLNode& node) {

	//base widget properties
	std::string bgColorStr = ReadXMLAttributeAsString(node, "bgColor");
	Rgba bgRgba = theUISystem->GetColorFromColorScheme(m_colorSchemeName, bgColorStr);

	std::string edgeColorStr = ReadXMLAttributeAsString(node, "edgeColor");
	Rgba edgeRgba = theUISystem->GetColorFromColorScheme(m_colorSchemeName, edgeColorStr);

	float newOpacity = ReadXMLAttributeAsFloat(node, "opacity", 1.0f);

	float edgeSize = ReadXMLAttributeAsFloat(node, "edgeSize", 4.0f);

	//set state properties
	m_widgetDefaultProperties.Set("bgColor", bgRgba);
	m_widgetDefaultProperties.Set("edgeColor", edgeRgba);
	m_widgetDefaultProperties.Set("opacity", newOpacity);
	m_widgetDefaultProperties.Set("edgeSize", edgeSize);
}

//-----------------------------------------------------------------------------------------------------------




//===========================================================================================================

