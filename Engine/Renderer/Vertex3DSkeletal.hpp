//==============================================================================================================
//Vertex3DSkeletal.hpp
//by Albert Chen Sep-23-2015.
//==============================================================================================================

#include "Vertex3D.hpp"

static const unsigned int MAX_BONES_PER_VERTEX = 4;
static const unsigned int MAX_BONES = 255;

//===========================================================================================================

struct Vertex3DSkeletal{

	//vars
	Vertex3D m_vertex;
	unsigned char m_boneIndices[MAX_BONES_PER_VERTEX];
	Vector3 m_boneWeights; //d is assumed to be 1.0f - (a+b+c)

	//methods
	Vertex3D(){

	}

	~Vertex3D(){

	}

	Vertex3DSkeletal(const unsigned char[4]& boneIndices, const Vector3& boneWeights, const Vector3& position, const Rgba& color, const Vector2& texCoords = Vector2::ZERO, const Vector3& normal = Vector3(-1.0f, 0.0f, 0.0f), const Vector3& tangent = Vector3::ZERO, const Vector3& bitangent = Vector3::ZERO);
	
}

//===========================================================================================================