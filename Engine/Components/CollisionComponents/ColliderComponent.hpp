//==============================================================================================================
//ColliderComponent.MPp
//by Albert Chen Jan-26-2016.
//==============================================================================================================

#pragma once

#ifndef _included_ColliderComponent__
#define _included_ColliderComponent__

#include "Engine/Components/BaseComponent.hpp"

//===========================================================================================================

enum ColliderShape {
	COLLIDER_AABB2,
	COLLIDER_DISC2,
	NUM_COLLIDER_SHAPES
};

enum ColliderType {
	TYPE_2D,
	NUM_COLLIDER_TYPES
};

//===========================================================================================================

class ColliderComponent;

typedef std::vector<ColliderComponent*> ColliderComponents;
typedef std::vector<ColliderComponent*>::iterator ColliderComponentsIterator;

class ColliderComponent : public BaseComponent{
public:

	ColliderComponent(){

	}
	~ColliderComponent();

	ColliderComponent(const std::string& name);

	ColliderComponent(const XMLNode& node);
	
	static BaseComponent* CreateComponent(const std::string& name){ return new ColliderComponent(name); }

	static ComponentRegistration s_ColliderComponentRegistration;

	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Update(double deltaSeconds);

	//collision helpers
	virtual void SetOnCollisionEventCallback(const std::string& event_name, EventCallbackFunc* func, void* data = NULL);

	virtual bool IsColliding(ColliderComponent* otherCollider);

	virtual bool DoCollisionCheck(ColliderComponent* otherCollider);

	virtual void ExecuteOnCollisionEventCallback();

	ColliderShape GetColliderShape() { return m_shape; }
	void SetColliderShape(const ColliderShape& shape) { m_shape = shape; }
	ColliderType GetType() { return m_type;  }
	void SetType(const ColliderType& type) { m_type = type; }

protected:
	//vars
	ColliderType m_type;
	ColliderShape m_shape;
	
	bool isActive = true;
	EventCallback* m_collEventCallback = NULL; //event to be called on coll

};



//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline ColliderComponent::~ColliderComponent(){

}


//===========================================================================================================

#endif