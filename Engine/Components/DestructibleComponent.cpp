//==============================================================================================================
//DestructibleComponent.cpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#include "DestructibleComponent.hpp"

ComponentRegistration DestructibleComponent::s_DestructibleComponentRegistration("Destructible", &DestructibleComponent::CreateComponent);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Destructible Component

DestructibleComponent::DestructibleComponent(const XMLNode& node): BaseComponent(node)
{
	int healthToSet = ReadXMLAttributeAsInt(node, "health", 100);

	SetHealth(healthToSet);
}

DestructibleComponent::DestructibleComponent(const std::string& name) : BaseComponent(name){

}

//-----------------------------------------------------------------------------------------------------------

void DestructibleComponent::OnCreate(){
	//can specify something on create here
}

//-----------------------------------------------------------------------------------------------------------

void DestructibleComponent::OnDestroy(){
	//can specify something on destroy here
}

//-----------------------------------------------------------------------------------------------------------

void DestructibleComponent::Update(double deltaSeconds){
	UNUSED(deltaSeconds);

	ClampHealth();

}

//===========================================================================================================


