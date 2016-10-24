//==============================================================================================================
//CheckBoxWidget.cpp
//by Albert Chen Apr-24-2016.
//==============================================================================================================

#include "CheckBoxWidget.hpp"
//#include "..\UISystem.hpp"

//===========================================================================================================

WidgetRegistration CheckBoxWidget::s_CheckBoxWidgetRegistration("CheckBox", &CheckBoxWidget::CreateWidget, &CheckBoxWidget::CreateWidgetXML);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors

CheckBoxWidget::CheckBoxWidget(){
	//do nothing

}

CheckBoxWidget::CheckBoxWidget(const std::string& name) : ButtonWidget(name){
	
}

CheckBoxWidget::CheckBoxWidget(const XMLNode& node) : ButtonWidget(node){
	
}

CheckBoxWidget::~CheckBoxWidget(){
	//do nothing
}

///----------------------------------------------------------------------------------------------------------
///virtual overrides

void CheckBoxWidget::SetDefaultPropertiesFromXML(const XMLNode& node) {
	ButtonWidget::SetDefaultPropertiesFromXML(node);
}


void CheckBoxWidget::LoadBaseAttributesFromXML(const XMLNode& node) {
	ButtonWidget::LoadBaseAttributesFromXML(node);

	int isChecked = ReadXMLAttributeAsInt(node, "isChecked", 0);
	if (isChecked == 0) 
		m_widgetDefaultProperties.Set("isChecked", false);
	if (isChecked == 1) 
		m_widgetDefaultProperties.Set("isChecked", true);
}

//-----------------------------------------------------------------------------------------------------------

void CheckBoxWidget::ProcessInput(double deltaSeconds) {
	UNUSED(deltaSeconds);

	if (m_UIColliderComponent.OnHover()) {
		SetState(WIDGET_STATE_HIGHLIGHTED);
		if (m_UIColliderComponent.OnLeftClick()) {
			bool isChecked;
			m_widgetDefaultProperties.Get("isChecked", isChecked);
			m_widgetDefaultProperties.Set("isChecked", !isChecked);
		}
	}
	else {
		bool isChecked;
		m_widgetDefaultProperties.Get("isChecked", isChecked);
		if (isChecked) {
			SetState(WIDGET_STATE_ACTIVATED);
		}
		else {
			SetState(WIDGET_STATE_IDLE);
		}
	}

}

//upkeep
void CheckBoxWidget::Update(double deltaSeconds) {
	ButtonWidget::Update(deltaSeconds);
}

//render
void CheckBoxWidget::Render(OpenGLRenderer* renderer) {

	ButtonWidget::Render(renderer);

}

//===========================================================================================================

