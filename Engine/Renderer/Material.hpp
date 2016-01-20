//==============================================================================================================
//Material.hpp
//by Albert Chen Jul-4-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Material__
#define _included_Material__

#include "Engine/Renderer/ShaderWizard.hpp"

//===========================================================================================================

class Material{
public:
	GLShader m_glProgram;
	GLSampler m_glSampler;
	bool m_samplerInUse = false;

	Material();
	~Material();
	
	void InitializeMaterial(const char* vert_file, const char* frag_file);
	void InitializeShaderFromShaderName(const std::string& shaderName);

	//other initializers
	void InitializeDefaultTexturedMaterial(const char* filePath);
	void InitializeDefaultMaterial();
	void InitializeShaderProgram(const char* vert_file, const char* frag_file);
	void InitializeDefaultSampler();

	void SetTextureInMap(const std::string& texName, const std::string& filePath);
	void SetTextureInMap(const std::string& texName, Texture* tex);

	unsigned int GetProgramID();
	
	void SetDefaultSamplerData();

	void SetTexture(const std::string& filePath);

	void SetDiffuseTexture(const std::string& filePath);

	void SetNormalTexture(const std::string& filePath);

	void SetSpecularTexture(const std::string& filePath);

	void SetEmissiveTexture(const std::string& filePath);
	
	void BindTextureIndexToShader(unsigned int texIndex, const std::string& texName, Texture* myTexture = NULL);

	void BindTextureWithName(unsigned int texIndex, const std::string& texName);

	void BindMaterialTextures();

	void PrepareShaderForRendering(Camera3D& camera, bool isPerspective, ModelViewMatrix* modelView = NULL);
	void PrepareShaderForRendering2D(ModelViewMatrix* modelView = NULL);
};


//===========================================================================================================
//inline methods

inline Material::Material() :
m_samplerInUse(false)
{
	
}

inline Material::~Material(){
	//do nothing

	//in the future if I change the prog and sampler to ptrs
// 	if (m_glProgram){
// 		delete m_glProgram;
// 		m_glProgram = NULL;
// 	}
// 	if (m_glSampler){
// 		delete m_glSampler;
// 		m_glSampler = NULL;
// 	}

}

//-----------------------------------------------------------------------------------------------------------

inline void Material::InitializeShaderFromShaderName(const std::string& shaderName){
	GLShader* shader = GLShader::CreateOrGetShader(shaderName);
	if (shader){
		m_glProgram = *(shader);
	}
}

///----------------------------------------------------------------------------------------------------------
///material texture helpers

inline void Material::SetTexture(const std::string& filePath){
	m_glSampler.m_texture = Texture::CreateOrGetTexture(filePath);
}

inline void Material::SetDiffuseTexture(const std::string& filePath){
	m_glSampler.m_diffuseTex = Texture::CreateOrGetTexture(filePath);
}

inline void Material::SetNormalTexture(const std::string& filePath){
	m_glSampler.m_normalTex = Texture::CreateOrGetTexture(filePath);

}

inline void Material::SetSpecularTexture(const std::string& filePath){
	m_glSampler.m_specularTex = Texture::CreateOrGetTexture(filePath);
}

inline void Material::SetEmissiveTexture(const std::string& filePath){
	m_glSampler.m_emissiveTex = Texture::CreateOrGetTexture(filePath);
}

//-----------------------------------------------------------------------------------------------------------

#endif