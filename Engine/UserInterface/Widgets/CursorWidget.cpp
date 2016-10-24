//==============================================================================================================
//CursorWidget.cpp
//by Albert Chen Apr-24-2016.
//==============================================================================================================

#include "CursorWidget.hpp"
//#include "../UISystem.hpp"
#include "Engine/Input/InputSystem.hpp"

//===========================================================================================================

WidgetRegistration CursorWidget::s_CursorWidgetRegistration("Cursor", &CursorWidget::CreateWidget, &CursorWidget::CreateWidgetXML);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors

CursorWidget::CursorWidget(){
	//do nothing

}

CursorWidget::CursorWidget(const std::string& name) : GroupWidget(name){
	
}

CursorWidget::CursorWidget(const XMLNode& node) : GroupWidget(node){
	
}

CursorWidget::~CursorWidget(){
	//do nothing
}

///----------------------------------------------------------------------------------------------------------
///virtual overrides

void CursorWidget::SetDefaultPropertiesFromXML(const XMLNode& node) {
	GroupWidget::SetDefaultPropertiesFromXML(node);
}


void CursorWidget::LoadBaseAttributesFromXML(const XMLNode& node) {
	BaseWidget::LoadBaseAttributesFromXML(node);
}

//-----------------------------------------------------------------------------------------------------------

void CursorWidget::UpdateRenderComponent(double deltaSeconds) {

	IF_INPUT_SYSTEM_EXISTS{
		Vector2 mouseScreenPos = theInputSystem->GetMouseScreenPosition();
		SetOffset(mouseScreenPos);
		//GetCurrentStateProperties().SetMyDefaultProperties(&m_widgetDefaultProperties);
	}

	BaseWidget::UpdateRenderComponent(deltaSeconds);

}

//-----------------------------------------------------------------------------------------------------------

//upkeep
void CursorWidget::Update(double deltaSeconds) {

	GroupWidget::Update(deltaSeconds);

}

//render
void CursorWidget::Render(OpenGLRenderer* renderer) {

	BaseWidget::Render(renderer);

}

//===========================================================================================================

