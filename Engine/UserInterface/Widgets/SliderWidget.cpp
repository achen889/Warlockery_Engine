//==============================================================================================================
//SliderWidget.cpp
//by Albert Chen May-8-2016.
//==============================================================================================================

#include "SliderWidget.hpp"
#include "..\UISystem.hpp"
#include "Engine/Input/InputSystem.hpp"

#include "ButtonWidget.hpp"

//===========================================================================================================

WidgetRegistration SliderWidget::s_SliderWidgetRegistration("Slider", &SliderWidget::CreateWidget, &SliderWidget::CreateWidgetXML);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors

SliderWidget::SliderWidget(){
	//do nothing

}

SliderWidget::SliderWidget(const std::string& name) : ProgressBarWidget(name){
	
}

SliderWidget::SliderWidget(const XMLNode& node) : ProgressBarWidget(node){
	
}

SliderWidget::~SliderWidget(){
	//do nothing
}

///----------------------------------------------------------------------------------------------------------
///virtual overrides

void SliderWidget::LoadBaseAttributesFromXML(const XMLNode& node) {
	ProgressBarWidget::LoadBaseAttributesFromXML(node);

	//construct the slider button from existing button name
	std::string sliderButtonName = ReadXMLAttributeAsString(node, "sliderButton");
	if (!m_sliderButton) {
		m_sliderButton = (ButtonWidget*)theUISystem->CreateWidget(sliderButtonName, Vector2::ZERO, WIDGET_STATE_IDLE);
	}
}

void SliderWidget::StartUp() {
	ProgressBarWidget::StartUp();

	if (m_sliderButton) {
		m_sliderButton->SetParent(this);
		m_sliderButton->SetAnchorID(UI_ANCHOR_CENTER);
		m_sliderButton->SetOffset(Vector2::ZERO);
		
		m_sliderButton->StartUp();
	}

	m_widgetDefaultProperties.Set("percent", 0.5f);
	
}

//-----------------------------------------------------------------------------------------------------------

void SliderWidget::SetDefaultPropertiesFromXML(const XMLNode& node) {
	ProgressBarWidget::SetDefaultPropertiesFromXML(node);
}

//-----------------------------------------------------------------------------------------------------------

void SliderWidget::ProcessInput(double deltaSeconds) {
	UNUSED(deltaSeconds);
	if (OnHover()) {
		SetState(WIDGET_STATE_HIGHLIGHTED);
		if (m_sliderButton->OnHeld(MOUSE_BUTTON_LEFT)) {
			Vector2 mouseScreenPos = theInputSystem->GetMouseScreenPosition();

			float newPercent = SetSliderButtonXPosAlongProgressBar(mouseScreenPos);
			m_widgetDefaultProperties.Set("percent", newPercent);
		}
		
	}
}

//upkeep
void SliderWidget::Update(double deltaSeconds) {

	ProgressBarWidget::Update(deltaSeconds);
	
	AABB2 mySliderBounds = GetRenderBounds();
	Vector2 mySliderLength = mySliderBounds.CalcLengthOfSides();
	m_sliderButton->SetDimensions(Vector2(mySliderLength.x * 0.1f, mySliderLength.y * 1.25f));

	//set percent num
	WidgetStateProperties& currentStateProperties = GetCurrentStateProperties();
	float myPercent;
	currentStateProperties.GetProperty("percent", myPercent);
	std::string percentString = FloatToString(myPercent * 100, "%.0f") + "%";
	m_sliderButton->SetText(percentString);

	m_sliderButton->Update(deltaSeconds);

}

//render
void SliderWidget::Render(OpenGLRenderer* renderer) {

	BaseWidget::Render(renderer);

	m_sliderButton->Render(renderer);

}

//-----------------------------------------------------------------------------------------------------------

///----------------------------------------------------------------------------------------------------------
///helpers

float SliderWidget::SetSliderButtonXPosAlongProgressBar(const Vector2& mouseScreenPos) {
	IF_INPUT_SYSTEM_EXISTS{

		Vector2 mySliderPos = this->GetPosition();
		AABB2 mySliderBounds = GetRenderBounds();
		//float xMin = mySliderBounds.mins.x;
		//float xMax = mySliderBounds.maxs.x;

		Vector2 mouseSliderPercent;
		mouseSliderPercent = mySliderBounds.GetNormalizedPositionForPointWithinBox(mouseScreenPos);
		//sets button offset based on mouse slider percent
		if (mouseSliderPercent.x >= 0.0f && mouseSliderPercent.x <= 1.0f) {
			Vector2 newSliderButtonOffset;

			newSliderButtonOffset = mySliderBounds.GetPointAtNormalizedPositionWithinBox(
				Vector2(mouseSliderPercent.x, 0.5f));
			newSliderButtonOffset.x -= mySliderPos.x;
			newSliderButtonOffset.y = 0.0f;

			m_sliderButton->SetOffset(newSliderButtonOffset);
		}
		
		Vector2 sliderButtonPos = m_sliderButton->GetPosition();

		//calc percent
		Vector2 sliderButtonPercent;
		sliderButtonPercent = mySliderBounds.GetNormalizedPositionForPointWithinBox(sliderButtonPos);
		return sliderButtonPercent.x;
	}
	return 1.0f;
}

//===========================================================================================================

