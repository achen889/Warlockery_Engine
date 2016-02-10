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

	VertexArrayObject m_pStringVAO;

	MeshRenderer m_particleStringRenderer;

	ParticleString(){
		//InitializeParticleString(m_particles, 10);
		
		//InitializeParticleStringVAO();
	}

	ParticleString(int numParticles, const Rgba& stringColor = Rgba::WHITE){
		InitializeParticleString(m_particles, numParticles, stringColor);

		m_particleStringRenderer = MeshRenderer();

		m_particleStringRenderer.m_material = new Material();
		m_particleStringRenderer.m_mesh = new Mesh();

		m_particleStringRenderer.m_material->InitializeMaterial("Data/Shaders/basic.vert", "Data/Shaders/basic.frag");
		m_particleStringRenderer.m_material->m_samplerInUse = false;
		m_particleStringRenderer.m_mesh->SetDrawMode(GL_LINE_STRIP);

		m_particleStringRenderer.BindVertexArray();

		//InitializeParticleStringVAO();
	}

	void InitializeParticleString(Particles& m_particles, int numberOfParticles, const Rgba& stringColor = Rgba::WHITE);

	Particle& GetOrigin(){
		return (*m_particles.begin() );
	}

	Particle& GetEnd(){
		return (*m_particles.end() );
	}

	void UpdateParticleString(double deltaSeconds);

	void UpdateParticleStringInput(Particles& m_particles, double deltaSeconds);

	void RenderParticleString(OpenGLRenderer* renderer, const Rgba& viewColor);

	void RenderParticleStringMesh(OpenGLRenderer* renderer, Camera3D& camera, const Rgba& viewColor = Rgba::WHITE);

	void InitializeParticleStringVAO();

	void RenderParticleStringVAO(OpenGLRenderer* renderer, Camera3D& camera, const Rgba& viewColor );

	void GenerateParticleStringVertexArray(Vertex3Ds& out_vertexArray, const Rgba& viewColor);
};

//===========================================================================================================

class ParticleSystem{
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
	
	~ParticleSystem(){
		//do nothing
		g_particleEmitters.clear();
		g_particles.clear();
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
	void ClearAllParticles(){ g_particles.clear(); }
	
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