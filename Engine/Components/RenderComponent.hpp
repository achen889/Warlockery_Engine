//==============================================================================================================
//RenderComponent.hpp
//by Albert Chen Aug-25-2015.
//==============================================================================================================

#pragma once

#ifndef _included_RenderComponent__
#define _included_RenderComponent__

#include "BaseComponent.hpp"
#include "Engine/Renderer/OGLRenderingUtils.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Math/AABB2.hpp"

class RenderComponent : public BaseComponent{
public:
	RenderComponent(){

	}
	~RenderComponent();
	RenderComponent(const std::string& name);

	RenderComponent(const XMLNode& node);
	
	void SetRenderMesh2D(const Vector2& screenPosition, const Vector2& newSize, const Rgba& elementColor, const std::string& diffuseTexturePath = "", const std::string& shaderName = "basicSampler" );
	
	void SetTextureInMaterial(const std::string& texBindName, const std::string& texturePath );

	void SetRenderMesh3D(const Vector3& screenPosition, const Vector3& newSize, const Rgba& elementColor, const std::string& diffuseTexturePath = "", const std::string& shaderName = "basicSampler" );

	static BaseComponent* CreateComponent(const std::string& name){ return new RenderComponent(name); }

	static ComponentRegistration s_RenderComponentRegistration;
	
	virtual void OnCreate();
	virtual void OnDestroy();
	
	//rendering helpers
	void SetRenderBounds();

	void InitializeMeshRenderer(const std::string& diffuseTexturePath = "", const std::string& shaderName = "basicSampler", const GLuint& drawMode = GL_TRIANGLE_STRIP);

	//must use model view matrix to change render position
	void Render2D(OpenGLRenderer* renderer, ModelViewMatrix* modelView = NULL, Light* light = NULL);
	void Render3D(OpenGLRenderer* renderer, Camera3D& camera, bool isPerspective = true, Lights* lights = NULL, ModelViewMatrix* modelView = NULL);

private:
	//vars
	MeshRenderer* m_meshRenderer = NULL;
	Rgba m_color;
	AABB3 m_renderBounds;
	Vector3 m_renderCoordinates;
	Vector3 renderScale; //sphere uses renderSize.x

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline RenderComponent::~RenderComponent(){
	if (m_meshRenderer){
		delete m_meshRenderer;
		m_meshRenderer = NULL;
	}
}

inline void RenderComponent::SetRenderBounds(){
	Vector3 renderMins = Vector3(m_renderCoordinates);
	Vector3 renderMaxs = Vector3(m_renderCoordinates + renderScale);

	m_renderBounds = AABB3(renderMins, renderMaxs);
}

//===========================================================================================================

#endif