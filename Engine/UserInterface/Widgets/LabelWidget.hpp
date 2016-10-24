//==============================================================================================================
//LabelWidget.hpp
//by Albert Chen Apr-24-2016.
//==============================================================================================================

#pragma once

#ifndef _included_LabelWidget__
#define _included_LabelWidget__

#include "BaseWidget.hpp"

//===========================================================================================================

class LabelWidget : public BaseWidget {
public:
	//constructors
	LabelWidget();
	LabelWidget(const std::string& name);
	LabelWidget(const XMLNode& node);
	~LabelWidget();


	//virtual override
	virtual void SetDefaultPropertiesFromXML(const XMLNode& node) override;
	virtual void LoadBaseAttributesFromXML(const XMLNode& node) override;

	//upkeep
	virtual void Update(double deltaSeconds);

	//render
	virtual void Render(OpenGLRenderer* renderer);

	//helpers
	virtual void SetText(const std::string& text);
	
	//self register
	static BaseWidget* CreateWidget(const std::string& name) { return new LabelWidget(name); }
	static BaseWidget* CreateWidgetXML(const XMLNode& node) { return new LabelWidget(node); }
	static WidgetRegistration s_LabelWidgetRegistration;

protected:
	//vars
	UIAnchorID m_textAnchor = UI_ANCHOR_CENTER;
	Vector2 m_textOffset;

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

//===========================================================================================================

#endif //__includedLabelWidget__

