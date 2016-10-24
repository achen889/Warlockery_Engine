//==============================================================================================================
//Render3DComponent.cpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#include "Render3DComponent.hpp"
#include "Engine/Math/Math3D.hpp"

ComponentRegistration Render3DComponent::s_Render3DComponentRegistration("Render3D", &Render3DComponent::CreateComponent);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Render Component

Render3DComponent::Render3DComponent(const XMLNode& node): RenderComponent(node){

	std::string diffuseTexturePath = ReadXMLAttributeAsString(node, "texturePath");
	std::string shaderName = ReadXMLAttributeAsString(node, "shader");

	InitializeMeshRenderer(diffuseTexturePath, shaderName, true, true, GL_TRIANGLE_STRIP);

}

//-----------------------------------------------------------------------------------------------------------

Render3DComponent::Render3DComponent(const std::string& name) : RenderComponent(name){

	//InitializeMeshRenderer("", "basicSampler", GL_TRIANGLE_STRIP);

}

//-----------------------------------------------------------------------------------------------------------

void Render3DComponent::OnCreate(){
	//can specify something on create here
}

//-----------------------------------------------------------------------------------------------------------

void Render3DComponent::OnDestroy(){
	//can specify something on destroy here
}

///----------------------------------------------------------------------------------------------------------
///render helpers

void Render3DComponent::SetRenderMesh3D(const Vector3& screenPosition, const Vector3& newSize, const Rgba& elementColor, const std::string& diffuseTexturePath, const std::string& shaderName) {
	m_renderCoordinates = screenPosition;
	renderScale = newSize;
	m_color = elementColor;

	SetRenderBounds();

	InitializeMeshRenderer(diffuseTexturePath, shaderName);

	//sets render box
	m_meshRenderer->m_mesh->InitializeQuad3DMesh(m_renderBounds, m_color); //box
}

//-----------------------------------------------------------------------------------------------------------

void Render3DComponent::Render(OpenGLRenderer* renderer, Camera3D& camera, bool isPerspective, Lights* lights, ModelViewMatrix* modelView){
	//PROFILE_SECTION();
	
	UNUSED(renderer);

	if (m_meshRenderer->m_material && m_meshRenderer->m_mesh){

		//sets render sphere
		//m_meshRenderer->m_mesh->InitializeSphereMesh(Sphere3(m_renderBounds.CalcCenter(), renderSize.x), Rgba::BLUE); //sphere
		
		if (lights){
			m_meshRenderer->RenderMeshWithLights(camera, isPerspective, *lights, modelView);
		}
		else{
			m_meshRenderer->RenderMesh(camera, isPerspective);
		}

	}

}

//===========================================================================================================


