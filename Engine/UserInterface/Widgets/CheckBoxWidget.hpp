//==============================================================================================================
//CheckBoxWidget.hpp
//by Albert Chen Apr-24-2016.
//==============================================================================================================

#pragma once

#ifndef _included_CheckBoxWidget__
#define _included_CheckBoxWidget__

#include "ButtonWidget.hpp"

//===========================================================================================================

class CheckBoxWidget : public ButtonWidget {
public:
	//constructors
	CheckBoxWidget();
	CheckBoxWidget(const std::string& name);
	CheckBoxWidget(const XMLNode& node);
	~CheckBoxWidget();

	//virtual overrides
	virtual void SetDefaultPropertiesFromXML(const XMLNode& node) override;
	virtual void LoadBaseAttributesFromXML(const XMLNode& node) override;

	virtual void ProcessInput(double deltaSeconds);

	//upkeep
	virtual void Update(double deltaSeconds);

	//render
	virtual void Render(OpenGLRenderer* renderer);

	//self register
	static BaseWidget* CreateWidget(const std::string& name) { return new CheckBoxWidget(name); }
	static BaseWidget* CreateWidgetXML(const XMLNode& node) { return new CheckBoxWidget(node); }
	static WidgetRegistration s_CheckBoxWidgetRegistration;

protected:

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods


//===========================================================================================================

#endif //__includedCheckBoxWidget__

