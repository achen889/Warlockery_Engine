//==============================================================================================================
//BaseGameState.cpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#include "BaseGameState.hpp"
#include "..\Renderer\OpenGLRenderer.hpp"

#include "Engine/UserInterface/UIScreen.hpp"

GameStateRegistry* GameStateRegistration::s_gameStateRegistryMap;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Base Game State Registration

///----------------------------------------------------------------------------------------------------------
///friend methods

GameStateRegistration* FindGameStateByName(const std::string& name){
	GameStateRegistryIterator myRegistration = GameStateRegistration::s_gameStateRegistryMap->find(name);

	if (myRegistration != GameStateRegistration::s_gameStateRegistryMap->end()){
		return (myRegistration->second);
	}

	return NULL;
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Base Game State

BaseGameState::BaseGameState(const std::string& name):
m_name(name)
{

}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///virtual methods

void BaseGameState::Enter(){
	
	UIScreen& gameStateUIScreen = FindUIScreenByName(m_name);
	gameStateUIScreen.ShowScreen();

}

//-----------------------------------------------------------------------------------------------------------

void BaseGameState::Exit(){
	UIScreen& gameStateUIScreen = FindUIScreenByName(m_name);
	gameStateUIScreen.HideScreen();
}

//-----------------------------------------------------------------------------------------------------------

void BaseGameState::ProcessInput(double deltaSeconds){
	UNUSED(deltaSeconds);
}

//-----------------------------------------------------------------------------------------------------------

void BaseGameState::Update(double deltaSeconds){
	UNUSED(deltaSeconds);

	UIScreen& gameStateUIScreen = FindUIScreenByName(m_name);
	gameStateUIScreen.Update();

}

//-----------------------------------------------------------------------------------------------------------

void BaseGameState::Render(OpenGLRenderer* renderer){

	//RenderUIScreen(renderer);

}

//-----------------------------------------------------------------------------------------------------------

void BaseGameState::RenderUIScreen(OpenGLRenderer* renderer) {

	UIScreen& gameStateUIScreen = FindUIScreenByName(m_name);
	gameStateUIScreen.Render(renderer);

}

//-----------------------------------------------------------------------------------------------------------

UIScreen* BaseGameState::GetUIScreen() {
	UIScreen* gameStateUIScreen = &FindUIScreenByName(m_name);

	return gameStateUIScreen;
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///global helper methods

void ProcessInputCurrentState(double deltaSeconds){
	//if (IsCurrentStateValid()){
	GetCurrentState()->ProcessInput(deltaSeconds);
	//}
}

void UpdateCurrentState(double deltaSeconds ){
	//if (IsCurrentStateValid()){
		GetCurrentState()->Update(deltaSeconds);
	//}
}

void RenderCurrentState(OpenGLRenderer* renderer ){
	//PROFILE_SECTION();
	//if (IsCurrentStateValid()){
		GetCurrentState()->Render(renderer);
	//}
}

void SetCurrentGameState(const std::string& stateName){
	GameStateRegistration* tempGameState = FindGameStateByName(stateName);
	//exit current state
	if (IsCurrentStateValid())
		GetCurrentState()->Exit();
	//push new state on
	g_currentState.push(tempGameState->CreateGameState());
	//enter new state
	GetCurrentState()->Enter();
}

void ReturnToPreviousState() {
	if (IsCurrentStateValid())
		GetCurrentState()->Exit();
	//pop state off
	g_currentState.pop();
	//enter old state
	GetCurrentState()->Enter();
}

//===========================================================================================================\