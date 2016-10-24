//==============================================================================================================
//UIScreen.hpp
//by Albert Chen May-8-2016.
//==============================================================================================================

#pragma once

#ifndef _included_UIScreen__
#define _included_UIScreen__

#include "Widgets/BaseWidget.hpp"

//===========================================================================================================

class OpenGLRenderer;

//===========================================================================================================

struct UIScreen {
	std::string m_name = "";
	Widgets m_widgetInstances;

	static UIScreen INVALID;

	UIScreen() {
		//do nothing
	}
	UIScreen(const std::string& name) :
		m_name(name)
	{
		//give the invalid one a name
		if (UIScreen::INVALID.m_name == "") {
			UIScreen::INVALID.m_name = "Invalid";
		}
	}
	~UIScreen() {
		for (Widgets::iterator it = m_widgetInstances.begin(); it != m_widgetInstances.end();) {
			BaseWidget* widget = (*it);
			if (widget) {
				delete widget;
				widget = NULL;
			}
			it = m_widgetInstances.erase(it);
		}
	}

	//init
	static void LoadAllUIScreensFromXML(const XMLNode& node);

	//mutators
	void HideScreen() {
		for (BaseWidget* widget : m_widgetInstances) {
			widget->HideWidget();
		}
	}
	void ShowScreen() {
		for (BaseWidget* widget : m_widgetInstances) {
			widget->ShowWidget();
		}
	}

	BaseWidget* GetWidget(const std::string& widgetName) {
		for (BaseWidget* widget : m_widgetInstances) {
			if (widget && widget->GetName() == widgetName) {
				return widget;
			}
		}
		return NULL;
	}

	void HideWidget(const std::string& widgetName) {
		for (BaseWidget* widget : m_widgetInstances) {
			if (widget && widget->GetName() == widgetName) {
				widget->HideWidget();
				break;
			}
		}
	}
	void ShowWidget(const std::string& widgetName) {
		for (BaseWidget* widget : m_widgetInstances) {
			if (widget && widget->GetName() == widgetName) {
				widget->ShowWidget();
				break;
			}
		}
	}
	
	void Update();

	void Render(OpenGLRenderer* renderer);

};

typedef std::map<std::string, UIScreen> UIScreenMap;
typedef std::map<std::string, UIScreen>::iterator UIScreenMapIterator;
typedef std::pair<std::string, UIScreen> UIScreenMapEntry;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///globals

extern std::map<std::string, UIScreen> g_UIScreenMap;

///----------------------------------------------------------------------------------------------------------
///global helpers

void InitUIScreensFromGameStateRegistry();

void ClearUIScreens();

UIScreen& FindUIScreenByName(const std::string& name);

//===========================================================================================================

#endif //__includedUIScreen__

