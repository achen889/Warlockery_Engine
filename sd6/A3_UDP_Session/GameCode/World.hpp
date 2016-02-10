//==============================================================================================================
//World.hpp
//by Albert Chen Mar-26-2015.
//==============================================================================================================

#include "Engine/Math/Math2D.hpp"
#include "Engine/Math/LineSegment2.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/AnimatedTexture.hpp"
#include "Engine/Renderer/Number.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/Text/TextSystem.hpp"
//#include "Engine/Renderer/OGLRenderingUtils.hpp"

//game entities

#include <iostream>
#include <fstream>
#include "Engine/Math/Noise.hpp"
#include "Engine/Renderer/Light.hpp"
#include "Engine/Renderer/GraphicsUtils.hpp"
#include "Engine/Core/BaseGameState.hpp"

#include "Engine/Networking/NetworkSystem.hpp"

class World{
public:
	World(const Vector2& displaySize);
	~World();
	//void LoadStreamingSounds();

	//basic
	void StartUp();
	void ShutDown();

	//upkeep methods
	//updating
	void Update(double deltaSeconds );
	//rendering
	void Render(OpenGLRenderer* renderer);

private:
	
	Vector2 m_displaySize;

	Camera3D* m_mainCamera;
	
	MeshRenderer* m_axesRenderer;
	
	MeshRenderer* m_debugRenderer;

	GameStateRegistry* m_gameStateRegistry;

};

