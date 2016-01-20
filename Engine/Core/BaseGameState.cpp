//==============================================================================================================
//BaseGameState.cpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#include "BaseGameState.hpp"
#include "..\Renderer\OpenGLRenderer.hpp"

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

}

//-----------------------------------------------------------------------------------------------------------

void BaseGameState::Exit(){

}

//-----------------------------------------------------------------------------------------------------------

void BaseGameState::ProcessInput(double deltaSeconds){
	UNUSED(deltaSeconds);
}

//-----------------------------------------------------------------------------------------------------------

void BaseGameState::Update(double deltaSeconds){
	UNUSED(deltaSeconds);

}

//-----------------------------------------------------------------------------------------------------------

void BaseGameState::Render(OpenGLRenderer* renderer){
	UNUSED(renderer);

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