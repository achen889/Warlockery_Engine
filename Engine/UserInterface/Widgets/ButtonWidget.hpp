//==============================================================================================================
//ButtonWidget.hpp
//by Albert Chen Apr-18-2016.
//==============================================================================================================

#pragma once

#ifndef _included_ButtonWidget__
#define _included_ButtonWidget__

#include "LabelWidget.hpp"

//===========================================================================================================

/*
textScale - multiplier for text cell height(defaults to fixed % of widget height)
textColor - color(tint) for text, including alpha
textOpacity - multiplied by TextColor.alpha AND Opacity.alpha(for fading widget)
eventName -  to fire when the button is clicked.
*/

//===========================================================================================================

class ButtonWidget : public LabelWidget {
public:
	//constructors
	ButtonWidget();
	ButtonWidget(const std::string& name);
	ButtonWidget(const XMLNode& node);
	~ButtonWidget();
	
	//start up
	virtual void LoadBaseAttributesFromXML(const XMLNode& node);
	virtual void SetDefaultPropertiesFromXML(const XMLNode& node);
	virtual void SetAltKey(int key) { m_altKey = key; }

	void ProcessInput(double deltaSeconds);

	void FireClickEvent(double deltaSeconds);

	//upkeep
	virtual void Update(double deltaSeconds);

	//render
	virtual void Render(OpenGLRenderer* renderer);

	bool IsAltKeyReleased();

	//self register
	static BaseWidget* CreateWidget(const std::string& name) { return new ButtonWidget(name); }
	static BaseWidget* CreateWidgetXML(const XMLNode& node) { return new ButtonWidget(node); }

	static WidgetRegistration s_ButtonWidgetRegistration;

protected:
	//vars
	int m_altKey = -1;//alt key to activate the button

};


//===========================================================================================================

#endif //__includedButtonWidget__

