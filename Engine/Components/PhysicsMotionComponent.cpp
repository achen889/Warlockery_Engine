//==============================================================================================================
//PhysicsMotionComponent.cpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#include "PhysicsMotionComponent.hpp"

ComponentRegistration PhysicsMotionComponent::s_PhysicsMotionComponentRegistration("PhysicsMotion", &PhysicsMotionComponent::CreateComponent);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///PhysicsMotion Component

PhysicsMotionComponent::PhysicsMotionComponent(const XMLNode& node): BaseComponent(node)
{

}

PhysicsMotionComponent::PhysicsMotionComponent(const std::string& name) : BaseComponent(name){

}

//-----------------------------------------------------------------------------------------------------------

void PhysicsMotionComponent::OnCreate(){
	//can specify something on create here
}

//-----------------------------------------------------------------------------------------------------------

void PhysicsMotionComponent::OnDestroy(){
	//can specify something on destroy here
}

//-----------------------------------------------------------------------------------------------------------

void PhysicsMotionComponent::Update(double deltaSeconds){

	//fwd euler integration
	UpdateAccelerationWithForces(deltaSeconds);
	UpdateVelocityWithAcceleration(deltaSeconds);
	UpdatePositionWithVelocity(deltaSeconds);
	
}



//===========================================================================================================


