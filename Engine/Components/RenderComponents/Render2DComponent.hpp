//==============================================================================================================
//Render2DComponent.hpp
//by Albert Chen Jan-26-2016.
//==============================================================================================================

#pragma once

#ifndef _included_Render2DComponent__
#define _included_Render2DComponent__

#include "Engine/Math/AABB2.hpp"

#include "RenderComponent.hpp"

class Render2DComponent : public RenderComponent{
public:
	Render2DComponent(){

	}
	~Render2DComponent();
	Render2DComponent(const std::string& name);

	Render2DComponent(const XMLNode& node);

	static BaseComponent* CreateComponent(const std::string& name){ return new Render2DComponent(name); }

	static ComponentRegistration s_Render2DComponentRegistration;
	
	virtual void OnCreate();
	virtual void OnDestroy();
	
	//rendering helpers
	void SetRenderBounds();

	void SetRenderMesh2D(const Vector2& screenPosition, const Vector2& newSize, const Rgba& elementColor, const std::string& diffuseTexturePath = "", const std::string& shaderName = "basicSampler");

	//must use model view matrix to change render position
	virtual void Render(OpenGLRenderer* renderer, Camera3D& camera, bool isPerspective = true, Lights* lights = NULL, ModelViewMatrix* modelView = NULL);

private:
	//vars
	AABB2 m_renderBounds;
	Vector2 m_renderCoordinates;
	Vector2 renderScale; //sphere uses renderSize.x

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline Render2DComponent::~Render2DComponent(){
	//do nothing
}

inline void Render2DComponent::SetRenderBounds(){
	Vector2 renderMins = Vector2(m_renderCoordinates);
	Vector2 renderMaxs = Vector2(m_renderCoordinates + renderScale);

	m_renderBounds = AABB2(renderMins, renderMaxs);
}

//===========================================================================================================

#endif