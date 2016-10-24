//==============================================================================================================
//UICollider2DComponent.MPp
//by Albert Chen Jan-26-2016.
//==============================================================================================================

#pragma once

#ifndef _included_UICollider2DComponent__
#define _included_UICollider2DComponent__

#include "Engine/Math/Math2D.hpp"
#include "Collider2DComponent.hpp"

//===========================================================================================================

enum MouseButtonID;

//===========================================================================================================

class UICollider2DComponent : public Collider2DComponent{
public:
	UICollider2DComponent();

	~UICollider2DComponent();

	UICollider2DComponent(const std::string& name);

	UICollider2DComponent(const XMLNode& node);
	

	static BaseComponent* CreateComponent(const std::string& name) { return new UICollider2DComponent(name); }

	static ComponentRegistration s_UICollider2DComponentRegistration;

	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Update(double deltaSeconds);

	//UI collider helpers
	virtual bool OnHover();
	virtual bool OnHeld(MouseButtonID mouseButtonClicked);
	virtual bool OnClick(MouseButtonID mouseButtonClicked);

	virtual bool OnLeftClick();
	virtual bool OnRightClick();

protected:
	//vars

	Vector2 m_position;
	Vector2 m_radius;//use x for most things
	float m_orientation = 0.0f;

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline UICollider2DComponent::UICollider2DComponent() {
	//do nothing
}

inline UICollider2DComponent::~UICollider2DComponent(){

}


//===========================================================================================================

#endif