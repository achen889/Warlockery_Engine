//==============================================================================================================
//MeshRenderer.hpp
//by Albert Chen Nov-15-2015.
//==============================================================================================================

#pragma once
#include "Mesh.hpp"
#include "Material.hpp"

#include "Engine/Math/MatrixUtils.hpp"

#ifndef _included_MeshRenderer__
#define _included_MeshRenderer__

//===========================================================================================================

class Camera3D;
struct MeshRenderer;

//===========================================================================================================

typedef std::map<std::string, MeshRenderer*> MeshRendererMap;
typedef std::map<std::string, MeshRenderer*>::iterator MeshRendererMapIterator;
typedef std::pair<std::string, MeshRenderer*> MeshRendererMapEntry;

//===========================================================================================================

struct MeshRenderer{
	Mesh* m_mesh = NULL;
	Material* m_material = NULL;

	bool m_didAllocMesh;
	bool m_didAllocMat;
	char* m_name;

	unsigned int m_vaoID; //vertex array ID for this
	
	MeshRenderer(const std::string name = "", bool allocMat = true, bool allocMesh = true);
	~MeshRenderer();

	unsigned int CreateVertexArrayObject();

	void DestroyVertexArrayObject();

	void InitializeMeshRenderer(const Mesh& mesh, const Material& material);

	void InitializeCoordinateAxesMeshRenderer(const int& length = 1);

	void BindVertexArray();

	void AllocMaterial();
	void AllocMesh();

	void FreeMaterial();
	void FreeMesh();

	void SetName(const std::string& name) { m_name = StringToWritableCStr(name); }
	const std::string GetName() { return std::string(m_name); }
	
	void SetMaterial(const Material& material);
	void SetMesh(const Mesh& mesh);

	void SetMesh(Mesh* mesh);
	void SetMaterial(Material* material);

	//assumes no texture, assumes basic.vert exists
	void SetDefaultMaterial(){
		m_material->InitializeDefaultMaterial();
	}

	void SetCustomMaterial(const std::string& vertFile, const std::string& fragFile){
		m_material = new Material();
		m_material->m_samplerInUse = true;
		m_material->InitializeMaterial(vertFile.c_str(), fragFile.c_str());
	}
	void SetCustomMaterial(const std::string& shader_name) {
		m_material = new Material();
		m_material->m_samplerInUse = true;
		m_material->InitializeMaterial(shader_name.c_str());
	}

	//attribute binding
	void BindPosition(unsigned int programID);
	void BindColor(unsigned int programID);
	void BindTexCoords(unsigned int programID);
	void BindNormals(unsigned int programID);
	void BindUnifiedLight(Light* m_light);

	//set shader
	void SetShader(const std::string& shaderName);

	//sets common texture types
	void SetTexture(const std::string& textureName, const std::string& texturePath = "");
	void SetDiffuseTexture(const std::string& diffuseTexturePath = "");
	void SetNormalTexture(const std::string& normalTexturePath = "");
	void SetSpecularTexture(const std::string& specularTexturePath = "");
	void SetEmissiveTexture(const std::string& emissiveTexturePath = "");
	
	//render mesh 3D
	void RenderMesh(Camera3D& camera, bool isPerspective, Light* m_light = NULL, ModelViewMatrix* modelView = NULL);
	void RenderMeshWithLights(Camera3D& camera, bool isPerspective, Lights& lights, ModelViewMatrix* modelView = NULL);
	//render 2D
	void RenderMesh2D(ModelViewMatrix* modelView = NULL, Light* light = NULL);
	void RenderMesh2DWithLights(ModelViewMatrix* modelView, Lights& lights);
	
	void UnbindMeshRenderer();

	void DrawMeshVertexArray();
	void DrawIndexedMeshVertexArray();
	void RenderWithTextureSampler();

	friend void SetDefaultMeshRenderer(MeshRenderer& meshRenderer);
	friend void SetDefaultTexturedMeshRenderer(MeshRenderer& meshRenderer);
	friend bool IsMeshRendererValid(const MeshRenderer* meshRenderer);

 	friend void ClearGlobalMeshRenderers();


	static MeshRendererMap s_globalMeshRenderers;
	static MeshRenderer* CreateOrGetMeshRenderer(const std::string& meshName, bool allocMat = true, bool allocMesh = true, bool allowNullMeshRenderer = false);
};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline void MeshRenderer::SetTexture(const std::string& textureName, const std::string& texturePath) {
	if (!m_material) {
		m_material = new Material();
	}
	//loads default if no texture specified
	if (texturePath == "") {
		m_material->SetTextureInMap(textureName, COMMON_TEXTURE_INVALID);
	}
	else {
		m_material->SetTextureInMap(textureName, texturePath);
	}
}

inline void MeshRenderer::SetDiffuseTexture(const std::string& diffuseTexturePath) {
	if (!m_material) {
		m_material = new Material();
	}
	//loads default if no texture specified
	if (diffuseTexturePath == "") {
		m_material->SetTextureInMap("gTexDiffuse", COMMON_TEXTURE_DIFFUSE);
		m_material->SetTextureInMap("gTexture", COMMON_TEXTURE_DIFFUSE);
	}
	else {
		m_material->SetTextureInMap("gTexDiffuse", diffuseTexturePath);
		m_material->SetTextureInMap("gTexture", diffuseTexturePath);
	}
}

//-----------------------------------------------------------------------------------------------------------

inline void MeshRenderer::SetNormalTexture(const std::string& normalTexturePath) {
	if (!m_material) {
		m_material = new Material();
	}
	//loads default if no texture specified
	if (normalTexturePath == "") {
		m_material->SetTextureInMap("gTexNormal", COMMON_TEXTURE_NORMAL);
	}
	else {
		m_material->SetTextureInMap("gTexNormal", normalTexturePath);
	}
}

//-----------------------------------------------------------------------------------------------------------

inline void MeshRenderer::SetSpecularTexture(const std::string& specularTexturePath) {
	if (!m_material) {
		m_material = new Material();
	}
	//loads default if no texture specified
	if (specularTexturePath == "") {
		m_material->SetTextureInMap("gTexSpecular", COMMON_TEXTURE_SPECULAR);
	}
	else {
		m_material->SetTextureInMap("gTexSpecular", specularTexturePath);
	}
}

//-----------------------------------------------------------------------------------------------------------

inline void MeshRenderer::SetEmissiveTexture(const std::string& emissiveTexturePath) {
	if (!m_material) {
		m_material = new Material();
	}
	//loads default if no texture specified
	if (emissiveTexturePath == "") {
		m_material->SetTextureInMap("gTexEmissive", COMMON_TEXTURE_EMISSIVE);
	}
	else {
		m_material->SetTextureInMap("gTexEmissive", emissiveTexturePath);
	}
}

//-----------------------------------------------------------------------------------------------------------

inline void MeshRenderer::SetShader(const std::string& shaderName) {
	AllocMaterial();
	m_material->InitializeShaderFromShaderName(shaderName);
}

//-----------------------------------------------------------------------------------------------------------

inline void MeshRenderer::AllocMaterial() {
	if (!m_material) {
		m_material = new Material();
	}
}

inline void MeshRenderer::AllocMesh() {
	if (!m_mesh) {
		m_mesh = new Mesh();
	}
}

inline void MeshRenderer::FreeMaterial() {
	if (m_material != NULL) {
		delete m_material;
		m_material = NULL;
	}
}

inline void MeshRenderer::FreeMesh() {
	if (m_mesh != NULL) {
		delete m_mesh;
		m_mesh = NULL;
	}
}

inline void MeshRenderer::SetMesh(const Mesh& mesh) {
	
	AllocMesh();
	*m_mesh = mesh;
}

inline void MeshRenderer::SetMaterial(const Material& material) {
	AllocMaterial();
	*m_material = material;
}

//-----------------------------------------------------------------------------------------------------------

inline void MeshRenderer::SetMesh(Mesh* mesh) {
	//AllocMesh();
	m_mesh = mesh;
}

inline void MeshRenderer::SetMaterial(Material* material) {
	//AllocMaterial();
	m_material = material;
}



//-----------------------------------------------------------------------------------------------------------

inline bool IsMeshRendererValid(const MeshRenderer* meshRenderer){
	return (meshRenderer && meshRenderer->m_mesh && meshRenderer->m_material);
}

//-----------------------------------------------------------------------------------------------------------

inline void ClearGlobalMeshRenderers() {
	MeshRendererMapIterator it;
	for (it = MeshRenderer::s_globalMeshRenderers.begin(); it != MeshRenderer::s_globalMeshRenderers.end(); ) {
		MeshRenderer* meshRen = (it->second);
		if (meshRen) {
			delete meshRen;
			meshRen = NULL;
		}
		it = MeshRenderer::s_globalMeshRenderers.erase(it);
	}
	MeshRenderer::s_globalMeshRenderers.clear();

}

//===========================================================================================================

#endif //__includedMeshRenderer__

