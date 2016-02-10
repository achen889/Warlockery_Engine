//==============================================================================================================
//MainMenuGameState.cpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#include "MainMenuGameState.hpp"
#include "Engine\ParticleSystem\ParticleSystem.hpp"
#include "Engine\Input\InputSystem.hpp"
#include "Engine\Renderer\Text\TextSystem.hpp"

GameStateRegistration MainMenuGameState::s_MainMenuGameStateRegistration("MainMenu", &MainMenuGameState::CreateGameState);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///state

void MainMenuGameState::Enter(){
	ConsolePrintf("Entering Main Menu State");
}

//-----------------------------------------------------------------------------------------------------------

void MainMenuGameState::Exit(){
	ConsolePrintf("Exiting Main Menu State");
}

//-----------------------------------------------------------------------------------------------------------

void MainMenuGameState::ProcessInput(double deltaSeconds){
	UNUSED(deltaSeconds);

	IF_INPUT_SYSTEM_EXISTS{
		if (INPUT_IS_KEY_RELEASED(KEY_ENTER) || INPUT_IS_KEY_RELEASED(KEY_SPACEBAR)){

			SetCurrentGameState("Play");
		}
	}

}

//-----------------------------------------------------------------------------------------------------------

void MainMenuGameState::Update(double deltaSeconds){
	UNUSED(deltaSeconds);

}

//-----------------------------------------------------------------------------------------------------------

void MainMenuGameState::Render(OpenGLRenderer* renderer){
	UNUSED(renderer);

	OUTPUT_COLOR_STRING_TO_SCREEN("===MAIN MENU===\nMain Menu stuff here", renderer->GetDisplayWidth() * 0.05f, renderer->GetDisplayHeight() * 0.9f, Rgba::WHITE);
}

//===========================================================================================================