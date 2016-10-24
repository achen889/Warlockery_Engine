//==============================================================================================================
//ButtonWidget.cpp
//by Albert Chen Apr-18-2016.
//==============================================================================================================

#include "ButtonWidget.hpp"
//#include "..\UISystem.hpp"
#include "..\..\Renderer\Text\TextSystem.hpp"



//===========================================================================================================

WidgetRegistration ButtonWidget::s_ButtonWidgetRegistration("Button", &ButtonWidget::CreateWidget, &ButtonWidget::CreateWidgetXML);

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///constructors

ButtonWidget::ButtonWidget(){
	//do nothing
}

ButtonWidget::ButtonWidget(const std::string& name) : LabelWidget(name){

}

ButtonWidget::ButtonWidget(const XMLNode& node) : LabelWidget(node){

// 	LoadBaseAttributesFromXML(node);
// 
// 	LoadWidgetStatesFromXML(node);

}

ButtonWidget::~ButtonWidget(){
	//do nothing
}

///----------------------------------------------------------------------------------------------------------
///start up

void ButtonWidget::LoadBaseAttributesFromXML(const XMLNode& node) {
	
	LabelWidget::LoadBaseAttributesFromXML(node);

	std::string clickEvent = ReadXMLAttributeAsString(node, "clickEvent", "");
	m_widgetDefaultProperties.Set("clickEvent", clickEvent);

	std::string altKey = ReadXMLAttributeAsString(node, "altKey", "");
	if (altKey == "") {
		m_altKey = -1;
	}
	else {
		m_altKey = (int)altKey[0];
	}

}

//-----------------------------------------------------------------------------------------------------------

void ButtonWidget::SetDefaultPropertiesFromXML(const XMLNode& node) {

	LabelWidget::SetDefaultPropertiesFromXML(node);

}

//-----------------------------------------------------------------------------------------------------------

void ButtonWidget::ProcessInput(double deltaSeconds) {
	BaseWidget::ProcessInput(deltaSeconds);

// 	if (IsAltKeyReleased() && GetCurrentState() != WIDGET_STATE_HIGHLIGHTED) {
// 		m_isSelected = !m_isSelected;
// 	}
// 	if (IsAltKeyReleased() && GetCurrentState() == WIDGET_STATE_HIGHLIGHTED) {
// 		SetState(WIDGET_STATE_ACTIVATED);
// 		FireClickEvent(deltaSeconds);
// 	}
// 
// 	if (m_isSelected) {
// 		SetState(WIDGET_STATE_HIGHLIGHTED);
// 	}
// 	else {
// 		SetState(WIDGET_STATE_IDLE);
// 	}

	if (m_UIColliderComponent.OnHover()) {
		if (m_UIColliderComponent.OnLeftClick()) {
			FireClickEvent(deltaSeconds);
		}
	}//end of outer if


}

//-----------------------------------------------------------------------------------------------------------

void ButtonWidget::FireClickEvent(double deltaSeconds) {
	WidgetStateProperties& currentStateProperties = GetCurrentStateProperties();
	std::string clickEventNameStr;
	currentStateProperties.GetProperty("clickEvent", clickEventNameStr);
	if (clickEventNameStr != "") {
		NamedProperties clickEventParams;
		clickEventParams.Set("widgetInstance", (BaseWidget*)this);
		clickEventParams.Set("deltaSeconds", deltaSeconds);
		EventSystem::GetInstance().FireEvent(clickEventNameStr, clickEventParams);
	}
}

//===========================================================================================================

//upkeep
void ButtonWidget::Update(double deltaSeconds) {

	LabelWidget::Update(deltaSeconds);

}

//-----------------------------------------------------------------------------------------------------------

//render
void ButtonWidget::Render(OpenGLRenderer* renderer) {

	LabelWidget::Render(renderer);

// 	AABB2 debugBox = m_UIColliderComponent.GetBox2DCollider();
// 	renderer->DrawMeshQuad2D(debugBox);

}

//-----------------------------------------------------------------------------------------------------------

bool ButtonWidget::IsAltKeyReleased() {
	return INPUT_IS_KEY_RELEASED(m_altKey);
}

//===========================================================================================================

