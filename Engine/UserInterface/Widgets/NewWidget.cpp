//==============================================================================================================
//ProgressBarWidget.cpp
//by Albert Chen Apr-24-2016.
//==============================================================================================================

#include "ProgressBarWidget.hpp"
#include "..\UISystem.hpp"

//===========================================================================================================

WidgetRegistration ProgressBarWidget::s_ProgressBarWidgetRegistration("ProgressBar", &ProgressBarWidget::CreateWidget, &ProgressBarWidget::CreateWidgetXML);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors

ProgressBarWidget::ProgressBarWidget(){
	//do nothing

}

ProgressBarWidget::ProgressBarWidget(const std::string& name) : BaseWidget(name){
	
}

ProgressBarWidget::ProgressBarWidget(const XMLNode& node) : BaseWidget(node){
	
}

ProgressBarWidget::~ProgressBarWidget(){
	//do nothing
}

///----------------------------------------------------------------------------------------------------------
///virtual overrides

void ProgressBarWidget::SetDefaultPropertiesFromXML(const XMLNode& node) {
	BaseWidget::SetDefaultPropertiesFromXML(node);
}


void ProgressBarWidget::LoadBaseAttributesFromXML(const XMLNode& node) {
	BaseWidget::LoadBaseAttributesFromXML(node);
}

//-----------------------------------------------------------------------------------------------------------

//upkeep
void ProgressBarWidget::Update(double deltaSeconds) {

}

//render
void ProgressBarWidget::Render(OpenGLRenderer* renderer) {

}

//===========================================================================================================

