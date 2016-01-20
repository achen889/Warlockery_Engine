//==============================================================================================================
//ParticlePhysics.cpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#include "ParticlePhysics.hpp"


//===========================================================================================================

Vector3 CalcGravityForce(const float& particleMass){
	return GRAVITY_VECTOR3 * particleMass;
}

//-----------------------------------------------------------------------------------------------------------

Vector3 CalcWindForce(Particle& particleToApply){
	const float dampingC = 0.05f;
	Vector3 windForce = GetRandomVector3InRange(0.0f, 2.0f);
	Vector3 finalwindForce = particleToApply.GetVelocity();
	finalwindForce -= windForce;
	finalwindForce = -dampingC * finalwindForce;

	//windForce.x = 0.0f;
	windForce.z = 0.0f;

	return windForce * particleToApply.GetMass();
}

//-----------------------------------------------------------------------------------------------------------

Vector3 GetRandomInitialVelocity(const float& newmagnitude){
	return newmagnitude * Vector3(GetRandomFloatInRange(-1.0f, 1.0f), GetRandomFloatInRange(-1.0f, 1.0f), GetRandomFloatInRange(-1.0f, 1.0f));
}

//===========================================================================================================

