//==============================================================================================================
//UIElement.cpp
//by Albert Chen Sep-1-2015.
//==============================================================================================================

#include "UIElement.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Input/InputSystem.hpp"


//===========================================================================================================

UIElement::UIElement():
m_elementRenderer(NULL)
{
	//do nothing
	elementID++;
	m_id = elementID;
}

//-----------------------------------------------------------------------------------------------------------

UIElement::~UIElement() {
	//do nothing
	if (m_elementRenderer) {
		delete m_elementRenderer;
	}

}


//-----------------------------------------------------------------------------------------------------------

UIElement::UIElement(const Vector2& screenPosition, const Vector2& newSize, const Vector2& scaleFactor, const Rgba& elementColor) :
m_screenCoordinates(screenPosition),
m_scaleFactor(scaleFactor),
m_size(newSize),
m_color(elementColor)
{

	SetRenderBounds();

	InitializeDefaultTexturedElementMeshRenderer();


}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///setters

//this assumes a texture will be passed to the material
void UIElement::InitializeDefaultTexturedElementMeshRenderer(){
	
	if (!m_elementRenderer){
		m_elementRenderer = new MeshRenderer();
		SetDefaultTexturedMeshRenderer(*m_elementRenderer);
	}

}

//-----------------------------------------------------------------------------------------------------------

void UIElement::SetDefaultElementTexture(const std::string& texturePath){
	m_elementRenderer->m_material->SetTextureInMap("gTexture", texturePath);

}


/////----------------------------------------------------------------------------------------------------------
///interaction methods
bool UIElement::OnHover(){

	if (theInputSystem){
		return theInputSystem->IsMouseHoverOnAABB2(m_renderBounds);
	}
	
	return false;

}

//-----------------------------------------------------------------------------------------------------------

bool UIElement::OnClick(MouseButtonID mouseButtonClicked){

	if (OnHover()){
		if (theInputSystem->WasMouseButtonJustReleased(mouseButtonClicked)){
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------------------------------------

bool UIElement::OnKeyDown(int keyData){

	if (theInputSystem){
		if (theInputSystem->IsKeyDown(keyData)){
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------------------------------------

bool UIElement::OnWasKeyJustReleased(int keyData){
	if (theInputSystem){
		if (theInputSystem->WasKeyJustReleased(keyData)){
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///rendering

//render
void UIElement::RenderMesh(OpenGLRenderer* renderer, ModelViewMatrix* modelMat){
	UNUSED(renderer);
	m_elementRenderer->m_mesh->InitializeQuad2DMesh(m_renderBounds, m_color);

	m_elementRenderer->RenderMesh2D(modelMat);
}

//-----------------------------------------------------------------------------------------------------------

//this binds to gTexture in the basic sampler
void UIElement::RenderMesh(OpenGLRenderer* renderer, const std::string& texturePath){
	UNUSED(renderer);
	//set texture
	SetDefaultElementTexture(texturePath);

	m_elementRenderer->m_mesh->InitializeQuad2DMesh(m_renderBounds, m_color);

	m_elementRenderer->RenderMesh2D();
}

//-----------------------------------------------------------------------------------------------------------




//===========================================================================================================