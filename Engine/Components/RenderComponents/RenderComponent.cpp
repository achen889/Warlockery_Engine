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

	
	//this one is probably getting set and then not being released
	InitializeMeshRenderer(diffuseTexturePath, shaderName, true, true, GL_TRIANGLE_STRIP);

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

void RenderComponent::InitializeMeshRenderer(const std::string& diffuseTexturePath, const std::string& shaderName, bool allocMat, bool allocMesh, const GLuint& drawMode) {
	UNUSED(drawMode);

	std::string myName = GetName();
	
	
	if (!m_meshRenderer) {
		
		if (!allocMat || !allocMesh) {
			//ConsolePrintf("\ncreate or get meshrender name: %s", myName.c_str());
			m_meshRenderer = MeshRenderer::CreateOrGetMeshRenderer(myName, allocMat, allocMesh);
		}
		else {
			//ConsolePrintf("\nnew meshrender name: %s", myName.c_str());
			m_meshRenderer = new MeshRenderer(myName, allocMat, allocMesh);
		}
	}
	//refactor this to do what I said it would do
	Material* renderMat = Material::CreateOrGetMaterial(myName);
	
	renderMat->InitializeDefaultSampler();
	renderMat->InitializeShaderFromShaderName(shaderName);

	//loads default if no texture specified
	if (diffuseTexturePath == "") {
		renderMat->SetTextureInMap("gTexDiffuse", COMMON_TEXTURE_DIFFUSE);
		renderMat->SetTextureInMap("gTexture", COMMON_TEXTURE_DIFFUSE);
	}
	else {
		renderMat->SetTextureInMap("gTexDiffuse", diffuseTexturePath);
		renderMat->SetTextureInMap("gTexture", diffuseTexturePath);
	}
	m_meshRenderer->SetMaterial(*renderMat);

	if (!allocMesh) {
		Mesh* renderMesh = Mesh::CreateOrGetMesh(myName);
		m_meshRenderer->SetMesh(*renderMesh);
	}
	
	m_meshRenderer->BindVertexArray();
	//the children will create the mesh
}

//-----------------------------------------------------------------------------------------------------------

void RenderComponent::SetTextureInMaterial(const std::string& texBindName, const std::string& texturePath) {
	if (m_meshRenderer->m_material)
		m_meshRenderer->m_material->SetTextureInMap(texBindName, texturePath);

}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================


