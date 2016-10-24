//==============================================================================================================
//Emitter.hpp
//by Albert Chen May-28-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Emitter__
#define _included_Emitter__


//#include "ParticleSystem.hpp"
#include "Engine/ParticleSystem/Effect.hpp"

//===========================================================================================================

class Emitter{
public:
	Emitter(){
		//do nothing 
	}
	Emitter(XMLNode& emitterNode);
	~Emitter();

	Emitter(const EffectType& newEffectType, const Vector3& position, const unsigned int& particleMax,
		const float& lifespanSeconds = 1.0f, const unsigned int& spawnIntervals = 1, 
		const float& maginitude = 10.0f, const float& spread = 3.0f, const Rgba& color = Rgba::BLACK, bool is2DMode = false );
	
	//upkeep methods
	void Update(double deltaSeconds, Particles& particlesToUpdate );
	void PushParticleToParticleSystem(Particles& particlesToUpdate, const float& newLifeSpanSeconds = 10.0f);

	//accessors
	void SetLifeSpan(const float& newLifespan){ m_lifespanSeconds = newLifespan; }
	void SetMagnitude(const float& newmagnitude){ m_effect->magnitude = newmagnitude; }
	void SetPosition(const Vector3& newPosition){ m_position = newPosition;  }
	void SetParticleColor(const Rgba& newColor){ m_color = newColor;  }
	void SetParticleLifeSpan(const float& newLifespan) { m_particleLifespanSeconds = newLifespan; }
	void SetParticleImmortal() { m_particleLifespanSeconds = -1.0f; }

	//calc methods
	float CalcSpawnRate();
	unsigned int CalcParticlesPerSpawn();
	void MakeImmortal(){ m_lifespanSeconds = -1.0f; }
	void Reset(){ m_secondsSinceEmitterCreation = 0.0f; }
	void Kill(){ 
		m_lifespanSeconds = 1.0f;
		m_secondsSinceEmitterCreation = m_lifespanSeconds + 1.0f;
	}
	bool IsDead();
	

	//vars
	//Particles m_particles;
	Effect* m_effect = NULL;
	bool m_is2D = false;
	Rgba m_color;
	float m_lifespanSeconds; //-1 means immortal //*
	float m_particleLifespanSeconds = 10;
	float m_secondsSinceEmitterCreation; //starts at 0
	float m_secondsTillParticleSpawn; // starts at 0
	unsigned int m_particleMax; 
	Vector3 m_position; 
	unsigned int m_spawnIntervals; 
	//derived vars
	float m_spawnRate;
	unsigned int m_particlesPerSpawn;
};

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///inline methods

inline bool Emitter::IsDead(){
	//immortal if lifespan is negative
	if (m_lifespanSeconds < 0.0f){
		return false;
	}

	return (m_secondsSinceEmitterCreation > m_lifespanSeconds);
}

#endif



