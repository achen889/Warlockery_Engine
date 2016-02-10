//==============================================================================================================
//ColliderComponent.cpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#include "ColliderComponent.hpp"

ComponentRegistration ColliderComponent::s_ColliderComponentRegistration("Collider", &ColliderComponent::CreateComponent);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Collider Component

ColliderComponent::ColliderComponent(const XMLNode& node): BaseComponent(node){

}

ColliderComponent::ColliderComponent(const std::string& name) : BaseComponent(name){

}

//-----------------------------------------------------------------------------------------------------------

void ColliderComponent::OnCreate(){
	//can specify something on create here
}

//-----------------------------------------------------------------------------------------------------------

void ColliderComponent::OnDestroy(){
	//can specify something on destroy here
}

//-----------------------------------------------------------------------------------------------------------

void ColliderComponent::Update(double deltaSeconds){
	UNUSED(deltaSeconds);
	
	if (isActive && IsColliding()) {
		OnCollisionEventCallback();
	}

}

//-----------------------------------------------------------------------------------------------------------

bool ColliderComponent::IsColliding() {
	if (isActive) {
		return true;
	}
	else {
		return false;
	}
}

//-----------------------------------------------------------------------------------------------------------

void ColliderComponent::OnCollisionEventCallback(){
	//do something
}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================


