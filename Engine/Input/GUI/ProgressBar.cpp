//==============================================================================================================
//ProgressBar.cpp
//by Albert Chen Sep-1-2015.
//==============================================================================================================

#include "ProgressBar.hpp"

//===========================================================================================================

ProgressBar::ProgressBar(){
	//do nothing
}

ProgressBar::ProgressBar(const Vector2& screenPosition, const Vector2& newSize, const Vector2& scaleFactor, const Rgba& elementColor)
: UIElement(screenPosition, newSize, scaleFactor, elementColor) //call super constructor
{

	SetDefaultProgressBarMaterial();

}

ProgressBar::~ProgressBar(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///setters

void ProgressBar::SetDefaultProgressBarMaterial(){
	m_elementRenderer.m_material = new Material();
	m_elementRenderer.m_material->InitializeMaterial("Data/Shaders/basic.vert", "Data/Shaders/basic.frag");
	m_elementRenderer.m_material->m_samplerInUse = false;
}

//-----------------------------------------------------------------------------------------------------------

void ProgressBar::ScaleRenderBoundsByPercentAlongX(double deltaSeconds){
	UNUSED(deltaSeconds);
	//change size by percent
	//m_size.x *= m_percent;
	
	//Interpolate(m_renderBounds.maxs.x, m_screenCoordinates.x + (m_size.x * m_percent), deltaSeconds)

	//change bounds based on new size
	m_renderBounds.maxs.x = m_screenCoordinates.x + (m_size.x * m_percent); 
	//SetRenderBounds();
}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///upkeep

void ProgressBar::Update(double deltaSeconds, const float& percent){
	//debug update
// 	static float percentTest = 1.0f;
// 	
// 	//update internal percentage
// 	m_percent = percentTest + deltaSeconds * percentTest;
// 	percentTest -= 0.01;

	m_percent = percent + (float)deltaSeconds * percent;
	
	//cap at 0 and 1
	if (m_percent < 0.0f){
		m_percent = 0.0f;
	}	
	else if (m_percent > 1.0f){
		m_percent = 1.0f;
	}

	


	//scale using internal percent
	ScaleRenderBoundsByPercentAlongX(deltaSeconds);

}



void ProgressBar::RenderMesh(OpenGLRenderer* renderer){

	UIElement::RenderMesh(renderer);

}
