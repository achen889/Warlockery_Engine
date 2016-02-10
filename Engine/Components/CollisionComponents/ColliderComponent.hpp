//==============================================================================================================
//ColliderComponent.hpp
//by Albert Chen Jan-26-2016.
//==============================================================================================================

#pragma once

#ifndef _included_ColliderComponent__
#define _included_ColliderComponent__

#include "Engine/Components/BaseComponent.hpp"

//===========================================================================================================

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
	virtual bool IsColliding();

	virtual void OnCollisionEventCallback();
	
protected:
	//vars
	bool isActive = true;
	EventCallback* m_collEventCallback; //event to be called on coll
};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline ColliderComponent::~ColliderComponent(){

}


//===========================================================================================================

#endif