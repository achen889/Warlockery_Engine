//==============================================================================================================
//HealthComponent.MPp
//by Albert Chen Aug-25-2015.
//==============================================================================================================

#pragma once

#ifndef _included_HealthComponent__
#define _included_HealthComponent__

#include "BaseComponent.hpp"
#include "Engine\Math\MathUtils.hpp"

class HealthComponent : public BaseComponent{
public:

	HealthComponent(){

	}
	~HealthComponent();

	HealthComponent(const std::string& name);

	HealthComponent(const XMLNode& node);
	//accessors
	int GetCurrentHealth() { return m_currentHealth; }
	int GetMaxHealth() { return m_maxHealth; }
	float GetHealthPercent() { return (float)m_currentHealth / (float)m_maxHealth; }

	void SetHealth(const int& maxHealth, const int& startingHealth = -1);
	bool IsDamaged();
	bool IsDestroyed();
	void ApplyDamage(const int& damageVal) { m_currentHealth -= damageVal; }

	//upkeep
	void ClampHealth(){ m_currentHealth = ClampInt(m_currentHealth, 0, m_maxHealth); }
	
	static BaseComponent* CreateComponent(const std::string& name){ return new HealthComponent(name); }

	static ComponentRegistration s_HealthComponentRegistration;
	
	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Update(double deltaSeconds);
	
protected:
	//vars
	int m_currentHealth;
	int m_maxHealth;

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline HealthComponent::~HealthComponent(){

}

//-----------------------------------------------------------------------------------------------------------

inline void HealthComponent::SetHealth(const int& maxHealth, const int& startingHealth ){
	m_maxHealth = maxHealth;
	if (startingHealth != -1){
		m_currentHealth = startingHealth;
	}
	else{
		m_currentHealth = m_maxHealth;
	}
}

///----------------------------------------------------------------------------------------------------------
///queries

inline bool HealthComponent::IsDamaged(){ return (m_currentHealth < m_maxHealth); }

inline bool HealthComponent::IsDestroyed(){ return (m_currentHealth <= 0); }


//===========================================================================================================

#endif