//==============================================================================================================
//ClockComponent.hpp
//by Albert Chen Jan-26-2016.
//==============================================================================================================

#pragma once

#ifndef _included_ClockComponent__
#define _included_ClockComponent__

#include "BaseComponent.hpp"
#include "Engine/Core/Clock.hpp"

//===========================================================================================================

class ClockComponent : public BaseComponent{
public:

	ClockComponent();
	~ClockComponent();

	ClockComponent(const std::string& name);

	ClockComponent(const XMLNode& node);
	
	static BaseComponent* CreateComponent(const std::string& name){ return new ClockComponent(name); }

	static ComponentRegistration s_ClockComponentRegistration;

	void AddChild(ClockComponent* child );

	Clock& GetClock() { return m_clock;  }

	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Update(double deltaSeconds);
	
protected:
	//vars
	Clock m_clock;

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods
inline ClockComponent::ClockComponent() {
	//add this guy to the game clock if no parent is specified
	GetGameClock().AddChild(&this->m_clock);
}

inline ClockComponent::~ClockComponent(){

}

//-----------------------------------------------------------------------------------------------------------

inline void ClockComponent::AddChild(ClockComponent* child) {
	//add the other clock component as a child of my clock
	m_clock.AddChild(&child->m_clock);
}



//===========================================================================================================

#endif