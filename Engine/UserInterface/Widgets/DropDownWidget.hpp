//==============================================================================================================
//DropDownWidget.hpp
//by Albert Chen May-7-2016.
//==============================================================================================================

#pragma once

#ifndef _included_DropDownWidget__
#define _included_DropDownWidget__

//===========================================================================================================

#include "GroupWidget.hpp"

//===========================================================================================================

class DropDownWidget : public GroupWidget {
public:
	//constructors
	DropDownWidget();
	DropDownWidget(const std::string& name);
	DropDownWidget(const XMLNode& node);
	~DropDownWidget();

	//virtual overrides
	virtual void StartUp() override;
	virtual void CreateAllChildren() override;

	virtual void SetDefaultPropertiesFromXML(const XMLNode& node) override;
	virtual void LoadBaseAttributesFromXML(const XMLNode& node) override;

	virtual void ProcessInput(double deltaSeconds) override;
	virtual void UpdateRenderComponent(double deltaSeconds) override;

	//upkeep
	virtual void Update(double deltaSeconds);

	//render
	virtual void Render(OpenGLRenderer* renderer);

	//children helpers
	void ShowDropDownOptions();
	void HideDropDownOptions();

	//self register
	static BaseWidget* CreateWidget(const std::string& name) { return new DropDownWidget(name); }
	static BaseWidget* CreateWidgetXML(const XMLNode& node) { return new DropDownWidget(node); }
	static WidgetRegistration s_DropDownWidgetRegistration;

protected:

	BaseWidget* m_currentOption = NULL;


};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods


//===========================================================================================================

#endif //__includedDropDownWidget__

