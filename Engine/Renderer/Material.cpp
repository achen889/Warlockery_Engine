//==============================================================================================================
//Material.cpp
//by Albert Chen Jul-4-2015.
//==============================================================================================================

#include "Material.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Math/MatrixUtils.hpp"
#include "Engine/Input/InputSystem.hpp"

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///material methods

void Material::InitializeMaterial(const char* vert_file, const char* frag_file){
	InitializeDefaultSampler();

	InitializeShaderProgram(vert_file, frag_file);
}

//-----------------------------------------------------------------------------------------------------------
void Material::InitializeDefaultTexturedMaterial(const char* filePath){
	InitializeMaterial("Data/Shaders/basicSampler.vert", "Data/Shaders/basicSampler.frag");
	std::string fileString = filePath;
	SetTexture(fileString);

}

//-----------------------------------------------------------------------------------------------------------
void Material::InitializeDefaultMaterial(){
	this->InitializeMaterial("Data/Shaders/basic.vert", "Data/Shaders/basic.frag");
	
	m_samplerInUse = false;
}

//-----------------------------------------------------------------------------------------------------------

void Material::InitializeShaderProgram(const char* vert_file, const char* frag_file){
	m_glProgram = GLShader(vert_file, frag_file);
}

//-----------------------------------------------------------------------------------------------------------

void Material::InitializeDefaultSampler(){
	SetDefaultSamplerData();

	m_samplerInUse = true;
	if (theOGLRenderer){
		m_glSampler.m_samplerID = theOGLRenderer->CreateSampler(m_glSampler);
	}

}

//-----------------------------------------------------------------------------------------------------------

void Material::SetTextureInMap(const std::string& texName, Texture* tex){
	m_glSampler.SetTextureInMap(texName, tex);
}

//-----------------------------------------------------------------------------------------------------------

void Material::SetTextureInMap(const std::string& texName, const std::string& filePath){
	m_glSampler.SetTextureInMap(texName, filePath);
}

//-----------------------------------------------------------------------------------------------------------

void Material::SetDefaultSamplerData(){
	m_glSampler.SetGLSamplerData(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);
	m_glSampler.SetDefaultTextures();
}

//-----------------------------------------------------------------------------------------------------------

unsigned int Material::GetProgramID(){
	return m_glProgram.m_programID;
}

//-----------------------------------------------------------------------------------------------------------

//this assumes the texture is already in the shader and does nothing otherwise
void Material::BindTextureIndexToShader(unsigned int texIndex, const std::string& texName, Texture* myTexture ){
	if (myTexture){
		glActiveTexture(GL_TEXTURE0 + texIndex);
		theOGLRenderer->ProgramBindSamplerIndex(m_glProgram.m_programID, texName.c_str(), texIndex);
		glBindTexture(GL_TEXTURE_2D, myTexture->GetPlatformHandle());
		glBindSampler(texIndex, m_glSampler.m_samplerID);
	}
}

void Material::BindTextureWithName(unsigned int texIndex, const std::string& texName){
	BindTextureIndexToShader(texIndex, texName, m_glSampler.GetTextureInMap(texName));
}

void Material::BindMaterialTextures(){
	GLuint texIndex = 0;
	UNUSED(texIndex);
	if (theOGLRenderer){

		m_glSampler.BindTextureMapToShader(m_glProgram.m_programID);

	}


}

void Material::PrepareShaderForRendering(Camera3D& camera, bool isPerspective, ModelViewMatrix* modelView ){
	if (theOGLRenderer){
		GLuint& programToRenderWith = m_glProgram.m_programID;
		theOGLRenderer->SetShaderProgramToUse(programToRenderWith);

		m_glProgram.BindViewMatrix(camera, isPerspective);

		if (modelView){
			m_glProgram.BindModelViewMatrix(*modelView);
		}
		else{
			//bind identity model
			ModelViewMatrix modelMatrix = ModelViewMatrix();
			m_glProgram.BindModelViewMatrix(modelMatrix);
		}
		
		m_glProgram.BindCameraTransform(camera);
		m_glProgram.BindGameTime();
		
	}
	
}

//-----------------------------------------------------------------------------------------------------------


void Material::PrepareShaderForRendering2D(ModelViewMatrix* modelView){
	if (theOGLRenderer){
		GLuint& programToRenderWith = m_glProgram.m_programID;
		theOGLRenderer->SetShaderProgramToUse(programToRenderWith);
		
		m_glProgram.BindViewMatrix2D();

		if (modelView){
			m_glProgram.BindModelViewMatrix(*modelView);
		}
		else{
			//bind identity model
			ModelViewMatrix modelMatrix = ModelViewMatrix();
			m_glProgram.BindModelViewMatrix(modelMatrix);
		}

		//m_glProgram.BindCameraTransform(camera);
		m_glProgram.BindGameTime();

	}

}



