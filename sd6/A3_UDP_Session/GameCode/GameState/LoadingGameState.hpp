//==============================================================================================================
//LoadingGameState.hpp
//by Albert Chen Aug-25-2015.
//==============================================================================================================

#pragma once

#ifndef _included_LoadingGameState__
#define _included_LoadingGameState__

#include "Engine/Core/BaseGameState.hpp"

class LoadingGameState : public BaseGameState{
public:
	~LoadingGameState();

	LoadingGameState(const std::string& name) : BaseGameState(name)
	{
		
	}
	
	static BaseGameState* CreateGameState(const std::string& name){ return new LoadingGameState(name); }

	static GameStateRegistration s_LoadingGameStateRegistration;

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