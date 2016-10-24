//==============================================================================================================
//Emitter.cpp
//by Albert Chen May-28-2015.
//==============================================================================================================

#include "Emitter.hpp"



//===========================================================================================================

Emitter::Emitter(const EffectType& newEffectType, const Vector3& position, const unsigned int& particleMax, 
	const float& lifespanSeconds, const unsigned int& spawnIntervals, const float& maginitude, const float& spread, const Rgba& color, bool is2DMode ) :
	m_position(position),
	m_particleMax(particleMax),
	m_lifespanSeconds(lifespanSeconds),
	m_spawnIntervals(spawnIntervals),
	m_is2D(is2DMode),
	m_color(color)
{
	m_spawnRate = CalcSpawnRate();
	m_particlesPerSpawn = CalcParticlesPerSpawn();
	m_secondsTillParticleSpawn = 0;
	m_secondsSinceEmitterCreation = 0;

	m_effect = new Effect(newEffectType, maginitude, spread );
	
}

Emitter::Emitter(XMLNode& emitterNode){
	UNUSED(emitterNode);
	//ReadXMLAttributeAsString("name", emitterNode);
	
}

Emitter::~Emitter(){

	if (m_effect) {
		delete m_effect;
		m_effect = NULL;
	}

}
//===========================================================================================================

void Emitter::Update(double deltaSeconds, Particles& particlesToUpdate){
	m_secondsSinceEmitterCreation += (float)deltaSeconds;
	//kill on lifespan
	if (!IsDead()){
		//every x spawn
		m_secondsTillParticleSpawn += (float)deltaSeconds;
		if (m_secondsTillParticleSpawn >= m_spawnRate){

			PushParticleToParticleSystem(particlesToUpdate, m_particleLifespanSeconds);

		}
		//ConsolePrintf("Number of particles in emitter1 = %d\n", GetNumberOfParticles());
	}//if not dead
}

//-----------------------------------------------------------------------------------------------------------

void Emitter::PushParticleToParticleSystem(Particles& particlesToUpdate, const float& newLifeSpanSeconds){

	//spawn one set!
	for (unsigned int i = 0; i < m_particlesPerSpawn; i++){
		particlesToUpdate.push_back(new Particle(m_position, m_effect->GetInitialVelocity(), m_color, m_is2D));
		Particle& particleToPush = *particlesToUpdate.back();
		//additional things
		particleToPush.SetLifeSpanSeconds(newLifeSpanSeconds);
		if (m_color == Rgba::BLACK){
			particleToPush.SetColor(GetRandomColor());
		}
		//particlesToUpdate.back()->SetColor(GetRandomColor());
		particleToPush.m_state.forces = m_effect->GetForce(particleToPush);
	}

	m_secondsTillParticleSpawn = 0.0f;
}


//===========================================================================================================

float Emitter::CalcSpawnRate(){
	//immortal emitters spawn rate = 1 sec / intervals
	if (m_lifespanSeconds == -1.0f){
		return ( 1.0f / (float)m_spawnIntervals );
	}

	return (m_lifespanSeconds / (float)m_spawnIntervals );	
}

//-----------------------------------------------------------------------------------------------------------

unsigned int Emitter::CalcParticlesPerSpawn(){
	return (unsigned int)(m_particleMax / m_spawnIntervals );
}

//===========================================================================================================

