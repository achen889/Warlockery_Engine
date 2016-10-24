//==============================================================================================================
//MeshRenderer.cpp
//by Albert Chen Nov-15-2015.
//==============================================================================================================

#include "MeshRenderer.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"

//===========================================================================================================

MeshRendererMap MeshRenderer::s_globalMeshRenderers;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///mesh renderer

MeshRenderer::MeshRenderer(const std::string name, bool allocMat, bool allocMesh):
	m_name(StringToWritableCStr(name)),
	m_didAllocMat(allocMat),
	m_didAllocMesh(allocMesh)
{
	m_vaoID = CreateVertexArrayObject();
	if (allocMat) {
		AllocMaterial();
	}
	if (allocMesh) {
		AllocMesh();
	}

}

//-----------------------------------------------------------------------------------------------------------

MeshRenderer::~MeshRenderer(){

	FreeMaterial();
	
	FreeMesh();	

	DestroyVertexArrayObject();
}

//-----------------------------------------------------------------------------------------------------------

unsigned int MeshRenderer::CreateVertexArrayObject(){
	return theOGLRenderer->CreateVertexArrayObject(m_vaoID);
}

//-----------------------------------------------------------------------------------------------------------

void MeshRenderer::DestroyVertexArrayObject() {

	return theOGLRenderer->DestroyVertexArrayObject(m_vaoID);

}

//-----------------------------------------------------------------------------------------------------------

void MeshRenderer::InitializeMeshRenderer(const Mesh& mesh, const Material& material){
	SetMesh(mesh);
	SetMaterial(material);

	BindVertexArray();
}

//-----------------------------------------------------------------------------------------------------------

void MeshRenderer::InitializeCoordinateAxesMeshRenderer(const int& length){
	m_mesh->InitializeMeshCoordinateAxes(length);

	m_material->InitializeMaterial("Data/Shaders/basic.vert", "Data/Shaders/basic.frag");
	m_material->m_samplerInUse = false;
	BindVertexArray();
}


//-----------------------------------------------------------------------------------------------------------

void MeshRenderer::BindVertexArray(){

	glBindVertexArray(m_vaoID);

	m_mesh->BindVertexArray();

// 	glBindBuffer(GL_ARRAY_BUFFER, (GLuint)m_mesh->m_vboID);
// 
// 	if (m_mesh->m_numIndicesToDraw != 0) {
// 		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)m_mesh->m_iboID);
// 	}

	GLuint materialProgramID = (GLuint)m_material->GetProgramID();

	if (theOGLRenderer){
		BindPosition(materialProgramID);
		BindColor(materialProgramID);
		BindTexCoords(materialProgramID);
		BindNormals(materialProgramID);

		//tangent and bi tangent for lighting
		theOGLRenderer->ProgramBindAttribute((GLuint)materialProgramID, "inTangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), offsetof(Vertex3D, m_tangent));

		theOGLRenderer->ProgramBindAttribute((GLuint)materialProgramID, "inBiTangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), offsetof(Vertex3D, m_biTangent));
	}
}

//-----------------------------------------------------------------------------------------------------------

void MeshRenderer::BindPosition(unsigned int programID){
	//Bind Position Attribute Vector3
	theOGLRenderer->ProgramBindAttribute((GLuint)programID, "inPosition", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), offsetof(Vertex3D, m_position));

}

//-----------------------------------------------------------------------------------------------------------

void MeshRenderer::BindColor(unsigned int programID){
	//Bind Color Attribute Rgba
	theOGLRenderer->ProgramBindAttribute((GLuint)programID, "inColor", 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex3D), offsetof(Vertex3D, m_color));

}

//-----------------------------------------------------------------------------------------------------------

void MeshRenderer::BindTexCoords(unsigned int programID){
	//Bind TexCoords Attribute Vector2
	theOGLRenderer->ProgramBindAttribute((GLuint)programID, "inUV", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), offsetof(Vertex3D, m_texCoords));

}

//-----------------------------------------------------------------------------------------------------------

void MeshRenderer::BindNormals(unsigned int programID){
	//Bind Normal Attribute Vector3
	theOGLRenderer->ProgramBindAttribute((GLuint)programID, "inNormal", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), offsetof(Vertex3D, m_normal));
}

//-----------------------------------------------------------------------------------------------------------

//unified light binding
void MeshRenderer::BindUnifiedLight(Light* m_light){
	if (m_light){
		m_material->m_glProgram.BindFloat("gLightPower", m_light->m_lightPower);
		m_material->m_glProgram.BindColor("gLightColor", m_light->m_lightColor);

		m_material->m_glProgram.BindVector2("gLightIntensity", m_light->m_lightIntensity);
		m_material->m_glProgram.BindVector3("gLightPosition", m_light->m_lightPosition);
		m_material->m_glProgram.BindVector3("gLightFacing", m_light->m_lightFacing);

		m_material->m_glProgram.BindFloat("gLightPositionFactor", m_light->m_lightPositionFactor);
		m_material->m_glProgram.BindFloat("gLightFacingFactor", m_light->m_lightFacingFactor);
		m_material->m_glProgram.BindFloat("gLightAngle", m_light->GetLightAngle());
		m_material->m_glProgram.BindFloat("gSpotPower", m_light->m_spotPower);
	}
}

//-----------------------------------------------------------------------------------------------------------


void MeshRenderer::RenderMeshWithLights(Camera3D& camera, bool isPerspective, Lights& lights, ModelViewMatrix* modelView){
	if (theOGLRenderer){

		//binds material shader and all the uniforms I might ever need
		m_material->PrepareShaderForRendering(camera, isPerspective, modelView);
		
		m_material->BindUnifiedLights(lights);
		// 		if (light){
		// 			BindUnifiedLight(light);
		// 		}

		//theOGLRenderer->PrepareMaterialShaderForRendering(*m_material, camera, isPerspective);
		//if it has a texture and sampler
		if (m_material->m_samplerInUse){

			RenderWithTextureSampler();

		}
		else{
			if (m_mesh->m_numIndicesToDraw == 0){
				DrawMeshVertexArray();
			}
			else{
				DrawIndexedMeshVertexArray();
			}
		}

		UnbindMeshRenderer();
	}
}

//-----------------------------------------------------------------------------------------------------------

void MeshRenderer::RenderMesh2D(ModelViewMatrix* modelView, Light* light){
	if (theOGLRenderer){

		//binds material shader and all the uniforms I might ever need
		ASSERT_AND_DIE((m_material != NULL), "mat is null!!");
		m_material->PrepareShaderForRendering2D(modelView);

		if (light){
			BindUnifiedLight(light);
		}
		
		//if it has a texture and sampler
		if (m_material->m_samplerInUse){

			RenderWithTextureSampler();

		}
		else{

			if (m_mesh->m_numIndicesToDraw == 0){
				DrawMeshVertexArray();
			}
			else{
				DrawIndexedMeshVertexArray();
			}

		}

		UnbindMeshRenderer();

	}
}

//-----------------------------------------------------------------------------------------------------------

void MeshRenderer::RenderMesh2DWithLights(ModelViewMatrix* modelView, Lights& lights) {
	if (theOGLRenderer) {

		//binds material shader and all the uniforms I might ever need
		ASSERT_AND_DIE((m_material != NULL), "mat is null!!");
		m_material->PrepareShaderForRendering2D(modelView);

		m_material->BindUnifiedLights(lights);
		
		//if it has a texture and sampler
		if (m_material->m_samplerInUse) {

			RenderWithTextureSampler();

		}
		else {
			if (m_mesh->m_numIndicesToDraw == 0) {
				DrawMeshVertexArray();
			}
			else {
				DrawIndexedMeshVertexArray();
			}

		}
		UnbindMeshRenderer();

	}
}


//-----------------------------------------------------------------------------------------------------------

//this one works and is even used for mesh 2d
void MeshRenderer::RenderMesh(Camera3D& camera, bool isPerspective, Light* light, ModelViewMatrix* modelView){
	//PROFILE_SECTION();
	if (theOGLRenderer){
		//PROFILE_START("PrepareMesh");
		//binds material shader and all the uniforms I might ever need
		m_material->PrepareShaderForRendering(camera, isPerspective, modelView);

		if (light){

			BindUnifiedLight(light);
		}

		//PROFILE_STOP();

		//if it has a texture and sampler
		if (m_material->m_samplerInUse){

			RenderWithTextureSampler();

		}
		else{

			if (m_mesh->m_numIndicesToDraw == 0){
				DrawMeshVertexArray();
			}
			else{
				DrawIndexedMeshVertexArray();
			}

		}

		UnbindMeshRenderer();

	}
}

//-----------------------------------------------------------------------------------------------------------

void MeshRenderer::UnbindMeshRenderer(){
	//PROFILE_SECTION();
	
	theOGLRenderer->DisableShaderProgram();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);

	//glDeleteBuffers(1, &m_mesh->m_vboID);

	//if (m_vaoID != NULL) {
	//glDeleteVertexArrays(1, &m_vaoID);
	//}


	//theOGLRenderer->g_BufferCount--;
	//if (myVAO.m_Program != NULL) {
	//	glDeleteProgram(myVAO.m_Program);
	//	}


}

//-----------------------------------------------------------------------------------------------------------

void MeshRenderer::DrawMeshVertexArray(){
	glBindVertexArray(m_vaoID);

	theOGLRenderer->DrawVertexArray((GLenum)m_mesh->m_drawMode, (GLuint)m_vaoID, (size_t)m_mesh->m_numVerticesToDraw);

	glBindVertexArray(0);
}


//-----------------------------------------------------------------------------------------------------------

void MeshRenderer::DrawIndexedMeshVertexArray(){
	//PROFILE_SECTION()
	
	glBindVertexArray(m_vaoID);

	theOGLRenderer->DrawVertexElements((GLenum)m_mesh->m_drawMode, (GLuint)m_mesh->m_iboID, (GLint)m_mesh->m_numIndicesToDraw, (size_t)0);

	glBindVertexArray(0);

}

//-----------------------------------------------------------------------------------------------------------

void MeshRenderer::RenderWithTextureSampler(){
	//PROFILE_SECTION();

	glEnable(GL_TEXTURE_2D);

	m_material->BindMaterialTextures();

	//theOGLRenderer->CreateVertexArrayWithShader((GLuint)m_material->m_glProgram.m_programID, (GLuint)m_mesh->m_vboID, (GLuint)m_vaoID);

	if (m_mesh->m_numIndicesToDraw == 0){
		DrawMeshVertexArray();
	}
	else{
		DrawIndexedMeshVertexArray();
	}
	glDisable(GL_TEXTURE_2D);
}

//-----------------------------------------------------------------------------------------------------------

///----------------------------------------------------------------------------------------------------------
///friend methods

void SetDefaultMeshRenderer(MeshRenderer& meshRenderer){
	//meshRenderer = MeshRenderer();

	//meshRenderer.m_material = new Material();
	meshRenderer.SetDefaultMaterial();

	//meshRenderer.m_mesh = new Mesh();
	meshRenderer.m_mesh->SetDrawMode(GL_QUADS);

	meshRenderer.BindVertexArray();
}

//-----------------------------------------------------------------------------------------------------------

//assumes mesh renderer has already been newed off
void SetDefaultTexturedMeshRenderer(MeshRenderer& meshRenderer){

	meshRenderer.m_material->InitializeMaterial("Data/Shaders/basicSampler.vert", "Data/Shaders/basicSampler.frag");

	meshRenderer.m_mesh->SetDrawMode(GL_QUADS);

	meshRenderer.BindVertexArray();
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///static helpers

MeshRenderer* MeshRenderer::CreateOrGetMeshRenderer(const std::string& name, bool allocMat, bool allocMesh, bool allowNullMeshRenderer) {

	UNUSED(allowNullMeshRenderer);

	MeshRendererMapIterator meshIter = s_globalMeshRenderers.find(name);
	if (meshIter != s_globalMeshRenderers.end()) {
		return meshIter->second;
	}
	MeshRenderer* newMeshRenderer = new MeshRenderer(name, allocMat, allocMesh); //mem leak here

														//if not null add to list
	if (!(newMeshRenderer->GetName() == "")) {
		s_globalMeshRenderers[name] = newMeshRenderer;
	}
	return newMeshRenderer;

}

//===========================================================================================================