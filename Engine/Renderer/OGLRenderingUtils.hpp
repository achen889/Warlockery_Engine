//==============================================================================================================
//OGLRenderingUtils.hpp
//by Albert Chen Jun-24-2015.
//==============================================================================================================

#pragma once

#ifndef _included_OGLRenderingUtils__
#define _included_OGLRenderingUtils__


#include "Engine\Core\Rgba.hpp"
#include "Engine\Math\Vector2.hpp"
#include "Engine\Renderer\Texture.hpp"
#include "Engine\Renderer\Camera3D.hpp"
#include "Engine\Math\Sphere3.hpp"
#include "Engine\Math\AABB2.hpp"
#include "Engine\Renderer\Vertex3D.hpp"
#include "Engine\Renderer\MeshRenderer.hpp"

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///VAO class used for rendering
struct VertexArrayObject{
public:
	unsigned int m_vaoID;
	unsigned int m_vboID;
	Vertex3Ds m_vertexArray;
	unsigned int m_drawMode;

	unsigned int m_Program;
	GLSampler m_glSampler;
	unsigned int m_samplerID;

	bool m_samplerIsValid = false;

	VertexArrayObject(){
		//do nothing
// 		m_glSampler.m_texture = NULL;
// 		m_glSampler.m_diffuseTex = NULL;
// 		m_glSampler.m_normalTex = NULL;
// 		m_glSampler.m_emissiveTex = NULL;
	}

	void SetSampler(unsigned int minFilter, unsigned int magFilter, unsigned int uWrap, unsigned int vWrap);

	void SetIsUsingSampler(bool usingSampler){ m_samplerIsValid = usingSampler; }

	size_t CalcVAOSize(){ return sizeof(Vertex3D) * m_vertexArray.size(); }

	void SetDrawMode(unsigned int drawMode){ m_drawMode = drawMode; }

};

//===========================================================================================================



//===========================================================================================================

#endif

