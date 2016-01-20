//==============================================================================================================
//Entity.cpp
//by Albert Chen Dec-26-2015.
//==============================================================================================================

#include "Entity.hpp"

//===========================================================================================================

Clock* g_entitiesClock = NULL;

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///constructors

Entity::Entity(const std::string& name):
m_name(name)
{
	SetID();

	InitializeClock();
}

//-----------------------------------------------------------------------------------------------------------

Entity::Entity(const XMLNode& node){
	UNUSED(node);
	//do something with the node

	m_destructibleComponent = DestructibleComponent(node);
	m_renderComponent = RenderComponent(node);
}

//-----------------------------------------------------------------------------------------------------------

void Entity::Update(){
	//do nothing

	//fetch my clock deltaSeconds
	double deltaSeconds = m_clock.GetDeltaSeconds();
	
	m_renderComponent.Update(deltaSeconds);
	m_destructibleComponent.Update(deltaSeconds);

}

//-----------------------------------------------------------------------------------------------------------

void Entity::UpdateModelTransformMatrix(double deltaSeconds){
	UNUSED(deltaSeconds);

	m_modelViewTransform.SetTranslation3D(m_worldPosition);

}

//-----------------------------------------------------------------------------------------------------------

void Entity::Render(OpenGLRenderer* renderer){

	m_renderComponent.Render2D(renderer, &m_modelViewTransform);

}

//===========================================================================================================

