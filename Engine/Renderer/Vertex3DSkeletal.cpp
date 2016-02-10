//==============================================================================================================
//Vertex3DSkeletal.cpp
//by Albert Chen Sep-23-2015.
//==============================================================================================================

#include "Vertex3DSkeletal.hpp"

//===========================================================================================================

Vertex3DSkeletal::Vertex3DSkeletal(
	const unsigned char[]& boneIndices, const Vector3& boneWeights,
	const Vector3& position, const Rgba& color, 
	const Vector2& texCoords, const Vector3& normal,
	const Vector3& tangent, const Vector3& bitangent) : 
	m_boneIndices(boneIndices),
	m_boneWeights(boneWeights)
{
		
	m_vertex = Vertex3D(position, color, texCoords, normal, tangent, bitangent);

	//ConsolePrintf("Size of Vertex3DSkeletal = ", sizeof(Vertex3DSkeletal));

}

//-----------------------------------------------------------------------------------------------------------