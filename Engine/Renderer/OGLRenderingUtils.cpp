//==============================================================================================================
//OGLRenderingUtils.cpp
//by Albert Chen Jun-24-2015.
//==============================================================================================================

#include "OGLRenderingUtils.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Core/Time.hpp"
#include "Light.hpp"

//===========================================================================================================

void VertexArrayObject::SetSampler(unsigned int minFilter, unsigned int magFilter, unsigned int uWrap, unsigned int vWrap){
	m_glSampler.SetGLSamplerData(minFilter, magFilter, uWrap, vWrap);
	if (theOGLRenderer){
		m_samplerID = theOGLRenderer->CreateSampler(m_glSampler);
	}
}

//===========================================================================================================

//===========================================================================================================