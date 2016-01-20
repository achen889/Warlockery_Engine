//==============================================================================================================
//Mesh.hpp
//by Albert Chen Jul-4-2015.
//==============================================================================================================

#pragma once

#include "Engine\Renderer\Vertex3D.hpp"

#ifndef _included_Mesh__
#define _included_Mesh__

class Sphere3;
class AABB2;

struct Mesh{
	//unsigned int m_vaoID;
	//vars
	unsigned int m_vboID;
	unsigned int m_iboID;
	unsigned int m_drawMode;
	unsigned int m_numVerticesToDraw;
	unsigned int m_numIndicesToDraw;
	
	Mesh();
	~Mesh();


	void CopyMeshVertexData(Vertex3Ds& in_vertexArray);
	void CopyMeshIndexData(std::vector<unsigned int>& in_indices);

	void InitializeSphereMesh(Sphere3 sphere, const Rgba& color = Rgba::WHITE);
	void InitializeMeshCoordinateAxes(const int& length, bool includeNegAxes = false);

	void InitializeQuadMesh(const Rgba& viewColor); //unit quad
	void InitializeQuadMesh(const AABB2& quad, const Rgba& viewColor);
	void InitializeQuad2DMesh(AABB2 quad, const Rgba& viewColor = Rgba::WHITE);
	void InitializeQuad3DMesh(AABB3 quadBox, const Rgba& viewColor = Rgba::WHITE);

	void InitializeCursorRenderBoundsMesh(const float& cursorImageSize = 24.0f);

	size_t CalcVertexArraySize(const Vertex3Ds& m_vertexArray){ 
		return sizeof(Vertex3D) * ( m_vertexArray.size()); 
	}

	void SetDrawMode(unsigned int drawMode){ m_drawMode = drawMode; }
	void SetNumIndicesToDraw(const unsigned int indicesNum){ 
		//0 means don't use indices
		m_numIndicesToDraw = indicesNum; 
	}
	
};


#endif