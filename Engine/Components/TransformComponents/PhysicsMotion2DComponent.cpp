//==============================================================================================================
//PhysicsMotion2DComponent.cpp
//by Albert Chen Feb-25-2016.
//==============================================================================================================

#include "PhysicsMotion2DComponent.hpp"

ComponentRegistration PhysicsMotion2DComponent::s_PhysicsMotion2DComponentRegistration("PhysicsMotion2D", &PhysicsMotion2DComponent::CreateComponent);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Transform Component

PhysicsMotion2DComponent::PhysicsMotion2DComponent(const XMLNode& node): Transform2DComponent(node){
	ZeroThisComponent();
}

PhysicsMotion2DComponent::PhysicsMotion2DComponent(const std::string& name) : Transform2DComponent(name){
	ZeroThisComponent();
}

//-----------------------------------------------------------------------------------------------------------

void PhysicsMotion2DComponent::OnCreate(){
	//can specify something on create here
}

//-----------------------------------------------------------------------------------------------------------

void PhysicsMotion2DComponent::OnDestroy(){
	//can specify something on destroy here
}

//-----------------------------------------------------------------------------------------------------------

void PhysicsMotion2DComponent::Update(double deltaSeconds){
	
	//update the physics integration
	//if (m_lockToOrientation) {
		m_physicsMotionHandle.SetOrientationDegrees(m_orientation);
	//}

	m_physicsMotionHandle.Update(deltaSeconds, m_lockToOrientation);

	//sets the transform from the physics motion 2D
	SetPosition(m_physicsMotionHandle.m_position);
	SetOrientationDegrees(m_physicsMotionHandle.m_orientationDegrees);

}

//===========================================================================================================


