//==============================================================================================================
//AdvancedParticleUtils.cpp
//by Albert Chen Jul-5-2015.
//==============================================================================================================

#include "AdvancedParticleUtils.hpp"
#include "Engine\Math\IntVec2.hpp"
#include "Engine\Input\InputSystem.hpp"
#include <algorithm>
#include "Engine\Renderer\Text\TextSystem.hpp"

static float g_particleSpringSpacing;

ParticleGrid::ParticleGrid(){

	m_particleGridRenderer.m_material->InitializeDefaultMaterial();
	m_particleGridRenderer.m_mesh->SetDrawMode(GL_POINTS);
	m_particleGridRenderer.BindVertexArray();


}

ParticleGrid::~ParticleGrid(){
	//do nothing


}

//-----------------------------------------------------------------------------------------------------------

void ParticleGrid::InitializeParticleGrid(int rows, int cols, float spacing){
	m_rows = rows;
	m_cols = cols;
	m_spacing = spacing;
	g_particleSpringSpacing = spacing;

	//create grid
	for (int r = 0; r < rows; r++){
		for (int c = 0; c < cols; c++){

			ParticleNode particleGridNode;// ParticleNode();
			
			particleGridNode.m_location = Vector2((float)r, (float)c);
							
			particleGridNode.m_particle = Particle(Vector3(ToVector3(particleGridNode.m_location * spacing, 0.0f) ), Vector3(0.0f, 0.0f, 0.0f));
			particleGridNode.m_particle.SetLifeSpanSeconds(-1.0f);
			particleGridNode.m_particle.SetColor(Rgba::BLUE);
			particleGridNode.m_particle.m_state.radius = spacing * 1.1f; //anything over 1 gives neat results, controls how fast ripples propagates
			particleGridNode.m_particle.m_state.LockXY();
			particleGridNode.m_zOffset = particleGridNode.m_particle.GetPosition().z;
							
			particleGridNode.m_particle.SetMass(1.0f); //mass of each particle, affects particle density of the system
							
			particleGridNode.SetBoundingSphere();

			m_particleGrid.push_back(particleGridNode);

		}
	}//nested for

	//check if order matters
	//std::random_shuffle(m_particleGrid.begin(), m_particleGrid.end());
	
	for (std::vector<ParticleNode>::iterator it = m_particleGrid.begin(); it != m_particleGrid.end(); ++it){
		ParticleNode& particleNode = (*it);

		SetLinkedParticlesInBoundingSphere(particleNode);

	}

}

//-----------------------------------------------------------------------------------------------------------

void ParticleGrid::SetLinkedParticlesInBoundingSphere(ParticleNode& particleNode){
	for (std::vector<ParticleNode>::iterator it = m_particleGrid.begin(); it != m_particleGrid.end(); ++it){
		ParticleNode& particleNode1 = (*it);

		if (IsPointInsideSphere(particleNode1.m_particle.GetPosition(), particleNode.m_boundingSphere) ){
			if (particleNode.m_location != particleNode1.m_location){
				m_springLinks.push_back(SpringLink(&particleNode.m_particle, &particleNode1.m_particle));
			}
		}//nested if

	}
}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///spring methods

void SpringLink::CalcSpringForceOnParticles(){
	//CollisionResponseSpring(*m_head, *m_tail, false, 0.02f, 20.0f, 0.5f);
	const float springK = 8.0f;
	const float dampingC = 0.5f;

	float L = CalcDistance(m_tail->GetPosition(), m_head->GetPosition());
	L = abs(L);
	//L = Clamp(L, 0.0f, 0.2f);

	const float L0 = g_particleSpringSpacing;

	float displacementLL0 = abs(L - L0);
	displacementLL0 = Clamp(displacementLL0, 0.0f, L0);

	Vector3 eOfPoints = m_tail->GetPosition() - m_head->GetPosition();
	eOfPoints.Normalize();

	//Vector3 springDampingForce = (dampingC * m_tail->GetVelocity());
	Vector3 springDampingForce = -dampingC * DotProduct(eOfPoints, (m_tail->GetVelocity() - m_head->GetVelocity())) * eOfPoints;

	static Vector3 externalForces = Vector3::ZERO;

	//final spring force
	Vector3 forceSpring = externalForces + ( springK * (displacementLL0 * eOfPoints)) + (springDampingForce );
	m_tail->AddForce(forceSpring);

	//newton's third law //given that all the springs work both ways, this causes instability
	//m_head->AddForce(-1.0f * m_tail->GetForces());
	 
// 	CollisionResponseSpring(*m_tail, *m_head, false, 0.02f, 20.0f, 0.5f);
// 
// 	//newton's third law
// 	m_tail->AddForce(-1.0f * m_head->GetForces());
}


//-----------------------------------------------------------------------------------------------------------
Vector3 SpringLink::CalcCurrentDisplacement(){
	return m_head->GetPosition() - m_tail->GetPosition();
}

//===========================================================================================================

void ParticleGrid::Update(double deltaSeconds){
	//PROFILE_SECTION();

	if (theInputSystem->WasKeyJustReleased('0')){
		for (ParticleNodesIterator pIter = m_particleGrid.begin(); pIter != m_particleGrid.end(); ++pIter){
			ParticleNode& particleNode = (*pIter);
			particleNode.m_particle.SetForce(Vector3::ZERO);
		}
	}

// 	//lock/unlock xy
	if (theInputSystem->WasKeyJustReleased('X')){
		for (ParticleNodesIterator pIter = m_particleGrid.begin(); pIter != m_particleGrid.end(); ++pIter){
		 	ParticleNode& particleNode = (*pIter);
			if (particleNode.m_particle.m_state.lockX) {
				particleNode.m_particle.m_state.UnlockXY();
			}						   
			else {
					particleNode.m_particle.m_state.LockXY();
			}
			
		 	//particleNode.m_particle->SetForce(Vector3::ZERO);
 		}
	}

	//affect the grid
	if (theInputSystem->WasKeyJustReleased('1')){
		int randomVectorIndex = GetRandomIntInRange(0, m_particleGrid.size() - 1);
		ParticleNode& affectedNode = m_particleGrid[randomVectorIndex];
		affectedNode.m_particle.SetPosition(affectedNode.m_particle.GetPosition() - Vector3(0.0f, 0.0f, 0.1f));
		
		//color code disturbed particle
		//m_particleGrid[randomVectorIndex]->m_particle->SetColor(Rgba::RED);
	}

	
	//accumulate all the forces in the spring system
	for (std::vector<SpringLink>::iterator sIter = m_springLinks.begin(); sIter != m_springLinks.end(); ++sIter){
		SpringLink& spring = (*sIter);

		spring.CalcSpringForceOnParticles();

		//add in equilibrium force
		const float envK = 10.0f;
		const float equilibriumZ0 = 0.0f;
		const float envDampingC = 4.0f;

		Particle& headParticle = *spring.m_head;

		float envEquilibriumForce = (-envK * (headParticle.GetPosition().z - equilibriumZ0) ) - (envDampingC * headParticle.GetVelocity().z);
		if (envEquilibriumForce != 0.0f)
			headParticle.AddForce(Vector3(0.0f, 0.0f, envEquilibriumForce));

	}

	//integrate the motion in the system
	//std::random_shuffle(m_particleGrid.begin(), m_particleGrid.end());
	for (ParticleNodesIterator pIter = m_particleGrid.begin(); pIter != m_particleGrid.end(); ++pIter){
		ParticleNode& particleNode = (*pIter);
		Particle& particle = (particleNode.m_particle);

		//pure verlet integration
		UpdateParticleVerletIntegration(particle, deltaSeconds);
	
		//standard integration uses symplectic
		//particleNode.m_particle->Update(deltaSeconds);

		const float zConstraint = 0.5f;
		//clamp to Z min and max
		if (particleNode.m_particle.GetPosition().z > zConstraint) particleNode.m_particle.m_state.vertex.m_position.z = zConstraint;
		if (particleNode.m_particle.GetPosition().z < -zConstraint) particleNode.m_particle.m_state.vertex.m_position.z = -zConstraint;

		//set zOffset
		particleNode.m_zOffset = particleNode.m_particle.GetPosition().z;
		//attempt to lerp on color
		float lerpFractionOfNode = Interpolate(-zConstraint, zConstraint, particleNode.m_zOffset);
		lerpFractionOfNode = GetLoopedParametric(lerpFractionOfNode);
		Rgba lerpedColor = m_colorOverZOffset.GetValueAtParametric(lerpFractionOfNode); 
		//Rgba lerpedColor = Interpolate(Rgba::BLUE, Rgba::TRON_BLUE, lerpFractionOfNode);
		particleNode.m_particle.SetColor(lerpedColor);
	}

	//constraint relaxation
	for (std::vector<SpringLink>::iterator sIter = m_springLinks.begin(); sIter != m_springLinks.end(); ++sIter){
		SpringLink& spring = (*sIter);

		Vector3 deltaVec = spring.CalcCurrentDisplacement();
		float deltaLength = deltaVec.CalcLength();
		float diff = (deltaLength -spring.restingLength) / deltaLength;

		Vector3 relaxationVec = deltaVec* 0.5f * diff;

		spring.m_head->SetPosition( spring.m_head->GetPosition() - relaxationVec);
		spring.m_tail->SetPosition(spring.m_tail->GetPosition() + relaxationVec);

	}


}

//-----------------------------------------------------------------------------------------------------------


void ParticleGrid::Render(OpenGLRenderer* renderer, Camera3D& camera){
	UNUSED(renderer);
	Vertex3Ds particlesVertexArray;

	particlesVertexArray.clear();
	particlesVertexArray.reserve(m_rows * m_cols);

	//m_particleGridRenderer.m_mesh->SetDrawMode(GL_LINE_STRIP);
	for (ParticleNodesIterator pIter = m_particleGrid.begin(); pIter != m_particleGrid.end(); ++pIter){
		ParticleNode& particleNode = (*pIter);

		if (theParticleSystem){
			particlesVertexArray.push_back(theParticleSystem->GenerateParticleVertexPoint(particleNode.m_particle));
			//GenerateVertexArraySphere3D(particlesVertexArray, Sphere3(particleNode.m_particle->GetPosition(), particleNode.m_particle->m_state.radius), 6, 6, Rgba::BLUE);
		}

	}

	m_particleGridRenderer.m_mesh->CopyMeshVertexData(particlesVertexArray);
	m_particleGridRenderer.RenderMesh(camera, true);

}

 
