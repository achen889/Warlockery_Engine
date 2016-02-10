//==============================================================================================================
//ModelUtils.hpp
//by Albert Chen Aug-24-2015.
//==============================================================================================================

#pragma once

#ifndef _included_ModelUtils__
#define _included_ModelUtils__

#include "Engine/Core/BinaryUtils.hpp"
//#include "Engine/Renderer/OpenGLRenderer.hpp
//#include "Engine/Renderer/OGLRenderingUtils.hpp"

#include "Engine/Renderer/MeshRenderer.hpp"

struct FileHeader{
	unsigned char* fourCC = new unsigned char[4];
	unsigned char subtype;
	unsigned char version;
	std::string comment;

	//file header readers assume bfp already constructed

	void ReadFileHeader(BinaryFileParser& bfp){
		ReadFourCC(bfp);

		ReadSubtypeAndVersion(bfp);

		ReadComment(bfp);
	}

	void ReadFourCC(BinaryFileParser& bfp){
		fourCC[0] = bfp.ReadNextUChar();
		fourCC[1] = bfp.ReadNextUChar();
		fourCC[2] = bfp.ReadNextUChar();
		fourCC[3] = bfp.ReadNextUChar();
	}

	void ReadSubtypeAndVersion(BinaryFileParser& bfp){
		bfp.ReadNextUChar(subtype);
		bfp.ReadNextUChar(version);
	}

	void ReadComment(BinaryFileParser& bfp){
		bfp.ReadNextString(comment);
	}

};

struct WarlockeryModelLoader{
	
	//magic mesh rendererer loader
	MeshRenderer CreateNewMeshRendererFromWarlockeryModelFile(const std::string& fileName );
	
	//WarlockeryModel loading helpers
	void LoadWarlockeryModelFileToMesh(const std::string& fileName, Mesh* mesh, bool useCommonFilePath = true);
	void LoadModelTexturesForWarlockeryModelFile(Material* material, const std::string& fileName);
	bool VerifyWarlockeryModelFile(FileHeader* fileHeader);

	//vars
	FileHeader m_fileHeader;

	const std::string c_diffuseTexExt = "_Diffuse.png";
	const std::string c_normalTexExt = "_Normal.png";
	const std::string c_specularTexExt = "_SpecGlossEmit.png";

protected:
	//Assignment Operator
	const WarlockeryModelLoader& operator=(const WarlockeryModelLoader& modelLoaderToAssign){
		*this = modelLoaderToAssign;

		return *this;
	}

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///model helpers

std::string GetCommonFilePathForModelName(const std::string& modelName);

void SetDefaultMaterial(Material* material);

void SetCustomModelMaterial(Material* material, const std::string& vertFileName, const std::string& fragFileName, bool useCommonFilePath = true);

void LoadModelTextureToMaterial(Material* material, const std::string& modelName, const std::string& textureName, const std::string& textureBindName = "gTexture", bool useCommonFilePath = true);

//===========================================================================================================

#endif
