//==============================================================================================================
//CursorWidget.hpp
//by Albert Chen Apr-24-2016.
//==============================================================================================================

#pragma once

#ifndef _included_CursorWidget__
#define _included_CursorWidget__

#include "GroupWidget.hpp"

//===========================================================================================================

class CursorWidget : public GroupWidget {
public:
	//constructors
	CursorWidget();
	CursorWidget(const std::string& name);
	CursorWidget(const XMLNode& node);
	~CursorWidget();

	//virtual overrides
	virtual void SetDefaultPropertiesFromXML(const XMLNode& node) override;
	virtual void LoadBaseAttributesFromXML(const XMLNode& node) override;

	virtual void UpdateRenderComponent(double deltaSeconds) override;

	//upkeep
	virtual void Update(double deltaSeconds);

	//render
	virtual void Render(OpenGLRenderer* renderer);

	//self register
	static BaseWidget* CreateWidget(const std::string& name) { return new CursorWidget(name); }
	static BaseWidget* CreateWidgetXML(const XMLNode& node) { return new CursorWidget(node); }
	static WidgetRegistration s_CursorWidgetRegistration;

protected:

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods


//===========================================================================================================

#endif //__includedCursorWidget__

