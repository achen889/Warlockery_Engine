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

	std::string xmlFillColorStr = ReadXMLAttributeAsString(node, "fillColor", "");
	Rgba fillColorRgba = theUISystem->GetColorFromColorScheme(m_colorScheme, xmlFillColorStr);
	
	float xmlFillOpacity = ReadXMLAttributeAsFloat(node, "fillOpacity", 1.0f);

	std::string fillTexturePath = ReadXMLAttributeAsString(node, "fillTexturePath", "");
	if (fillTexturePath != "") {
		m_widgetDefaultProperties.Set("fillTexturePath", fillTexturePath);
	}
	
	m_widgetDefaultProperties.Set("fillColor", fillColorRgba);
	m_widgetDefaultProperties.Set("fillOpacity", xmlFillOpacity);

}


void ProgressBarWidget::LoadBaseAttributesFromXML(const XMLNode& node) {
	BaseWidget::LoadBaseAttributesFromXML(node);

	float xmlPercent = ReadXMLAttributeAsFloat(node, "startPercent", 1.0f);
// 	xmlPercent /= 100.0f;
// 	AnimatedProperty<float> animPercent(1.0f);
// 	animPercent.AddKeyFrame(0.0f, 1.0f);

 	m_widgetDefaultProperties.Set("percent", xmlPercent);

}

//-----------------------------------------------------------------------------------------------------------

void ProgressBarWidget::SetRenderComponent() {

	BaseWidget::SetRenderComponent();

	//copy info from render comp
	//m_FillRenderComponent = m_renderComponent;

	Vector2 fillDimensions = m_renderComponent.GetRenderBounds().CalcLengthOfSides();

	Vector2 renderCompPos = m_renderComponent.GetPosition() - (fillDimensions * 0.5f);
	//m_FillRenderComponent.SetPosition(renderCompPos);
	//m_FillRenderComponent.SetRenderBounds();

	WidgetStateProperties& currentStateProperties = GetCurrentStateProperties();

	float myFillOpacity;
	currentStateProperties.GetProperty("fillOpacity", myFillOpacity);

	Rgba myFillColor;
	currentStateProperties.GetProperty("fillColor", myFillColor);

	myFillColor.a = Rgba::cA(myFillOpacity);

	//m_FillRenderComponent.SetColor(myFillColor);
	
	std::string texturePath;
	currentStateProperties.GetProperty("fillTexturePath", texturePath);


	m_FillRenderComponent.SetName(GetName() + "_Fill");

	m_FillRenderComponent.SetRenderMesh2D(renderCompPos, fillDimensions, myFillColor, 
		texturePath, "basicSampler", false,
		false, false);

}	

//-----------------------------------------------------------------------------------------------------------

void ProgressBarWidget::UpdateRenderComponent(double deltaSeconds) {
	
	BaseWidget::UpdateRenderComponent(deltaSeconds);

	WidgetStateProperties& currentStateProperties = GetCurrentStateProperties();

	AABB2 fillRenderBounds = m_renderComponent.GetRenderBounds();

	ScaleFillBoundsXByPercent(fillRenderBounds);

	float myFillOpacity;
	currentStateProperties.GetProperty("opacity", myFillOpacity);

	Rgba myFillColor;
	currentStateProperties.GetProperty("fillColor", myFillColor);

	myFillColor.a = Rgba::cA(myFillOpacity);

	m_FillRenderComponent.SetRenderBounds(fillRenderBounds);

	m_FillRenderComponent.UpdateMesh(fillRenderBounds, myFillColor, false);

}

//-----------------------------------------------------------------------------------------------------------


void ProgressBarWidget::ProcessInput(double deltaSeconds) {
	//do nothing
	UNUSED(deltaSeconds);
}

//-----------------------------------------------------------------------------------------------------------

//upkeep
void ProgressBarWidget::Update(double deltaSeconds) {

// 	float myPercent;
// 	WidgetStateProperties& currentStateProperties = GetCurrentStateProperties();
// 	currentStateProperties.GetProperty("percent", myPercent);
// 	float currentPercent = myPercent;

	BaseWidget::Update(deltaSeconds);

}

//render
void ProgressBarWidget::Render(OpenGLRenderer* renderer) {
	if (m_isHidden)
		return;

	if (m_fillRenderInFront) {
		BaseWidget::Render(renderer);

		m_FillRenderComponent.Render2D(renderer);
	}
	else {
		m_FillRenderComponent.Render2D(renderer);

		BaseWidget::Render(renderer);
	}


}

///----------------------------------------------------------------------------------------------------------
///progress helpers

void ProgressBarWidget::ScaleFillBoundsXByPercent(AABB2& outVal) {

	WidgetStateProperties& currentStateProperties = GetCurrentStateProperties();

	Vector2 myDimensions;
	currentStateProperties.GetProperty("dimensions", myDimensions);

	float myPercent;
	currentStateProperties.GetProperty("percent", myPercent);
	
	Vector2 adjustedFillBoundsMaxPos = outVal.GetPointAtNormalizedPositionWithinBox(Vector2(myPercent, 1.0f));

	outVal.maxs = adjustedFillBoundsMaxPos;
}

//-----------------------------------------------------------------------------------------------------------

void ProgressBarWidget::ScaleFillBoundsYByPercent(float& outVal) {

	WidgetStateProperties& currentStateProperties = GetCurrentStateProperties();

	Vector2 myDimensions;
	currentStateProperties.GetProperty("dimensions", myDimensions);

	Vector2 myOffset;
	currentStateProperties.GetProperty("offset", myOffset);

	float myPercent;
	currentStateProperties.GetProperty("percent", myPercent);

	outVal = myOffset.y + (myDimensions.y * myPercent);

}


//===========================================================================================================

