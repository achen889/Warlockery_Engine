//==============================================================================================================
//RenderComponent.MPp
//by Albert Chen Aug-25-2015.
//==============================================================================================================

#pragma once

#ifndef _included_RenderComponent__
#define _included_RenderComponent__

#include "Engine/Components/BaseComponent.hpp"
#include "Engine/Math/AABB2.hpp"

//find a way to move these two out of here
#include "Engine/Renderer/MeshRenderer.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"

class RenderComponent : public BaseComponent{
public:
	RenderComponent(){
		//do nothing
	}
	virtual ~RenderComponent();

	RenderComponent(const std::string& name);

	RenderComponent(const XMLNode& node);

	static BaseComponent* CreateComponent(const std::string& name){ return new RenderComponent(name); }

	static ComponentRegistration s_RenderComponentRegistration;
	
	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Render(OpenGLRenderer* renderer, Camera3D& camera, bool isPerspective = true, Lights* lights = NULL, ModelViewMatrix* modelView = NULL);
	
	//rendering helpers

	//accessors
	virtual void SetName(const std::string& name) override {
		BaseComponent::SetName(name);
		if (m_meshRenderer) {
			m_meshRenderer->SetName(name);
		}
	}
	virtual void SetColor(const Rgba& color) { m_color = color; }
	virtual Rgba GetColor() { return m_color; }

	//mesh helpers
	virtual MeshRenderer* GetMeshRenderer() { return m_meshRenderer; }
	virtual void InitializeMeshRenderer(const std::string& diffuseTexturePath = "", const std::string& shaderName = "basicSampler", 
		bool allocMat = true, bool allocMesh = true, const GLuint& drawMode = GL_TRIANGLES);
	virtual void SetTextureInMaterial(const std::string& texBindName, const std::string& texturePath);

protected:
	//vars
	MeshRenderer* m_meshRenderer = NULL;
	Rgba m_color;

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline RenderComponent::~RenderComponent(){
	//this needs to work to fix a mem leak, but crashes
// 	if (m_meshRenderer) {
// 		delete m_meshRenderer;
// 		m_meshRenderer = NULL;
// 	}

}

//===========================================================================================================

#endif