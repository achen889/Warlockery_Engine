//==============================================================================================================
//RenderComponent.cpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#include "RenderComponent.hpp"
#include "Engine/Math/Math3D.hpp"

ComponentRegistration RenderComponent::s_RenderComponentRegistration("Render", &RenderComponent::CreateComponent);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Render Component

RenderComponent::RenderComponent(const XMLNode& node): BaseComponent(node){

	std::string diffuseTexturePath = ReadXMLAttributeAsString(node, "texturePath");
	std::string shaderName = ReadXMLAttributeAsString(node, "shader");

	InitializeMeshRenderer(diffuseTexturePath, shaderName, GL_TRIANGLE_STRIP);

}

//-----------------------------------------------------------------------------------------------------------

RenderComponent::RenderComponent(const std::string& name) : BaseComponent(name){

	//InitializeMeshRenderer("", "basicSampler", GL_TRIANGLE_STRIP);

}

//-----------------------------------------------------------------------------------------------------------

void RenderComponent::OnCreate(){
	//can specify something on create here
}

//-----------------------------------------------------------------------------------------------------------

void RenderComponent::OnDestroy(){
	//can specify something on destroy here
}

//-----------------------------------------------------------------------------------------------------------

void RenderComponent::Render(OpenGLRenderer* renderer, Camera3D& camera, bool isPerspective , Lights* lights , ModelViewMatrix* modelView ){

	UNUSED(renderer);
	UNUSED(camera);
	UNUSED(isPerspective);
	UNUSED(lights);
	UNUSED(modelView);

}

//-----------------------------------------------------------------------------------------------------------

void RenderComponent::InitializeMeshRenderer(const std::string& diffuseTexturePath, const std::string& shaderName, const GLuint& drawMode) {
	//if (!m_meshRenderer) {

	m_meshRenderer.m_material->InitializeDefaultSampler();
	m_meshRenderer.m_material->InitializeShaderFromShaderName(shaderName);

	//loads default if no texture specified
	if (diffuseTexturePath == "") {
		m_meshRenderer.m_material->SetTextureInMap("gTexDiffuse", COMMON_TEXTURE_DIFFUSE);
	}
	else {
		m_meshRenderer.m_material->SetTextureInMap("gTexDiffuse", diffuseTexturePath);
	}

	m_meshRenderer.m_mesh->SetDrawMode(drawMode);

	m_meshRenderer.BindVertexArray();
}

//-----------------------------------------------------------------------------------------------------------

void RenderComponent::SetTextureInMaterial(const std::string& texBindName, const std::string& texturePath) {
	if (m_meshRenderer.m_material)
		m_meshRenderer.m_material->SetTextureInMap(texBindName, texturePath);

}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================


