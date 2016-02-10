//==============================================================================================================
//TheApp.hpp
//by Albert Chen Jan-13-2015.
//==============================================================================================================

#pragma once
#ifndef _included_TheApp__
#define _included_TheApp__

#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "World.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Input/InputSystem.hpp"
//#include "Engine/Sound/SoundSystem.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/ParticleSystem/ParticleSystem.hpp"
#include "Engine/Renderer/Text/TextSystem.hpp"
#include "Engine/Multithreading/JobManager.hpp"
#include "Engine/Core/Memory.hpp"

#include "Engine/Networking/NetworkSystem.hpp"

class TheApp{
public:
	TheApp();
	~TheApp();
	
	void StartUp( void* windowHandle );
	void ShutDown();
	void Run();
	void ProcessInput();
	bool ProcessKeyDown(int keyData );
	bool ProcessKeyUp(int keyData );
	void Update();
	void RenderWorld();

private:
	World* m_world;
	void* m_windowHandle;
	OpenGLRenderer* m_renderer;
	bool m_isRunning;
	InputSystem* m_inputSystem;
	//SoundSystem* m_soundSystem;
	ParticleSystem* m_particleSystem;
	JobManager* m_jobManager;
	TextSystem* m_textSystem;
	Camera3D* m_mainCamera;
	NetworkSystem* m_networkSystem;
};//end of class

#endif