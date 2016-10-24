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
	
// 	if (isActive && IsColliding()) {
// 		ExecuteOnCollisionEventCallback();
// 	}

}

//-----------------------------------------------------------------------------------------------------------

void ColliderComponent::SetOnCollisionEventCallback(const std::string& event_name, EventCallbackFunc* func, void* data ){
	//sets my callback func
	if (!m_collEventCallback) {
		m_collEventCallback = new EventCallback(event_name, func, data);
	}
}

//-----------------------------------------------------------------------------------------------------------

bool ColliderComponent::IsColliding(ColliderComponent* otherCollider) {
	if (isActive) {
		return DoCollisionCheck(otherCollider);
	}
	else {
		return false;
	}
}

//-----------------------------------------------------------------------------------------------------------

bool ColliderComponent::DoCollisionCheck(ColliderComponent* otherCollider) {
	UNUSED(otherCollider);

	return true;
}

//-----------------------------------------------------------------------------------------------------------

void ColliderComponent::ExecuteOnCollisionEventCallback(){
	//do something
	if (m_collEventCallback) {
		m_collEventCallback->Execute();
	}
}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================


