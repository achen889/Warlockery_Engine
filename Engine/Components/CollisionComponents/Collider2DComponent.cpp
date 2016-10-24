//==============================================================================================================
//Collider2DComponent.cpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#include "Collider2DComponent.hpp"

ComponentRegistration Collider2DComponent::s_Collider2DComponentRegistration("Collider", &Collider2DComponent::CreateComponent);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Collider Component

Collider2DComponent::Collider2DComponent(const XMLNode& node): ColliderComponent(node){

}

Collider2DComponent::Collider2DComponent(const std::string& name) : ColliderComponent(name){

}

//-----------------------------------------------------------------------------------------------------------

void Collider2DComponent::OnCreate(){
	//can specify something on create here
}

//-----------------------------------------------------------------------------------------------------------

void Collider2DComponent::OnDestroy(){
	//can specify something on destroy here
}

//-----------------------------------------------------------------------------------------------------------

void Collider2DComponent::Update(double deltaSeconds){
	UNUSED(deltaSeconds);
	
// 	if (isActive && IsColliding()) {
// 		ExecuteOnCollisionEventCallback();
// 	}

}

//-----------------------------------------------------------------------------------------------------------

void Collider2DComponent::SetOnCollisionEventCallback(const std::string& event_name, EventCallbackFunc* func, void* data ){
	//sets my callback func
	if (!m_collEventCallback) {
		m_collEventCallback = new EventCallback(event_name, func, data);
	}
}

//-----------------------------------------------------------------------------------------------------------

bool Collider2DComponent::IsColliding(ColliderComponent* otherCollider) {
	if (isActive) {
		return DoCollisionCheck(otherCollider);
	}
	return false;
}

//-----------------------------------------------------------------------------------------------------------

bool Collider2DComponent::DoCollisionCheck(ColliderComponent* otherCollider) {
	//PROFILE_SECTION();
	if (otherCollider->GetType() == TYPE_2D) {
		Collider2DComponent* otherCollider2D = (Collider2DComponent*)otherCollider;
		if (!otherCollider2D) return false;

		if (m_shape == COLLIDER_AABB2) {
			AABB2 myBox = GetBox2DCollider();

			//do box to box collision
			if (otherCollider2D->GetColliderShape() == COLLIDER_AABB2) {
				AABB2 otherBox = otherCollider2D->GetBox2DCollider();

				return DoAABBsOverlap(myBox, otherBox);

			}
			//do box to disc collision
			else if (otherCollider2D->GetColliderShape() == COLLIDER_DISC2) {
				Disc2 otherDisc = otherCollider2D->GetDisc2DCollider();

				return DoShapesOverlap(myBox, otherDisc);

			}

		}
		else if (m_shape == COLLIDER_DISC2) {
			Disc2 myDisc = GetDisc2DCollider();

			//do disc to box collision
			if (otherCollider2D->GetColliderShape() == COLLIDER_AABB2) {
				AABB2 otherBox = otherCollider2D->GetBox2DCollider();

				return DoShapesOverlap(myDisc, otherBox);

			}
			//do disc to disc collision
			else if (otherCollider2D->GetColliderShape() == COLLIDER_DISC2) {
				Disc2 otherDisc = otherCollider2D->GetDisc2DCollider();

				return DoDiscsOverlap(myDisc, otherDisc);

			}

		}
	}

	return false;
}

//-----------------------------------------------------------------------------------------------------------

void Collider2DComponent::ExecuteOnCollisionEventCallback(){
	//do something
	if (m_collEventCallback) {
		m_collEventCallback->Execute();
	}
}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================


