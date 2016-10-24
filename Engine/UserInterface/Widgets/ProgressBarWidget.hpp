//==============================================================================================================
//ProgressBarWidget.hpp
//by Albert Chen Apr-24-2016.
//==============================================================================================================

#pragma once

#ifndef _included_ProgressBarWidget__
#define _included_ProgressBarWidget__

#include "BaseWidget.hpp"
#include "..\..\Components\HealthComponent.hpp"

//===========================================================================================================

enum FillAlignment {
	FILL_ALIGNMENT_LEFT_TO_RIGHT,
	FILL_ALIGNMENT_CENTER_OUT,
	NUM_FILL_ALIGNMENTS
};

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

	void SetRenderComponent();
	void UpdateRenderComponent(double deltaSeconds);

	virtual void ProcessInput(double deltaSeconds);

	//upkeep
	virtual void Update(double deltaSeconds);

	//render
	virtual void Render(OpenGLRenderer* renderer);

	//progress helpers
	void ScaleFillBoundsXByPercent(AABB2& outVal);
	void ScaleFillBoundsYByPercent(float& outVal);

	void SetPercent(const float& newPercent ) {
		m_widgetDefaultProperties.Set("percent", newPercent);
	}

	void SetPercent(const size_t& minVal, const size_t& maxVal) {
		float outPercent = (float)minVal / (float)maxVal;
		m_widgetDefaultProperties.Set("percent", outPercent);
	}

	//self register
	static BaseWidget* CreateWidget(const std::string& name) { return new ProgressBarWidget(name); }
	static BaseWidget* CreateWidgetXML(const XMLNode& node) { return new ProgressBarWidget(node); }
	static WidgetRegistration s_ProgressBarWidgetRegistration;

protected:
	Render2DComponent m_FillRenderComponent;

	bool m_fillRenderInFront = true;
};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods


//===========================================================================================================

#endif //__includedProgressBarWidget__

