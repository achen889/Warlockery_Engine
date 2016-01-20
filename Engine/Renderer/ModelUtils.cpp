//==============================================================================================================
//ModelUtils.cpp
//by Albert Chen Aug-24-2015.
//==============================================================================================================

#include "ModelUtils.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"

//assumes only common model file path
MeshRenderer WarlockeryModelLoader::CreateNewMeshRendererFromWarlockeryModelFile(const std::string& fileName){

	MeshRenderer meshRenderer = MeshRenderer();

	meshRenderer.m_mesh = new Mesh();

	meshRenderer.m_mesh->SetDrawMode(GL_TRIANGLES);

	LoadWarlockeryModelFileToMesh(fileName, meshRenderer.m_mesh, true);

	meshRenderer.m_material = new Material();

	std::string modelShaderName = "ModelLighting";

	SetCustomModelMaterial(meshRenderer.m_material, modelShaderName + ".vert", modelShaderName + ".frag");

	LoadModelTexturesForWarlockeryModelFile(meshRenderer.m_material, fileName);
	
	meshRenderer.BindVertexArray();

	return meshRenderer;

}

//-----------------------------------------------------------------------------------------------------------

bool WarlockeryModelLoader::VerifyWarlockeryModelFile(FileHeader* fileHeader){

	unsigned char* WarlockeryModelFourCC = new unsigned char[4];
	WarlockeryModelFourCC[0] = 'G';
	WarlockeryModelFourCC[1] = 'C';
	WarlockeryModelFourCC[2] = '2';
	WarlockeryModelFourCC[3] = '3';

	if (CompareByteArrays(fileHeader->fourCC, WarlockeryModelFourCC, 4)){

		if (fileHeader->subtype == (unsigned char)2){

			if (fileHeader->version == (unsigned char)1){

				return true;
			}
		}
	}

	return false;
}

//-----------------------------------------------------------------------------------------------------------


//simple hacked together load function, assumes file exists and mesh is initialized to default
void WarlockeryModelLoader::LoadWarlockeryModelFileToMesh(const std::string& fileName, Mesh* mesh, bool useCommonFilePath){
	const std::string WarlockeryModelExt = ".c23";
	std::string fullModelPath = fileName + WarlockeryModelExt;

	if (useCommonFilePath)
		fullModelPath = COMMON_MODEL_FILE_PATH + fileName + "/" + fileName + WarlockeryModelExt;

	BinaryFileParser WarlockeryModelParser = BinaryFileParser(fullModelPath);

	m_fileHeader.ReadFileHeader(WarlockeryModelParser);

	if (VerifyWarlockeryModelFile(&m_fileHeader)){
		//read num vertices
		//mesh = new Mesh();
		mesh->m_numVerticesToDraw = WarlockeryModelParser.ReadNextUInt();
		
		//read to vertex 3Ds
		Vertex3Ds meshVerts;
		meshVerts.clear();
		meshVerts.reserve(mesh->m_numVerticesToDraw * sizeof(Vertex3D));

		for (unsigned int i = 0; i < mesh->m_numVerticesToDraw; i++){
			meshVerts.push_back(WarlockeryModelParser.ReadNextVertex3D());
		}

		mesh->CopyMeshVertexData(meshVerts);

		//bind indices
		mesh->m_numIndicesToDraw = WarlockeryModelParser.ReadNextUInt();

		//DEBUG temporarily test lighting with vertex arrays
		mesh->m_numIndicesToDraw = 0;

		std::vector<unsigned int> meshIndices;
		meshIndices.clear();
		meshIndices.reserve(mesh->m_numIndicesToDraw * sizeof(unsigned int));

	

		if (mesh->m_numIndicesToDraw > 0){
			for (unsigned int i = 0; i < mesh->m_numIndicesToDraw; i++){
				meshIndices.push_back(WarlockeryModelParser.ReadNextUInt());
			}

			mesh->CopyMeshIndexData(meshIndices);
		}
		else{
			mesh->m_numIndicesToDraw = 0;
		}

	

	}
}

//-----------------------------------------------------------------------------------------------------------

//assumes basic material setup complete
void WarlockeryModelLoader::LoadModelTexturesForWarlockeryModelFile(Material* material, const std::string& fileName){
	std::string dynamicFilePath = GetCommonFilePathForModelName(fileName);
	
	std::string diffuseTextureName = fileName + c_diffuseTexExt;
	std::string normalTextureName = fileName + c_normalTexExt;
	std::string specularTextureName = fileName + c_specularTexExt;

	//load diffuse
	if (!VerifyTexturePath(dynamicFilePath + diffuseTextureName )){
		material->SetTextureInMap("gTexDiffuse", COMMON_TEXTURE_DIFFUSE);
	}
	else{
		LoadModelTextureToMaterial(material, fileName, diffuseTextureName, "gTexDiffuse", true);
	}

	//load normal
	if (!VerifyTexturePath(dynamicFilePath + normalTextureName)){
		material->SetTextureInMap("gTexNormal", COMMON_TEXTURE_NORMAL);
	}
	else{
		LoadModelTextureToMaterial(material, fileName, normalTextureName, "gTexNormal", true);
	}

	//load specular
	if (!VerifyTexturePath(dynamicFilePath + specularTextureName)){
		material->SetTextureInMap("gTexSpecular", COMMON_TEXTURE_SPECULAR);
	}
	else{
		LoadModelTextureToMaterial(material, fileName, specularTextureName, "gTexSpecular", true);
	}
}

//-----------------------------------------------------------------------------------------------------------


//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///model helpers

void SetDefaultMaterial(Material* material){
	material = new Material();

	material->m_samplerInUse = false;

	material->InitializeMaterial( (COMMON_SHADER_FILE_PATH + "basic.vert").c_str() , (COMMON_SHADER_FILE_PATH + "basic.frag").c_str() );

}

//-----------------------------------------------------------------------------------------------------------

//assume ptr already set to default
void SetCustomModelMaterial(Material* material, const std::string& vertFile, const std::string& fragFile, bool useCommonFilePath){
	std::string vertPath = vertFile;
	std::string fragPath = fragFile;

	if (useCommonFilePath){
		vertPath = COMMON_SHADER_FILE_PATH + vertFile;
		fragPath = COMMON_SHADER_FILE_PATH + fragFile;
	}

	//material = new Material();
	material->m_samplerInUse = true;
	material->InitializeMaterial(vertPath.c_str(), fragPath.c_str());
}

//-----------------------------------------------------------------------------------------------------------\

//assume model already set up
void LoadModelTextureToMaterial(Material* material, const std::string& modelName, const std::string& textureName , const std::string& textureBindName , bool useCommonFilePath ){

	std::string modelTexturePath = modelName;

	if (useCommonFilePath){
		modelTexturePath = COMMON_MODEL_FILE_PATH + modelName + "/" + textureName;
	}
	
	material->SetTextureInMap(textureBindName, modelTexturePath /*"DefaultDiffuseTexel" */);

}

//-----------------------------------------------------------------------------------------------------------

std::string GetCommonFilePathForModelName(const std::string& modelName){
	return std::string(COMMON_MODEL_FILE_PATH + modelName + "/");
}