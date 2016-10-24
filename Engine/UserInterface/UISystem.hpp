//==============================================================================================================
//UserInterfaceSystem.hpp
//by Albert Chen Apr-20-2016.
//==============================================================================================================

#pragma once

#ifndef _included_UserInterfaceSystem__
#define _included_UserInterfaceSystem__

#include "Engine/Core/Event/NamedProperties.hpp"

#include "Engine/Core/Clock.hpp"
//#include "Widgets/WidgetFactory.hpp"
#include "Widgets/WidgetTypes.hpp"

#include "UIScreen.hpp"
#include "Widgets/BaseWidget.hpp"
#include "Engine/Core/ObjectFactory.hpp"

//===========================================================================================================

class UISystem {
public:
	UISystem();
	~UISystem();

	//methods
	void StartUp();

	void ShutDown();

	//start up helpers
	void LoadAllColorSchemeFiles();
	void SetDefaultPropertiesUsingTempDefaultWidget();
	void SetDefaultNamedProperties();
	void SetDefaultStateProperties();
	const NamedProperties& GetDefaultStateProperties(const WidgetStateID& stateID) {
		return m_defaultStateProperties[stateID];
	}
	const NamedProperties& GetDefaultNamedProperties() {
		return m_defaultProperties;
	}

	//upkeep
	void Update();
	//render
	void Render(OpenGLRenderer* renderer);

	//helper
	void SetWidgetStateByName(const std::string& widgetName, const WidgetStateID& stateID);

	BaseWidget* AddWidgetToUIScreen(const std::string& UIScreenName, const std::string& widgetName, const Vector2& offset,
		const WidgetStateID& stateID = WIDGET_STATE_IDLE);

	BaseWidget* AddWidgetToUIScreen(const std::string& UIScreenName, BaseWidget* widget, const Vector2& offset, const WidgetStateID& stateID);
	BaseWidget* AddWidget(const std::string& widgetName, const Vector2& offset,
		const WidgetStateID& stateID = WIDGET_STATE_IDLE);
	
	BaseWidget* CreateWidget(const std::string& widgetName, const Vector2& offset, const WidgetStateID& stateID = WIDGET_STATE_IDLE, bool doStartUp = true);
	BaseWidget* CreateWidget(NamedProperties& creationArgs );
	
	template<typename T_WIDGET_TYPE>
	BaseWidget* CreateWidget(ObjectFactory<BaseWidget>* myWidgetFactory, const Vector2& offset, 
		const WidgetStateID& stateID, bool doStartUp);

	Rgba GetColorFromColorScheme(const std::string& schemeName, const std::string& colorName) {
		return m_colorSchemeMap[schemeName].colors[colorName];
	}

	void LoadColorSchemeToMapFromXML(const XMLNode& node);
	
private:
	NamedProperties m_defaultProperties;

	NamedProperties m_defaultStateProperties[NUM_WIDGET_STATES];

	ColorSchemeMap m_colorSchemeMap;

	Widgets m_widgetInstances;
	
	static ObjectFactory<BaseWidget>* s_defaultWidgetFactory;
};

extern UISystem* theUISystem;

///----------------------------------------------------------------------------------------------------------
///inline methods

template<typename T_WIDGET_TYPE>
inline BaseWidget* UISystem::CreateWidget(
	ObjectFactory<BaseWidget>* myWidgetFactory, const Vector2& offset, const WidgetStateID& stateID, bool doStartUp) {

	BaseWidget* myNewWidget = NULL;

	//if widget factory exists create one
	if (myWidgetFactory) {
		myNewWidget = myWidgetFactory->SpawnAsType<T_WIDGET_TYPE>();
	}
	//set starting stuff
	if (myNewWidget) {
		myNewWidget->SetState(stateID);
		myNewWidget->SetOffset(offset);
		if (doStartUp) {
			myNewWidget->StartUp();
		}
	}

	return myNewWidget;
}

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///global helpers

BaseWidget* AllocWidget(const XMLNode& node);

///----------------------------------------------------------------------------------------------------------
///inline globals


inline BaseWidget* AllocWidget(const XMLNode& node) {
	std::string widgetType = ReadXMLAttributeAsString(node, "type", "");
	WidgetRegistration* registration = FindWidgetByName(widgetType);
	if (registration) {
		return registration->CreateWidget(node);
	}
	return NULL;
}


//===========================================================================================================

#endif //__includedUserInterfaceSystem__

