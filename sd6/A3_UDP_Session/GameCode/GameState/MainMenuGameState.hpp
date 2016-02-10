//==============================================================================================================
//MainMenuGameState.hpp
//by Albert Chen Aug-25-2015.
//==============================================================================================================

#pragma once

#ifndef _included_MainMenuGameState__
#define _included_MainMenuGameState__

#include "Engine/Core/BaseGameState.hpp"

class MainMenuGameState : public BaseGameState{
public:
	~MainMenuGameState();

	MainMenuGameState(const std::string& name) : BaseGameState(name)
	{
		
	}
	
	static BaseGameState* CreateGameState(const std::string& name){ return new MainMenuGameState(name); }

	static GameStateRegistration s_MainMenuGameStateRegistration;

	//overwrite
	void Enter();
	void Exit();

	void ProcessInput(double deltaSeconds);
	void Update(double deltaSeconds);
	void Render(OpenGLRenderer* renderer);

private:
	
};

///----------------------------------------------------------------------------------------------------------
///inline methods

#endif