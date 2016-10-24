//==============================================================================================================
//LayoutWidget.cpp
//by Albert Chen Apr-24-2016.
//==============================================================================================================

#include "LayoutWidget.hpp"
#include "..\UISystem.hpp"
#include "..\UICommon.hpp"

//===========================================================================================================

WidgetRegistration LayoutWidget::s_LayoutWidgetRegistration("Layout", &LayoutWidget::CreateWidget, &LayoutWidget::CreateWidgetXML);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors

LayoutWidget::LayoutWidget(){
	//do nothing

}

LayoutWidget::LayoutWidget(const std::string& name) : GroupWidget(name){
	
}

LayoutWidget::LayoutWidget(const XMLNode& node) : GroupWidget(node){
	
}

LayoutWidget::~LayoutWidget() {
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

void LayoutWidget::StartUp() {
	GroupWidget::StartUp();

	Vector2 displaySize = theOGLRenderer->GetDisplaySize();
	SetOffset(displaySize * 0.5f);
	SetDimensions(displaySize);

}

//-----------------------------------------------------------------------------------------------------------

void LayoutWidget::LoadBaseAttributesFromXML(const XMLNode& node) {

	GroupWidget::LoadBaseAttributesFromXML(node);

}

//-----------------------------------------------------------------------------------------------------------

//upkeep
void LayoutWidget::Update(double deltaSeconds){

	GroupWidget::Update(deltaSeconds);

}

//render
void LayoutWidget::Render(OpenGLRenderer* renderer) {

	GroupWidget::Render(renderer);

}

//===========================================================================================================

