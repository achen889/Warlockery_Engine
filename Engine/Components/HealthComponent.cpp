//==============================================================================================================
//HealthComponent.cpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#include "HealthComponent.hpp"

ComponentRegistration HealthComponent::s_HealthComponentRegistration("Health", &HealthComponent::CreateComponent);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Destructible Component

HealthComponent::HealthComponent(const XMLNode& node): BaseComponent(node)
{
	int healthToSet = ReadXMLAttributeAsInt(node, "health", 100);

	SetHealth(healthToSet);
}

HealthComponent::HealthComponent(const std::string& name) : BaseComponent(name){

}

//-----------------------------------------------------------------------------------------------------------

void HealthComponent::OnCreate(){
	//can specify something on create here
}

//-----------------------------------------------------------------------------------------------------------

void HealthComponent::OnDestroy(){
	//can specify something on destroy here
}

//-----------------------------------------------------------------------------------------------------------

void HealthComponent::Update(double deltaSeconds){
	UNUSED(deltaSeconds);
	
	

	ClampHealth();

}

//===========================================================================================================


