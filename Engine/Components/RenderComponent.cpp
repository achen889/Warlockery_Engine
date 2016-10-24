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

}

//-----------------------------------------------------------------------------------------------------------

void RenderComponent::InitializeMeshRenderer(const std::string& diffuseTexturePath, const std::string& shaderName, const GLuint& drawMode){
	if (!m_meshRenderer){
		m_meshRenderer = new MeshRenderer();

		m_meshRenderer->m_material->InitializeDefaultSampler();
		m_meshRenderer->m_material->InitializeShaderFromShaderName(shaderName);

		//loads default if no texture specified
		if (diffuseTexturePath == ""){
			m_meshRenderer->m_material->SetTextureInMap("gTexDiffuse", COMMON_TEXTURE_DIFFUSE);
		}
		else{
			m_meshRenderer->m_material->SetTextureInMap("gTexDiffuse", diffuseTexturePath);
		}

		m_meshRenderer->m_mesh->SetDrawMode(drawMode);

		m_meshRenderer->BindVertexArray();
	}
}

//-----------------------------------------------------------------------------------------------------------

void RenderComponent::SetTextureInMaterial(const std::string& texBindName, const std::string& texturePath){
	if (m_meshRenderer)
		m_meshRenderer->m_material->SetTextureInMap(texBindName, texturePath);

}

//-----------------------------------------------------------------------------------------------------------

void RenderComponent::SetRenderMesh2D(const Vector2& screenPosition, const Vector2& newSize, const Rgba& elementColor, const std::string& diffuseTexturePath , const std::string& shaderName){
	m_renderCoordinates = ToVector3(screenPosition);
	renderSize = ToVector3(newSize);
	m_color = elementColor;

	SetRenderBounds();

	InitializeMeshRenderer(diffuseTexturePath, shaderName);

}

//-----------------------------------------------------------------------------------------------------------

void RenderComponent::SetRenderMesh3D(const Vector3& screenPosition, const Vector3& newSize, const Rgba& elementColor, const std::string& diffuseTexturePath, const std::string& shaderName){
	m_renderCoordinates = screenPosition;
	renderSize = newSize;
	m_color = elementColor;

	SetRenderBounds();

	InitializeMeshRenderer(diffuseTexturePath, shaderName);
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

void RenderComponent::Render2D(OpenGLRenderer* renderer, ModelViewMatrix* modelView, Light* light){

	UNUSED(renderer);

	if (m_meshRenderer){
		//gives it a basic 2D quad mesh
		m_meshRenderer->m_mesh->InitializeQuad2DMesh(ToAABB2(m_renderBounds), m_color);

		m_meshRenderer->RenderMesh2D(modelView, light);
	}
}

//-----------------------------------------------------------------------------------------------------------

void RenderComponent::Render3D(OpenGLRenderer* renderer, Camera3D& camera, bool isPerspective, Lights* lights, ModelViewMatrix* modelView){
	UNUSED(renderer);

	if (m_meshRenderer){

		m_meshRenderer->m_mesh->InitializeQuad3DMesh(m_renderBounds, m_color); //box

		//meshRenderer->m_mesh->InitializeSphereMesh(Sphere3(m_screenCoordinates, renderSize.x), Rgba::BLUE); //sphere
		if (lights){
			m_meshRenderer->RenderMeshWithLights(camera, isPerspective, *lights, modelView);
		}
		else{
			m_meshRenderer->RenderMesh(camera, isPerspective);
		}

	}

}

//===========================================================================================================


