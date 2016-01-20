//==============================================================================================================
//Entity.hpp
//by Albert Chen Dec-26-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Entity__
#define _included_Entity__

#include "Engine/Core/Clock.hpp"
#include "Engine/Core/Utilities.hpp"
#include <set>

#include "Engine/Core/XML/XMLUtils.hpp"
#include "Engine/Components/RenderComponent.hpp"
#include "Engine/Components/DestructibleComponent.hpp"

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///globals

extern Clock* g_entitiesClock; //the parent clock for all entities

static size_t s_entityID = 0;

//===========================================================================================================

class Entity;

typedef std::vector<Entity*> Entities;
typedef std::vector<Entity*>::iterator EntityIterator;

typedef std::set<Entity*> EntitySet;
typedef std::set<Entity*>::iterator EntitySetIterator;

//===========================================================================================================

class Entity{

public:
	//methods

	//constructors
	Entity();
	Entity(const XMLNode& node);
	Entity(const std::string& name);
	~Entity();
	
	//construction helpers
	void InitializeClock();

	//accessors
	
	//getters
	std::string GetNameID();
	std::string GetName(){ return m_name; }
	size_t GetID(){ return m_id;  }

	//setters
	void SetName(const std::string& name);
	void SetID();

	//update
	virtual void Update();

	void UpdateModelTransformMatrix(double deltaSeconds);

	//rendering
	virtual void Render(OpenGLRenderer* renderer);
	
	//figure out what to do with these, consider other components
	Clock m_clock;
	ModelViewMatrix m_modelViewTransform;

	//ideally change to a vector of components that can be instantiated dynamically, possibly via xml like ai behaviors
	RenderComponent m_renderComponent;
	DestructibleComponent m_destructibleComponent;

	Vector3 m_worldPosition; //for 2D mode ignore the z

protected:
	size_t m_id; //unique ID for entity
	std::string m_name;
	Rgba m_color;

	int m_debugLevel = 0;

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline Entity::Entity(){
	SetID();

	//InitializeClock();
}

//-----------------------------------------------------------------------------------------------------------

inline Entity::~Entity(){
	//do nothing
// 	if (m_clock){
// 		delete m_clock;
// 		m_clock = NULL;
// 	}
}

//-----------------------------------------------------------------------------------------------------------

inline void Entity::InitializeClock(){
	m_clock = Clock(GetNameID(), g_entitiesClock);
}

//-----------------------------------------------------------------------------------------------------------

inline std::string Entity::GetNameID(){
	return m_name + " " + IntToString(m_id);
}

//-----------------------------------------------------------------------------------------------------------

inline void Entity::SetName(const std::string& name){
	//also create a string id entry for name
	int stringID = GetStringID(name);

	m_name = GetStringFromID(stringID);
}

//-----------------------------------------------------------------------------------------------------------

inline void Entity::SetID(){
	m_id = s_entityID;

	//increment id to preserve uniqueness
	s_entityID++;
}

//-----------------------------------------------------------------------------------------------------------


//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///global helpers

inline void InitializeEntitiesClock(){
	if (!g_entitiesClock){
		g_entitiesClock = new Clock("GlobalEntitiesClock");
		GetGameClock().AddChild(g_entitiesClock);
	}
}

//-----------------------------------------------------------------------------------------------------------

inline void ShutDownEntitiesClock(){
	if (g_entitiesClock){
		delete g_entitiesClock;
		g_entitiesClock = NULL;
	}
}


#endif //__includedEntity__

