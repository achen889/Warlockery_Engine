//==============================================================================================================
//EditLineWidget.cpp
//by Albert Chen May-3-2016.
//==============================================================================================================

#include "EditLineWidget.hpp"
#include "..\UISystem.hpp"
#include "..\..\Renderer\Text\TextSystem.hpp"

//===========================================================================================================

WidgetRegistration EditLineWidget::s_EditLineWidgetRegistration("EditLine", &EditLineWidget::CreateWidget, &EditLineWidget::CreateWidgetXML);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors

EditLineWidget::EditLineWidget(){
	//do nothing

}

EditLineWidget::EditLineWidget(const std::string& name) : CheckBoxWidget(name){
	
}

EditLineWidget::EditLineWidget(const XMLNode& node) : CheckBoxWidget(node){
	
}

EditLineWidget::~EditLineWidget(){
	//do nothing
}

///----------------------------------------------------------------------------------------------------------
///virtual overrides

void EditLineWidget::LoadBaseAttributesFromXML(const XMLNode& node) {
	CheckBoxWidget::LoadBaseAttributesFromXML(node);

	//m_widgetDefaultProperties.Get("text", m_textInputBuffer);
}


void EditLineWidget::SetDefaultPropertiesFromXML(const XMLNode& node) {
	CheckBoxWidget::SetDefaultPropertiesFromXML(node);
}

//-----------------------------------------------------------------------------------------------------------

void EditLineWidget::ProcessInput(double deltaSeconds) {
	CheckBoxWidget::ProcessInput(deltaSeconds);
	
	bool isChecked;
	m_widgetDefaultProperties.Get("isChecked", isChecked);
	if (isChecked) {
		
		if (theTextSystem) {

			WidgetStateProperties& currentStateProperties = GetCurrentStateProperties();

			Vector2 myDimensions;
			currentStateProperties.GetProperty("dimensions", myDimensions);

			if (theTextSystem->CalcTextWidth(m_textInputBuffer) < myDimensions.x) {
				theTextSystem->InputTextToBuffer(m_textInputBuffer);
			}
			else {
				m_widgetDefaultProperties.Set("isChecked", false);
				SetState(WIDGET_STATE_IDLE);
			}
			//confirm edits 
			if (theInputSystem->WasKeyJustReleased(KEY_ENTER)) {

				m_widgetDefaultProperties.Set("isChecked", false);
				SetState(WIDGET_STATE_IDLE);
			}

		}
	}

}

//-----------------------------------------------------------------------------------------------------------

//upkeep
void EditLineWidget::Update(double deltaSeconds) {
	CheckBoxWidget::Update(deltaSeconds);

	WidgetStateProperties& currentStateProperties = GetCurrentStateProperties();
	if (currentStateProperties.m_defaultProperties) {
		currentStateProperties.m_defaultProperties->Set("text", m_textInputBuffer);
	}
}

//-----------------------------------------------------------------------------------------------------------

//render
void EditLineWidget::Render(OpenGLRenderer* renderer) {

	CheckBoxWidget::Render(renderer);
}

//===========================================================================================================

