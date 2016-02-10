//==============================================================================================================
//LoadingGameState.cpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#include "LoadingGameState.hpp"
#include "Engine\Input\InputSystem.hpp"
#include "Engine\Renderer\Text\TextSystem.hpp"
#include "Engine\ParticleSystem\ParticleSystem.hpp"

GameStateRegistration LoadingGameState::s_LoadingGameStateRegistration("Loading", &LoadingGameState::CreateGameState);

//===========================================================================================================

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///state use methods


void LoadingGameState::Enter(){
	ConsolePrintf("\nEntering Loading State");

}

//-----------------------------------------------------------------------------------------------------------

void LoadingGameState::Exit(){
	ConsolePrintf("Exiting Loading State");
}

//-----------------------------------------------------------------------------------------------------------

void LoadingGameState::ProcessInput(double deltaSeconds){
	UNUSED(deltaSeconds);

	IF_INPUT_SYSTEM_EXISTS{
		if (INPUT_IS_KEY_RELEASED(KEY_ENTER) || INPUT_IS_KEY_RELEASED(KEY_SPACEBAR) ){

			SetCurrentGameState("MainMenu");
		}
	}
	
}

//-----------------------------------------------------------------------------------------------------------

void LoadingGameState::Update(double deltaSeconds){
	UNUSED(deltaSeconds);

}

//-----------------------------------------------------------------------------------------------------------

void LoadingGameState::Render(OpenGLRenderer* renderer){
	UNUSED(renderer);

	OUTPUT_COLOR_STRING_TO_SCREEN("===LOADING===\n", renderer->GetDisplayWidth() * 0.05f, renderer->GetDisplayHeight() * 0.9f, Rgba::WHITE);

}
