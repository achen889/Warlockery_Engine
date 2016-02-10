//==============================================================================================================
//Render3DComponent.hpp
//by Albert Chen Jan-26-2016.
//==============================================================================================================

#pragma once

#ifndef _included_Render3DComponent__
#define _included_Render3DComponent__

#include "Engine/Math/AABB3.hpp"

#include "RenderComponent.hpp"

//===========================================================================================================

class Render3DComponent : public RenderComponent{
public:
	Render3DComponent(){

	}
	~Render3DComponent();
	Render3DComponent(const std::string& name);

	Render3DComponent(const XMLNode& node);

	static BaseComponent* CreateComponent(const std::string& name){ return new Render3DComponent(name); }

	static ComponentRegistration s_Render3DComponentRegistration;
	
	virtual void OnCreate();
	virtual void OnDestroy();
	
	//rendering helpers
	void SetRenderBounds();

	void SetRenderMesh3D(const Vector3& screenPosition, const Vector3& newSize, const Rgba& elementColor, const std::string& diffuseTexturePath = "", const std::string& shaderName = "basicSampler");

	virtual void Render(OpenGLRenderer* renderer, Camera3D& camera, bool isPerspective = true, Lights* lights = NULL, ModelViewMatrix* modelView = NULL);

private:
	//vars
	AABB3 m_renderBounds;
	Vector3 m_renderCoordinates;
	Vector3 renderScale; //sphere uses renderSize.x as the radius

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline Render3DComponent::~Render3DComponent(){
	//do nothing
}

inline void Render3DComponent::SetRenderBounds(){
	Vector3 renderMins = Vector3(m_renderCoordinates);
	Vector3 renderMaxs = Vector3(m_renderCoordinates + renderScale);

	m_renderBounds = AABB3(renderMins, renderMaxs);
}

//===========================================================================================================

#endif