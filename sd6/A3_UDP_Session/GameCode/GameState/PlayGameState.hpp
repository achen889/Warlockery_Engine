//==============================================================================================================
//PlayGameState.hpp
//by Albert Chen Aug-25-2015.
//==============================================================================================================

#pragma once

#ifndef _included_PlayGameState__
#define _included_PlayGameState__

#include "Engine/Core/BaseGameState.hpp"

class PlayGameState : public BaseGameState{
public:
	~PlayGameState();

	PlayGameState(const std::string& name) : BaseGameState(name)
	{
		
	}
	
	static BaseGameState* CreateGameState(const std::string& name){ return new PlayGameState(name); }

	static GameStateRegistration s_PlayGameStateRegistration;

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