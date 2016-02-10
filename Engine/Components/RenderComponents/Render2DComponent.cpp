//==============================================================================================================
//Render2DComponent.cpp
//by Albert Chen Jan-26-2016.
//==============================================================================================================

#include "Render2DComponent.hpp"
#include "Engine/Math/Math2D.hpp"

ComponentRegistration Render2DComponent::s_Render2DComponentRegistration("Render2D", &Render2DComponent::CreateComponent);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Render Component

Render2DComponent::Render2DComponent(const XMLNode& node): RenderComponent(node){

	//do something

}

//-----------------------------------------------------------------------------------------------------------

Render2DComponent::Render2DComponent(const std::string& name) : RenderComponent(name){

	//InitializeMeshRenderer("", "basicSampler", GL_TRIANGLE_STRIP);

}


//-----------------------------------------------------------------------------------------------------------

void Render2DComponent::OnCreate(){
	//can specify something on create here
}

//-----------------------------------------------------------------------------------------------------------

void Render2DComponent::OnDestroy(){
	//can specify something on destroy here
}

//-----------------------------------------------------------------------------------------------------------


///----------------------------------------------------------------------------------------------------------
///render helpers

void Render2DComponent::SetRenderMesh2D(const Vector2& screenPosition, const Vector2& newSize, const Rgba& elementColor, const std::string& diffuseTexturePath, const std::string& shaderName) {
	m_renderCoordinates = screenPosition;
	renderScale = newSize;
	m_color = elementColor;

	SetRenderBounds();

	InitializeMeshRenderer(diffuseTexturePath, shaderName);

	//gives it a basic 2D quad mesh
	m_meshRenderer.m_mesh->InitializeQuad2DMesh(m_renderBounds, m_color);

}

//-----------------------------------------------------------------------------------------------------------


void Render2DComponent::Render(OpenGLRenderer* renderer, Camera3D& camera, bool isPerspective, Lights* lights, ModelViewMatrix* modelView){

	//PROFILE_SECTION();

	UNUSED(renderer);
	UNUSED(camera);
	UNUSED(isPerspective);

	if (m_meshRenderer.m_material && m_meshRenderer.m_mesh){
		if (lights) {
			m_meshRenderer.RenderMesh2D(modelView, (*lights)[0]);
		}
		else {
			m_meshRenderer.RenderMesh2D(modelView);
		}	
	}
}

//===========================================================================================================


