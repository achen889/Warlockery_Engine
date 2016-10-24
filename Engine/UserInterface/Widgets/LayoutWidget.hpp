//==============================================================================================================
//LayoutWidget.hpp
//by Albert Chen Apr-24-2016.
//==============================================================================================================

#pragma once

#ifndef _included_LayoutWidget__
#define _included_LayoutWidget__

#include "GroupWidget.hpp"

//===========================================================================================================

class LayoutWidget : public GroupWidget {
public:
	//constructors
	LayoutWidget();
	LayoutWidget(const std::string& name);
	LayoutWidget(const XMLNode& node);
	virtual ~LayoutWidget();

	//virtual overrides
	virtual void StartUp() override;
	virtual void LoadBaseAttributesFromXML(const XMLNode& node) override;


	//upkeep
	virtual void Update(double deltaSeconds);

	//render
	virtual void Render(OpenGLRenderer* renderer);

	//helpers
	virtual void AddChild(BaseWidget* childWidget);

	//self register
	static BaseWidget* CreateWidget(const std::string& name) { return new LayoutWidget(name); }
	static BaseWidget* CreateWidgetXML(const XMLNode& node) { return new LayoutWidget(node); }
	static WidgetRegistration s_LayoutWidgetRegistration;

protected:

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods


inline void LayoutWidget::AddChild(BaseWidget* childWidget) {
	if (childWidget) {
		m_children.push_back(childWidget);
		m_children.back()->SetParent(this);
	}
}

//===========================================================================================================

#endif //__includedLayoutWidget__

