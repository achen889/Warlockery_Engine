//==============================================================================================================
//BaseGameState.hpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#pragma once

#ifndef _included_BaseGameState__
#define _included_BaseGameState__

#include <map>
#include "Engine/Core/Utilities.hpp"
#include <stack>

class GameStateRegistration;
class BaseGameState;

typedef std::map<std::string, GameStateRegistration*> GameStateRegistry;
typedef std::map<std::string, GameStateRegistration*>::iterator GameStateRegistryIterator;
typedef BaseGameState* (GameStateCreationFunc)(const std::string& name );

//===========================================================================================================
//self-registering class
class GameStateRegistration{
	public:
		
		~GameStateRegistration(){
			//do nothing
		}

		GameStateRegistration(const std::string& name,  GameStateCreationFunc* creationFunction );

		//registry helpers
		const std::string& GetName() const { return m_name;  }
		static GameStateRegistry* GetGameStateRegistry(){ return s_gameStateRegistryMap;  }
		BaseGameState* CreateGameState(){ return (*m_creationFunc)(m_name);  }

		//friend methods
		friend GameStateRegistration* FindGameStateByName(const std::string& name);
		
		
		
		
private:
		//vars
		GameStateCreationFunc* m_creationFunc;
		std::string m_name;
		static GameStateRegistry* s_gameStateRegistryMap;
		
};



///----------------------------------------------------------------------------------------------------------
///inline methods

//means of registering stuff to the registry
//name must be lowercase
inline GameStateRegistration::GameStateRegistration(const std::string& name, GameStateCreationFunc* creationFunction):
m_name(name)
//m_creationFunc(creationFunction)
{
	m_creationFunc = creationFunction;
	//create a new registry
	if (!s_gameStateRegistryMap){
		s_gameStateRegistryMap = new GameStateRegistry();
	}
	//insert into that registry
	s_gameStateRegistryMap->insert(std::pair<std::string, GameStateRegistration*>(name, this));
}

//===========================================================================================================

class OpenGLRenderer;

class BaseGameState{
public:

	BaseGameState(){
		//do nothing
	}
	~BaseGameState(){
		//do nothing
	}

	BaseGameState(const std::string& name);

	//virtual methods
	virtual void Enter();
	virtual void Exit();

	virtual void ProcessInput(double deltaSeconds);
	virtual void Update(double deltaSeconds);
	virtual void Render(OpenGLRenderer* renderer);

	std::string GetName(){
		return m_name;
	}
	
	//vars
	std::string m_name;

};

typedef std::stack<BaseGameState*> GameStateStack;
static GameStateStack g_currentState;

///----------------------------------------------------------------------------------------------------------
///global helper methods

BaseGameState* GetCurrentState();

void ReturnToPreviousState(); //do this next

bool IsCurrentStateValid(); //still has issues

void ProcessInputCurrentState(double deltaSeconds);

void UpdateCurrentState(double deltaSeconds);

void RenderCurrentState(OpenGLRenderer* renderer);

void SetCurrentGameState(const std::string& stateName);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline helpers

inline BaseGameState* GetCurrentState(){

	return g_currentState.top();

}

//-----------------------------------------------------------------------------------------------------------

inline bool IsCurrentStateValid(){

	if ( g_currentState.size() != 0){
		//if (g_currentState.top()){
			return true;
		//}
		//return false;
	}
	return false;
}

//===========================================================================================================

#endif