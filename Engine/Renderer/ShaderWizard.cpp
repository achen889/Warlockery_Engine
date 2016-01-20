//==============================================================================================================
//ShaderWizard.cpp
//by Albert Chen Dec-27-2015.
//==============================================================================================================

#include "ShaderWizard.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Core/Time.hpp"

//===========================================================================================================

GLShaderMap GLShader::s_globalShaders;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///GL sampler methods

GLSampler::GLSampler() :
m_samplerID(0)
{

}

GLSampler::~GLSampler(){
	//do something
	m_textureMap.clear();
}

//-----------------------------------------------------------------------------------------------------------

void GLSampler::SetDefaultTextures(){

	SetTextureInMap("gTexDiffuse",	COMMON_TEXTURE_DIFFUSE);
	SetTextureInMap("gTexNormal",	COMMON_TEXTURE_NORMAL);
	SetTextureInMap("gTexSpecular", COMMON_TEXTURE_SPECULAR);
	SetTextureInMap("gTexEmissive", COMMON_TEXTURE_EMISSIVE);

}

//-----------------------------------------------------------------------------------------------------------

void GLSampler::SetTextureInMap(const std::string& texName, const std::string& texFilePath){
	m_textureMap[texName] = Texture::CreateOrGetTexture(texFilePath);
}

//-----------------------------------------------------------------------------------------------------------

void GLSampler::SetTextureInMap(const std::string& texName, Texture* tex){
	if (tex){
		m_textureMap[texName] = tex;
	}
}

//-----------------------------------------------------------------------------------------------------------

Texture* GLSampler::GetTextureInMap(const std::string& texName){
	if (m_textureMap.find(texName) != m_textureMap.end()){
		return m_textureMap[texName];// = Texture::CreateOrGetTexture(texFilePath);
	}
	return NULL;
}

//-----------------------------------------------------------------------------------------------------------

void GLSampler::BindTextureMapToShader(unsigned int programID){
	int texIndex = 0;
	for (TextureMapIterator it = m_textureMap.begin(); it != m_textureMap.end(); ++it){
		Texture* myTexture = (it->second);

		std::string texName = (std::string)it->first;

		if (myTexture){
			glActiveTexture(GL_TEXTURE0 + texIndex);
			theOGLRenderer->ProgramBindSamplerIndex((GLuint)programID, texName.c_str(), texIndex);
			glBindTexture(GL_TEXTURE_2D, myTexture->GetPlatformHandle());
			glBindSampler(texIndex, m_samplerID);
		}

		texIndex++;
	}
}

//-----------------------------------------------------------------------------------------------------------

void GLSampler::SetGLSamplerData(GLenum newminFilter, GLenum newmagFilter, GLenum newuWrap, GLenum newvWrap){
	m_minFilter = newminFilter;
	m_magFilter = newmagFilter;
	m_uWrap = newuWrap;
	m_vWrap = newvWrap;
}

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///GLShader methods

GLShader::GLShader() :
m_programID(0)
{
	//do nothing
}

GLShader::~GLShader(){
	//do nothing
}

GLShader::GLShader(const char* vert_file, const char* frag_file){
	if (theOGLRenderer){
		m_programID = theOGLRenderer->LoadProgram(vert_file, frag_file);
	}

	//name the shader after the vert file
	m_name = vert_file;
	
}

//----------------------------------------------------------------------------------------------------------

void GLShader::BindViewMatrix3D(Camera3D& camera){
	theOGLRenderer->BindViewMatricesToProgram(m_programID, camera.GetCameraViewMatrix(), theOGLRenderer->MakeDefaultPerspectiveProjectionMatrix());
}

//-----------------------------------------------------------------------------------------------------------

void GLShader::BindViewMatrix2D(){
	theOGLRenderer->BindViewMatricesToProgram(m_programID, IDENTITY_MATRIX, theOGLRenderer->MakeDefaultOrthographicProjectionMatrix());
}

//-----------------------------------------------------------------------------------------------------------

void GLShader::BindModelViewMatrix(ModelViewMatrix& modelView){
	//if (theOGLRenderer){
	theOGLRenderer->ProgramBindMatrix(m_programID, "gModel", modelView.GetModelViewTransformMatrix(), true);
	//}
}

//-----------------------------------------------------------------------------------------------------------

void GLShader::BindViewMatrix(Camera3D& camera, bool isPerspective){
	if (isPerspective){
		BindViewMatrix3D(camera);
	}
	else{
		BindViewMatrix2D();
	}
}

//-----------------------------------------------------------------------------------------------------------

void GLShader::BindCameraTransform(Camera3D& camera){
	theOGLRenderer->BindCameraTransformToProgram((GLuint)m_programID, camera);
}

//-----------------------------------------------------------------------------------------------------------

void GLShader::BindGameTime(){
	theOGLRenderer->ProgramBindFloat((GLuint)m_programID, "gTime", (float)GetCurrentSeconds());
}

//-----------------------------------------------------------------------------------------------------------

void GLShader::BindLightData(float lightIntensity, Vector3 lightPosition){
	theOGLRenderer->ProgramBindFloat((GLuint)m_programID, "gLightPower", lightIntensity);
	theOGLRenderer->ProgramBindVec3((GLuint)m_programID, "gLightPosition", lightPosition);
}

//-----------------------------------------------------------------------------------------------------------

void GLShader::BindDefaultFarNearPlanes(){
	theOGLRenderer->ProgramBindFloat((GLuint)m_programID, "gNear", 0.1f);
	theOGLRenderer->ProgramBindFloat((GLuint)m_programID, "gFar", 1000.0f);
}

//-----------------------------------------------------------------------------------------------------------

void GLShader::BindFloat(const char* bindName, const float& bindFloatValue){
	theOGLRenderer->ProgramBindFloat((GLuint)m_programID, bindName, bindFloatValue);
}

//-----------------------------------------------------------------------------------------------------------

void GLShader::BindVector2(const char* bindName, const Vector2& bindVec2Value){
	theOGLRenderer->ProgramBindVec2((GLuint)m_programID, bindName, bindVec2Value);
}

//-----------------------------------------------------------------------------------------------------------

void GLShader::BindVector3(const char* bindName, const Vector3& bindVec3Value){
	theOGLRenderer->ProgramBindVec3((GLuint)m_programID, bindName, bindVec3Value);
}

//-----------------------------------------------------------------------------------------------------------

void GLShader::BindVector4(const char* bindName, const Vector4& bindVec4Value){
	theOGLRenderer->ProgramBindVec4((GLuint)m_programID, bindName, bindVec4Value);
}

//-----------------------------------------------------------------------------------------------------------

void GLShader::BindColor(const std::string& bindName, Rgba& bindColorValue){
	//float red = bindColorValue.fR();
	//float green = bindColorValue.fG();
	Vector4 bindColorNormVec4 = bindColorValue.ToVector4();

	theOGLRenderer->ProgramBindVec4((GLuint)m_programID, bindName.c_str(), bindColorNormVec4);
}

//-----------------------------------------------------------------------------------------------------------

void GLShader::BindColorArray(const std::string& bindName, Rgba* bindColorArray, int arraySize){
	//float red = bindColorValue.fR();
	//float green = bindColorValue.fG();
	Vector4* colorVec4s = new Vector4[arraySize];
	for (int i = 0; i < arraySize; i++){
		colorVec4s[i] = bindColorArray[i].ToVector4();
	}
	//Vector4 bindColorNormVec4 = bindColorValue.ToVector4();

	theOGLRenderer->ProgramBindVec4Array((GLuint)m_programID, bindName.c_str(), colorVec4s, arraySize);
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///helpers

GLShader* GLShader::CreateOrGetShader(const std::string& shaderName, const std::string& vertShaderPath, const std::string& fragShaderPath){

	if (vertShaderPath.find("\\") != std::string::npos){
		std::string errorMessageTitle = "ERROR: Forward Slash Detected in File Path!\n";
		std::string errorMessageTexture = "DEBUG: " + vertShaderPath + " contains a forward slash //\n";
		ERROR_RECOVERABLE(errorMessageTitle + errorMessageTexture);
	}

	GLShaderMapIterator shaderIt = s_globalShaders.find(shaderName);
	if (shaderIt != s_globalShaders.end()){
		return shaderIt->second;
	}


	GLShader* newShader = new GLShader((vertShaderPath).c_str(), (fragShaderPath).c_str());
	//if not null add to list
	if (!(newShader->GetName() == "")){
		s_globalShaders[shaderName] = newShader;
	}


	return newShader;
}


//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///global helpers

void LoadAllShadersFromFiles(){
	Strings vertShaderFilePaths;
	Strings fragShaderFilePaths;

	bool vertShadersLoaded = FindAllFilesOfType("Data/Shaders/", ".vert", vertShaderFilePaths);
	bool fragShadersLoaded = FindAllFilesOfType("Data/Shaders/", ".frag", fragShaderFilePaths);

	if (vertShadersLoaded && fragShadersLoaded){
		//nested loop to find the matching shaders
		for (StringsIterator it = vertShaderFilePaths.begin(); it != vertShaderFilePaths.end(); ++it){
			std::string& vertFilePath = (*it);
			FileDefinition vertFileDef = StringToFileDefinition(vertFilePath);
			for (StringsIterator fragIt = fragShaderFilePaths.begin(); fragIt != fragShaderFilePaths.end(); ++fragIt){
				std::string& fragFilePath = (*fragIt);
				FileDefinition fragFileDef = StringToFileDefinition(fragFilePath);

				//the actual loading part
				if (vertFileDef.m_fileName == fragFileDef.m_fileName){
// 					if (theJobManager){
// 						Job* job = new LoadShaderFileJob(vertFileDef, fragFileDef);
// 						theJobManager->AddPendingJob(job, PRIORITY_HIGH, NULL, JOB_TYPE_FILEIO);
// 					}

					//does loading shaders need to be on the same thread as the rendering context?
					std::string vertFullDef = vertFileDef.GetDefinition();
					std::string fragFullDef = fragFileDef.GetDefinition();

					GLShader::CreateOrGetShader(vertFileDef.m_fileName, vertFullDef, fragFullDef);

					ConsolePrintString("Loaded Shader: " + vertFileDef.m_fileName + "\n    " + vertFullDef + " & " + fragFullDef + "\n");

				}


			}
		}//end of nested for
	}
}

