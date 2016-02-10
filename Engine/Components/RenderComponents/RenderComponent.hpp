//==============================================================================================================
//RenderComponent.hpp
//by Albert Chen Aug-25-2015.
//==============================================================================================================

#pragma once

#ifndef _included_RenderComponent__
#define _included_RenderComponent__

#include "Engine/Components/BaseComponent.hpp"
#include "Engine/Renderer/MeshRenderer.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Math/AABB2.hpp"

class RenderComponent : public BaseComponent{
public:
	RenderComponent(){

	}
	~RenderComponent();

	RenderComponent(const std::string& name);

	RenderComponent(const XMLNode& node);

	static BaseComponent* CreateComponent(const std::string& name){ return new RenderComponent(name); }

	static ComponentRegistration s_RenderComponentRegistration;
	
	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Render(OpenGLRenderer* renderer, Camera3D& camera, bool isPerspective = true, Lights* lights = NULL, ModelViewMatrix* modelView = NULL);
	
	//rendering helpers

	void InitializeMeshRenderer(const std::string& diffuseTexturePath = "", const std::string& shaderName = "basicSampler", const GLuint& drawMode = GL_TRIANGLE_STRIP);
	void SetTextureInMaterial(const std::string& texBindName, const std::string& texturePath);

protected:
	//vars
	MeshRenderer m_meshRenderer;
	Rgba m_color;

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline RenderComponent::~RenderComponent(){
	//do nothing
}

//===========================================================================================================

#endif