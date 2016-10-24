//==============================================================================================================
//EditLineWidget.hpp
//by Albert Chen May-3-2016.
//==============================================================================================================

#pragma once

#ifndef _included_EditLineWidget__
#define _included_EditLineWidget__

#include "CheckBoxWidget.hpp"

//===========================================================================================================

class EditLineWidget : public CheckBoxWidget{
public:
	//constructors
	EditLineWidget();
	EditLineWidget(const std::string& name);
	EditLineWidget(const XMLNode& node);
	~EditLineWidget();

	//virtual overrides
	virtual void SetDefaultPropertiesFromXML(const XMLNode& node) override;
	virtual void LoadBaseAttributesFromXML(const XMLNode& node) override;

	virtual void ProcessInput(double deltaSeconds);

	//upkeep
	virtual void Update(double deltaSeconds);

	//render
	virtual void Render(OpenGLRenderer* renderer);

	//self register
	static BaseWidget* CreateWidget(const std::string& name) { return new EditLineWidget(name); }
	static BaseWidget* CreateWidgetXML(const XMLNode& node) { return new EditLineWidget(node); }
	static WidgetRegistration s_EditLineWidgetRegistration;

protected:

	std::string m_textInputBuffer;

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods


//===========================================================================================================

#endif //__includedEditLineWidget__

