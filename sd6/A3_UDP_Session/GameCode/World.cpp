//==============================================================================================================
//World.cpp
//by Albert Chen Mar-26-2015.
//==============================================================================================================

#include "World.hpp"
#include "Engine\ParticleSystem\ParticleSystem.hpp"
#include "Engine\Console\DevConsole.hpp"
#include "Engine\Core\Clock.hpp"

//===========================================================================================================

World::World(const Vector2& displaySize):
	m_displaySize(displaySize)
{
	//do something
}

//-----------------------------------------------------------------------------------------------------------

World::~World(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

Vector3 startPosition = Vector3(-3.0f, -1.0f, 0.5f);
EulerAngles startOrientation = EulerAngles(0.0f, 0.0f, 0.0f);

void World::StartUp(){
	
	if (theInputSystem){
		theInputSystem->HideMouseCursor();
	}

	m_axesRenderer = new MeshRenderer();
	m_axesRenderer->InitializeCoordinateAxesMeshRenderer();

	m_mainCamera = new Camera3D(startPosition, startOrientation);

	//theParticleSystem->AddEmitter("pTest", EFFECT_TYPE_EXPLOSION, Vector3::ZERO, 1000, -1.0f, 10);

	SetCurrentGameState("Loading");

	//ConsolePrintf("size of entity = %d\n", SIZE_OF(Entity));

	//for debugging
	//m_gameStateRegistry = GameStateRegistration::GetGameStateRegistry();

	//CalcPath(m_path, m_map, m_map->GetRandomMapPosition(), m_map->GetRandomMapPosition());

	//run command from code macro
	EXECUTE_CONSOLE_COMMAND(create_session 1234);

	EXECUTE_CONSOLE_COMMAND(add_connection MEW77-PC 1777);

	EXECUTE_CONSOLE_COMMAND(add_connection 10.8.138.31 12345);

	//EXECUTE_CONSOLE_COMMAND(add_connection 10.8.154.107 111);
	
	//EXECUTE_CONSOLE_COMMAND(add_connection MEW77-PC 1678);

}

void World::ShutDown(){
	//shut down world

	if (m_mainCamera){
		delete m_mainCamera;
		m_mainCamera = NULL;
	}
}

//-----------------------------------------------------------------------------------------------------------

void World::Update(double deltaSeconds ){
	PROFILE_SECTION();

	//GetSystemClock().GetChild("GameClock")->SetTimeScale(2.0f);

	if (!theTextSystem->m_IsDevelopmentConsoleActive){
		m_mainCamera->UpdateCameraFromInput((float)deltaSeconds);

		IF_INPUT_SYSTEM_EXISTS{
			IF_INPUT_IS_KEY_RELEASED('P'){
				GetSystemClock().GetChild("GameClock")->TogglePause();
			}
			ProcessInputCurrentState(deltaSeconds);
		}
		UpdateCurrentState(deltaSeconds);

	}
}

//-----------------------------------------------------------------------------------------------------------

void World::Render(OpenGLRenderer* renderer){
	PROFILE_SECTION();
	//rendering settings
	renderer->SetTextureViewTransparent();
	renderer->SetModelViewDepthTest();
	//renderer->DisableModelCullFaceMode();
	renderer->SetModelCullFaceMode(GL_BACK);

	m_axesRenderer->RenderMesh(*m_mainCamera, true);
	
	RenderCurrentState(renderer);

	if (theParticleSystem){
		theParticleSystem->RenderParticlesMesh(renderer, *m_mainCamera, true);
	}
	
	OUTPUT_STRING_TO_SCREEN("FPS: " + IntToString((int)GetFramesPerSecond()), 50, 850);
	
	RenderProfileMapToScreen();
	
}

//===========================================================================================================



