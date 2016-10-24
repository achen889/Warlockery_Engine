//==============================================================================================================
//Render2DComponent.MPp
//by Albert Chen Jan-26-2016.
//==============================================================================================================

#pragma once

#ifndef _included_Render2DComponent__
#define _included_Render2DComponent__

#include "RenderComponent.hpp"

//===========================================================================================================

class Render2DComponent : public RenderComponent{
public:
	Render2DComponent(){

	}
	
	Render2DComponent(const std::string& name);

	Render2DComponent(const XMLNode& node);

	static BaseComponent* CreateComponent(const std::string& name){ return new Render2DComponent(name); }

	static ComponentRegistration s_Render2DComponentRegistration;
	
	virtual void OnCreate();
	virtual void OnDestroy();
	
	//rendering helpers
	void SetRenderBounds();
    void SetRenderBounds(const AABB2& newBounds);
	//note you will have to recalc render bounds now
	void SetPosition(const Vector2& newPos) { m_renderCoordinates = newPos; }
	Vector2 GetPosition() {	return m_renderCoordinates; }

	//accessors
	const AABB2& GetRenderBounds() const { return m_renderBounds; }
	const Vector2& GetRenderScale() const { return renderScale;  }

	void SetRenderMesh2D(const Vector2& screenPosition, const Vector2& newSize, const Rgba& elementColor, 
						 const std::string& diffuseTexturePath = "", const std::string& shaderName = "basicSampler", 
						 bool useTextureScale = true, bool allocMat = true, bool allocMesh = true);

	void UpdateMesh(const Vector2& screenPosition, const Vector2& newSize, 
		const Rgba& elementColor, bool useTextureScale);

	void UpdateMesh(const AABB2& newRenderBounds, const Rgba& elementColor, bool useTextureScale);

	//must use model view matrix to change render position
	virtual void Render(OpenGLRenderer* renderer, Camera3D& camera, bool isPerspective = true, Lights* lights = NULL, ModelViewMatrix* modelView = NULL);
	virtual void Render2D(OpenGLRenderer* renderer, ModelViewMatrix* modelView = NULL, Lights* lights = NULL);
	void SetRenderScale(const Vector2& myRenderScale) {
		renderScale = myRenderScale;
	}

	void CreateQuadMesh2D() {
		m_meshRenderer->m_mesh->InitializeQuad2DMesh(m_renderBounds, m_color);
	}

private:
	//vars
	AABB2 m_renderBounds;
	Vector2 m_renderCoordinates;
	Vector2 renderScale; //sphere uses renderSize.x
	Vector2 texScale = Vector2::ONE; //if has texture, texScale

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline void Render2DComponent::SetRenderBounds(){
	Vector2 renderScaleRadius = renderScale * 0.5f;

	Vector2 renderMins = Vector2(m_renderCoordinates - renderScaleRadius);
	Vector2 renderMaxs = Vector2(m_renderCoordinates + renderScaleRadius);

	m_renderBounds = AABB2(renderMins, renderMaxs);
}

//-----------------------------------------------------------------------------------------------------------

inline void Render2DComponent::SetRenderBounds(const AABB2& newBounds) {
	m_renderBounds = newBounds;
	m_renderCoordinates = m_renderBounds.CalcCenter();
	renderScale = m_renderBounds.CalcLengthOfSides();
}

//===========================================================================================================

#endif