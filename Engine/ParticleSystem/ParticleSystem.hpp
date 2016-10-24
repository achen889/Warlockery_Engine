//==============================================================================================================
//ParticleSystem.hpp
//by Albert Chen May-29-2015.
//==============================================================================================================


#pragma once

#ifndef _included_ParticleSystem__
#define _included_ParticleSystem__

#include <map>
#include "Emitter.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
//#include "Engine/Renderer/OGLRenderingUtils.hpp"
#include "Engine/Multithreading/JobManager.hpp"

#include "Engine/Renderer/MeshRenderer.hpp"

//===========================================================================================================

typedef std::map<std::string, Emitter*> ParticleEmitterMap;
typedef std::map<std::string, Emitter*>::iterator ParticleEmitterMapIterator;

struct ParticleString{
	Particles m_particles;

	int m_numParticles;

	VertexArrayObject m_pStringVAO;

	MeshRenderer m_particleStringRenderer;

	ParticleString(){
		//InitializeParticleString(m_particles, 10);
		
		//InitializeParticleStringVAO();
	}

	ParticleString(int numParticles, const Rgba& stringColor = Rgba::WHITE);

	void InitializeParticleString(Particles& m_particles, int numberOfParticles, const Rgba& stringColor = Rgba::WHITE);

	Particle& GetOrigin(){
		return *(*m_particles.begin() );
	}

	Particle& GetEnd(){
		return *(*m_particles.end() );
	}

	void UpdateParticleString(double deltaSeconds);

	void UpdateParticleStringInput(Particles& m_particles, double deltaSeconds);

	void RenderParticleString(OpenGLRenderer* renderer, const Rgba& viewColor);

	void RenderParticleStringMesh(OpenGLRenderer* renderer, Camera3D& camera, const Rgba& viewColor = Rgba::WHITE);

	void RenderParticleStringMesh2D(OpenGLRenderer* renderer, ModelViewMatrix* modelView = NULL);

	void InitializeParticleStringVAO();

	void RenderParticleStringVAO(OpenGLRenderer* renderer, Camera3D& camera, const Rgba& viewColor );

	void GenerateParticleStringVertexArray(Vertex3Ds& out_vertexArray, const Rgba& viewColor);
};

//===========================================================================================================

class ParticleSystem : EventSubscriberObject{
public:

	Particles g_particles;
	static ParticleEmitterMap g_particleEmitters;
	unsigned int m_vboID;
	size_t m_numOFVBOVertexes;
	VertexArrayObject m_particleVAO;
	MeshRenderer m_particleRenderer;

	ParticleSystem();

	void InitializeParticleMeshRenderer();
	void RegisterParticleCommands();

	void EventSpawnParticleEffect(NamedProperties& params);

	void ClearAllEmitters() {
		for (ParticleEmitterMapIterator it = g_particleEmitters.begin(); it != g_particleEmitters.end(); ++it) {
			Emitter* emitter = (it->second);
			if (emitter) {
				delete emitter;
				emitter = NULL;
			}
		}
		g_particleEmitters.clear();
	}
	
	~ParticleSystem(){
		//do nothing
		ClearAllEmitters();
		
		ClearAllParticles();
	}
	
	//mesh/material stuff

	void UpdateParticlesInMesh();

	void InitializeParticleVAO();

	void Update(double deltaSeconds);
	void UpdateEmitters(double deltaSeconds);
	void UpdateParticles(double deltaSeconds);

	void UpdateParticlesMultithreaded(double deltaSeconds);

	//queries and accessors
	unsigned int GetTotalParticles();

	void ClearAllParticles(){ 
		for (ParticlesIterator pIter = g_particles.begin(); pIter != g_particles.end(); ++pIter ) {
			Particle* p = *(pIter);
			if (p) {
				delete p;
				p = NULL;
			}
		}
		g_particles.clear();
	}
	
	bool HasEmitterByName(const std::string& emitterName);
	Emitter& GetEmitterByName(const std::string& emitterName);
	void AddEmitter(const std::string& emitterName, const EffectType& newEffectType, const Vector3& position, const unsigned int& particleMax,
		const float& lifespanSeconds = 1.0f, const unsigned int& spawnIntervals = 1);
	void AddEmitter(const std::string& emitterName, const Emitter& newEmitter);
	void Render(OpenGLRenderer* renderer);
	
	void RenderParticlesMesh(OpenGLRenderer* renderer, Camera3D& camera, bool isPerspective = true );
	void GenerateVertexArrayAndVBO();
	void GenerateParticlesVertexArray(Vertex3Ds& out_vertexArray);
	const Vertex3D GenerateParticleVertexPoint(Particle& particle);
	void DrawVBO();
	void RenderWithVBOs();
	void RenderWithVAO(OpenGLRenderer* PRenderer, Camera3D& camera);

	friend void InitializeParticleString(Particles& m_particles, int numberOfParticles);
	friend void ParticleStringIntegration(Particles& m_particles, double deltaSeconds);
	friend void RenderParticleStringParticles(OpenGLRenderer* renderer, Particles& m_particles, const Rgba& viewColor);

};

extern ParticleSystem* theParticleSystem;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///job helpers

static int g_numUpdateParticleJobsStarted = 0;

inline void UpdateParticleJobFinished(void* arg){
	UNUSED(arg);

	//particle job finished
}

struct UpdateParticleJob : Job{

	//vars
	int startIndex;
	int numParticlesToUpdate;
	double m_deltaSeconds;

	//methods
	UpdateParticleJob(const int& myStartIndex, const int& numParticlesToUpdate, const double& deltaseconds);

	void Execute();

	
protected:
	//Assignment Operator
	const UpdateParticleJob& operator=(const UpdateParticleJob& jobToAssign){
		*this = jobToAssign;

		return *this;
	}

};


//===========================================================================================================

#endif