//==============================================================================================================
//Collider2DComponent.MPp
//by Albert Chen Jan-26-2016.
//==============================================================================================================

#pragma once

#ifndef _included_Collider2DComponent__
#define _included_Collider2DComponent__

#include "Engine/Math/Math2D.hpp"
#include "ColliderComponent.hpp"

//===========================================================================================================

class Collider2DComponent : public ColliderComponent{
public:
	Collider2DComponent();

	virtual ~Collider2DComponent();

	Collider2DComponent(const std::string& name);

	Collider2DComponent(const XMLNode& node);
	
	static BaseComponent* CreateComponent(const std::string& name) { return new Collider2DComponent(name); }

	static ComponentRegistration s_Collider2DComponentRegistration;

	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Update(double deltaSeconds);

	//collision helpers
	virtual void SetOnCollisionEventCallback(const std::string& event_name, EventCallbackFunc* func, void* data = NULL);

	virtual bool IsColliding(ColliderComponent* otherCollider);

	virtual bool DoCollisionCheck(ColliderComponent* otherCollider);
	virtual void ExecuteOnCollisionEventCallback();

	virtual AABB2 GetBox2DCollider() { 

		//AABB2 boxCollider(m_position, m_radius.x, m_radius.y);
		//AABB2 boxCollider = AABB2::ZERO_TO_ONE;
		AABB2 boxCollider(Vector2::ZERO, m_radius.x, m_radius.y);
		//boxCollider.AddPadding(m_radius.x, m_radius.y);
		boxCollider.RotateDegrees(m_orientation);
		boxCollider.Translate(m_position);

		return boxCollider;
	}

	virtual Disc2 GetDisc2DCollider() { return Disc2(m_position, m_radius.x ); } //doesn't care about orientation
	
	virtual const Vector2 GetRadius() { return m_radius; }

	virtual void SetPosition(Vector2 pos) { m_position = pos; }
	virtual void SetRadius(Vector2 radius) { m_radius = radius; }
	virtual void SetOrientation(float orientation) { m_orientation = orientation; }

protected:
	//vars

	Vector2 m_position;
	Vector2 m_radius;//use x for most things
	float m_orientation = 0.0f;

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline Collider2DComponent::Collider2DComponent() {
	//do nothing
}

inline Collider2DComponent::~Collider2DComponent(){

}


//===========================================================================================================

#endif