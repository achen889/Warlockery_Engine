//==============================================================================================================
//Vertex3D.hpp
//by Albert Chen Jul-4-2015.
//==============================================================================================================
#pragma once

#ifndef _included_Vertex3D__
#define _included_Vertex3D__

#include <vector>

#include "Engine\Math\Sphere3.hpp"
#include "Engine\Math\Disc2.hpp"

class Rgba;

struct Vertex3D;

typedef std::vector<Vertex3D> Vertex3Ds;
typedef std::vector<Vertex3D>::const_iterator Vertex3DConstIterator;

class AABB2;
class AABB3;

//---------------------------------------------------------------------------
// initial struct by Prof. Squirrel Eiserloh, heavily modified by Albert Chen
// Data structure which holds all the Vertex Attributes for a single vertex
//	PCT = Position, Color, TexCoords
struct Vertex3D{
	Vector3		m_position;
	Rgba		m_color;		// Rgba is 4 unsigned bytes
	Vector2		m_texCoords;

	Vector3		m_normal;
	Vector3		m_tangent;
	Vector3     m_biTangent;

	Vertex3D();
	~Vertex3D();

	
	Vertex3D(const Vector3& position, const Rgba& color = Rgba::WHITE, const Vector2& texCoords = Vector2::ZERO , const Vector3& normal = Vector3(-1.0f,0.0f,0.0f), const Vector3& tangent = Vector3::ZERO, const Vector3& bitangent = Vector3::ZERO );

	
	//generate basic meshes
	friend void GenerateVertexArrayQuad2D(Vertex3Ds& out_vertexArray, const AABB2& boxCoords, const Rgba& vertexColor, bool clearVerts);
	friend void GenerateVertexArrayQuad3D(Vertex3Ds& out_vertexArray, const AABB3& boxCoords, const Rgba& vertexColor, bool clearVerts);
	friend void GenerateVertexArrayTextureQuad(Vertex3Ds& out_vertexArray, const AABB2& boxCoords, const AABB2& textureCoords, const Rgba& vertexColor, bool clearVerts = true);
	friend void GenerateVertexArrayCoordinateAxes(Vertex3Ds& out_vertexArray);
	friend void GenerateVertexArraySphere3D(Vertex3Ds& out_vertexArray, Sphere3 sphereToDraw, const int& wedges = 32, const int& slices = 32, const Rgba& viewColor = Rgba::MAGENTA);
	friend void GenerateVertexArrayDisc2D(Vertex3Ds& out_verts, Disc2 DiscToDraw, const Rgba& viewColor);

	friend void AppendVerticesToExistingVertexArray(Vertex3Ds& out_vertexArray, const Vertex3Ds& vertsToAppend);
	
};


#endif
