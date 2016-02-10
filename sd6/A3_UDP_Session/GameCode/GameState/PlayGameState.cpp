//==============================================================================================================
//PlayGameState.cpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#include "PlayGameState.hpp"
#include "Engine\Input\InputSystem.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine\Renderer\Text\TextSystem.hpp"

GameStateRegistration PlayGameState::s_PlayGameStateRegistration("Play", &PlayGameState::CreateGameState);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///state

void PlayGameState::Enter(){
	ConsolePrintf("Entering Play State");
	
}

//-----------------------------------------------------------------------------------------------------------

void PlayGameState::Exit(){
	ConsolePrintf("Exit Play State");
}

//-----------------------------------------------------------------------------------------------------------

void PlayGameState::ProcessInput(double deltaSeconds){
	UNUSED(deltaSeconds);

	IF_INPUT_SYSTEM_EXISTS{
		if (INPUT_IS_KEY_RELEASED(KEY_BACKSPACE) ){

			SetCurrentGameState("MainMenu");
		}
	}
}

//-----------------------------------------------------------------------------------------------------------

void PlayGameState::Update(double deltaSeconds){
	UNUSED(deltaSeconds);

}

//-----------------------------------------------------------------------------------------------------------

void PlayGameState::Render(OpenGLRenderer* renderer){
	UNUSED(renderer);

	OUTPUT_COLOR_STRING_TO_SCREEN("===PLAY GAME===\nPlay game stuff here", renderer->GetDisplayWidth() * 0.05f, renderer->GetDisplayHeight() * 0.9f, Rgba::WHITE);
}

//===========================================================================================================