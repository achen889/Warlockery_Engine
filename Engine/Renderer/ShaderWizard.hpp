//==============================================================================================================
//ShaderWizard.hpp
//by Albert Chen Dec-27-2015.
//==============================================================================================================

#pragma once

#ifndef _included_ShaderWizard__
#define _included_ShaderWizard__

#include "Texture.hpp"
#include "Engine\Math\Vector3.hpp"
#include "Engine\Math\Vector4.hpp"

#include "Engine/Core/FileWizard.hpp"
#include "Engine/Multithreading/JobManager.hpp"

class Camera3D;
struct ModelViewMatrix;

//===========================================================================================================

static const unsigned int MAX_TEXTURES_FOR_SAMPLER = 32;

//===========================================================================================================

struct GLSampler{

	unsigned int m_samplerID;

	unsigned int m_minFilter;
	unsigned int m_magFilter;
	unsigned int m_uWrap;
	unsigned int m_vWrap;

	TextureMap m_textureMap;

	//legacy
	Texture* m_texture    = NULL;
	Texture* m_diffuseTex = NULL;
	Texture* m_normalTex  = NULL;
	Texture* m_specularTex= NULL;
	Texture* m_emissiveTex= NULL;

	GLSampler();
	~GLSampler();

	void SetDefaultTextures();

	void SetTextureInMap(const std::string& texName, const std::string& texFilePath);
	void SetTextureInMap(const std::string& texName, Texture* tex);
	Texture* GetTextureInMap(const std::string& texName);

	void BindTextureMapToShader(unsigned int programID);

	void SetGLSamplerData(unsigned int newminFilter, unsigned int newmagFilter, unsigned int newuWrap, unsigned int newvWrap);
	
	//legacy
	void SetTexture(std::string texturePath){
		m_texture = Texture::CreateOrGetTexture(texturePath);
	}
};

//===========================================================================================================

struct GLShader;

typedef std::map<std::string, GLShader*> GLShaderMap;
typedef std::map<std::string, GLShader*>::iterator GLShaderMapIterator;

struct GLShader{
	unsigned int m_programID;
	std::string m_name;

	static GLShaderMap s_globalShaders;

	GLShader();
	~GLShader();

	GLShader(const char* vert_file, const char* frag_file);

	static void ShutDown(){
		//s_globalShaders.clear();
		
		GLShaderMapIterator shaderIt = s_globalShaders.begin();
		while(shaderIt != s_globalShaders.end()){

			if (shaderIt->second){
				delete shaderIt->second;
				shaderIt->second = NULL;


				shaderIt = s_globalShaders.erase(shaderIt);
				continue;
			}

			++shaderIt;
		}

		//ConsolePrintf("shaders are gone!");
	}

	std::string GetName(){ return m_name; }

	//program data, requires theOpenGLRenderer to be initialized
	void BindViewMatrix3D(Camera3D& camera);

	void BindViewMatrix2D();

	void BindViewMatrix(Camera3D& camera, bool isPerspective);

	void BindModelViewMatrix(ModelViewMatrix& modelView);

	void BindCameraTransform(Camera3D& camera);

	void BindGameTime();

	void BindLightData(float lightIntensity, Vector3 lightPosition);

	void BindDefaultFarNearPlanes();

	//universal bindings
	void BindFloat(const   char* bindName, const float& bindFloatValue);
	void BindVector2(const char* bindName, const Vector2& bindVec2Value);
	void BindVector3(const char* bindName, const Vector3& bindVec3Value);
	void BindVector4(const char* bindName, const Vector4& bindVec4Value);
	void BindColor(const std::string& bindName, Rgba& bindColorValue);
	void BindColorArray(const std::string& bindName, Rgba* bindColorArray, int arraySize);

	static GLShader* CreateOrGetShader(const std::string& shaderName, const std::string& vertShaderPath = "", const std::string& fragShaderPath = "");

};


//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///global helpers

void LoadAllShadersFromFiles();

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///job helpers

//will only work if run on a dedicated rendering thread
struct LoadShaderFileJob : Job{

	//vars
	FileDefinition vertFileDefinition;
	FileDefinition fragFileDefinition;

	//methods
	LoadShaderFileJob(const FileDefinition& newVertDef, const FileDefinition& newFragDef ) :
		vertFileDefinition(newVertDef),
		fragFileDefinition(newFragDef)
	{
		
	}

	void Execute(){
		
		//does loading shaders need to be on the same thread as the rendering context?
		std::string vertFullDef = vertFileDefinition.GetDefinition();
		std::string fragFullDef = fragFileDefinition.GetDefinition();

		GLShader::CreateOrGetShader(vertFileDefinition.m_fileName, vertFullDef, fragFullDef );

		ConsolePrintString("Loaded Shader: " + vertFileDefinition.m_fileName + "\n    " + vertFullDef + " & " + fragFullDef + "\n");
	}
protected:
	//Assignment Operator
	const LoadShaderFileJob& operator=(const LoadShaderFileJob& jobToAssign){
		*this = jobToAssign;

		return *this;
	}

};



//===========================================================================================================

#endif //__includedShaderWizard__

