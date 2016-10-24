//==============================================================================================================
//PhysicsMotion2D.hpp
//by Albert Chen Jan-22-2015.
//==============================================================================================================

#ifndef _included_PhysicsMotion2D__
#define _included_PhysicsMotion2D__

#include "EngineMath.hpp"

struct PhysicsMotion2D{

	//vars
	Vector2 m_position;
	Vector2 m_velocityUnitsPerSecond;
	Vector2 m_accelerationUnitsPerSecondSquared;
	float m_orientationDegrees;
	float m_angularVelocityDegreesPerSecond;
	float m_angularAccelerationDegreesPerSecondSquared;
	float m_maxVelocityUnitsPerSecond;

	bool m_lockMotionToOrientation = true;

	//methods
	PhysicsMotion2D();

	void SetPosition(const Vector2& pos) { m_position = pos; }
	void SetOrientationDegrees(const float& orientationDegrees) { m_orientationDegrees = orientationDegrees; }

	void SetVelocity(const float& speedUnitsPerSecond, const float& orientationDegrees);

	void SetMaxVelocity(const float& maxVel) { m_maxVelocityUnitsPerSecond = maxVel; }
	float ClampVelocity(const float& velToClamp ) { return Clamp(velToClamp, velToClamp, m_maxVelocityUnitsPerSecond); }

	void SetAcceleration(const float& accelUnitsPerSecondSquared, const float& orientationDegrees);
	
	const Vector2& UpdatePositionWithVelocity(double deltaSeconds);
	const Vector2& UpdateVelocity(const Vector2& accelPerSecondSquared, double deltaSeconds);
	const Vector2& UpdateAcceleration(double deltaSeconds);
	const float UpdateAngularVelocity(double deltaSeconds);

	void Update(double deltaSeconds, bool lockMotionToOrientation = true) { 
		if (!lockMotionToOrientation && m_lockMotionToOrientation ) {
			m_lockMotionToOrientation = false;
		}
		UpdateFwdEulerIntegration(deltaSeconds);  
	}

	void UpdateFwdEulerIntegration(double deltaSeconds );
	
	void ZeroThisPhysicsController();

	friend void ConsolePrintPhysicsMotion2D(const PhysicsMotion2D& consolePhysicsMotion2D);
	
};

///----------------------------------------------------------------------------------------------------------
///constructor
inline PhysicsMotion2D::PhysicsMotion2D(){
	//do nothing
	ZeroThisPhysicsController();
}

inline void PhysicsMotion2D::ZeroThisPhysicsController(){
	m_orientationDegrees = 0.0;
	m_velocityUnitsPerSecond = Vector2( 0.0f, 0.0f );
	m_angularVelocityDegreesPerSecond = 0.0f;
	m_angularAccelerationDegreesPerSecondSquared = 0.0f;
	m_accelerationUnitsPerSecondSquared = Vector2( 0.0f, 0.0f );
}

//-----------------------------------------------------------------------------------------------------------

inline void PhysicsMotion2D::SetVelocity(const float& speedUnitsPerSecond, 
	const float& orientationDegrees) {

	m_velocityUnitsPerSecond.SetLengthAndHeadingDegrees(speedUnitsPerSecond, orientationDegrees);

}

//-----------------------------------------------------------------------------------------------------------

inline void PhysicsMotion2D::SetAcceleration(const float& accelUnitsPerSecondSquared, 
	const float& orientationDegrees){
	
	m_accelerationUnitsPerSecondSquared.SetLengthAndHeadingDegrees(accelUnitsPerSecondSquared, 
																	orientationDegrees);
}


//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///fwd euler

inline const Vector2& PhysicsMotion2D::UpdatePositionWithVelocity(double deltaSeconds) {

	m_position += (m_velocityUnitsPerSecond * (float)deltaSeconds);

	return m_position;
}

inline const Vector2& PhysicsMotion2D::UpdateVelocity(const Vector2& accelUnitsPerSecondSquared, double deltaSeconds) {
	m_velocityUnitsPerSecond += accelUnitsPerSecondSquared * (float)deltaSeconds;
	
	//update velocity direction with angular velocity
	if (m_lockMotionToOrientation) {
		m_velocityUnitsPerSecond.SetLengthAndHeadingDegrees(ClampVelocity(m_velocityUnitsPerSecond.CalcLength()), m_orientationDegrees);
	}
	else {
		ClampVelocity(m_velocityUnitsPerSecond.CalcLength());
	}
	
	return m_velocityUnitsPerSecond;
}

inline const Vector2& PhysicsMotion2D::UpdateAcceleration(double deltaSeconds) {
	UNUSED(deltaSeconds);
	
	if (m_lockMotionToOrientation) {
		m_accelerationUnitsPerSecondSquared.SetLengthAndHeadingDegrees(m_accelerationUnitsPerSecondSquared.CalcLength(), m_orientationDegrees);
	}

	return m_accelerationUnitsPerSecondSquared;
}

inline const float PhysicsMotion2D::UpdateAngularVelocity(double deltaSeconds) {
	//update angular vel
	m_angularVelocityDegreesPerSecond += m_angularAccelerationDegreesPerSecondSquared * (float)deltaSeconds;

	m_orientationDegrees += m_angularVelocityDegreesPerSecond * (float)deltaSeconds;

	return m_orientationDegrees;
}

//-----------------------------------------------------------------------------------------------------------

inline void ConsolePrintPhysicsMotion2D(const PhysicsMotion2D& consolePhysicsMotion2D){
	ConsolePrintf("| PhysicsMotion2D |\n| Position< ");
	ConsolePrintVector2(consolePhysicsMotion2D.m_position);

	//if(consolePhysicsMotion2D.m_velocityUnitsPerSecond != Vector2(0.0f, 0.0f) ){
		ConsolePrintf(" >\n| Velocity< ");
		ConsolePrintVector2(consolePhysicsMotion2D.m_velocityUnitsPerSecond);
	//}

	//if(consolePhysicsMotion2D.m_accelerationUnitsPerSecondSquared != Vector2(0.0f, 0.0f) ){
		ConsolePrintf(" >\n| Acceleration< ");
		ConsolePrintVector2(consolePhysicsMotion2D.m_accelerationUnitsPerSecondSquared);
	//}

	ConsolePrintf(" >\n| OrientationDegrees =");
	ConsolePrintf(" %.02f |\n", consolePhysicsMotion2D.m_orientationDegrees);
	
	//if(consolePhysicsMotion2D.m_angularVelocityDegreesPerSecond != 0.0f){
		ConsolePrintf("| AngularVelocityDegrees =");
		ConsolePrintf(" %.02f |\n", consolePhysicsMotion2D.m_angularVelocityDegreesPerSecond);
	//}

}

#endif