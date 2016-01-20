//==============================================================================================================
//ProgressBar.cpp
//by Albert Chen Sep-1-2015.
//==============================================================================================================

#include "UIProgressBar.hpp"

//===========================================================================================================

UIProgressBar::UIProgressBar(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

UIProgressBar::UIProgressBar(const Vector2& screenPosition, const Vector2& newSize, const Vector2& scaleFactor, const Rgba& elementColor)
: UIElement(screenPosition, newSize, scaleFactor, elementColor) //call super constructor
{
	m_fullBarRenderBounds = m_renderBounds; //somehow material is null
	SetDefaultProgressBarMaterial();
	
}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///setters

void UIProgressBar::SetDefaultProgressBarMaterial(){
	//m_elementRenderer = new MeshRenderer();
	m_elementRenderer->m_material = new Material();
	m_elementRenderer->SetDefaultMaterial();
}

//-----------------------------------------------------------------------------------------------------------

void UIProgressBar::ScaleRenderBoundsByPercentAlongX(double deltaSeconds){
	UNUSED(deltaSeconds);
	//change size by percent
	//m_size.x *= m_percent;
	
	//Interpolate(m_renderBounds.maxs.x, m_screenCoordinates.x + (m_size.x * m_percent), deltaSeconds)

	//change bounds based on new size
	m_renderBounds.maxs.x = m_screenCoordinates.x + (m_size.x * m_percent); 
	//SetRenderBounds();
}

void UIProgressBar::ScaleRenderBoundsByPercentAlongY(double deltaSeconds){
	UNUSED(deltaSeconds);
	//change size by percent
	//m_size.x *= m_percent;

	//Interpolate(m_renderBounds.maxs.x, m_screenCoordinates.x + (m_size.x * m_percent), deltaSeconds)

	//change bounds based on new size
	m_renderBounds.maxs.y = m_screenCoordinates.y + (m_size.y * m_percent);
	//SetRenderBounds();
}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///upkeep

void UIProgressBar::Update(double deltaSeconds, const float& percent){
	//debug update
// 	static float percentTest = 1.0f;
// 	
// 	//update internal percentage
// 	m_percent = percentTest + deltaSeconds * percentTest;
// 	percentTest -= 0.01;

	m_percent = percent;// +(float)deltaSeconds * percent;
	
	//cap at 0 and 1
	if (m_percent < 0.0f){
		m_percent = 0.0f;
	}	
	else if (m_percent > 1.0f){
		m_percent = 1.0f;
	}

	//may need to change loc of full bar
	SetRenderBounds(m_fullBarRenderBounds);

	//may need to change loc based on screen coords
	SetRenderBounds(m_renderBounds);
	if (m_scaleAlongX){
		//scale using internal percent
		ScaleRenderBoundsByPercentAlongX(deltaSeconds);
	}
	else{
		ScaleRenderBoundsByPercentAlongY(deltaSeconds);
	}

}

//-----------------------------------------------------------------------------------------------------------

void UIProgressBar::RenderMesh(OpenGLRenderer* renderer, ModelViewMatrix* modelMat){
	UNUSED(renderer);

	if (IsMeshRendererValid(m_elementRenderer)){
		//render bar bg
		
		m_elementRenderer->m_mesh->InitializeQuad2DMesh(m_fullBarRenderBounds, m_color1);
		m_elementRenderer->RenderMesh2D(modelMat);

		//render percent bar
		m_elementRenderer->m_mesh->InitializeQuad2DMesh(m_renderBounds, m_color);
		m_elementRenderer->RenderMesh2D(modelMat);
	}
	

}

//-----------------------------------------------------------------------------------------------------------