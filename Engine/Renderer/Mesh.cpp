//==============================================================================================================
//Mesh.cpp
//by Albert Chen Jul-4-2015.
//==============================================================================================================

#include "Mesh.hpp"
#include "Engine\Renderer\OpenGLRenderer.hpp"
#include "Engine\Math\AABB2.hpp"
#include "Engine\Math\Sphere3.hpp"
#include "Engine\Input\InputSystem.hpp"

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Mesh Methods

Mesh::Mesh():
m_vboID(0),
m_iboID(0),
m_numVerticesToDraw(0),
m_numIndicesToDraw(0),
m_drawMode(GL_POINTS)
{
	if (theOGLRenderer){

		m_vboID = theOGLRenderer->InitializeVertexBuffer(m_vboID);

		m_iboID = theOGLRenderer->InitializeIndexBuffer(m_iboID);
	}

}

//-----------------------------------------------------------------------------------------------------------

Mesh::~Mesh(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

void Mesh::CopyMeshVertexData(Vertex3Ds& vertexArray){

	if (theOGLRenderer){ 
		m_numVerticesToDraw = CalcVertexArraySize(vertexArray); //vertexArray.size();
		theOGLRenderer->CopyVertexBufferData((GLuint)m_vboID, vertexArray.data(), (size_t)m_numVerticesToDraw);
		//m_numVerticesToDraw = vertexArray.size();//CalcVertexArraySize(vertexArray);
	}

}

//-----------------------------------------------------------------------------------------------------------

void Mesh::CopyMeshIndexData(std::vector<unsigned int>& in_indices){

	if (theOGLRenderer){
		m_numIndicesToDraw = in_indices.size();
		theOGLRenderer->CopyIndexBufferData((GLuint)m_iboID, in_indices.data(), (size_t)m_numIndicesToDraw * sizeof(unsigned int));
	}

}

//-----------------------------------------------------------------------------------------------------------

void Mesh::InitializeSphereMesh(Sphere3 sphere, const Rgba& color ){
	Vertex3Ds m_vertexArray;

	m_vertexArray.clear();

	GenerateVertexArraySphere3D(m_vertexArray, sphere, 32, 32, color );

	CopyMeshVertexData(m_vertexArray);

	SetDrawMode(GL_TRIANGLE_STRIP);
}

//-----------------------------------------------------------------------------------------------------------

void Mesh::InitializeMeshCoordinateAxes(const int& length, bool includeNegAxes ){
	Vertex3Ds m_vertexArray;
	float lenF = (float)length;
	//use draw line strip
	m_vertexArray.clear();
	m_vertexArray.reserve(10000);
	//coordinate axes for normal
	//x axis
	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), Rgba::RED, Vector2::ZERO));
	m_vertexArray.push_back(Vertex3D(Vector3(lenF, 0.0f, 0.0f), Rgba::RED, Vector2::ZERO));
	//x arrowhead
// 	m_vertexArray.push_back(Vertex3D(Vector3(lenF, 0.1f, 0.0f), Rgba::RED, Vector2::ZERO));
// 	m_vertexArray.push_back(Vertex3D(Vector3(lenF + 0.2f, 0.0f, 0.0f), Rgba::RED, Vector2::ZERO));
// 	m_vertexArray.push_back(Vertex3D(Vector3(lenF, -0.1f, 0.0f), Rgba::RED, Vector2::ZERO));
// 	m_vertexArray.push_back(Vertex3D(Vector3(lenF, 0.0f, 0.0f), Rgba::RED, Vector2::ZERO));
	//y axis
	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), Rgba::GREEN, Vector2::ZERO));
	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, lenF, 0.0f), Rgba::GREEN, Vector2::ZERO));
	//y arrowhead
// 	m_vertexArray.push_back(Vertex3D(Vector3(0.1f, lenF, 0.0f), Rgba::GREEN, Vector2::ZERO));
// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, lenF + 0.2f, 0.0f), Rgba::GREEN, Vector2::ZERO));
// 	m_vertexArray.push_back(Vertex3D(Vector3(-0.1f, lenF, 0.0f), Rgba::GREEN, Vector2::ZERO));
// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, lenF, 0.0f), Rgba::GREEN, Vector2::ZERO));
	//z axis
	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), Rgba::BLUE, Vector2::ZERO));
	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, lenF), Rgba::BLUE, Vector2::ZERO));
	//z arrowhead
// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.1f, lenF), Rgba::BLUE, Vector2::ZERO));
// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, lenF + 0.2f), Rgba::BLUE, Vector2::ZERO));
// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, -0.1f, lenF), Rgba::BLUE, Vector2::ZERO));
// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, lenF), Rgba::BLUE, Vector2::ZERO));
	
	if (includeNegAxes){
		//neg x axis
		m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), Rgba::RED, Vector2::ZERO));
		m_vertexArray.push_back(Vertex3D(Vector3(-lenF, 0.0f, 0.0f), Rgba::RED, Vector2::ZERO));
		//x arrowhead
		// 	m_vertexArray.push_back(Vertex3D(Vector3(-lenF, 0.1f, 0.0f), Rgba::MAGENTA, Vector2::ZERO));
		// 	m_vertexArray.push_back(Vertex3D(Vector3(-lenF + 0.2f, 0.0f, 0.0f), Rgba::ORANGE, Vector2::ZERO));
		// 	m_vertexArray.push_back(Vertex3D(Vector3(-lenF, -0.1f, 0.0f), Rgba::MAGENTA, Vector2::ZERO));
		// 	m_vertexArray.push_back(Vertex3D(Vector3(-lenF, 0.0f, 0.0f), Rgba::RED, Vector2::ZERO));
		//neg y axis
		m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), Rgba::GREEN, Vector2::ZERO));
		m_vertexArray.push_back(Vertex3D(Vector3(0.0f, -lenF, 0.0f), Rgba::GREEN, Vector2::ZERO));
		//y arrowhead
		// 	m_vertexArray.push_back(Vertex3D(Vector3(0.1f, -lenF, 0.0f), Rgba::NEON_GREEN, Vector2::ZERO));
		// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, -lenF + 0.2f, 0.0f), Rgba::MINT_GREEN, Vector2::ZERO));
		// 	m_vertexArray.push_back(Vertex3D(Vector3(-0.1f, -lenF, 0.0f), Rgba::NEON_GREEN, Vector2::ZERO));
		// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, -lenF, 0.0f), Rgba::GREEN, Vector2::ZERO));
		//neg z axis
		m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), Rgba::BLUE, Vector2::ZERO));
		m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, -lenF), Rgba::BLUE, Vector2::ZERO));
		//z arrowhead
		// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.1f, -lenF), Rgba::LAVENDER, Vector2::ZERO));
		// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, -lenF + 0.2f), Rgba::TRON_BLUE, Vector2::ZERO));
		// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, -0.1f, -lenF), Rgba::LAVENDER, Vector2::ZERO));
		// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, -lenF), Rgba::BLUE, Vector2::ZERO));
	}
	

	CopyMeshVertexData(m_vertexArray);

	SetDrawMode(GL_LINES);
}

//-----------------------------------------------------------------------------------------------------------

//vertex array of a quad from 0 to 1
void Mesh::InitializeQuadMesh(const Rgba& viewColor){
	Vertex3Ds m_vertexArray;

	m_vertexArray.clear();
	m_vertexArray.reserve(10000);

	//const Vector2s defaultTexCoords = GetDefaultTextureCoordinates();
	const float quadSize = 1.0f;
	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, quadSize), viewColor, Vector2(0, 0), Vector3(-1.0f, 0.0f, 0.0f)));
	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, quadSize, quadSize), viewColor, Vector2(1, 0), Vector3(-1.0f, 0.0f, 0.0f)));
	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, quadSize, 0.0f), viewColor, Vector2(1, 1), Vector3(-1.0f, 0.0f, 0.0f)));
	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), viewColor, Vector2(0, 1), Vector3(-1.0f, 0.0f, 0.0f)));
	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, quadSize), viewColor, Vector2(0, 0), Vector3(-1.0f, 0.0f, 0.0f)));

	CopyMeshVertexData(m_vertexArray);

	SetDrawMode(GL_TRIANGLE_STRIP);
}

//vertex array of a quad facing -X with correct 3d texturing
void Mesh::InitializeQuadMesh(const AABB2& quad, const Rgba& viewColor){
	Vertex3Ds m_vertexArray;

	m_vertexArray.clear();
	m_vertexArray.reserve(10000);

	//const Vector2s defaultTexCoords = GetDefaultTextureCoordinates();
	//const float quadSize = 1.0f;
	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, quad.mins.x, quad.maxs.y), viewColor, Vector2(0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f)));
	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, quad.maxs.x, quad.maxs.y), viewColor, Vector2(1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f)));
	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, quad.maxs.x, quad.mins.y), viewColor, Vector2(1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f)));
	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, quad.mins.x, quad.mins.y), viewColor, Vector2(0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f)));
	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, quad.mins.x, quad.maxs.y), viewColor, Vector2(0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f)));

	CopyMeshVertexData(m_vertexArray);

	SetDrawMode(GL_TRIANGLE_STRIP);
}

//-----------------------------------------------------------------------------------------------------------

//this one still has some issues with texture tearing.
void Mesh::InitializeQuad2DMesh(AABB2 quad, const Rgba& viewColor){
	Vertex3Ds in_vertexArray;
	
	GenerateVertexArrayTextureQuad(in_vertexArray, quad, AABB2::ZERO_TO_ONE, viewColor); //AABB2::ZERO_TO_ONE
 
 	CopyMeshVertexData(in_vertexArray);

	m_numIndicesToDraw = 6;
	
	std::vector<unsigned int> meshIndices;
	meshIndices.clear();
	meshIndices.reserve( m_numIndicesToDraw * sizeof(unsigned int));

	meshIndices.push_back(0);
	meshIndices.push_back(1);
	meshIndices.push_back(2);
	
	meshIndices.push_back(2);
	meshIndices.push_back(3);
	meshIndices.push_back(0);

	CopyMeshIndexData(meshIndices);
	SetDrawMode(GL_TRIANGLES);
}

//-----------------------------------------------------------------------------------------------------------


void Mesh::InitializeQuad3DMesh(AABB3 quadBox, const Rgba& viewColor){
	//PROFILE_SECTION();

	Vertex3Ds in_vertexArray;

	GenerateVertexArrayQuad3D(in_vertexArray, quadBox, viewColor, true);

	CopyMeshVertexData(in_vertexArray);

	SetDrawMode(GL_TRIANGLES);

}


//-----------------------------------------------------------------------------------------------------------

void Mesh::InitializeCursorRenderBoundsMesh(const float& cursorImageSize){
	Vertex3Ds inMouseVerts;
	inMouseVerts.reserve(4);

	if (theInputSystem){
		AABB2 cursorBox = AABB2(Vector2(0.0f, 0.0f), Vector2(-cursorImageSize, -cursorImageSize)) +
			(theInputSystem->GetMouseScreenPosition() + Vector2(cursorImageSize * 0.5f, cursorImageSize* 0.5f));
		GenerateVertexArrayTextureQuad(inMouseVerts, cursorBox, AABB2::ZERO_TO_ONE, Rgba::WHITE, true);
	}
	
	CopyMeshVertexData(inMouseVerts);
}

//===========================================================================================================
