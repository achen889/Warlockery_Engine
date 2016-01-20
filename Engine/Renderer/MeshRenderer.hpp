//==============================================================================================================
//MeshRenderer.hpp
//by Albert Chen Nov-15-2015.
//==============================================================================================================

#pragma once
#include "Mesh.hpp"
#include "Material.hpp"
#include "Engine/Math/MatrixUtils.hpp"
#include "Light.hpp"

#ifndef _included_MeshRenderer__
#define _included_MeshRenderer__

//===========================================================================================================

class Camera3D;

//===========================================================================================================

struct MeshRenderer{
	Mesh* m_mesh = NULL;
	Material* m_material = NULL;

	unsigned int m_vaoID; //vertex array ID for this

	MeshRenderer();
	~MeshRenderer();

	unsigned int CreateVertexArrayObject();

	void InitializeMeshRenderer(const Mesh& mesh, const Material& material);

	void InitializeCoordinateAxesMeshRenderer(const int& length = 1);

	void BindVertexArray();

	void SetMesh(const Mesh& mesh){
		m_mesh = new Mesh(mesh);
	}

	void SetMaterial(const Material& material){
		m_material = new Material(material);
	}

	//assumes no texture, assumes basic.vert exists
	void SetDefaultMaterial(){
		m_material->InitializeDefaultMaterial();
	}

	void SetCustomMaterial(const std::string& vertFile, const std::string& fragFile){
		m_material = new Material();
		m_material->m_samplerInUse = true;
		m_material->InitializeMaterial(vertFile.c_str(), fragFile.c_str());
	}

	//attribute binding
	void BindPosition(unsigned int programID);
	void BindColor(unsigned int programID);
	void BindTexCoords(unsigned int programID);
	void BindNormals(unsigned int programID);
	void BindUnifiedLight(Light* m_light);

	//render mesh
	void RenderMesh(Camera3D& camera, bool isPerspective, Light* m_light = NULL, ModelViewMatrix* modelView = NULL);
	void RenderMesh2D(ModelViewMatrix* modelView = NULL, Light* light = NULL);
	void RenderMeshWithLights(Camera3D& camera, bool isPerspective, Lights lights, ModelViewMatrix* modelView = NULL);

	void UnbindMeshRenderer();
	void DrawMeshVertexArray();
	void DrawIndexedMeshVertexArray();
	void RenderWithTextureSampler();

	friend void SetDefaultMeshRenderer(MeshRenderer& meshRenderer);
	friend void SetDefaultTexturedMeshRenderer(MeshRenderer& meshRenderer);
	friend bool IsMeshRendererValid(const MeshRenderer* meshRenderer);
};

//===========================================================================================================

inline bool IsMeshRendererValid(const MeshRenderer* meshRenderer){
	return (meshRenderer && meshRenderer->m_mesh && meshRenderer->m_material);
}

//===========================================================================================================

#endif //__includedMeshRenderer__

