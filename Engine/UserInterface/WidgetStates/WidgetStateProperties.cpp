//==============================================================================================================
//WidgetStateProperties.cpp
//by Albert Chen Apr-20-2016.
//==============================================================================================================

#include "WidgetStateProperties.hpp"
#include "..\UISystem.hpp"
#include "..\KeyFrameAnimation\AnimatedProperty.hpp"

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///constructors

WidgetStateProperties::WidgetStateProperties(const std::string& widgetType, const XMLNode& node, const std::string& colorScheme ):
	m_colorScheme(colorScheme){

	//base widget properties
	std::string bgColorStr = ReadXMLAttributeAsString(node, "bgColor", "");
	Rgba bgRgba = theUISystem->GetColorFromColorScheme(m_colorScheme, bgColorStr);

	std::string edgeColorStr = ReadXMLAttributeAsString(node, "edgeColor", "");
	Rgba edgeRgba = theUISystem->GetColorFromColorScheme(m_colorScheme, edgeColorStr);

	if (HasXMLAttribute(node, "edgeSize")) {
		float newEdgeSize = ReadXMLAttributeAsFloat(node, "edgeSize");
		m_stateProperties.Set("edgeSize", newEdgeSize);
	}

	float newOpacity = ReadXMLAttributeAsFloat(node, "opacity", 1.0f);

	//set state properties

	m_stateProperties.Set("bgColor", bgRgba);
	m_stateProperties.Set("edgeColor", edgeRgba);
	m_stateProperties.Set("opacity", newOpacity);


	if (widgetType == "Button") {
		//button Widget Properties
// 		float newTextScale = ReadXMLAttributeAsFloat(node, "textScale", 1.0f);
// 		m_stateProperties.Set("textScale", newTextScale);
		
		if (HasXMLAttribute(node, "textColor")) {
			std::string buttonTextColor = ReadXMLAttributeAsString(node, "textColor", "");
			if (buttonTextColor != "") {
				Rgba buttonTextRgba = theUISystem->GetColorFromColorScheme(m_colorScheme, buttonTextColor);
				m_stateProperties.Set("textColor", buttonTextRgba);
			}
		}

		float newTextOpacity = ReadXMLAttributeAsFloat(node, "textOpacity", 1.0f);
		m_stateProperties.Set("textOpacity", newTextOpacity);

		std::string buttonEvent = ReadXMLAttributeAsString(node, "eventName", "");
		m_stateProperties.Set("eventName", buttonEvent);
	}

	int num = node.nChildNode("Anim");
	for (int i = 0; i < num; i++) {
		XMLNode tempNode = node.getChildNode("Anim", i);

		std::string propertyName = ReadXMLAttributeAsString(tempNode, "property");
		if (propertyName == "opacity" || propertyName =="percent") {
			LoadAnimFromXML<float>(propertyName.c_str(), tempNode);
		}
		else if (propertyName == "bgColor") {
			LoadAnimFromXML<Rgba>(propertyName.c_str(), tempNode);
		}
		else if (propertyName == "fillColor") {
			LoadAnimFromXML<Rgba>(propertyName.c_str(), tempNode);
		}
		else if (propertyName == "edgeColor") {
			LoadAnimFromXML<Rgba>(propertyName.c_str(), tempNode);
		}
	}

}

//-----------------------------------------------------------------------------------------------------------

WidgetStateProperties::~WidgetStateProperties() {

	//mem leak here, crashes when I clear UI screens
	for (AnimatedProperties::iterator it = m_animProperties.begin(); it != m_animProperties.end();  ){
		BaseAnimatedProperty* animProperty = (*it);
		if (animProperty) {
// 			delete animProperty;
// 			animProperty = NULL;

			it = m_animProperties.erase(it);
		}
		else {
			++it;
		}
	}

}

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///upkeep

void WidgetStateProperties::Update(double deltaSeconds) {

	for(BaseAnimatedProperty* anim : m_animProperties) {
		if (anim) {
			anim->Update(deltaSeconds);
		}
	}

}

//-----------------------------------------------------------------------------------------------------------




//===========================================================================================================

