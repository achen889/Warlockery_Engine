//==============================================================================================================
//PhysicsMotionComponent.hpp
//by Albert Chen Aug-25-2015.
//==============================================================================================================

#pragma once

#ifndef _included_PhysicsMotionComponent__
#define _included_PhysicsMotionComponent__

#include "TransformComponent.hpp"

enum PhysicsIntegrationType{
	PHYSICS_FWD_EULER,
	PHYSICS_VERLET,
	NUM_PHYSICS_INTEGRATION_TYPES
};

class RigidBodyComponent : public TransformComponent{
public:

	RigidBodyComponent(){

	}
	~RigidBodyComponent();

	RigidBodyComponent(const std::string& name);
	RigidBodyComponent(const XMLNode& node);
	
	static BaseComponent* CreateComponent(const std::string& name){ return new RigidBodyComponent(name); }

	static ComponentRegistration s_PhysicsMotionComponentRegistration;
	
	virtual void OnCreate();
	virtual void OnDestroy();

	//upkeep
	virtual void Update(double deltaSeconds);
	
	//fwd euler integration
	void UpdateForwardEulerIntegration(double deltaSeconds);
	const Vector3& UpdatePositionWithVelocity(double deltaSeconds);
	const Vector3& UpdateVelocityWithAcceleration(double deltaSeconds);
	const Vector3& UpdateAccelerationWithForces();

	//verlet integration
	void UpdateVerletIntegration(double deltaSeconds);


	
	//vars - for 2D ignore the z
	Vector3 m_position;

	//integration vars
	PhysicsIntegrationType m_physicsIntegrationType = PHYSICS_FWD_EULER;
	Vector3 m_prevPrevPosition; //for verlet integration
	Vector3 m_velocity;
	Vector3 m_acceleration;
	Vector3 m_forces;

	//physics properties
	float m_mass;

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline RigidBodyComponent::~RigidBodyComponent(){

}

//-----------------------------------------------------------------------------------------------------------

inline const Vector3& RigidBodyComponent::UpdatePositionWithVelocity(double deltaSeconds){

	m_position += (m_velocity * (float)deltaSeconds);

	DoLockXYZ(m_position);

	return m_position;
}

//-----------------------------------------------------------------------------------------------------------

inline const Vector3& RigidBodyComponent::UpdateVelocityWithAcceleration(double deltaSeconds){

	m_velocity += m_acceleration* (float)deltaSeconds;

	DoLockXYZ(m_velocity);

	return m_velocity;
}

//-----------------------------------------------------------------------------------------------------------

inline const Vector3& RigidBodyComponent::UpdateAccelerationWithForces(){

	const float inverseMass = 1.0f / m_mass;

	m_acceleration = m_forces * inverseMass;

	DoLockXYZ(m_acceleration);

	return m_acceleration;
}

//-----------------------------------------------------------------------------------------------------------



//===========================================================================================================

#endif