//==============================================================================================================
//PhysicsMotion2D.cpp
//by Albert Chen Jan-22-2015.
//==============================================================================================================

#include "PhysicsMotion2D.hpp"

//2nd order scheme, can lead to physics errors
void PhysicsMotion2D::UpdateFwdEulerIntegration(double deltaSeconds) {
	UpdateAngularVelocity(deltaSeconds);

	UpdateAcceleration(deltaSeconds);

	UpdateVelocity(m_accelerationUnitsPerSecondSquared, deltaSeconds);
	UpdatePositionWithVelocity(deltaSeconds);

}
