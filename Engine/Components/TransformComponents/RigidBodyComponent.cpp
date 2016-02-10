//==============================================================================================================
//PhysicsMotionComponent.cpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#include "RigidBodyComponent.hpp"

ComponentRegistration RigidBodyComponent::s_PhysicsMotionComponentRegistration("PhysicsMotion", &RigidBodyComponent::CreateComponent);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///RigidBody Component

RigidBodyComponent::RigidBodyComponent(const XMLNode& node): TransformComponent(node)
{

}

RigidBodyComponent::RigidBodyComponent(const std::string& name) : TransformComponent(name){

}

//-----------------------------------------------------------------------------------------------------------

void RigidBodyComponent::OnCreate(){
	//can specify something on create here
}

//-----------------------------------------------------------------------------------------------------------

void RigidBodyComponent::OnDestroy(){
	//can specify something on destroy here
}

//-----------------------------------------------------------------------------------------------------------

void RigidBodyComponent::Update(double deltaSeconds){
	switch (m_physicsIntegrationType) {
	case PHYSICS_FWD_EULER:
		UpdateForwardEulerIntegration(deltaSeconds);
		break;
	case PHYSICS_VERLET:
		UpdateVerletIntegration(deltaSeconds);
		break;
	}
	//we now have the physics updated position.

}

//-----------------------------------------------------------------------------------------------------------

void RigidBodyComponent::UpdateForwardEulerIntegration(double deltaSeconds) {
	//fwd euler integration
	UpdateAccelerationWithForces();
	UpdateVelocityWithAcceleration(deltaSeconds);
	UpdatePositionWithVelocity(deltaSeconds);
}

//-----------------------------------------------------------------------------------------------------------

void RigidBodyComponent::UpdateVerletIntegration(double deltaSeconds) {
	//verlet integration 4th order scheme
	Vector3 tempPosition = m_position;

	//calculate acceleration
	UpdateAccelerationWithForces();
	
	Vector3 accelerationModifier = m_acceleration * (float)(deltaSeconds*deltaSeconds);

	//calculate verlet position
	Vector3 nextPosition = (2.0f * tempPosition) - (m_prevPrevPosition) + (accelerationModifier);
	SetPosition(nextPosition);

	//prepare for next verlet frame
	m_prevPrevPosition = tempPosition;
	tempPosition = nextPosition;

	//maybe only the particle springs needed this part
	//m_forces = Vector3::ZERO;
}



//===========================================================================================================


