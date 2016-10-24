//==============================================================================================================
//LabelWidget.cpp
//by Albert Chen Apr-24-2016.
//==============================================================================================================

#include "LabelWidget.hpp"
#include "..\UISystem.hpp"
#include "..\..\Renderer\Text\TextSystem.hpp"

//===========================================================================================================

WidgetRegistration LabelWidget::s_LabelWidgetRegistration("Label", &LabelWidget::CreateWidget, &LabelWidget::CreateWidgetXML);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors

LabelWidget::LabelWidget(){
	//do nothing

}

LabelWidget::LabelWidget(const std::string& name) : BaseWidget(name){
	
}

LabelWidget::LabelWidget(const XMLNode& node) : BaseWidget(node){
	
}

LabelWidget::~LabelWidget(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

///----------------------------------------------------------------------------------------------------------
///virtual overrides

void LabelWidget::LoadBaseAttributesFromXML(const XMLNode& node) {

	BaseWidget::LoadBaseAttributesFromXML(node);

	std::string buttonText = ReadXMLAttributeAsString(node, "text", "");
	m_widgetDefaultProperties.Set("text", buttonText);

	std::string textAlignStr = ReadXMLAttributeAsString(node, "text-align", "center");
	m_textAnchor = GetUIAnchorForString(textAlignStr);

	Vector2 textOffsetXML = ReadXMLAttributeAsVec2(node, "text-offset");
	m_textOffset = textOffsetXML;

// 	float newTextScale = ReadXMLAttributeAsFloat(node, "textScale", 1.0f);
// 	m_widgetDefaultProperties.Set("textScale", newTextScale);

}

//-----------------------------------------------------------------------------------------------------------

void LabelWidget::SetDefaultPropertiesFromXML(const XMLNode& node) {

	BaseWidget::SetDefaultPropertiesFromXML(node);

	//the default properties 
	float newTextScale = ReadXMLAttributeAsFloat(node, "textScale", 1.0f);
	m_widgetDefaultProperties.Set("textScale", newTextScale);

	std::string textColor = ReadXMLAttributeAsString(node, "textColor", "");
	Rgba buttonTextRgba = theUISystem->GetColorFromColorScheme(m_colorScheme, textColor);
	m_widgetDefaultProperties.Set("textColor", buttonTextRgba);

	float newTextOpacity = ReadXMLAttributeAsFloat(node, "textOpacity", 1.0f);
	m_widgetDefaultProperties.Set("textOpacity", newTextOpacity);


}

//-----------------------------------------------------------------------------------------------------------

//upkeep
void LabelWidget::Update(double deltaSeconds) {

	BaseWidget::Update(deltaSeconds);

}

//render
void LabelWidget::Render(OpenGLRenderer* renderer) {

	//base widget render
	BaseWidget::Render(renderer);

	if (m_isHidden)
		return;

	WidgetStateProperties& currentStateProperties = GetCurrentStateProperties();

	std::string textToRender;
	currentStateProperties.GetProperty("text", textToRender);

	Rgba textRenderColor;
	currentStateProperties.GetProperty("textColor", textRenderColor);

	Vector2 textRenderPt;// = GetPosition();
	textRenderPt = GetPositionForUIAnchorInAABB2(m_textAnchor, GetRenderBounds());
	textRenderPt += m_textOffset;

	float textRenderScale;
	currentStateProperties.GetProperty("textScale", textRenderScale);

	if (theTextSystem) {
		textRenderPt.x -= theTextSystem->CalcTextWidth(textToRender) * 0.5f;

		NamedProperties np;
		np.Set("text", textToRender);
		np.Set("textColor", textRenderColor);
		np.Set("textPosition", textRenderPt);
		np.Set("textScale", textRenderScale);
		FireEvent("RenderText", np);
		//OUTPUT_COLOR_STRING_TO_SCREEN(textToRender, textRenderPt.x, textRenderPt.y, textRenderColor);
	}


}

//-----------------------------------------------------------------------------------------------------------

///----------------------------------------------------------------------------------------------------------
///helpers

void LabelWidget::SetText(const std::string& text) {
	m_widgetDefaultProperties.Set("text", text);
}

//===========================================================================================================

