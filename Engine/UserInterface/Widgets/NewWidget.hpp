//==============================================================================================================
//ProgressBarWidget.hpp
//by Albert Chen Apr-24-2016.
//==============================================================================================================

#pragma once

#ifndef _included_ProgressBarWidget__
#define _included_ProgressBarWidget__

#include "BaseWidget.hpp"

//===========================================================================================================

class ProgressBarWidget : public BaseWidget {
public:
	//constructors
	ProgressBarWidget();
	ProgressBarWidget(const std::string& name);
	ProgressBarWidget(const XMLNode& node);
	~ProgressBarWidget();

	//virtual overrides
	virtual void SetDefaultPropertiesFromXML(const XMLNode& node) override;
	virtual void LoadBaseAttributesFromXML(const XMLNode& node) override;

	//upkeep
	virtual void Update(double deltaSeconds);

	//render
	virtual void Render(OpenGLRenderer* renderer);

	//self register
	static BaseWidget* CreateWidget(const std::string& name) { return new ProgressBarWidget(name); }
	static BaseWidget* CreateWidgetXML(const XMLNode& node) { return new ProgressBarWidget(node); }
	static WidgetRegistration s_ProgressBarWidgetRegistration;

protected:

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods


//===========================================================================================================

#endif //__includedProgressBarWidget__

