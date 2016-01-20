//==============================================================================================================
//PhysicsMotionComponent.hpp
//by Albert Chen Aug-25-2015.
//==============================================================================================================

#pragma once

#ifndef _included_PhysicsMotionComponent__
#define _included_PhysicsMotionComponent__

#include "BaseComponent.hpp"
#include "Engine\Math\MathUtils.hpp"
#include "Engine\Math\Vector3.hpp"
#include "Engine\Math\EulerAngles.hpp"

class PhysicsMotionComponent : public BaseComponent{
public:

	PhysicsMotionComponent(){

	}
	~PhysicsMotionComponent();

	//fwd euler integration
	const Vector3& UpdatePositionWithVelocity(double deltaSeconds);
	const Vector3& UpdateVelocityWithAcceleration(double deltaSeconds);
	const Vector3& UpdateAccelerationWithForces(double deltaSeconds);

	PhysicsMotionComponent(const std::string& name);
	PhysicsMotionComponent(const XMLNode& node);

	//accessors
	
	//upkeep
	
	static BaseComponent* CreateComponent(const std::string& name){ return new PhysicsMotionComponent(name); }

	static ComponentRegistration s_PhysicsMotionComponentRegistration;
	
	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Update(double deltaSeconds);
	
	//vars - for 2D ignore the z
	Vector3 m_position;
	Vector3 m_velocity;
	Vector3 m_acceleration;
	Vector3 m_forces;
	Vector3 m_impulse;

	float m_mass;

	EulerAngles m_orientationDegrees;

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline PhysicsMotionComponent::~PhysicsMotionComponent(){

}

//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------

inline const Vector3& PhysicsMotionComponent::UpdatePositionWithVelocity(double deltaSeconds){

	m_position += (m_velocity * (float)deltaSeconds);

	return m_position;
}

//-----------------------------------------------------------------------------------------------------------

inline const Vector3& PhysicsMotionComponent::UpdateVelocityWithAcceleration(double deltaSeconds){

	m_velocity += m_acceleration* (float)deltaSeconds;

	return m_velocity;
}

//-----------------------------------------------------------------------------------------------------------

inline const Vector3& PhysicsMotionComponent::UpdateAccelerationWithForces(double deltaSeconds){
	UNUSED(deltaSeconds);

	const float inverseMass = 1.0f / m_mass;
	m_acceleration = m_forces * inverseMass;

	return m_acceleration;
}
//-----------------------------------------------------------------------------------------------------------


//===========================================================================================================

#endif