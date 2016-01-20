//==============================================================================================================
//DestructibleComponent.hpp
//by Albert Chen Aug-25-2015.
//==============================================================================================================

#pragma once

#ifndef _included_DestructibleComponent__
#define _included_DestructibleComponent__

#include "BaseComponent.hpp"
#include "Engine\Math\MathUtils.hpp"

class DestructibleComponent : public BaseComponent{
public:

	DestructibleComponent(){

	}
	~DestructibleComponent();

	DestructibleComponent(const std::string& name);

	DestructibleComponent(const XMLNode& node);
	//accessors
	void SetHealth(const int& maxHealth, const int& startingHealth = 0);
	bool IsDamaged();
	bool IsDestroyed();
	float GetHealthPercent(){ return (float)m_currentHealth / (float)m_maxHealth; }
	//upkeep
	void ClampHealth(){ m_currentHealth = ClampInt(m_currentHealth, 0, m_maxHealth); }
	
	static BaseComponent* CreateComponent(const std::string& name){ return new DestructibleComponent(name); }

	static ComponentRegistration s_DestructibleComponentRegistration;
	
	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Update(double deltaSeconds);
	
private:
	//vars
	int m_currentHealth;
	int m_maxHealth;

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline DestructibleComponent::~DestructibleComponent(){

}

//-----------------------------------------------------------------------------------------------------------

inline void DestructibleComponent::SetHealth(const int& maxHealth, const int& startingHealth ){
	m_maxHealth = maxHealth;
	if (startingHealth != 0){
		m_currentHealth = startingHealth;
	}
	else{
		m_currentHealth = m_maxHealth;
	}
}

///----------------------------------------------------------------------------------------------------------
///queries

inline bool DestructibleComponent::IsDamaged(){ return (m_currentHealth < m_maxHealth); }

inline bool DestructibleComponent::IsDestroyed(){ return (m_currentHealth <= 0); }


//===========================================================================================================

#endif