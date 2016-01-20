//==============================================================================================================
//ParticleSystem.cpp
//by Albert Chen May-29-2015.
//==============================================================================================================

#include "ParticleSystem.hpp"
#include "Engine\Console\DevConsole.hpp"

//===========================================================================================================

ParticleSystem* theParticleSystem = NULL;

//===========================================================================================================

UpdateParticleJob::UpdateParticleJob(const int& myStartIndex, const int& numParticlesToUpdate, const double& deltaseconds) :
numParticlesToUpdate(numParticlesToUpdate),
startIndex(myStartIndex),
m_deltaSeconds(deltaseconds)
{

	m_jobCompleteCallback = new JobCompleteCallback(UpdateParticleJobFinished, NULL);
}

void UpdateParticleJob::Execute(){

	if (theParticleSystem){

		Particles& particles = theParticleSystem->g_particles;

		for (ParticlesIterator pIter = particles.begin() + startIndex; pIter != particles.begin() + startIndex + numParticlesToUpdate;){
			
			Particle& particle = *(*pIter);
			
			if (particle.IsDead()){
				pIter = particles.erase(pIter);
			}
			else{
				particle.Update(m_deltaSeconds);

				++pIter; //vector iterator not incrementable //pIter is NULL this time, this time data is garbage...some sort of pointer collision maybe
				//somehow two threads get the same update particle job this time
			}

		}//end of for

		
	}

}

//===========================================================================================================

//query commands

CONSOLE_COMMAND(numParticles){
	UNUSED_COMMAND_ARGS
	unsigned int pTotal = theParticleSystem->GetTotalParticles();

	std::string pTotalText = "Particles in World = ";
	pTotalText += IntToString(pTotal) + "\n";

	ConsoleGenericMessageBox(pTotalText);
}

CONSOLE_COMMAND(GET_EMITTERS){
	UNUSED_COMMAND_ARGS
	if (theParticleSystem){
		std::string myEmitterDataText = "=====Particle Emitter Data=====\n";
		if (!theParticleSystem->g_particleEmitters.empty()){
			for (ParticleEmitterMapIterator it = theParticleSystem->g_particleEmitters.begin(); it != theParticleSystem->g_particleEmitters.end(); ++it){
				myEmitterDataText += "Name: "+it->first + "\n";
				myEmitterDataText += "Position: " + it->second->m_position.ToString() + "\n";
				myEmitterDataText += "Effect: " + it->second->m_effect->GetEffectTypeString() + "\n";
				myEmitterDataText += "Particles Per Tick: " + IntToString(it->second->m_particlesPerSpawn) + "\n\n";

			}

			ConsoleGenericMessageBox(myEmitterDataText);
		}//end of is empty
	}
}

//creation commands

CONSOLE_COMMAND(createExplosion){
	Vector3 expPosVec3 = Vector3::ZERO;
	unsigned int maxParticles = 1000;
	float lifespanExp = -1.0f;
	unsigned int intervalsExp = 10;

	if (COMMAND_HAS_ARGS){
		if (NUMBER_OF_ARG_TOKENS >= 3){

			expPosVec3.x = ARG_TOKEN_F(0);
			expPosVec3.y = ARG_TOKEN_F(1);
			expPosVec3.z = ARG_TOKEN_F(2);

			if (NUMBER_OF_ARG_TOKENS >= 4){
				maxParticles = (unsigned int)ARG_TOKEN_I(3);

				if (NUMBER_OF_ARG_TOKENS >= 5){
					lifespanExp = ARG_TOKEN_F(4);

					if (NUMBER_OF_ARG_TOKENS >= 6){
						intervalsExp = (unsigned int)ARG_TOKEN_I(5);
					}
				}
			}
		}
	}
	

	if (theParticleSystem){
		theParticleSystem->AddEmitter("ConsoleExplosion", EFFECT_TYPE_EXPLOSION, expPosVec3, maxParticles, lifespanExp, intervalsExp);
	}
	


}

CONSOLE_COMMAND(CREATE_FOUNTAIN){
	Vector3 expPosVec3 = Vector3::ZERO;
	unsigned int maxParticles = 1000;
	float lifespanExp = -1.0f;
	unsigned int intervalsExp = 10;

	if (COMMAND_HAS_ARGS){
		if (NUMBER_OF_ARG_TOKENS >= 3){

			expPosVec3.x = ARG_TOKEN_F(0);
			expPosVec3.y = ARG_TOKEN_F(1);
			expPosVec3.z = ARG_TOKEN_F(2);

			if (NUMBER_OF_ARG_TOKENS >= 4){
				maxParticles = (unsigned int)ARG_TOKEN_I(3);

				if (NUMBER_OF_ARG_TOKENS >= 5){
					lifespanExp = ARG_TOKEN_F(4);

					if (NUMBER_OF_ARG_TOKENS >= 6){
						intervalsExp = (unsigned int)ARG_TOKEN_I(5);
					}
				}
			}
		}
	}

	

	if (theParticleSystem){
		theParticleSystem->AddEmitter("ConsoleFountain", EFFECT_TYPE_FOUNTAIN, expPosVec3, maxParticles, lifespanExp, intervalsExp);
	}
}

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///global variables
//Particles ParticleSystem::g_particles;
ParticleEmitterMap ParticleSystem::g_particleEmitters;

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///constructors
ParticleSystem::ParticleSystem(){
	//InitializeParticleVAO();
	InitializeParticleMeshRenderer();
	
	RegisterParticleCommands();
	
	if (!theParticleSystem){
		theParticleSystem = this;
	}
}

//-----------------------------------------------------------------------------------------------------------

void ParticleSystem::InitializeParticleMeshRenderer(){
	//m_particleRenderer.m_material = new Material();
	m_particleRenderer = new MeshRenderer(); //mem leak: 12
	m_particleRenderer->m_material->InitializeMaterial("Data/Shaders/basic.vert", "Data/Shaders/basic.frag");
	m_particleRenderer->m_material->m_samplerInUse = false;
	m_particleRenderer->m_mesh->SetDrawMode(GL_POINTS);
	
	m_particleRenderer->BindVertexArray();

}

//-----------------------------------------------------------------------------------------------------------

void ParticleSystem::UpdateParticleInMesh(){
	//PROFILE_SECTION();
	Vertex3Ds particlesVertexArray;
	particlesVertexArray.clear();

	GenerateParticlesVertexArray(particlesVertexArray);
	
	m_particleRenderer->m_mesh->CopyMeshVertexData(particlesVertexArray);


}

//-----------------------------------------------------------------------------------------------------------

void ParticleSystem::RegisterParticleCommands(){
	//query commands
	REGISTER_CONSOLE_COMMAND(numParticles, "Display Number of Particles in world.");
	REGISTER_CONSOLE_COMMAND(GET_EMITTERS, "Displays Emitters in world.");
	//creation commands
	REGISTER_CONSOLE_COMMAND(createExplosion, "Creates an explosion effect.");
	REGISTER_CONSOLE_COMMAND(CREATE_FOUNTAIN, "Creates a fountain effect.");

	//destruction commands

}

//-----------------------------------------------------------------------------------------------------------

void ParticleSystem::InitializeParticleVAO(){
	if (theOGLRenderer){
		m_particleVAO.SetDrawMode(GL_POINTS);
		//GenerateVertexArrayCoordinateAxes(gVAOP.m_vertexArray);
		theOGLRenderer->InitializeVAO(m_particleVAO, "Data/Shaders/p_special.vert", "Data/Shaders/p_special.frag");
		theOGLRenderer->GL_CHECK_ERROR();
	}
}

//===========================================================================================================

void ParticleSystem::Update(double deltaSeconds){
	//PROFILE_SECTION();

	//Update Emitter
	for (ParticleEmitterMapIterator it = g_particleEmitters.begin(); it != g_particleEmitters.end(); ++it ){
		//if (g_particleEmitters.empty())break;

		Emitter& emitter = (*it->second);
		if (!emitter.IsDead()){
			emitter.Update(deltaSeconds, g_particles);
			
		}
	}//end of emitter update

	UpdateParticles(deltaSeconds);
	

}

//-----------------------------------------------------------------------------------------------------------

void ParticleSystem::UpdateParticles(double deltaSeconds){
	//PROFILE_SECTION();

	//update particles
	for (ParticlesIterator pIter = g_particles.begin(); pIter != g_particles.end();){
		if (g_particles.empty())break;
		Particle& particle = (**pIter);

		if (particle.IsDead()){
			pIter = g_particles.erase(pIter);
		}
		else{
			particle.Update(deltaSeconds);

			++pIter;
		}

	}//end of particle update
	
	//still full of bugs
	//if (theJobManager->GetActiveJobCount() == 0){
	//	UpdateParticlesMultithreaded(deltaSeconds);
	//}
	

}

//-----------------------------------------------------------------------------------------------------------

void ParticleSystem::UpdateParticlesMultithreaded(double deltaSeconds){
	//multi thread particles attempt
	if (!g_particles.empty()){
		size_t totalParticles = g_particles.size();
		Job* newJob = NULL;

		int numParticles = (int)((float)g_particles.size() * 0.1f);

		//cap particles size
		if (g_particles.size() < (unsigned int)numParticles){
			numParticles = g_particles.size();
		}

		int startIndex = 0;
		while ((unsigned int)startIndex < totalParticles - (unsigned int)numParticles - 1){
			if (theJobManager){
				newJob = new UpdateParticleJob(startIndex, numParticles, deltaSeconds);
				theJobManager->AddPendingJob(newJob, PRIORITY_HIGH);
			}
			startIndex += numParticles;
		}//end of while
	}

}

//-----------------------------------------------------------------------------------------------------------

unsigned int ParticleSystem::GetTotalParticles(){
	return g_particles.size();
}

///-----------------------------------------------------------------------------------------------------------
///particles with GLBegin
void ParticleSystem::Render(OpenGLRenderer* renderer){
	if (!g_particles.empty()){
		//load all particle positions
		Vector3s particlePoints;
		for (ParticlesIterator pIter = g_particles.begin(); pIter != g_particles.end(); ++pIter){
			if (g_particles.empty())break;
			Particle& particle = (**pIter);
			if (!particle.IsDead()){
				particlePoints.push_back(particle.GetPosition());
			}
			//if (particle != nullptr){	
			//}
			//particle->Draw(renderer);
		}//end of load points for

		//render the points
		renderer->SetModelViewColor(GetRandomColor());
		renderer->DrawPoints3D(particlePoints);
// 		for (int i = 0; i < particlePoints.size(); i++){
// 			renderer->DrawSphere3D(Sphere3(particlePoints[i],0.01f), 3, 3 );
// 		}

	}//end of is empty
}

///----------------------------------------------------------------------------------------------------------
///"modern" rendering particles
void ParticleSystem::RenderParticlesMesh(OpenGLRenderer* renderer, Camera3D& camera, bool isPerspective ){

	//PROFILE_SECTION();

	UNUSED(renderer);

	//ConsolePrintString(" " + IntToString(g_numUpdateParticleJobsStarted));

	//if (g_numUpdateParticleJobsStarted == 0){
		UpdateParticleInMesh();

		m_particleRenderer->RenderMesh(camera, isPerspective);
	//}

}

///----------------------------------------------------------------------------------------------------------
///queries

bool ParticleSystem::HasEmitterByName(const std::string& emitterName){
	ParticleEmitterMapIterator it = g_particleEmitters.find(emitterName);

	return (it != g_particleEmitters.end());
}

//-----------------------------------------------------------------------------------------------------------

Emitter& ParticleSystem::GetEmitterByName(const std::string& emitterName){
	ParticleEmitterMapIterator it = g_particleEmitters.find(emitterName);

	if (!HasEmitterByName(emitterName)){
		std::string errorPartText = "ERROR: Particle System Does not contain " + emitterName + "\n";
		std::string errorPartTitle = "DEBUG: Particle Emitter not found.\n"+errorPartText;
		ERROR_RECOVERABLE(errorPartTitle);
		//ConsoleGenericMessageBox(errorPartText, errorPartTitle);
	}

	return (*it->second);
}

//-----------------------------------------------------------------------------------------------------------

void ParticleSystem::AddEmitter(const std::string& emitterName, const EffectType& newEffectType, const Vector3& position, const unsigned int& particleMax,
	const float& lifespanSeconds , const unsigned int& spawnIntervals ){

	delete g_particleEmitters[emitterName];

	g_particleEmitters[emitterName] = new Emitter(newEffectType, position, particleMax, lifespanSeconds, spawnIntervals);

}

//-----------------------------------------------------------------------------------------------------------

void ParticleSystem::AddEmitter(const std::string& emitterName, const Emitter& newEmitter){
	delete g_particleEmitters[emitterName];

	g_particleEmitters[emitterName] = new Emitter(newEmitter);
}

//===========================================================================================================

void ParticleSystem::CalcForcesOnParticles(){
// 	for (ParticlesIterator pIter = g_particles.begin(); pIter != g_particles.end(); ++pIter){
// 		Particle& particle = (**pIter);
// 
// 		
// 		//calc forces
// 		//Effect* currentEffect;
// 	}
}

//===========================================================================================================

void ParticleSystem::GenerateVertexArrayAndVBO(){

	Vertex3Ds vertexArray;
	GenerateParticlesVertexArray(vertexArray);
	//m_vboID = 0;
	glGenBuffers(2, &m_vboID);

	m_numOFVBOVertexes = sizeof(Vertex3D) * vertexArray.size();
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, m_numOFVBOVertexes, vertexArray.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//m_isVboDirty = false;
}

//-----------------------------------------------------------------------------------------------------------

void ParticleSystem::GenerateParticlesVertexArray(Vertex3Ds& out_vertexArray){
	//PROFILE_SECTION();

	//push stuff back to the out vertex
	if (!g_particles.empty()){
		out_vertexArray.clear();
		out_vertexArray.reserve(g_particles.size());
		int i = 0;
		for (ParticlesIterator pIter = g_particles.begin(); pIter != g_particles.end(); ++pIter ){
			 Particle& particle = *(*pIter);
			 if (particle.IsDead()){ 
				 continue;
			 }
			 //create a single particle vertex3D point
			 Vertex3D& particleVertexData = particle.m_state.vertex;

			 out_vertexArray.push_back(particleVertexData);
			 i++;
		}
	}
}

//-----------------------------------------------------------------------------------------------------------

const Vertex3D ParticleSystem::GenerateParticleVertexPoint(Particle& particle){
	//PROFILE_SECTION();
	
	//create a single particle vertex3D point
	Vertex3D particleVertexData = Vertex3D();
	particleVertexData.m_position = particle.GetPosition();
	particleVertexData.m_color = particle.GetColor();
	particleVertexData.m_texCoords = Vector2::ZERO;
	particleVertexData.m_normal = Vector3::ZERO;

	return particleVertexData;
}

//-----------------------------------------------------------------------------------------------------------

void ParticleSystem::DrawVBO(){
	//glDisable(GL_CULL_FACE);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex3D), (const GLvoid*)offsetof(Vertex3D, m_position));
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex3D), (const GLvoid*)offsetof(Vertex3D, m_color));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex3D), (const GLvoid*)offsetof(Vertex3D, m_texCoords));

	glDrawArrays(GL_POINTS, 0, m_numOFVBOVertexes);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//-----------------------------------------------------------------------------------------------------------

void ParticleSystem::RenderWithVBOs(){
	// Sample code from prof Eiserloh which enables, draws, and the re-disables a set of
	//	interleaved VBOs (Vertex Buffer Objects), modified by Albert Chen
	//if (m_isVboDirty){
	GenerateVertexArrayAndVBO();
	//}

	//glPushMatrix();

	// 	glEnable( GL_TEXTURE_2D );
	// 	glBindTexture( GL_TEXTURE_2D, g_theTextureAtlas->GetPlatformHandle() );

	DrawVBO();

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glPopMatrix();
	
}

//===========================================================================================================

void ParticleSystem::RenderWithVAO( OpenGLRenderer* PRenderer, Camera3D& camera){
	PRenderer->RenderParticlesWithVAO(m_particleVAO, camera);
}

//===========================================================================================================


//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///particle string methods

void ParticleString::InitializeParticleString(Particles& m_particles, int numberOfParticles, const Rgba& stringColor ){
	//m_particles.push_back(new Particle(Vector3::ZERO, Vector3::ZERO));
	m_particles.push_back(new Particle(GetRandomVector3InRange(0.0f, 1.3f), Vector3::ZERO));
	m_particles.back()->SetLifeSpanSeconds(-1.0f);
	m_particles.back()->m_state.SetMass(GetRandomFloatInRange(0.01f, 0.03f)); //it becomes unstable at high mass
	m_particles.back()->SetColor(stringColor);

	for (int i = 1; i < numberOfParticles-1; i++){
		//m_particles.push_back(new Particle(Vector3::ZERO, Vector3::ZERO));
		m_particles.push_back(new Particle(GetRandomVector3InRange(0.0f, 0.3f), Vector3::ZERO));
		m_particles.back()->SetLifeSpanSeconds(-1.0f);
		m_particles.back()->m_state.SetMass(GetRandomFloatInRange(0.01f, 0.03f)); //it becomes unstable at high mass
		m_particles.back()->SetColor(stringColor);
	}

	m_particles.push_back(new Particle(GetRandomVector3InRange(0.0f, 1.3f), Vector3::ZERO));
	m_particles.back()->SetLifeSpanSeconds(-1.0f);
	m_particles.back()->m_state.SetMass(GetRandomFloatInRange(0.01f, 0.03f)); //it becomes unstable at high mass
	m_particles.back()->SetColor(stringColor);
}

void ParticleString::InitializeParticleStringVAO(){
	m_pStringVAO.SetDrawMode(GL_LINE_STRIP);
	//GenerateVertexArrayCoordinateAxes(gVAOAxes.m_vertexArray);
	if (theOGLRenderer){
		theOGLRenderer->InitializeVAO(m_pStringVAO, "Data/Shaders/basic.vert", "Data/Shaders/basic.frag");
	}
}

void ParticleString::UpdateParticleString(double deltaSeconds){
	UpdateParticleStringInput(m_particles, deltaSeconds);

	for (ParticlesIterator pIter = m_particles.begin(); pIter != m_particles.end();){
		Particle& particle = (**pIter);

		//spring coll with particle neighbor
		if (pIter + 1 != m_particles.end()){
			Particle& particle2 = (**std::next(pIter, 1));

			
			//unstable at very low L0, high K //good values 0.15, 15.5, 0.5
			CollisionResponseSpring(particle, particle2, true, 0.15f, 15.5f, 0.5f);
		}

// 		const float groundLv = 0.0f;
// 
// 		if (IsParticleAtGroundLevel(particle, groundLv)){
// 			//particle.m_state.velocity = Vector3::ZERO;
// 			Particle groundDummy = Particle(particle.GetPosition(), Vector3::ZERO);
// 			groundDummy.SetMass(10.0f);
// 			CollisionResponseBounce(particle, groundDummy, 0.5f, deltaSeconds);
// 		}

		
		
		++pIter;
	}//end of for loop

	for (ParticlesIterator pIter = m_particles.begin(); pIter != m_particles.end(); ++pIter){
		Particle& particle = (**pIter);

		particle.Update(deltaSeconds);

	}
}

void ParticleString::RenderParticleString(OpenGLRenderer* renderer, const Rgba& viewColor){
	for (ParticlesIterator pIter = m_particles.begin(); pIter != m_particles.end(); ++pIter){
		Particle& particle = (**pIter);

		if (pIter + 1 != m_particles.end()){
			Particle& particle2 = (**std::next(pIter, 1));

			renderer->DrawLineSegment3D(LineSegment3(particle.GetPosition(), particle2.GetPosition()), viewColor);
		}

		//renderer->DrawSphere3D(Sphere3(particle.GetPosition(), 0.01f), 12, 12, Rgba::MAGENTA );
	}
}

void ParticleString::RenderParticleStringMesh(OpenGLRenderer* renderer, Camera3D& camera, const Rgba& viewColor){
	UNUSED(renderer);
	UNUSED(viewColor);
	Vertex3Ds particlesVertexArray;

	particlesVertexArray.clear();
	particlesVertexArray.reserve(10000);

	//m_particleGridRenderer.m_mesh->SetDrawMode(GL_LINE_STRIP);


	//GenerateParticleStringVertexArray(particlesVertexArray, viewColor);
	for (ParticlesIterator pIter = m_particles.begin(); pIter != m_particles.end(); ++pIter){
		Particle& particle = (**pIter);

		if (theParticleSystem){
			particlesVertexArray.push_back(theParticleSystem->GenerateParticleVertexPoint(particle));
			//GenerateVertexArraySphere3D(particlesVertexArray, Sphere3(particleNode.m_particle->GetPosition(), particleNode.m_particle->m_state.radius), 6, 6, Rgba::BLUE);
		}

	}

	m_particleStringRenderer.m_mesh->CopyMeshVertexData(particlesVertexArray);
	m_particleStringRenderer.BindVertexArray();

	m_particleStringRenderer.RenderMesh(camera, true);
}

void ParticleString::RenderParticleStringVAO(OpenGLRenderer* renderer, Camera3D& camera, const Rgba& viewColor ){
	VertexArrayObject& myVAO = m_pStringVAO;

	GenerateParticleStringVertexArray(myVAO.m_vertexArray, viewColor);
	
	myVAO.m_vboID = renderer->CreateBuffer(myVAO.m_vboID, myVAO.m_vertexArray.data(), myVAO.CalcVAOSize());
	renderer->CreateVertexArrayWithShader(myVAO.m_Program, myVAO.m_vboID, myVAO.m_vaoID);

	renderer->PrepareVAOForRendering(myVAO, camera, true);

	renderer->DrawVertexArray(myVAO.m_drawMode, myVAO.m_vaoID, myVAO.m_vertexArray.size());

	renderer->DisableShaderProgram();
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	renderer->DestroyVAO(myVAO);
}

void ParticleString::GenerateParticleStringVertexArray(Vertex3Ds& out_vertexArray, const Rgba& viewColor ){
	out_vertexArray.clear();
	out_vertexArray.reserve(10000);
	//push stuff back to the out vertex
	
	for (ParticlesIterator pIter = m_particles.begin(); pIter != m_particles.end(); ++pIter){

		Particle& particle = (**pIter);
		Vertex3D particleVertexData;
		particleVertexData.m_position = particle.GetPosition();
		particleVertexData.m_color = viewColor;//particle.GetColor();
		particleVertexData.m_texCoords = Vector2::ZERO;

		out_vertexArray.push_back(particleVertexData);
	}
	
}

void ParticleString::UpdateParticleStringInput(Particles& m_particles, double deltaSeconds){
	const float moveSpeed = -4.5f;
	Particle& particleToMove = (*m_particles[0]);
	if (theInputSystem){
		if (theInputSystem->IsKeyDown('I')){
			Vector3 newPos = particleToMove.GetPosition();
			newPos.x += -moveSpeed * (float)deltaSeconds;
			particleToMove.SetPosition(newPos);
		}
		if (theInputSystem->IsKeyDown('K')){
			Vector3 newPos = particleToMove.GetPosition();
			newPos.x -= -moveSpeed  * (float)deltaSeconds;
			particleToMove.SetPosition(newPos);
		}
		if (theInputSystem->IsKeyDown('J')){
			Vector3 newPos = particleToMove.GetPosition();
			newPos.y += moveSpeed  * (float)deltaSeconds;
			particleToMove.SetPosition(newPos);
		}
		if (theInputSystem->IsKeyDown('L')){
			Vector3 newPos = particleToMove.GetPosition();
			newPos.y -= moveSpeed * (float)deltaSeconds;
			particleToMove.SetPosition(newPos);
		}

		if (theInputSystem->IsKeyDown('O')){
			Vector3 newPos = particleToMove.GetPosition();
			newPos.z += moveSpeed  * (float)deltaSeconds;
			particleToMove.SetPosition(newPos);
		}
		if (theInputSystem->IsKeyDown('U')){
			Vector3 newPos = particleToMove.GetPosition();
			newPos.z -= moveSpeed * (float)deltaSeconds;
			particleToMove.SetPosition(newPos);
		}
	}

}

//===========================================================================================================


