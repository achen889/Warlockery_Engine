//==============================================================================================================
//PhysicsMotion2DComponent.MPp
//by Albert Chen Feb-25-2016.
//==============================================================================================================

#pragma once

#ifndef _included_TransformComponent__
#define _included_PhysicsMotion2DComponent__

#include "Transform2DComponent.hpp"
#include "Engine/Math/PhysicsMotion2D.hpp"

class PhysicsMotion2DComponent : public Transform2DComponent{
public:

	PhysicsMotion2DComponent() : Transform2DComponent() {
		//do nothing
	}
	~PhysicsMotion2DComponent();

	
	PhysicsMotion2DComponent(const std::string& name);

	PhysicsMotion2DComponent(const XMLNode& node);
	
	static BaseComponent* CreateComponent(const std::string& name){ return new PhysicsMotion2DComponent(name); }

	static ComponentRegistration s_PhysicsMotion2DComponentRegistration;

	void SetLockMotionToOrientation(bool doLock) {
		m_lockToOrientation = doLock;
	}

	void SetPosition(const Vector2& position) {
		//update physics motion handle position
		m_physicsMotionHandle.SetPosition(position);

		Transform2DComponent::SetPosition(position);
	}

	virtual void SetOrientationDegrees(const float& degrees) { 
		m_physicsMotionHandle.SetOrientationDegrees(degrees);

		Transform2DComponent::SetOrientationDegrees(degrees);
	}

	const Vector2 GetVelocity() { return m_physicsMotionHandle.m_velocityUnitsPerSecond;  }

	void SetVelocity(const Vector2& vel) { m_physicsMotionHandle.m_velocityUnitsPerSecond = vel; }
	void SetVelocity(const float& speedUnitsPerSecond, const float orientationDegrees);

	void SetMaxVelocity(const float& maxVel) { m_physicsMotionHandle.m_maxVelocityUnitsPerSecond = maxVel; }
	
	void SetAcceleration(const Vector2& accel ) { m_physicsMotionHandle.m_accelerationUnitsPerSecondSquared = accel; }
	void SetAcceleration(const float& accelUnitsPerSecondPerSecond, const float orientationDegrees);
	
	void SetAngularVelocityDegrees(const float& angVelDegrees);
	void SetAngularAccelerationDegrees(const float& angAccel);

	void ZeroThisComponent() {
		m_physicsMotionHandle.ZeroThisPhysicsController();
		//zero my stuff too
		m_orientation = 0.0f;
		m_position = Vector2::ZERO;
	}

	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Update(double deltaSeconds);
	

protected:
	//vars
	bool m_lockToOrientation = true;

	PhysicsMotion2D m_physicsMotionHandle;

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline PhysicsMotion2DComponent::~PhysicsMotion2DComponent(){
	Transform2DComponent::~Transform2DComponent();
}

//-----------------------------------------------------------------------------------------------------------

inline void PhysicsMotion2DComponent::SetVelocity(const float& speedUnitsPerSecond, 
	const float orientationDegrees) { 
	m_physicsMotionHandle.SetVelocity(speedUnitsPerSecond, orientationDegrees);
}

//-----------------------------------------------------------------------------------------------------------

inline void PhysicsMotion2DComponent::SetAcceleration(const float& accelUnitsPerSecondSquared, 
	const float orientationDegrees) {

	m_physicsMotionHandle.SetAcceleration(accelUnitsPerSecondSquared, orientationDegrees);

}

//-----------------------------------------------------------------------------------------------------------

inline void PhysicsMotion2DComponent::SetAngularVelocityDegrees(const float& angVelDegrees) {
	m_physicsMotionHandle.m_angularVelocityDegreesPerSecond = angVelDegrees;
}

//-----------------------------------------------------------------------------------------------------------

inline void PhysicsMotion2DComponent::SetAngularAccelerationDegrees(const float& angAccel) {
	m_physicsMotionHandle.m_angularAccelerationDegreesPerSecondSquared = angAccel;
}

//===========================================================================================================

#endif