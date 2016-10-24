//==============================================================================================================
//UIScreen.cpp
//by Albert Chen May-8-2016.
//==============================================================================================================

#include "UIScreen.hpp"

#include "UISystem.hpp"

#include "Engine/Core/BaseGameState.hpp"

//===========================================================================================================

UIScreen UIScreen::INVALID;

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///init

void UIScreen::LoadAllUIScreensFromXML(const XMLNode& node) {

	if (theUISystem) {

		Strings widgetNames;
		Strings widgetStates;

		int num = node.nChildNode("Widget");
		for (int i = 0; i < num; i++) {
			XMLNode tempNode = node.getChildNode("Widget", i);
			//get widget params
			std::string widgetName = ReadXMLAttributeAsString(tempNode, "name", "");
			std::string widgetState = ReadXMLAttributeAsString(tempNode, "state", "");
			Vector2 widgetOffset = ReadXMLAttributeAsVec2(tempNode, "offset", Vector2::ZERO);
			//add to proper UI screen
			theUISystem->AddWidgetToUIScreen(widgetState, widgetName, widgetOffset);
		}

	}//end of does UI system exist


}

///----------------------------------------------------------------------------------------------------------
///upkeep

void UIScreen::Update() {
	double deltaSeconds = GetUIDeltaSeconds();

	for (BaseWidget* widget : m_widgetInstances) {
		if(widget)
			widget->Update(deltaSeconds);
	}
}

//-----------------------------------------------------------------------------------------------------------

///----------------------------------------------------------------------------------------------------------
///render

void UIScreen::Render(OpenGLRenderer* renderer) {

	if (m_widgetInstances.empty())
		return;

	for (BaseWidget* widget : m_widgetInstances) {
		widget->Render(renderer);
	}

}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///globals

UIScreenMap g_UIScreenMap;

///----------------------------------------------------------------------------------------------------------
///global helpers

void InitUIScreensFromGameStateRegistry() {
	GameStateRegistry* gameStateRegistry = GameStateRegistration::GetGameStateRegistry();
	if (gameStateRegistry) {
		for (GameStateRegistryEntry gameStateRegEntry : *gameStateRegistry) {
			GameStateRegistration* gameStateReg = gameStateRegEntry.second;
			if (gameStateReg) {
				std::string gameStateName = gameStateReg->GetName();
				g_UIScreenMap.insert(UIScreenMapEntry(gameStateName, UIScreen(gameStateName)));
			}
		}//end of for
	}
	//ConsolePrintf("We have UI Screens");
}

//-----------------------------------------------------------------------------------------------------------

void ClearUIScreens() {

	g_UIScreenMap.clear();

}

//-----------------------------------------------------------------------------------------------------------

UIScreen& FindUIScreenByName(const std::string& name) {
	UIScreenMapIterator found = g_UIScreenMap.find(name);
	if (found != g_UIScreenMap.end()) {
		return found->second;
	}
	return UIScreen::INVALID;
}

//===========================================================================================================