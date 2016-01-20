//==============================================================================================================
//PhysicsMotion2D.cpp
//by Albert Chen Jan-22-2015.
//==============================================================================================================

#include "PhysicsMotion2D.hpp"

const Vector2& PhysicsMotion2D::UpdatePositionWithVelocity(double deltaSeconds){
	if(m_angularVelocityDegreesPerSecond != 0.0f){
		m_velocityUnitsPerSecond.SetLengthAndHeadingDegrees(m_velocityUnitsPerSecond.CalcLength(), m_orientationDegrees );
	}
	m_position += ( m_velocityUnitsPerSecond * (float)deltaSeconds );
	return m_position;
}
const Vector2& PhysicsMotion2D::UpdateVelocity(const Vector2& deltaVelocity ){
	m_velocityUnitsPerSecond += deltaVelocity;

	return m_velocityUnitsPerSecond;
}
const float PhysicsMotion2D::UpdateAngularVelocity(double deltaSeconds){
	m_orientationDegrees += m_angularVelocityDegreesPerSecond * (float)deltaSeconds;
	return m_orientationDegrees;
}

void PhysicsMotion2D::Update(double deltaSeconds ){
	UpdateAngularVelocity(deltaSeconds);
	UpdateVelocity(m_accelerationUnitsPerSecondSquared);
	UpdatePositionWithVelocity(deltaSeconds);
	
	
}
