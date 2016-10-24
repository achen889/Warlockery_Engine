//==============================================================================================================
//SliderWidget.hpp
//by Albert Chen May-8-2016.
//==============================================================================================================

#pragma once

#ifndef _included_SliderWidget__
#define _included_SliderWidget__

#include "ProgressBarWidget.hpp"

//===========================================================================================================

class ButtonWidget;

//===========================================================================================================

class SliderWidget : public ProgressBarWidget {
public:
	//constructors
	SliderWidget();
	SliderWidget(const std::string& name);
	SliderWidget(const XMLNode& node);
	~SliderWidget();

	//virtual overrides
	virtual void StartUp() override;

	virtual void SetDefaultPropertiesFromXML(const XMLNode& node) override;
	virtual void LoadBaseAttributesFromXML(const XMLNode& node) override;

	virtual void ProcessInput(double deltaSeconds) override;

	//upkeep
	virtual void Update(double deltaSeconds);

	//render
	virtual void Render(OpenGLRenderer* renderer);

	//helpers
	float SetSliderButtonXPosAlongProgressBar(const Vector2& mouseScreenPos);

	//self register
	static BaseWidget* CreateWidget(const std::string& name) { return new SliderWidget(name); }
	static BaseWidget* CreateWidgetXML(const XMLNode& node) { return new SliderWidget(node); }
	static WidgetRegistration s_SliderWidgetRegistration;

protected:

	ButtonWidget* m_sliderButton = NULL;

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods


//===========================================================================================================

#endif //__includedSliderWidget__

