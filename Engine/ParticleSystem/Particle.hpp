//==============================================================================================================
//Particle.hpp
//by Albert Chen May-28-2015.
//==============================================================================================================

#pragma once


#ifndef _included_Particle__
#define _included_Particle__

#include "Engine\Math\Math3D.hpp"
//#include "Engine\Math\Vector3.hpp"
#include "Engine\Core\Time.hpp"
#include "..\Console\Console.hpp"
#include "Engine\Renderer\Vertex3D.hpp"

static const Vector3 GRAVITY_VECTOR3 = Vector3(0.0f, 0.0f, -9.81f);

//struct Vertex3D;


//===========================================================================================================

struct ParticleTraceResult{
	//normalized tau
	float TauEnter;
	float TauExit;

	float physicsEnterTime;
	float physicsExitTime;

	bool didCollide = false;

	float GetPhysicsStartTime(double deltaSeconds){
		return (float)GetCurrentSeconds() - (float)deltaSeconds;
	}

	float GetPhysicsEndTime(){
		return (float)GetCurrentSeconds();
	}

	float SetPhysicsTraceEnterTime(double deltaSeconds){
		float pstartTime = GetPhysicsStartTime(deltaSeconds);
		float pendTime = GetPhysicsEndTime();

		physicsEnterTime = pstartTime + TauEnter*(pendTime - pstartTime);

		return physicsEnterTime;
	}

	float SetPhysicsTraceExitTime(double deltaSeconds){
		float pstartTime = GetPhysicsStartTime(deltaSeconds);
		float pendTime = GetPhysicsEndTime();

		physicsExitTime = pstartTime + TauExit*(pendTime - pstartTime);

		return physicsExitTime;
	}


};

//===========================================================================================================

struct State{
	//vars

	Vertex3D vertex;

	float radius = 0.01f;

	//physics integration stuff
	Vector3 prevPrevPosition; //for verlet integration
	Vector3 velocity = Vector3::ZERO;
	Vector3 acceleration = Vector3::ZERO;
	Vector3 forces = Vector3::ZERO;
	
	LineSegment3 trajectory;
	
	float secondsSinceSpawn = 0.0f;
	float lifespanSeconds = 10.0f;
	float mass = 1.0f;
	float inverseMass = 1.0f / mass;

	bool lockX = false;
	bool lockY = false;
	bool lockZ = false;

	//stuff

	State(){
		//do nothing
	}

	~State(){
		//do nothing
	}

	State(const Vector3& initpos, const Vector3& initVel,const Rgba& color = Rgba::WHITE , bool is2DMode = false) :
		//position(initpos),
		velocity(initVel),
		vertex(Vertex3D(initpos, color)),
		prevPrevPosition(vertex.m_position)
	{
		
		if (is2DMode){
			LockZ();
		}
	}
	
	void CalcAccelerationFromForces(){	
		acceleration = forces * inverseMass;
		//forces = Vector3::ZERO;
	}

 	void UpdateAcceleration(const double& deltaSeconds){
		UNUSED(deltaSeconds);
		CalcAccelerationFromForces();
		//add anything else

		DoLockX(acceleration);
		DoLockY(acceleration);
		DoLockZ(acceleration);
 	}

	void UpdateVelocity(const double& deltaSeconds){
		velocity += acceleration * (float)deltaSeconds;

		DoLockX(velocity);
		DoLockY(velocity);
		DoLockZ(velocity);
	}

	void UpdatePosition(const double& deltaSeconds){
		vertex.m_position += velocity * (float)deltaSeconds;
	}

	bool IsDead(){ 
		if (lifespanSeconds < 0){ 
			return false;  
		}

		return (secondsSinceSpawn > lifespanSeconds);
	}

	void SetColor(const Rgba& newColor){ vertex.m_color = newColor;  }

	void SetMass(const float& newMass){ mass = newMass; inverseMass = 1.0f / mass; }

	Vector3 GetStartPosition(){ return trajectory.startPoint; }
	Vector3 GetEndPosition(){ return trajectory.endPoint;  }

	//lock coords methods
	void DoLockX(Vector3& vectorToLock){
		if (lockX){
			vectorToLock.x = 0.0f;
		}
	}
	void DoLockY(Vector3& vectorToLock){
		if (lockY){
			vectorToLock.y = 0.0f;
		}
	}
	void DoLockZ(Vector3& vectorToLock){
		if (lockZ){
			vectorToLock.z = 0.0f;
		}
	}

	void LockXY(){
		lockX = lockY = true;
	}
	void UnlockXY(){
		lockX = lockY = false;
	}
	void LockZ(){
		lockZ = true;
	}
	void UnlockZ(){
		lockZ = false;
	}


};

//===========================================================================================================

class Particle{
public:
	//vars
	State m_state;
	
	//methods

	Particle(const Vector3& position, const Vector3& initVel, const Rgba& color = Rgba::BLACK, bool is2DMode = false);

	Particle(){
		//do nothing	
	}

	~Particle(){
		//delete m_state;
	}
	
	//Upkeep methods
	void Update(const double& deltaSeconds);
	
	//accessors
	void SetState(const Vector3& newPosition, const Vector3&  newVelocity, const float& newlifespan = 10.0f, const float& newmass = 1.0f);
	void SetPosition(const Vector3& newPosition);
	void SetVelocity(const Vector3& newVelocity){ m_state.velocity = newVelocity; }
	void SetAcceleration(const Vector3& newAccel){ m_state.acceleration = newAccel;  }
	void SetLifeSpanSeconds(const float& newlifespan){ m_state.lifespanSeconds = newlifespan; }
	void SetColor(const Rgba& colorToSet){ m_state.vertex.m_color = colorToSet; }
	void SetMass(float mass);
	void SetForce(const Vector3& newForces){ m_state.forces = newForces; }
	void AddForce(Vector3 forceToAdd){ m_state.forces += forceToAdd;  }
	//getters
	Vector3 GetPosition(){ return m_state.vertex.m_position; }
	Vector3 GetVelocity(){ return m_state.velocity; }
	Rgba GetColor(){ return m_state.vertex.m_color; }
	Vector3 GetForces(){ return m_state.forces; }
	float GetMass(){ return m_state.mass; }

	//queries	
	bool IsDead(){ return m_state.IsDead(); }
	
	//friend methods //mostly integration stuff
	friend void UpdateParticleVerletIntegration(Particle& particle, double deltaSeconds);
	
	//collision detection
	friend bool DoParticleSpheresOverlap(const Particle& particleA, const Particle& particleB ); //sphere test
	friend ParticleTraceResult DoParticleTrajectoriesIntersect(const Particle& particleA, const Particle& particleB, double deltaSeconds); //trajectory trace
	friend bool IsParticleWithinBoundingSphere(Particle& particle, Sphere3 boundingSphere);
	friend bool IsParticleAtGroundLevel(Particle& p, const float& groundLevel = 0.0f);

	//collision response
	friend void CollisionResponseBounce(Particle& p1, Particle& q1, const float& bounciness, double deltaSeconds);
	friend void CollisionResponseSpring(Particle& originParticle, Particle& endParticle, bool hasGravity = true, const float& unstretchedLengthConstraint = 1.0f, const float& springConstantK = 10.0f, const float& dampingConstantC = 0.5f);
};

//===========================================================================================================

typedef std::vector<Particle> Particles; //somehow frame rate does silly things when not by ptr
typedef std::vector<Particle>::iterator ParticlesIterator;

///----------------------------------------------------------------------------------------------------------
///inline methods

inline void Particle::SetState(const Vector3& newPosition, const Vector3& newVelocity, const float& newlifespan , const float& newmass ){
	
	m_state.vertex.m_position = newPosition;
	m_state.velocity = newVelocity;
	m_state.lifespanSeconds = newlifespan;
	m_state.mass = newmass;

}

#endif