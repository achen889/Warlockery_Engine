//==============================================================================================================
//BaseWidget.hpp
//by Albert Chen Apr-18-2016.
//==============================================================================================================

#pragma once

#ifndef _included_BaseWidget__
#define _included_BaseWidget__

#include "Engine/Components/CollisionComponents/UICollider2DComponent.hpp"
#include "Engine/Components/RenderComponents/Render2DComponent.hpp"
#include "Engine/Core/Event/NamedProperties.hpp"
#include "../WidgetStates/WidgetStateProperties.hpp"

//===========================================================================================================

/*
properties
offset (Vec2) – positional displacement from Parent widget
dimensions(Vec2) – 2D widget dimensions
bgColor (Rgba) – fill color (texture tint) for widget area
edgeColor (Rgba) – edge (line) color for widget AABB2 perimeter
opacity (float) – multiplied by bgColor.alpha and edgeColor.alpha, to fade widget
*/

class BaseWidget2D : public EventSubscriberObject {
public:
	BaseWidget2D();
	virtual ~BaseWidget2D();

	//accessors
	const BaseWidget2D* GetParent() { return m_parent; }
	void SetParent(BaseWidget2D* parent) { m_parent = parent; }

	void SetState(WidgetStateID newState ){
		//m_nextState = newState;
		//do a thing

		m_prevState = m_currentState;
		m_currentState = newState;
	}

	WidgetStateProperties& GetCurrentStateProperties() {
		return m_properties[m_currentState];
	}

	void SetRenderComponent();
	void SetUICollider2DComponent();

	//upkeep
	virtual void ProcessInput(double deltaSeconds);

	virtual void Update(double deltaSeconds);

	//render
	virtual void Render(OpenGLRenderer* renderer);

protected:

	//vars
	WidgetStateProperties m_properties[NUM_WIDGET_STATES];
	//state transition
	WidgetStateID m_currentState;
	WidgetStateID m_prevState;
	WidgetStateID m_nextState;
	//components
	Render2DComponent m_renderComponent;
	UICollider2DComponent m_UIColliderComponent; //TODO: write a UIColliderComp...?

	BaseWidget2D* m_parent;

};



//===========================================================================================================

#endif //__includedBaseWidget__

