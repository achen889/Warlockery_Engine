//==============================================================================================================
//TheApp.cpp
//by Albert Chen Jan-13-2015.
//==============================================================================================================

#include "TheApp.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include "Engine/Core/Clock.hpp"



///----------------------------------------------------------------------------------------------------------
///
///----------------------------------------------------------------------------------------------------------
TheApp::TheApp(){
	m_inputSystem = NULL;
	m_renderer = NULL;
	m_jobManager = NULL;
	m_textSystem = NULL;
	m_particleSystem = NULL;
	//m_soundSystem = NULL;

	m_isRunning= true;	
}

///----------------------------------------------------------------------------------------------------------
///
///----------------------------------------------------------------------------------------------------------
TheApp:: ~TheApp(){
	
	if (m_renderer){
		delete m_renderer;
		m_renderer = NULL;
	}

	if (m_jobManager){
		delete m_jobManager;
		m_jobManager = NULL;
	}

	if (m_networkSystem) {
		delete m_networkSystem;
		m_networkSystem = NULL;
	}
	
	if (m_textSystem){
		delete m_textSystem;
		m_textSystem = NULL;
	}

	if (m_inputSystem){
		delete m_inputSystem;
		m_inputSystem = NULL;
	}

	if (m_particleSystem){
		delete m_particleSystem;
		m_particleSystem = NULL;
	}

// 	if (m_soundSystem){
// 		delete m_soundSystem;
// 		m_soundSystem = NULL;
// 	}

	if (m_world){
		delete m_world;
		m_world = NULL;
	}

}
///----------------------------------------------------------------------------------------------------------
///
void TheApp::StartUp( void* windowHandle ){
	m_windowHandle = windowHandle;

	InitializeTime();
	InitializeClockSystem();

 	m_jobManager = new JobManager();
	if (m_jobManager){
		m_jobManager->StartUp(0);
	}
	
	m_networkSystem = new NetworkSystem();
	if (m_networkSystem) {
		m_networkSystem->StartUp();
	}

// 	m_soundSystem = new SoundSystem();
// 	if (m_soundSystem){
// 		m_soundSystem->StartUp();
// 	}
	
	Vector2 displaySize;

	//initialize renderer
	m_renderer = new OpenGLRenderer();
	if (m_renderer){
		m_renderer->StartUp((HWND)windowHandle);
		displaySize = Vector2(m_renderer->GetDisplayWidth(), m_renderer->GetDisplayHeight());
	}

	//initialize text renderer
	m_textSystem = new TextSystem(); //mem leak: 648
	if (m_textSystem){
		m_textSystem->StartUp("Data/Fonts/lucidaCL.fnt", m_renderer);
	}

	//initialize input system
	m_inputSystem = new InputSystem();
	if (m_inputSystem){
		m_inputSystem->StartUp(m_windowHandle, displaySize);
	}
		
	m_particleSystem = new ParticleSystem();
	if (m_particleSystem){
		//do something
	}
	
	ConsolePrintf("\n===The App Start Up Completed===\n");

	//initialize world
	m_world = new World(displaySize );
	if (m_world){
		m_world->StartUp();
	}

}

void TheApp::ShutDown(){
	
	if (m_jobManager){
		m_jobManager->ShutDown();
	}

	if (m_networkSystem) {
		m_networkSystem->ShutDown();
	}

	if (m_renderer){
		m_renderer->ShutDown();
	}
	
	if(m_inputSystem){
		m_inputSystem->ShutDown();
	}

// 	if(m_soundSystem){
// 		m_soundSystem->ShutDown();
// 	}

	if (m_textSystem){
		m_textSystem->ShutDown();
	}

	if (m_world){
		m_world->ShutDown();
	}
	
	m_isRunning = false;

}

void TheApp::Run(){
	while( m_isRunning ){
		ProcessInput();

		Update();
		
		RenderWorld();
	}
}

//game loop methods
void TheApp::ProcessInput(){
	if(m_inputSystem ){
		m_inputSystem->Update();
		if(m_inputSystem->IsKeyDown( VK_ESCAPE) )
		   m_inputSystem->SetSystemQuit();

		if(m_inputSystem ->IsReadyToQuit() ){
			m_isRunning = false;
		}
	}

}

bool TheApp::ProcessKeyDown(int keyData ){
	//if(m_world)
	   //return m_world->ProcessKeyDown(keyData);
	UNUSED(keyData);

	return false;
}

bool TheApp::ProcessKeyUp(int keyData ){
	//if(m_world)
		//return m_world->ProcessKeyDown(keyData);
	UNUSED(keyData);

	return false;
}

void TheApp::Update(){
	static double lastTime = GetCurrentSeconds();
	double currentTime = GetCurrentSeconds();
	double deltaSeconds = currentTime - lastTime;
	lastTime = currentTime;

	if(deltaSeconds > 0.5){
		deltaSeconds = 0.5;
	}

	//system clock 
	GetSystemClock().AdvanceTime(deltaSeconds);

	deltaSeconds = GetSystemClock().GetChild("GameClock")->GetDeltaSeconds();

	if(m_world){
		m_world->Update(deltaSeconds );
	}

	if (m_jobManager){
		m_jobManager->Update();
	}

	if (m_networkSystem) {
		m_networkSystem->Update();
	}
	
	//update debug shapes
	if (m_renderer){
		g_debugShapes.Update(deltaSeconds);
	}

// 	if(m_soundSystem){
// 		m_soundSystem->Update();
// 	}

	if (m_textSystem){
		m_textSystem->Update(deltaSeconds);
	}

	if (m_particleSystem){
		m_particleSystem->Update(deltaSeconds);
	}

}

void TheApp::RenderWorld(){
	if (m_renderer){	
		m_renderer->ClearBuffer();
		
		if (m_world){
			m_world->Render(m_renderer);
		}

		if (m_textSystem) {
			m_textSystem->Render();
		}

		m_renderer->SwapBuffers();
	}

	
}