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

MaterialMap Material::s_globalMaterials;


//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///material methods

void Material::InitializeMaterial(const char* vert_file, const char* frag_file){
	InitializeDefaultSampler();

	InitializeShaderProgram(vert_file, frag_file);
}

//-----------------------------------------------------------------------------------------------------------

void Material::InitializeMaterial(const char* shaderName ) {
	InitializeDefaultSampler();

	InitializeShaderProgram(shaderName);
}

//-----------------------------------------------------------------------------------------------------------

void Material::InitializeDefaultTexturedMaterial(const char* texturePath){
	InitializeMaterial("Data/Shaders/basicSampler.vert", "Data/Shaders/basicSampler.frag");
	std::string fileString = texturePath;
	m_glSampler.SetTextureInMap("gTexture",fileString);

}

//-----------------------------------------------------------------------------------------------------------
void Material::InitializeDefaultMaterial(){
	this->InitializeMaterial("basic");
	
	m_samplerInUse = false;
}

//-----------------------------------------------------------------------------------------------------------

void Material::InitializeShaderProgram(const char* vert_file, const char* frag_file){
	m_glProgram = GLShader(vert_file, frag_file);
}

//-----------------------------------------------------------------------------------------------------------

void Material::InitializeShaderProgram(const char* shaderName) {
	m_glProgram = *GLShader::CreateOrGetShader(shaderName);
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
	m_glSampler.SetGLSamplerData(GL_LINEAR, GL_NEAREST, GL_REPEAT, GL_REPEAT);
	
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
	//PROFILE_SECTION();

	GLuint texIndex = 0;
	UNUSED(texIndex);
	if (theOGLRenderer){

		m_glSampler.BindTextureMapToShader(m_glProgram.m_programID);

	}


}

//-----------------------------------------------------------------------------------------------------------

void Material::BindUnifiedLights(Lights& lights) {

	int lightCount = lights.size();
	
	//light data array buffers
	float* lightPowers = new float[lightCount];
	Rgba* lightColors = new Rgba[lightCount];

	Vector2* lightIntensities = new Vector2[lightCount];
	Vector3* lightPositions = new Vector3[lightCount];
	Vector3* lightFacings = new Vector3[lightCount];

	float* lightPositionFactors = new float[lightCount];
	float* lightFacingFactors = new float[lightCount];
	float* lightAngles = new float[lightCount];
	float* lightSpotPowers = new float[lightCount];

	int lightIndex = 0;

	for (std::vector<Light*>::iterator itLight = lights.begin(); itLight != lights.end(); ++itLight) {
		Light& light = (**itLight);

		lightPowers[lightIndex] = light.m_lightPower;
		lightColors[lightIndex] = light.m_lightColor;

		lightIntensities[lightIndex] = light.m_lightIntensity;
		lightPositions[lightIndex] = light.m_lightPosition;
		lightFacings[lightIndex] = light.m_lightFacing;

		lightPositionFactors[lightIndex] = light.m_lightPositionFactor;
		lightFacingFactors[lightIndex] = light.m_lightFacingFactor;
		lightAngles[lightIndex] = light.GetLightAngle();
		lightSpotPowers[lightIndex] = light.m_spotPower;

		lightIndex++;
	}

	GLuint meshRendererMatProgramID = (GLuint)this->GetProgramID();

	this->m_glProgram.BindFloat("gLightCount", (float)lightCount);
	//bind the light arrays
	theOGLRenderer->ProgramBindFloatArray(meshRendererMatProgramID, "gLightPowers[0]", lightPowers, lightCount);
	this->m_glProgram.BindColorArray("gLightColors[0]", lightColors, lightCount);

	theOGLRenderer->ProgramBindVec2Array(meshRendererMatProgramID, "gLightIntensities[0]", lightIntensities, lightCount);
	theOGLRenderer->ProgramBindVec3Array(meshRendererMatProgramID, "gLightPositions[0]", lightPositions, lightCount);
	theOGLRenderer->ProgramBindVec3Array(meshRendererMatProgramID, "gLightFacings[0]", lightFacings, lightCount);

	theOGLRenderer->ProgramBindFloatArray(meshRendererMatProgramID, "gLightPositionFactors[0]", lightPositionFactors, lightCount);
	theOGLRenderer->ProgramBindFloatArray(meshRendererMatProgramID, "gLightFacingFactor[0]", lightFacingFactors, lightCount);
	theOGLRenderer->ProgramBindFloatArray(meshRendererMatProgramID, "gLightAngles[0]", lightAngles, lightCount);
	theOGLRenderer->ProgramBindFloatArray(meshRendererMatProgramID, "gSpotPowers[0]", lightSpotPowers, lightCount);

}


//===========================================================================================================

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
		SetShaderToUse();


		
		BindModelViewMatrix(modelView);
		m_glProgram.BindGameTime();
		//m_glProgram.BindCameraTransform(camera);
		//have binds in a named properties?

	}

}

//-----------------------------------------------------------------------------------------------------------

void Material::SetShaderToUse() {
	GLuint& programToRenderWith = m_glProgram.m_programID;
	theOGLRenderer->SetShaderProgramToUse(programToRenderWith);
}


//-----------------------------------------------------------------------------------------------------------

void Material::BindModelViewMatrix(ModelViewMatrix* modelView) {

	m_glProgram.BindViewMatrix2D();

	if (modelView) {
		m_glProgram.BindModelViewMatrix(*modelView);
	}
	else {
		//bind identity model
		ModelViewMatrix modelMatrix = ModelViewMatrix();
		m_glProgram.BindModelViewMatrix(modelMatrix);
	}

}

//-----------------------------------------------------------------------------------------------------------



//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///static methods

Material* Material::CreateOrGetMaterial(const std::string& matName, bool allowNullMat) {

	UNUSED(allowNullMat);

	MaterialMapIterator matIter = s_globalMaterials.find(matName);
	if (matIter != s_globalMaterials.end()) {
		return matIter->second;
	}
	Material* newMat = new Material(matName); //mem leak here

	//if not null add to list
	if (!(newMat->GetName() == "")) {
		s_globalMaterials[matName] = newMat;
	}
	return newMat;

}

///----------------------------------------------------------------------------------------------------------
///friend methods

void ClearGlobalMaterials() {
	MaterialMapIterator it;
	for (it = Material::s_globalMaterials.begin(); it != Material::s_globalMaterials.end(); ) {
		Material* mat = (it->second);
		if (mat) {
			delete mat;
			mat = NULL;
		}
		it = Material::s_globalMaterials.erase(it);
	}
	Material::s_globalMaterials.clear();
}

//===========================================================================================================


