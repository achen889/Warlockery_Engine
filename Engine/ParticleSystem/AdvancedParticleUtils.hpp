//==============================================================================================================
//AdvancedParticleUtils.hpp
//by Albert Chen Jul-5-2015.
//==============================================================================================================

#pragma once

#include "Engine\ParticleSystem\ParticleSystem.hpp"
#include <vector>

#ifndef _included_AdvancedParticleUtils__
#define _included_AdvancedParticleUtils__

static const float BOUNDING_SPHERE_RADIUS = 0.25f;

struct SpringLink{
	Particle* m_head;
	Particle* m_tail;

	float restingLength;

	SpringLink(Particle* head, Particle* tail){
		m_head = head;
		m_tail = tail;

		Vector3 restingDisplacement = CalcCurrentDisplacement();
		restingLength = restingDisplacement.CalcLength();
	}

	void CalcSpringForceOnParticles();

	Vector3 CalcCurrentDisplacement();

	
	
};


//===========================================================================================================

//single particle node
struct ParticleNode{
	Particle* m_particle;

	// 	ParticleGridNode* m_north;
	// 	ParticleGridNode* m_south;
	// 	ParticleGridNode* m_east;
	// 	ParticleGridNode* m_west;

	Vector2 m_location;

	Sphere3 m_boundingSphere;

	Particles m_linkedParticles;

	ParticleNode();
	~ParticleNode();

	void SetBoundingSphere(){
		m_boundingSphere = Sphere3(m_particle->GetPosition(), m_particle->m_state.radius);
	}

};

typedef std::vector<ParticleNode*>ParticleNodes;

//so far this grid simulates surface water ripples
struct ParticleGrid{
	ParticleNodes m_particleGrid;

	std::vector<SpringLink*> m_springLinks;
	
	int m_rows;
	int m_cols;

	float m_spacing;

	MeshRenderer* m_particleGridRenderer = NULL;

	ParticleGrid();
	~ParticleGrid();

	//takes a moment to complete
	void InitializeParticleGrid(int rows, int cols, float spacing);

	void SetLinkedParticlesInBoundingSphere(ParticleNode& particleNode);
	
	void Update(double deltaSeconds);
	void Render(OpenGLRenderer* renderer, Camera3D& camera);

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline ParticleNode::ParticleNode(){
	
}

inline ParticleNode::~ParticleNode(){

}



#endif