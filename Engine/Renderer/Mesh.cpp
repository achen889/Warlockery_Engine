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

MeshMap Mesh::s_globalMeshes;

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
	
	DestroyVertexBufferObject();
	DestroyIndexBufferObject();

}

//-----------------------------------------------------------------------------------------------------------

void Mesh::BindVertexArray() {
	theOGLRenderer->BindVertexArray(m_vboID, m_iboID, m_numIndicesToDraw);
}

void Mesh::DestroyVertexBufferObject() {
	//something about deleting the buffers
	theOGLRenderer->DestroyVertexBufferObject(m_vboID);
}

void Mesh::DestroyIndexBufferObject() {
	//something about deleting the buffers
	theOGLRenderer->DestroyIndexBufferObject(m_iboID);
}
//-----------------------------------------------------------------------------------------------------------

void Mesh::CopyMeshVertexData(Vertex3Ds& vertexArray){

	if (theOGLRenderer){ 
		m_numVerticesToDraw = vertexArray.size();

		size_t sizeOfVertexBuffer = CalcVertexArraySize(vertexArray);

		theOGLRenderer->CopyVertexBufferData((GLuint)m_vboID, vertexArray.data(), (size_t)sizeOfVertexBuffer);
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

void Mesh::ClearMesh() {
	//Vertex3Ds m_vertexArray;

	m_meshVertexArray.clear();

	CopyMeshVertexData(m_meshVertexArray);
}

//-----------------------------------------------------------------------------------------------------------

void Mesh::InitializeSphereMesh(Sphere3 sphere, const Rgba& color ){
	//Vertex3Ds m_vertexArray;

	m_meshVertexArray.clear();

	GenerateVertexArraySphere3D(m_meshVertexArray, sphere, 32, 32, color );

	CopyMeshVertexData(m_meshVertexArray);

	SetDrawMode(GL_TRIANGLE_STRIP);
}

//-----------------------------------------------------------------------------------------------------------

void Mesh::InitializeMeshCoordinateAxes(const int& length, bool includeNegAxes ){
	//Vertex3Ds m_vertexArray;
	float lenF = (float)length;
	//use draw line strip
	m_meshVertexArray.clear();
	m_meshVertexArray.reserve(10000);
	//coordinate axes for normal
	//x axis
	m_meshVertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), Rgba::RED, Vector2::ZERO));
	m_meshVertexArray.push_back(Vertex3D(Vector3(lenF, 0.0f, 0.0f), Rgba::RED, Vector2::ZERO));
	//x arrowhead
// 	m_vertexArray.push_back(Vertex3D(Vector3(lenF, 0.1f, 0.0f), Rgba::RED, Vector2::ZERO));
// 	m_vertexArray.push_back(Vertex3D(Vector3(lenF + 0.2f, 0.0f, 0.0f), Rgba::RED, Vector2::ZERO));
// 	m_vertexArray.push_back(Vertex3D(Vector3(lenF, -0.1f, 0.0f), Rgba::RED, Vector2::ZERO));
// 	m_vertexArray.push_back(Vertex3D(Vector3(lenF, 0.0f, 0.0f), Rgba::RED, Vector2::ZERO));
	//y axis
	m_meshVertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), Rgba::GREEN, Vector2::ZERO));
	m_meshVertexArray.push_back(Vertex3D(Vector3(0.0f, lenF, 0.0f), Rgba::GREEN, Vector2::ZERO));
	//y arrowhead
// 	m_vertexArray.push_back(Vertex3D(Vector3(0.1f, lenF, 0.0f), Rgba::GREEN, Vector2::ZERO));
// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, lenF + 0.2f, 0.0f), Rgba::GREEN, Vector2::ZERO));
// 	m_vertexArray.push_back(Vertex3D(Vector3(-0.1f, lenF, 0.0f), Rgba::GREEN, Vector2::ZERO));
// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, lenF, 0.0f), Rgba::GREEN, Vector2::ZERO));
	//z axis
	m_meshVertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), Rgba::BLUE, Vector2::ZERO));
	m_meshVertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, lenF), Rgba::BLUE, Vector2::ZERO));
	//z arrowhead
// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.1f, lenF), Rgba::BLUE, Vector2::ZERO));
// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, lenF + 0.2f), Rgba::BLUE, Vector2::ZERO));
// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, -0.1f, lenF), Rgba::BLUE, Vector2::ZERO));
// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, lenF), Rgba::BLUE, Vector2::ZERO));
	
	if (includeNegAxes){
		//neg x axis
		m_meshVertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), Rgba::RED, Vector2::ZERO));
		m_meshVertexArray.push_back(Vertex3D(Vector3(-lenF, 0.0f, 0.0f), Rgba::RED, Vector2::ZERO));
		//x arrowhead
		// 	m_vertexArray.push_back(Vertex3D(Vector3(-lenF, 0.1f, 0.0f), Rgba::MAGENTA, Vector2::ZERO));
		// 	m_vertexArray.push_back(Vertex3D(Vector3(-lenF + 0.2f, 0.0f, 0.0f), Rgba::ORANGE, Vector2::ZERO));
		// 	m_vertexArray.push_back(Vertex3D(Vector3(-lenF, -0.1f, 0.0f), Rgba::MAGENTA, Vector2::ZERO));
		// 	m_vertexArray.push_back(Vertex3D(Vector3(-lenF, 0.0f, 0.0f), Rgba::RED, Vector2::ZERO));
		//neg y axis
		m_meshVertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), Rgba::GREEN, Vector2::ZERO));
		m_meshVertexArray.push_back(Vertex3D(Vector3(0.0f, -lenF, 0.0f), Rgba::GREEN, Vector2::ZERO));
		//y arrowhead
		// 	m_vertexArray.push_back(Vertex3D(Vector3(0.1f, -lenF, 0.0f), Rgba::NEON_GREEN, Vector2::ZERO));
		// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, -lenF + 0.2f, 0.0f), Rgba::MINT_GREEN, Vector2::ZERO));
		// 	m_vertexArray.push_back(Vertex3D(Vector3(-0.1f, -lenF, 0.0f), Rgba::NEON_GREEN, Vector2::ZERO));
		// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, -lenF, 0.0f), Rgba::GREEN, Vector2::ZERO));
		//neg z axis
		m_meshVertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), Rgba::BLUE, Vector2::ZERO));
		m_meshVertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, -lenF), Rgba::BLUE, Vector2::ZERO));
		//z arrowhead
		// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.1f, -lenF), Rgba::LAVENDER, Vector2::ZERO));
		// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, -lenF + 0.2f), Rgba::TRON_BLUE, Vector2::ZERO));
		// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, -0.1f, -lenF), Rgba::LAVENDER, Vector2::ZERO));
		// 	m_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, -lenF), Rgba::BLUE, Vector2::ZERO));
	}
	

	CopyMeshVertexData(m_meshVertexArray);

	SetDrawMode(GL_LINES);
}

//-----------------------------------------------------------------------------------------------------------

//vertex array of a quad from 0 to 1
void Mesh::InitializeQuadMesh(const Rgba& viewColor){
	//Vertex3Ds m_vertexArray;

	m_meshVertexArray.clear();
	m_meshVertexArray.reserve(5);

	//const Vector2s defaultTexCoords = GetDefaultTextureCoordinates();
	const float quadSize = 1.0f;
	m_meshVertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, quadSize), viewColor, Vector2(0, 0), Vector3(-1.0f, 0.0f, 0.0f)));
	m_meshVertexArray.push_back(Vertex3D(Vector3(0.0f, quadSize, quadSize), viewColor, Vector2(1, 0), Vector3(-1.0f, 0.0f, 0.0f)));
	m_meshVertexArray.push_back(Vertex3D(Vector3(0.0f, quadSize, 0.0f), viewColor, Vector2(1, 1), Vector3(-1.0f, 0.0f, 0.0f)));
	m_meshVertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), viewColor, Vector2(0, 1), Vector3(-1.0f, 0.0f, 0.0f)));
	m_meshVertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, quadSize), viewColor, Vector2(0, 0), Vector3(-1.0f, 0.0f, 0.0f)));

	CopyMeshVertexData(m_meshVertexArray);

	SetDrawMode(GL_TRIANGLE_STRIP);
}

//vertex array of a quad facing -X with correct 3d texturing
void Mesh::InitializeQuadMesh(const AABB2& quad, const Rgba& viewColor){
	//Vertex3Ds m_vertexArray;

	m_meshVertexArray.clear();
	m_meshVertexArray.reserve(10000);

	//const Vector2s defaultTexCoords = GetDefaultTextureCoordinates();
	//const float quadSize = 1.0f;
	m_meshVertexArray.push_back(Vertex3D(Vector3(0.0f, quad.mins.x, quad.maxs.y), viewColor, Vector2(0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f)));
	m_meshVertexArray.push_back(Vertex3D(Vector3(0.0f, quad.maxs.x, quad.maxs.y), viewColor, Vector2(1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f)));
	m_meshVertexArray.push_back(Vertex3D(Vector3(0.0f, quad.maxs.x, quad.mins.y), viewColor, Vector2(1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f)));
	m_meshVertexArray.push_back(Vertex3D(Vector3(0.0f, quad.mins.x, quad.mins.y), viewColor, Vector2(0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f)));
	m_meshVertexArray.push_back(Vertex3D(Vector3(0.0f, quad.mins.x, quad.maxs.y), viewColor, Vector2(0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f)));

	CopyMeshVertexData(m_meshVertexArray);

	SetDrawMode(GL_TRIANGLE_STRIP);
}

//-----------------------------------------------------------------------------------------------------------

//this one still has some issues with texture tearing. uses default 0-1 texcoords
void Mesh::InitializeQuad2DMesh(AABB2 quad, const Rgba& viewColor){

	GenerateVertexArrayTextureQuad(m_meshVertexArray, quad, AABB2::ZERO_TO_ONE, viewColor); //AABB2::ZERO_TO_ONE
 
 	CopyMeshVertexData(m_meshVertexArray);
	
	m_numIndicesToDraw = 6;
	
	//std::vector<unsigned int> m_meshIndices;
	m_meshIndices.clear();
	m_meshIndices.reserve( m_numIndicesToDraw);

	m_meshIndices.push_back(0);
	m_meshIndices.push_back(1);
	m_meshIndices.push_back(2);

	m_meshIndices.push_back(2);
	m_meshIndices.push_back(3);
	m_meshIndices.push_back(0);

	//4 index triangle strip
// 	m_meshIndices.push_back(1);
// 	m_meshIndices.push_back(0);
// 	m_meshIndices.push_back(2);
// 	m_meshIndices.push_back(3);

	CopyMeshIndexData(m_meshIndices);
	SetDrawMode(GL_TRIANGLES);
	//SetDrawMode(GL_TRIANGLE_STRIP);
}

//-----------------------------------------------------------------------------------------------------------

void Mesh::InitializeDisc2DMesh(const Disc2& disc, const Rgba& viewColor) {
	
	Vertex3Ds discVerts;
	discVerts.clear();
	GenerateVertexArrayDisc2D(discVerts, disc, viewColor);

	CopyMeshVertexData(discVerts);

	//SetDrawMode(GL_QUADS);

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
///----------------------------------------------------------------------------------------------------------
///friend methods

void AppendIndicesToIndexArray(Indices& out_meshIndices, const Indices& indicesToAppend) {
	//append the out vert array
	for (IndicesConstIterator it = indicesToAppend.begin(); it != indicesToAppend.end(); ++it) {
		unsigned int idx = (*it);

		out_meshIndices.push_back(idx);
	}
}

//-----------------------------------------------------------------------------------------------------------

void ClearGlobalMeshes() {
	MeshMapIterator it;
	for (it = Mesh::s_globalMeshes.begin(); it != Mesh::s_globalMeshes.end(); ) {
		Mesh* mesh = (it->second);
		if (mesh) {
			delete mesh;
			mesh = NULL;
		}
		it = Mesh::s_globalMeshes.erase(it);
	}
	Mesh::s_globalMeshes.clear();
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///static methods

Mesh* Mesh::CreateOrGetMesh(const std::string& meshName, bool allowNullMesh) {

	UNUSED(allowNullMesh);

	MeshMapIterator meshIter = s_globalMeshes.find(meshName);
	if (meshIter != s_globalMeshes.end()) {
		return meshIter->second;
	}
	Mesh* newmesh = new Mesh(meshName); //mem leak here

	//if not null add to list
	if (!(newmesh->GetName() == "")) {
		s_globalMeshes[meshName] = newmesh;
	}
	return newmesh;

}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================