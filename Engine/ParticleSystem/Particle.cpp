//==============================================================================================================
//Particle.cpp
//by Albert Chen May-28-2015.
//==============================================================================================================

#include "Particle.hpp"
//#include "Engine\Math\Math3D.hpp"
#include "Engine\Renderer\OpenGLRenderer.hpp"


Particle::Particle(const Vector3& position, const Vector3& initVel,const Rgba& color, bool is2DMode ):
m_state(State(position, initVel, color, is2DMode))
{
	
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///update

void Particle::Update(double deltaSeconds){
	PROFILE_SECTION();
	m_state.secondsSinceSpawn += (float)deltaSeconds;
	//if particle is past it's lifespan
	if (!m_state.IsDead()){
		m_state.trajectory.startPoint = m_state.vertex.m_position;

		m_state.UpdateAcceleration(deltaSeconds);
		m_state.UpdateVelocity(deltaSeconds);
		m_state.UpdatePosition(deltaSeconds);
		
		m_state.trajectory.endPoint = m_state.vertex.m_position;
	}
}

//===========================================================================================================

void Particle::SetPosition(const Vector3& newPosition){ 

	Vector3 posToSet = newPosition;

	if (m_state.lockX)posToSet.x = m_state.vertex.m_position.x;
	if (m_state.lockY)posToSet.y = m_state.vertex.m_position.y;
	if (m_state.lockZ)posToSet.z = m_state.vertex.m_position.z;

	m_state.vertex.m_position = posToSet;

	//m_state.vertex.m_position = m_state.position;
}

void Particle::SetMass(float mass){
	m_state.mass = mass;
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///render helpers

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///friend methods

///----------------------------------------------------------------------------------------------------------
///verlet integration, uses only force, acceleration, 4th order scheme, pretty darned accurate
void UpdateParticleVerletIntegration(Particle& particle, double deltaSeconds){
	//verlet integration 4th order scheme
	Vector3 tempParticlePosition = particle.GetPosition();

	//calculate acceleration
	particle.m_state.CalcAccelerationFromForces();
	particle.m_state.DoLockX(particle.m_state.acceleration);
	particle.m_state.DoLockY(particle.m_state.acceleration);
	particle.m_state.DoLockZ(particle.m_state.acceleration);
	Vector3 accelerationModifier = particle.m_state.acceleration * (float)(deltaSeconds*deltaSeconds);
	
	//calculate verlet position
	Vector3 nextParticlePosition = (2.0f * tempParticlePosition) - (particle.m_state.prevPrevPosition) + (accelerationModifier);
	particle.SetPosition(nextParticlePosition);

	//prepare for next verlet frame
	particle.m_state.prevPrevPosition = tempParticlePosition;
	tempParticlePosition = nextParticlePosition;

	particle.m_state.forces = Vector3::ZERO;
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///collision detection

bool DoParticleSpheresOverlap(const Particle& particleA, const Particle& particleB){
	const float radius = 0.01f;
	Sphere3 sphereA = Sphere3(particleA.m_state.vertex.m_position, radius);
	Sphere3 sphereB = Sphere3(particleB.m_state.vertex.m_position, radius);

	return DoSpheresOverlap(sphereA, sphereB );

	//return (particleA.m_state.position == particleB.m_state.position);
}

//-----------------------------------------------------------------------------------------------------------

ParticleTraceResult DoParticleTrajectoriesIntersect(Particle& particleA, Particle& particleB, double deltaSeconds){
	ParticleTraceResult particleCollisionTrace;
	const float radiusA = 0.01f;
	const float radiusB = 0.01f;
// 	Sphere3 sphereA = Sphere3(particleA.m_state.position, radiusA);
// 	Sphere3 sphereB = Sphere3(particleB.m_state.position, radiusB);
	float R = radiusA + radiusB;

	//get vars for super math
	Vector3 A_StartPos = particleA.m_state.GetStartPosition();
	Vector3 A_EndPos = particleA.m_state.GetEndPosition();

	Vector3 B_StartPos = particleB.m_state.GetStartPosition();
	Vector3 B_EndPos = particleB.m_state.GetEndPosition();

	Vector3 EOfAB = B_EndPos - B_StartPos - A_EndPos + A_StartPos;
	Vector3 X0OfAB = B_StartPos - A_StartPos;

	float EDotX0 = DotProduct(EOfAB, X0OfAB);
	float EDotE = DotProduct(EOfAB, EOfAB);

	float X0DotX0 = DotProduct(X0OfAB, X0OfAB);
	float X0DotE = DotProduct(X0OfAB, EOfAB);

	float DiscriminantOfTauCalculation = (EDotX0*EDotX0) - (EDotE*(X0DotX0- R*R) );

	if (DiscriminantOfTauCalculation >= 0.0f){
		//line intersects the sphere
		float SqrtOfDiscriminant = sqrt(DiscriminantOfTauCalculation);
		float InverseEDotE = 1.0f / EDotE;

		particleCollisionTrace.TauEnter = (-X0DotE - SqrtOfDiscriminant) * InverseEDotE;
		particleCollisionTrace.TauExit = (-X0DotE + SqrtOfDiscriminant) * InverseEDotE;

		if (particleCollisionTrace.TauEnter <= 1.0f && particleCollisionTrace.TauExit >= 0.0f){
			particleCollisionTrace.SetPhysicsTraceEnterTime(deltaSeconds);
			particleCollisionTrace.SetPhysicsTraceExitTime(deltaSeconds);
			particleCollisionTrace.didCollide = true;

			return particleCollisionTrace;
		}
		else{
			particleCollisionTrace.didCollide = false;

			return particleCollisionTrace;
		}
	}
	else{
		particleCollisionTrace.didCollide = false;

		return particleCollisionTrace;
	}
}

//-----------------------------------------------------------------------------------------------------------

bool IsParticleAtGroundLevel(Particle& p, const float& groundLevel){
	return (bool)(p.GetPosition().z <= groundLevel);
}

bool IsParticleWithinBoundingSphere(Particle& particle, Sphere3 boundingSphere){
	return IsPointInsideSphere(particle.GetPosition(), boundingSphere);
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///collision response

void CollisionResponseBounce(Particle& p1, Particle& q1, const float& bounciness, double deltaSeconds){
	ParticleTraceResult PQCollisionResult = DoParticleTrajectoriesIntersect(p1, q1, deltaSeconds);
	//bounce collision with impulse and momemtum
	if (PQCollisionResult.didCollide){
		//collision response direct central impact calc
		float coeffOfRestitution = bounciness;

		Vector3 ImpulsePQ = coeffOfRestitution * (q1.GetVelocity() - p1.GetVelocity());

		Vector3 CombinationPQ = (p1.GetMass() * p1.GetVelocity()) + (q1.GetMass() * q1.GetVelocity());

		float detOfDirectCentral = 1.0f / (p1.GetMass() + q1.GetMass());

		Vector3 velAfterP = CombinationPQ + q1.GetMass()*ImpulsePQ;

		Vector3 velAfterQ = CombinationPQ - p1.GetMass()*ImpulsePQ;

		velAfterP *= detOfDirectCentral;
		velAfterQ *= detOfDirectCentral;

		p1.SetPosition(p1.m_state.GetStartPosition());
		q1.SetPosition(q1.m_state.GetStartPosition());

		p1.SetVelocity(velAfterP);
		q1.SetVelocity(velAfterQ);
		//ConsoleGenericMessageBox("Particles P and Q Are Colliding");
	}
}

void CollisionResponseSpring(Particle& originParticle, Particle& endParticle, bool hasGravity, const float& unstretchedLengthConstraint, const float& springConstantK, const float& dampingConstantC){
	Vector3 eOfPoints = endParticle.GetPosition() - originParticle.GetPosition();
	eOfPoints.Normalize();

	float L0 = unstretchedLengthConstraint;

	Vector3& endPointForces = endParticle.m_state.forces;
	endPointForces = Vector3::ZERO;
	//add spring force
	float K = springConstantK;
	float L = CalcDistance(endParticle.GetPosition(), originParticle.GetPosition());
	L = abs(L);
	//L = Clamp(L, 0.0f, 0.5f);
	Vector3 springForceK = -K * (L - L0) * eOfPoints;
	endPointForces += springForceK;
	//add damping force
	const float c = dampingConstantC;
	Vector3 dampingForceC = -c * DotProduct(eOfPoints, (endParticle.GetVelocity() - originParticle.GetVelocity())) * eOfPoints;
	endPointForces += dampingForceC;

	//add gravity force
	if (hasGravity){
		endPointForces += endParticle.m_state.mass * GRAVITY_VECTOR3;
	}

	//add env damping to force
	const float cE = 0.5f;
	Vector3 enviroDampingC = -cE * endParticle.GetVelocity();
	endPointForces += enviroDampingC;


}
