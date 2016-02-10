//==============================================================================================================
//OpenGLRenderer.cpp
//by Albert Chen Jan-15-2015.
//==============================================================================================================
//declarations here
//==============================================================================================================
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/ParticleSystem/ParticleSystem.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Console/DevConsole.hpp"
#include "Engine/Renderer/Text/TextSystem.hpp"


//#include "Engine\Renderer\OGLRenderingUtils.hpp"

// Globals "function pointer" variables
PFNGLGENBUFFERSPROC		glGenBuffers		= nullptr;
PFNGLBINDBUFFERPROC		glBindBuffer		= nullptr;
PFNGLBUFFERDATAPROC		glBufferData		= nullptr;
PFNGLGENERATEMIPMAPPROC	glGenerateMipmap	= nullptr;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;

PFNGLCREATESHADERPROC     glCreateShader = nullptr;
PFNGLSHADERSOURCEPROC     glShaderSource = nullptr;
PFNGLDELETESHADERPROC     glDeleteShader = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLGETSHADERIVPROC      glGetShaderiv = nullptr;
PFNGLCOMPILESHADERPROC    glCompileShader = nullptr;

PFNGLCREATEPROGRAMPROC    glCreateProgram = nullptr;
PFNGLATTACHSHADERPROC     glAttachShader = nullptr;
PFNGLDETACHSHADERPROC     glDetachShader = nullptr;
PFNGLLINKPROGRAMPROC      glLinkProgram = nullptr;
PFNGLDELETEPROGRAMPROC    glDeleteProgram = nullptr;
PFNGLPROGRAMPARAMETERIPROC glProgramParameteri = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;

PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLGETUNIFORMINDICESPROC glGetUniformIndices = nullptr;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
 
PFNGLUNIFORM1FVPROC glUniform1fv = nullptr;
PFNGLUNIFORM1IVPROC glUniform1iv = nullptr;
PFNGLUNIFORM2FVPROC glUniform2fv = nullptr;
PFNGLUNIFORM3FVPROC glUniform3fv = nullptr;
PFNGLUNIFORM4FVPROC glUniform4fv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;

PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;

PFNGLGENSAMPLERSPROC glGenSamplers = nullptr;
PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri = nullptr;
PFNGLDELETESAMPLERSPROC glDeleteSamplers = nullptr;
PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;
PFNGLBINDSAMPLERPROC glBindSampler = nullptr;

//===========================================================================================================

OpenGLRenderer* theOGLRenderer = NULL;

//===========================================================================================================

// Call some function like this during startup, after creating window & OpenGL rendering context
//from Squirrel Eiserloh, do this ONCE after making the renderer
void OpenGLRenderer::InitializeAdvancedOpenGLFunctions()
{
	//char const *gl_version = (char const*)glGetString(GL_VERSION);

	//Render Buffer Bindings
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	
 	//Shader Bindings
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
    glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");

	//Program Compilation Bindings
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	glProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC)wglGetProcAddress("glProgramParameteri");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	
	//Program Use Bindings
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC)wglGetProcAddress("glGetUniformIndices");
	glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)wglGetProcAddress("glGetActiveUniform");

	//Uniform Bindings
	glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
	glUniform1iv = (PFNGLUNIFORM1IVPROC)wglGetProcAddress("glUniform1iv");
	glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	
	//Attribute Bindings
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");

	//Sampler
	glGenSamplers = (PFNGLGENSAMPLERSPROC)wglGetProcAddress("glGenSamplers");
	glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)wglGetProcAddress("glSamplerParameteri");
	glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)wglGetProcAddress("glDeleteSamplers");
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
	glBindSampler = (PFNGLBINDSAMPLERPROC)wglGetProcAddress("glBindSampler");
}

///----------------------------------------------------------------------------------------------------------
///Checks Open GL version, adapted with code from Chris Forseth
bool OpenGLRenderer::CheckCompatability(){
	// Versions
	std::string gl_version = (char const*)glGetString(GL_VERSION);
	std::string shader_version = (char const*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	
	// Not supported by OpenGL1.* or lower, but modern cards will
// 	GLint major, minor;
// 	glGetIntegerv(GL_MAJOR_VERSION, &major);
// 	glGetIntegerv(GL_MINOR_VERSION, &minor);

	std::string GLVerText = "OpenGL Version: " + gl_version + "\nShader Version: " + shader_version+"\n";
	std::string GLVerTitle = "**Running OpenGL**";

	//ConsoleGenericMessageBox(GLVerText, GLVerTitle);
	ConsolePrintf(GLVerText.c_str());

	// But, if you want to be thorough, check first on bootup, and fail 
	// when it's not supported
	GLboolean compiler_available = GL_FALSE;
	glGetBooleanv(GL_SHADER_COMPILER, &compiler_available);
	assert(compiler_available == GL_TRUE);
	if (compiler_available == GL_FALSE){
		ConsoleGenericMessageBox(GLVerText, GLVerTitle);
	}

	GL_CHECK_ERROR();

	return (compiler_available == GL_TRUE);
}

void OpenGLRenderer::GLCheckError(){
	GLenum error = glGetError();
	//error = GL_INVALID_ENUM; //testing error codes
	if (error != GL_NO_ERROR){
		std::string gl_version = (char const*)glGetString(GL_VERSION);
		std::string shader_version = (char const*)glGetString(GL_SHADING_LANGUAGE_VERSION);
		std::string GLVerText = "OpenGL Version: " + gl_version + "\nShader Version: " + shader_version + "\n";
		//std::string GLVerTitle = "**Running OpenGL**";

		//ConsoleGenericMessageBox(GLVerText, GLVerTitle);
		std::string errorText = GLVerText + "GL Error Code: 0x" + IntToString(error, 16) + "\n";
		//std::string errorTitle = "*DEBUG* OpenGL Error!\n";

		ERROR_RECOVERABLE(errorText);
		//ConsoleGenericMessageBox(errorText, errorTitle);
	}
}

void OpenGLRenderer::GLCheckError(const char* file, int lineNum){
	GLenum error = glGetError();
	//error = GL_INVALID_ENUM; //testing error codes
	if (error != GL_NO_ERROR){
		std::string gl_version = (char const*)glGetString(GL_VERSION);
		std::string shader_version = (char const*)glGetString(GL_SHADING_LANGUAGE_VERSION);
		std::string GLVerText = "OpenGL Version: " + gl_version + "\nShader Version: " + shader_version + "\n";

		std::string errorText = GLVerText + "GL Error in "+(std::string)file + "At Line #"+ IntToString(lineNum) + "\nGL Error Code: 0x" + IntToString(error, 16) + "\n";
		errorText += GLTranslateErrorCode(error);

		//std::string errorTitle = "*DEBUG* OpenGL Error!\n";
		//ConsoleGenericMessageBox(errorText, errorTitle);
		ERROR_RECOVERABLE(errorText);
	}
}

std::string OpenGLRenderer::GLTranslateErrorCode(const GLenum& errorCode){
	std::string errorID = IntToString(errorCode, 16);
	std::string errorText = "ERROR CODE: "+errorID+"\n";
	if (errorID == "501"){
		errorText = "GL_INVALID_VALUE: A value is invalid for this method!";
	}
	if (errorID == "502"){
		errorText = "GL_INVALID_OPERATION: An operation is invalid for this method!";
	}
	
	else if (errorID == "505"){
		errorText = "GL_OUT_OF_MEMORY: Memory cannot be allocated for this method!";
	}

	return errorText;

}

//===========================================================================================================

void OpenGLRenderer::InitializeShaders(){
	//get the shader wizard to do cool stuff
	LoadAllShadersFromFiles();
}

//===========================================================================================================

void OpenGLRenderer::InitializeVAO(VertexArrayObject& myVAO, const char* vertFile, const char* fragFile){
	InitializeVAOWithShader(myVAO, vertFile, fragFile);
	InitializeVAOWithVertexArrayBuffer(myVAO);
}
 
void OpenGLRenderer::InitializeVAOWithVertexArrayBuffer(VertexArrayObject& myVAO){
	CreateVAOBuffer(myVAO);
	CreateVertexArrayWithShader(myVAO.m_Program, myVAO.m_vboID, myVAO.m_vaoID);
}

void OpenGLRenderer::InitializeVAOWithShader(VertexArrayObject& myVAO, const char* vertFile, const char* fragFile){
	myVAO.m_Program = LoadProgram(vertFile, fragFile);
}

void VertexArrayObject::SetSampler(unsigned int minFilter, unsigned int magFilter, unsigned int uWrap, unsigned int vWrap) {
	m_glSampler.SetGLSamplerData(minFilter, magFilter, uWrap, vWrap);
	if (theOGLRenderer) {
		m_samplerID = theOGLRenderer->CreateSampler(m_glSampler);
	}
}

int g_BufferCount = 0;

void OpenGLRenderer::DestroyVAO(VertexArrayObject& myVAO){
	//destroy VAO!
	//glDeleteSamplers(1, &myVAO.m_samplerID);

	glDeleteBuffers(1, &myVAO.m_vboID);
	g_BufferCount--;
	//if (myVAO.m_Program != NULL) {
	//	glDeleteProgram(myVAO.m_Program);
//	}

	if (myVAO.m_vaoID != NULL) {
		glDeleteVertexArrays(1, &myVAO.m_vaoID);
	}
}

/*
void OpenGLRenderer::DestroyMeshVertexArray(MeshRenderer& myMeshRenderer){
	glDeleteBuffers(1, &myMeshRenderer.m_mesh->m_vboID);
	g_BufferCount--;

	if (myMeshRenderer.m_vaoID != NULL) {
		glDeleteVertexArrays(1, &myMeshRenderer.m_vaoID);
	}
}
*/

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///VBO Shader Drawing Helpers

void OpenGLRenderer::CreateVAOVertexArrayWithShader(VertexArrayObject& myVAO){
	CreateVertexArrayWithShader(myVAO.m_Program, myVAO.m_vboID, myVAO.m_vaoID);
}

void OpenGLRenderer::CreateVAOBuffer(VertexArrayObject& myVAO){
	myVAO.m_vboID = CreateBuffer(myVAO.m_vboID, myVAO.m_vertexArray.data(), myVAO.CalcVAOSize());
	
}

GLuint OpenGLRenderer::InitializeVertexBuffer(GLuint& out_vboID){
	//Create buffer
	glGenBuffers(1, &out_vboID);
	//g_BufferCount++;
	GL_CHECK_ERROR();

	return out_vboID;
}

GLuint OpenGLRenderer::InitializeIndexBuffer(GLuint& out_iboID){
	//Create buffer
	glGenBuffers(1, &out_iboID);
	//g_BufferCount++;
	GL_CHECK_ERROR();

	return out_iboID;
}

void OpenGLRenderer::CopyVertexBufferData(GLuint vboID, const void* bufferData, size_t bufferSize){
	//Copy Data to Buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	GL_CHECK_ERROR();

	glBufferData(GL_ARRAY_BUFFER, bufferSize, bufferData, GL_DYNAMIC_DRAW);
	GL_CHECK_ERROR();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLRenderer::CopyIndexBufferData(GLuint iboID, const void* bufferData, size_t bufferSize){
	//Copy Data to Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	GL_CHECK_ERROR();

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, bufferData, GL_DYNAMIC_DRAW);
	GL_CHECK_ERROR();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint OpenGLRenderer::CreateBuffer(GLuint vboID, const void* bufferData, size_t bufferSize){
	//Create buffer
	glGenBuffers(1, &vboID);
	g_BufferCount++;
	GL_CHECK_ERROR();

	//BindBufferData(m_vboID, vertexArray.data(), vertexArray.size() * sizeof(Vertex3D) );

	//Copy Data to Buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	GL_CHECK_ERROR();

	glBufferData(GL_ARRAY_BUFFER, bufferSize, bufferData, GL_DYNAMIC_DRAW);
	GL_CHECK_ERROR(); //too many buffers triggers this to explode, gl 505, out of memory

	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vboID;
}

void OpenGLRenderer::CreateVertexArrayWithShader(GLuint programID, GLuint& bufferID, GLuint& vertexArrayID){
	//GL generate VAs
	glGenVertexArrays(1, &vertexArrayID);
	//bind VA
	glBindVertexArray(vertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	//Bind Position Attribute Vector3
	ProgramBindAttribute(programID, "inPosition", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), offsetof(Vertex3D, m_position));
	//Bind Color Attribute Rgba
	ProgramBindAttribute(programID, "inColor", 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex3D), offsetof(Vertex3D, m_color));
	//Bind TexCoords Attribute Vector2
	ProgramBindAttribute(programID, "inUV", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), offsetof(Vertex3D, m_texCoords));
	//Bind Normal Attribute Vector3
	ProgramBindAttribute(programID, "inNormal", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), offsetof(Vertex3D, m_normal));
	
	glBindVertexArray(0);


}

//-----------------------------------------------------------------------------------------------------------

int OpenGLRenderer::GetBufferCount(){
	return g_BufferCount;
}

//-----------------------------------------------------------------------------------------------------------

bool OpenGLRenderer::ProgramBindAttribute(GLuint program_id,
	char const *name,
	GLint count,
	GLenum type,
	GLboolean normalize,
	GLsizei stride,
	GLsizei offset )
{

	GLint programLocation = glGetAttribLocation(program_id, name);
	GL_CHECK_ERROR();

	if (programLocation < 0){
		return false;
	}

	// test to see if this even matters - that is, having more enabled than are used.
	glEnableVertexAttribArray(programLocation);
	GL_CHECK_ERROR();

	glVertexAttribPointer(programLocation, count, type, normalize, stride, (GLvoid*)offset);
	GL_CHECK_ERROR();

	return true;
}

///----------------------------------------------------------------------------------------------------------
///program loading methods
GLuint OpenGLRenderer::LoadProgram(const char* vert_file, const char* frag_file){
	// Sweet, have the shaders
	GLuint program_id = glCreateProgram();
	if (program_id == NULL) {
		return NULL;
	}
	
	GLuint vert_shader = LoadShader(vert_file, GL_VERTEX_SHADER);
	GL_CHECK_ERROR();

	GLuint frag_shader = LoadShader(frag_file, GL_FRAGMENT_SHADER);
	GL_CHECK_ERROR();

	if (!LinkProgram(program_id, vert_shader, frag_shader)) {
		glDeleteProgram(program_id);
		program_id = NULL;
	}

	// no longer needed no matter what
	FreeShader(vert_shader);
	GL_CHECK_ERROR();

	FreeShader(frag_shader);
	GL_CHECK_ERROR();

	return program_id;

}

//-----------------------------------------------------------------------------------------------------------

bool OpenGLRenderer::LinkProgram(GLuint program_id, GLuint vert_shader, GLuint frag_shader)
{
	if ((vert_shader == NULL) || (frag_shader == NULL) || (program_id == NULL)) {
		return false;
	}

	// At this point, you can still do operations on shader
	// and they'll take - just everything has to be set before
	// you do glLinkProgram or glProgramBinary
	glAttachShader(program_id, vert_shader);
	glAttachShader(program_id, frag_shader);
	GL_CHECK_ERROR();

	glLinkProgram(program_id);
	GL_CHECK_ERROR();

	// Only need to be attached if they're 
	glDetachShader(program_id, vert_shader);
	glDetachShader(program_id, frag_shader);
	GL_CHECK_ERROR();

	if (!CheckProgramStatus(program_id)) {
		ConsoleGenericMessageBox("Failed to link programID: " + program_id, "*Link Program Failed*");
		return false;
	}

	//ConsoleGenericMessageBox("Successfully Linked Shader Program!");
	//ConsolePrintf("Successfully Linked Shader Program!");

	return true;
}

//-----------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------
// A program MUST be bound for these, adapted from code by Chris Forseth
bool OpenGLRenderer::ProgramBindMatrix(GLuint programID, const char* name, const Matrix4& matrix, bool doTranspose){
	GLint location = glGetUniformLocation(programID, name);
	if (location < 0) {
		// didn't exist
		return false;
	}

	// If you want, you can make sure it actually is the type you want
	GLuint index;
	glGetUniformIndices(programID, 1, &name, &index);
	if (index >= 0) {
		GLint count;
		GLenum type;
		glGetActiveUniform(programID, index, 0, NULL, &count, &type, NULL);
		//ASSERT(type == GL_FLOAT_MAT4);
	}

	if (!doTranspose){
		// there is just glUniformMatrix4f, but 
		// this one is more general purpose
		glUniformMatrix4fv(location, 1, GL_FALSE, (GLfloat*)matrix.GetAsFloatArray());

	}
	else{
		glUniformMatrix4fv(location, 1, GL_TRUE, (GLfloat*)matrix.GetAsFloatArray());
	}
	
	return true;
}

bool OpenGLRenderer::ProgramBindFloatArray(GLuint programID, char const *name, float* floatArray, int arraySize ){
	GLint location = glGetUniformLocation(programID, name);
	if (location < 0) {
		// didn't exist
		return false;
	}

	glUniform1fv(location, arraySize, &floatArray[0]);
	return true;
}

bool OpenGLRenderer::ProgramBindFloat(GLuint programID, char const *name, const float& floatToPass ){
	GLint location = glGetUniformLocation(programID, name);
	if (location < 0) {
		// didn't exist
		return false;
	}
	
	glUniform1fv(location, 1, &floatToPass);
	return true;
}

bool OpenGLRenderer::ProgramBindVec2(GLuint programID, char const *name, const Vector2& vecToPass){
	GLint location = glGetUniformLocation(programID, name);
	if (location < 0) {
		// didn't exist
		return false;
	}
	
	glUniform2fv(location, 1, (GLfloat*)vecToPass.GetAsFloatArray());
	return true;
}

bool OpenGLRenderer::ProgramBindVec2Array(GLuint programID, char const *name, Vector2* vecArray, int arraySize){
	GLint location = glGetUniformLocation(programID, name);
	if (location < 0) {
		// didn't exist
		return false;
	}

	glUniform2fv(location, arraySize, (GLfloat*)&vecArray[0]);
	return true;
}

bool OpenGLRenderer::ProgramBindVec3(GLuint programID, char const *name, const Vector3& vecToPass){
	GLint location = glGetUniformLocation(programID, name);
	if (location < 0) {
		// didn't exist
		return false;
	}
	
	glUniform3fv(location, 1, (GLfloat*)vecToPass.GetAsFloatArray());
	return true;
}

bool OpenGLRenderer::ProgramBindVec3Array(GLuint programID, char const *name, Vector3* vecArray, int arraySize ){
	GLint location = glGetUniformLocation(programID, name);
	if (location < 0) {
		// didn't exist
		return false;
	}

	glUniform3fv(location, arraySize, (GLfloat*)&vecArray[0]);
	return true;
}


bool OpenGLRenderer::ProgramBindVec4(GLuint programID, char const *name, const Vector4& vecToPass){
	GLint location = glGetUniformLocation(programID, name);
	if (location < 0) {
		// didn't exist
		return false;
	}

	glUniform4fv(location, 1, (GLfloat*)vecToPass.GetAsFloatArray());
	return true;
}

bool OpenGLRenderer::ProgramBindVec4Array(GLuint programID, char const *name, Vector4* vecArray, int arraySize ){
	GLint location = glGetUniformLocation(programID, name);
	if (location < 0) {
		// didn't exist
		return false;
	}

	glUniform4fv(location, arraySize, (GLfloat*)&vecArray[0]);
	return true;
}

bool OpenGLRenderer::ProgramBindSamplerIndex(GLuint programID, char const *name, GLint textureUnitIndex){
	GLint location = glGetUniformLocation(programID, name);
	if (location < 0) {
		// didn't exist
		return false;
	}

	// there is just glUniformMatrix4f, but 
	// this one is more general purpose
	glUniform1iv(location, 1, (GLint*)&textureUnitIndex);

	return true;
}

//-----------------------------------------------------------------------------------------------------------

///----------------------------------------------------------------------------------------------------------
///checks program status
bool OpenGLRenderer::CheckProgramStatus(GLuint program_id){
	GLint link_status;
	glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);

	if (link_status == GL_FALSE) {
		GLint log_length = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
		if (log_length > 0) {
			char *buffer = (char*)malloc(log_length);
			glGetProgramInfoLog(program_id, sizeof(buffer), &log_length, (GLchar*)buffer);

			ConsoleGenericMessageBox(buffer, "Program Status");
			delete buffer;
		}
		else {
			GL_CHECK_ERROR();
		}

		return false;
	}

	return true;
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///shader loading methods

GLuint OpenGLRenderer::LoadShader(const char* filename, GLenum shader_type){
	char* source;
	size_t sourceLength;

	source = LoadTextFileToNewBuffer(filename);

	//ConsolePrintString(source,"In LoadShader: Loaded \n"+(std::string)filename);
	
	if (source == nullptr) {
		return NULL;
	}
	else{
		sourceLength = GetFileLength(filename);
		//source[sourceLength] = NULL;
	}
	//creates the shader
	GLuint shader_id = glCreateShader(shader_type);
	if (shader_id == NULL) {
		return NULL;
	}
	
	// can take multiple strings - internally these will all be concatenated end-to-end
	GLint len = (GLint)(sourceLength);
	glShaderSource(shader_id, 1, &source, &len);
	free(source);

	glCompileShader(shader_id);
	GL_CHECK_ERROR();

	if (CheckShaderStatus(shader_id, filename)) {
		return shader_id;
	}
	else {
		glDeleteShader(shader_id);
		return NULL;
	}
}

//-----------------------------------------------------------------------------------------------------------

void OpenGLRenderer::FreeShader(GLuint shader_id){
	if (shader_id != NULL) {
		glDeleteShader(shader_id);
	}
}

//-----------------------------------------------------------------------------------------------------------

///----------------------------------------------------------------------------------------------------------
///Check shader status, adapted from code from Chris Forseth
bool OpenGLRenderer::CheckShaderStatus(GLuint shader_id, const char* filename){
	GLint status;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		// failed to compile
		GLint logLength = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0) {
			char* log = (char*)malloc(logLength);

			glGetShaderInfoLog(shader_id, logLength, &logLength, (GLchar*)log);
			//FormatAndPrintLog(filename, log);
			std::string statusTitle = "ERROR - Shader File: " + (std::string)filename;
			std::string statusText = (std::string)log;
			//output log in message box
			ConsoleGenericMessageBox(statusText, statusTitle );
			free(log);
		}
		else {
			// Failed, unknown reason - shouldn't happen
			GL_CHECK_ERROR();
		}
		//ConsoleGenericMessageBox("NO", "Shader Status");
		return false;
	}
	//ConsoleGenericMessageBox("YES", "Shader Status");
	return true;
}

//===========================================================================================================

void OpenGLRenderer::CreateVAOSampler(VertexArrayObject& myVAO, GLenum minFilter, GLenum magFilter, GLenum uWrap, GLenum vWrap){

	//myVAO.m_glSampler.SetGLSamplerData(minFilter, magFilter, uWrap, vWrap);// = CreateSampler(GLuint samplerID, GLenum minFilter, GLenum magFilter, GLenum uWrap, GLenum vWrap)
	//myVAO.m_samplerID = CreateSampler(myVAO.m_samplerID, minFilter, magFilter, uWrap, vWrap);
	myVAO.SetSampler(minFilter, magFilter, uWrap, vWrap);
	myVAO.SetIsUsingSampler(true);
}

//------------------------------------------------------------------------

GLuint OpenGLRenderer::CreateSampler(GLSampler& myGLSampler ){

	glGenSamplers(1, &myGLSampler.m_samplerID);
	if (NULL == myGLSampler.m_samplerID) {
		return NULL;
	}

	glSamplerParameteri((GLenum)myGLSampler.m_samplerID, GL_TEXTURE_MIN_FILTER, (GLenum)myGLSampler.m_minFilter);			// Default: GL_NEAREST_MIPMAP_LINEAR
	glSamplerParameteri((GLenum)myGLSampler.m_samplerID, GL_TEXTURE_MAG_FILTER, (GLenum)myGLSampler.m_magFilter);			// Default: GL_LINEAR
	glSamplerParameteri((GLenum)myGLSampler.m_samplerID, GL_TEXTURE_WRAP_S,	  (GLenum)myGLSampler.m_uWrap);					// Default: GL_REPEAT
	glSamplerParameteri((GLenum)myGLSampler.m_samplerID, GL_TEXTURE_WRAP_T,     (GLenum)myGLSampler.m_vWrap);					// Default: GL_REPEAT

	return myGLSampler.m_samplerID;
}
//------------------------------------------------------------------------
void OpenGLRenderer::DestroySampler(GLuint samplerID){
	glDeleteSamplers(1, &samplerID);
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///modern rendering pipeline
void OpenGLRenderer::SetShaderProgramToUse(GLuint& programUse){
	glUseProgram(programUse);
}

void OpenGLRenderer::DisableShaderProgram(){
	glUseProgram(0);
}
//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///program binding wrapper functions

void OpenGLRenderer::BindViewMatricesToProgram(GLuint& programBinding, const Matrix4& viewMatrix, const Matrix4& perspectiveMatrix){
	ProgramBindMatrix(programBinding, "gView", viewMatrix);
	ProgramBindMatrix(programBinding, "gProj", perspectiveMatrix);
	ProgramBindMatrix(programBinding, "gModel", IDENTITY_MATRIX);
}

void OpenGLRenderer::BindCameraTransformToProgram(GLuint& programBinding, Camera3D& camera){
	ProgramBindVec3(programBinding, "gCamPos", camera.m_position);
	ProgramBindVec3(programBinding, "gCamRot", camera.m_orientation.ToVector3());
}

void OpenGLRenderer::DrawVertexArray(GLenum drawMode, GLint vaoID, size_t vaoSize){
	
	glBindVertexArray(vaoID);

	glDrawArrays(drawMode, 0, vaoSize);

	glBindVertexArray(0);
}

void OpenGLRenderer::DrawVertexElements(GLenum drawMode, GLint iboID, size_t iboSize, size_t indexStartDraw){
	//UNUSED(iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);

	glDrawElements(drawMode, iboSize, GL_UNSIGNED_INT ,(void*)indexStartDraw);

	//glBindBuffer(0);
}

void OpenGLRenderer::RenderWithVBOs(Camera3D& camera){
	UNUSED(camera);
	//SetModelViewDepthTest();

// 	RenderVAO(gVAO, camera);
 	//RenderVAO(gVAOBall, camera);

 	//RenderParticlesWithVAO(gVAOP, camera);
 
//  	RenderVAO(gVAONormal, camera);
// 
// 	RenderVAO(gVAOFlat, camera);
// 	RenderVAO(gVAOFlat2, camera);

 	//RenderVAO(gVAOAxes, camera );
	//RenderVAO(gVAOAxes, camera, false);
}

void OpenGLRenderer::PrepareVAOForRendering(VertexArrayObject& myVAO, Camera3D& camera, bool isPerspective ){
	GLuint& programToRenderWith = myVAO.m_Program;
	SetShaderProgramToUse(programToRenderWith);

	if (isPerspective){
		BindViewMatricesToProgram(programToRenderWith, camera.GetCameraViewMatrix(), MakeDefaultPerspectiveProjectionMatrix());
	}
	else{
		Matrix4 idenMat;
		idenMat.MakeIdentity();
		BindViewMatricesToProgram(programToRenderWith, idenMat, MakeDefaultOrthographicProjectionMatrix() );
	}
	
	BindCameraTransformToProgram(programToRenderWith, camera);
	ProgramBindFloat(programToRenderWith, "gTime", (float)GetCurrentSeconds());
}

///----------------------------------------------------------------------------------------------------------
///render vertex array, pass in camera
void OpenGLRenderer::RenderVAO(VertexArrayObject& myVAO, Camera3D& camera, bool isPerspective){
	PrepareVAOForRendering(myVAO, camera, isPerspective);

	const float lightPower = 10.0f;

	Vector3 pointLightPosition = Vector3(sin((float)GetCurrentSeconds()), sin(2.0f * (float)GetCurrentSeconds()), 0.5f);

	ProgramBindFloat(myVAO.m_Program, "gLightPower", lightPower);
	ProgramBindVec3(myVAO.m_Program, "gLightPosition", pointLightPosition/*camera.m_position*/);
// 	ProgramBindVec3(myVAO.m_Program, "gAmbientLight", 1, &ambient_light);
// 	ProgramBindVec3(myVAO.m_Program, "gLightColor", 1, &light_color);

	if (myVAO.m_samplerIsValid ){
		
		glEnable(GL_TEXTURE_2D);
		GLuint texIndex = 0;
		
		if (myVAO.m_glSampler.m_texture){
			glActiveTexture(GL_TEXTURE0 + texIndex);
			ProgramBindSamplerIndex(myVAO.m_Program, "gTexture", texIndex);
			glBindTexture(GL_TEXTURE_2D, myVAO.m_glSampler.m_texture->GetPlatformHandle());
			glBindSampler(texIndex, myVAO.m_samplerID);
		}
		if (myVAO.m_glSampler.m_diffuseTex){
			texIndex++;
			glActiveTexture(GL_TEXTURE0 + texIndex);
			ProgramBindSamplerIndex(myVAO.m_Program, "gTexDiffuse", texIndex);
			glBindTexture(GL_TEXTURE_2D, myVAO.m_glSampler.m_diffuseTex->GetPlatformHandle());
			glBindSampler(texIndex, myVAO.m_samplerID);
		}
		if (myVAO.m_glSampler.m_normalTex){
			texIndex++;
			glActiveTexture(GL_TEXTURE0 + texIndex);
			ProgramBindSamplerIndex(myVAO.m_Program, "gTexNormal", texIndex);
			glBindTexture(GL_TEXTURE_2D, myVAO.m_glSampler.m_normalTex->GetPlatformHandle());
			glBindSampler(texIndex, myVAO.m_samplerID);
		}

		

		DrawVertexArray(myVAO.m_drawMode, myVAO.m_vaoID, myVAO.m_vertexArray.size() );

		glDisable(GL_TEXTURE_2D);
	}
	else{
		DrawVertexArray(myVAO.m_drawMode, myVAO.m_vaoID, myVAO.m_vertexArray.size() );
	}
	
	DisableShaderProgram();
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void OpenGLRenderer::RenderParticlesWithVAO(VertexArrayObject& myVAO, Camera3D& camera){
// 	myVAO.m_vertexArray.clear();
// 	myVAO.m_vertexArray.reserve(10000);

	if (theParticleSystem){
		theParticleSystem->GenerateParticlesVertexArray(myVAO.m_vertexArray);
	}

	myVAO.SetDrawMode(GL_POINTS);

 	myVAO.m_vboID = CreateBuffer(myVAO.m_vboID, myVAO.m_vertexArray.data(), myVAO.CalcVAOSize());
 	CreateVertexArrayWithShader(myVAO.m_Program, myVAO.m_vboID, myVAO.m_vaoID);
	
	PrepareVAOForRendering(myVAO, camera, true);

	DrawVertexArray(myVAO.m_drawMode, myVAO.m_vaoID, myVAO.m_vertexArray.size());

	DisableShaderProgram();
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	DestroyVAO(myVAO);
}

//===========================================================================================================

CONSOLE_COMMAND(OGLVER){
	UNUSED_COMMAND_ARGS
	const unsigned int GLVERLineSkipValue = 500;
	std::string gl_version = (char const*)glGetString(GL_VERSION);
	std::string shader_version = (char const*)glGetString(GL_SHADING_LANGUAGE_VERSION);

	// Not supported by OpenGL1.* or lower, but modern cards will
	// 	GLint major, minor;
	// 	glGetIntegerv(GL_MAJOR_VERSION, &major);
	// 	glGetIntegerv(GL_MINOR_VERSION, &minor);
	std::string GLVerInfo = "OpenGL Version: " + gl_version + "\nShader Version: " + shader_version + "\n";
	std::string GLVerText = "**Running OpenGL**\n";
	GLVerText += GLVerInfo;

	OUTPUT_STRING_TO_CONSOLE(GLVerInfo, GLVERLineSkipValue);
}

CONSOLE_COMMAND(GET_PERSPECTIVE_MATRIX){
	UNUSED(args);
	const unsigned int matrixLineSkipVal = 300;
	Matrix4 perspectiveMatrix = theOGLRenderer->MakeDefaultPerspectiveProjectionMatrix();
	std::string perspMatrixText = "Perspective Projection Matrix\n"+perspectiveMatrix.ToString();

	OUTPUT_STRING_TO_CONSOLE(perspMatrixText, matrixLineSkipVal);
	
}

// CONSOLE_COMMAND(DRAW_SPHERE){
// 	Vector3 sphereCenter = Vector3::ZERO;
// 	float sphereRadius = 1.0f;
// 	if (COMMAND_HAS_ARGS){
// 		if (NUMBER_OF_ARG_TOKENS >= 4){
// 
// 			sphereCenter.x = ARG_TOKEN_F(0);
// 			sphereCenter.y = ARG_TOKEN_F(1);
// 			sphereCenter.z = ARG_TOKEN_F(2);
// 
// 			sphereRadius = ARG_TOKEN_F(3);
// 			
// 		}
// 	}
// 
// 
// 	g_debugShapes.AddDebugShape(new DebugShape(Sphere3(sphereCenter, sphereRadius), -1.0f, Rgba::MAGENTA, false ) );
// }

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///Set up
OpenGLRenderer::OpenGLRenderer(): m_windowHandle( NULL), m_deviceContext( NULL), m_renderingContext( NULL){
	//do nothing
	REGISTER_CONSOLE_COMMAND(OGLVER, "Displays OpenGL Version.");

	REGISTER_CONSOLE_COMMAND(GET_PERSPECTIVE_MATRIX, "Displays Default Perspective Projection Matrix.");
	
	m_frameCount = 0;

	if (!theOGLRenderer){
		theOGLRenderer = this;
	}

	
}

OpenGLRenderer::~OpenGLRenderer(){
	//do nothing
}

///----------------------------------------------------------------------------------------------------------
///start up
void OpenGLRenderer::StartUp( HWND windowHandle ){
	m_windowHandle = windowHandle;
	m_deviceContext = GetDC( windowHandle );

	ConsolePrintf("\n===OpenGL Renderer Start Up===\n");

	CreateOpenGLRenderingContext();

	InitializeAdvancedOpenGLFunctions();

 	CheckCompatability();

	CreateCommonEngineTextures();

}
///----------------------------------------------------------------------------------------------------------

///----------------------------------------------------------------------------------------------------------
void OpenGLRenderer::ShutDown(){

	Texture::ShutDown(); //just clearing the map doesn't seem to free up the stuff
	GLShader::ShutDown(); //must delete global shaders too

	DestroyOpenGLRenderingContext();

}

///-------------------------------------------------------------------------------------------------------------
///Creates OpenGL Rendering Context
void OpenGLRenderer::CreateOpenGLRenderingContext(){
	//setting pixel formatter to 32 color bit
	PIXELFORMATDESCRIPTOR pixelFormatDescription;
	memset( &pixelFormatDescription, 0, sizeof( PIXELFORMATDESCRIPTOR) );
	pixelFormatDescription.cColorBits = 32;
	pixelFormatDescription.nVersion = 1;
	pixelFormatDescription.nSize = sizeof( PIXELFORMATDESCRIPTOR );
	pixelFormatDescription.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDescription.cStencilBits = 1;

	//pick pixel format
	HWND myWindowHandle = (HWND)( m_windowHandle );
	HDC hdc = GetDC( myWindowHandle );//device context
	int pixelFormat = ChoosePixelFormat(hdc, &pixelFormatDescription );
	BOOL didSetFormat = SetPixelFormat( hdc, pixelFormat, &pixelFormatDescription );
	if(didSetFormat){
		m_renderingContext = wglCreateContext( hdc );
		wglMakeCurrent ( m_deviceContext, m_renderingContext);
		float dimensions[4];
		glGetFloatv(GL_VIEWPORT, dimensions );
		m_displayWidth = dimensions[2];
		m_displayHeight = dimensions[3];
	}
	glLineWidth(1.5f );
	glEnable(GL_LINE_SMOOTH);
	glPointSize(3.0f );

}

///----------------------------------------------------------------------------------------------------------
///Destroys Rendering Context
void OpenGLRenderer::DestroyOpenGLRenderingContext(){
	wglDeleteContext( m_renderingContext );
}

//===========================================================================================================
//Set Methods

///----------------------------------------------------------------------------------------------------------
///set orthographic
void OpenGLRenderer::SetOrthographicView(){
	glLoadIdentity();
	glOrtho(0.0f, m_displayWidth, 0.0f, m_displayHeight, 0.0f, 0.1f );
}

void OpenGLRenderer::SetOrthographicView(float setLeft, float setRight, float setBottom, float setTop ){
	glLoadIdentity();
	glOrtho(setLeft, setRight, setBottom, setTop, 0.0f, 0.1f);
}

///----------------------------------------------------------------------------------------------------------
///sets default perspective view

void OpenGLRenderer::SetPerspectiveView(){
	glLoadIdentity();
	float fieldOfViewDegreesVertical = 45.0f;
	float aspectRatio = (16.0f / 9.0f);
	float nearDepth = 0.1f;
	float farDepth = 100.0f;

	gluPerspective(fieldOfViewDegreesVertical, aspectRatio, nearDepth, farDepth );
}

void OpenGLRenderer::SetPerspectiveView(float fieldOfViewDegreesVertical, float aspectRatio, float nearDepth, float farDepth){
	glLoadIdentity();

	gluPerspective(fieldOfViewDegreesVertical, aspectRatio, nearDepth, farDepth );
}

void OpenGLRenderer::SetModelViewColor(const Rgba& viewColor ){
	float viewColorR = (float) viewColor.r * g_oneOverColorBits;
	float viewColorG = (float) viewColor.g * g_oneOverColorBits;
	float viewColorB = (float) viewColor.b * g_oneOverColorBits;
	float viewColorA = (float) viewColor.a * g_oneOverColorBits;
	glColor4f( viewColorR , viewColorG, viewColorB , viewColorA );
}
void OpenGLRenderer::SetModelViewTranslation(float x, float y, float z){
	glTranslatef(x,y,z);
}
void OpenGLRenderer::SetModelViewScale(float x, float y, float z ){
	glScalef(x, y, z );
}

void OpenGLRenderer::SetTextureViewTransparent(){
	//these two functions make the texture background transparent with gl blend magic :D
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //black magic method
}

void OpenGLRenderer::SetModelViewMatrixMode(){
	glMatrixMode( GL_MODELVIEW );
}
void OpenGLRenderer::SetModelViewRotation(float angleDegrees, float amountAroundX, float amountAroundY, float amountAroundZ ){
	glRotatef(angleDegrees, amountAroundX,amountAroundY,amountAroundZ);
}

void OpenGLRenderer::SetModelPolygonMode(bool isLineMode){

	GLenum polyMode = GL_FILL;
	if (isLineMode)
		polyMode = GL_LINE;

	glPolygonMode(GL_FRONT_AND_BACK, polyMode);

}

void OpenGLRenderer::SwapBuffers(){
	//PROFILE_SECTION();
	m_frameCount++;

	::SwapBuffers( m_deviceContext );
}

//-----------------------------------------------------------------------------------------------------------

void OpenGLRenderer::ClearBuffer(){
	//use default buffer values
	ClearColor(Rgba::BLACK);
	ClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}

//-----------------------------------------------------------------------------------------------------------

void OpenGLRenderer::ClearBuffer(Rgba clearColor, float clearDepth){
	//use default buffer values
	ClearColor(clearColor);
	ClearDepth(clearDepth);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}

//-----------------------------------------------------------------------------------------------------------

///----------------------------------------------------------------------------------------------------------
///stencil buffer stuff

void OpenGLRenderer::SetModelViewStencilTest(){
	glEnable(GL_STENCIL_TEST);
}

//-----------------------------------------------------------------------------------------------------------

void OpenGLRenderer::DisableModelViewStencilTest(){
	glDisable(GL_STENCIL_TEST);
}

//-----------------------------------------------------------------------------------------------------------

//depth test on screws up the stencil buffer, call set model view stencil test first
void OpenGLRenderer::EnableStencilBufferDrawing(){

	// Set the stencil space to 1
	glStencilFunc(GL_ALWAYS, 1, 0xFF); 
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//clear stencil
	glClearStencil(0);
	// Write to stencil buffer
	glStencilMask(0xFF); 
	//Don't write color mask
	SetModelColorMask(0);
	//Don't write to depth buffer
	glDepthMask(GL_FALSE); 



//	glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
// 	glStencilMask(0xFF); //stencil buffer free to write
// 
// 	glClearStencil(0);
// 	
// 	SetModelColorMask(0);
// 	SetModelViewStencilTest();
// 
// 	glStencilFunc(GL_ALWAYS, 1, 1);
// 	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
// 	//might have to disable depth test now

}

//-----------------------------------------------------------------------------------------------------------

void OpenGLRenderer::DisableStencilBufferDrawing(){

	// Pass test if stencil value is 1 //only draw stuff inside the quad
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	
	// Don't write anything to stencil buffer
	glStencilMask(0x00); 
	
	//can write to depth buffer again
	glDepthMask(GL_TRUE); 

 	SetModelColorMask(1);
// 
// 	glStencilMask(0x00); //set stencil to 0
// 
// 	DisableModelViewStencilTest();

	//might have to enable depth test
}

//-----------------------------------------------------------------------------------------------------------

void OpenGLRenderer::ApplyCameraTransform(const Vector3& cameraPosition, EulerAngles cameraEulerAngles ){
	//fix view orientation with x east forward, y north left, z up 
	SetModelViewRotation(-90.0f , 1.0f, 0.0f, 0.0f );
	SetModelViewRotation( 90.0f , 0.0f, 0.0f, 1.0f );

	//do anti rotation relative to camera Euler angles
	SetModelViewRotation(-cameraEulerAngles.rollDegreesAboutX, 1.0f, 0.0f, 0.0f );
	SetModelViewRotation(-cameraEulerAngles.pitchDegreesAboutY, 0.0f, 1.0f, 0.0f );
	SetModelViewRotation(-cameraEulerAngles.yawDegreesAboutZ, 0.0f, 0.0f, 1.0f );

	//do anti translation relative to camera
	SetModelViewTranslation(-cameraPosition.x,-cameraPosition.y,-cameraPosition.z );
}

void OpenGLRenderer::ApplyCameraTransform(const Camera3D& camera ){
	//fix view orientation with x east forward, y north left, z up 
	SetModelViewRotation(-90.0f , 1.0f, 0.0f, 0.0f );
	SetModelViewRotation( 90.0f , 0.0f, 0.0f, 1.0f );

	//do anti rotation relative to camera Euler angles
	SetModelViewRotation(-camera.m_orientation.rollDegreesAboutX, 1.0f, 0.0f, 0.0f );
	SetModelViewRotation(-camera.m_orientation.pitchDegreesAboutY, 0.0f, 1.0f, 0.0f );
	SetModelViewRotation(-camera.m_orientation.yawDegreesAboutZ, 0.0f, 0.0f, 1.0f );

	//do anti translation relative to camera
	SetModelViewTranslation(-camera.m_position.x,-camera.m_position.y,-camera.m_position.z );
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///draw methods
void OpenGLRenderer::DrawTriangle(const Vector2& v0, const Vector2& v1, const Vector2& v2){
	//glColor3f( 1.0f, 0.0f, 0.0f );
	glBegin( GL_TRIANGLES );
	glVertex2f( v0.x, v0.y); 
	glVertex2f( v1.x, v1.y);
	glVertex2f( v2.x, v2.y);
	glEnd();
}

void OpenGLRenderer::DrawTriangle3D(const Vector3& v0, const Vector3& v1, const Vector3& v2 ){
	glBegin( GL_TRIANGLES );
	glVertex3f( v0.x, v0.y, v0.z ); 
	glVertex3f( v1.x, v1.y, v1.z );
	glVertex3f( v2.x, v2.y, v2.z );
	glEnd();
}

void OpenGLRenderer::DrawQuad(const Vector2s& vertices ){
	glBegin( GL_QUADS );
	for(std::vector<Vector2>::const_iterator vertexIter = vertices.begin(); vertexIter != vertices.end(); ++vertexIter){
		const Vector2& vertex = (*vertexIter);
		glVertex2f(vertex.x, vertex.y);
	}
	glEnd();
}

void OpenGLRenderer::DrawQuad3D(const Vector3s& vertices ){
	glBegin( GL_QUADS);
	for(std::vector<Vector3>::const_iterator vertexIter = vertices.begin(); vertexIter != vertices.end(); ++vertexIter){
		const Vector3& vertex = (*vertexIter);
		glVertex3f(vertex.x, vertex.y, vertex.z);
	}
	glEnd();
}

///----------------------------------------------------------------------------------------------------------
///draw points
void OpenGLRenderer::DrawPoints(const Vector2s& vertices ){
	//glColor3f( 1.0f, 1.0f, 1.0f );
	glBegin( GL_POINTS);
	for(std::vector<Vector2>::const_iterator vertexIter = vertices.begin(); vertexIter != vertices.end(); ++vertexIter){
		const Vector2& vertex = (*vertexIter);
		glVertex2f(vertex.x, vertex.y);
	}
	glEnd();
}
void OpenGLRenderer::DrawPoints3D(const Vector3s& vertices ){
	//glColor3f( 1.0f, 1.0f, 1.0f );
	
	glBegin( GL_POINTS);
	for(std::vector<Vector3>::const_iterator vertexIter = vertices.begin(); vertexIter != vertices.end(); ++vertexIter){
		const Vector3& vertex = (*vertexIter);
		glVertex3f(vertex.x, vertex.y, vertex.z);
	}
	glEnd();
}

void OpenGLRenderer::DrawPoint3D(const Vector3& vertexPt){
	 DrawSphere3D(Sphere3(vertexPt, 0.01f), 3, 3, Rgba::MAGENTA);
}

void OpenGLRenderer::DrawLines(const Vector2s& vertices ){
	if(vertices.size() % 2 == 0){

	glColor3f( 1.0f, 1.0f, 1.0f );
	glBegin( GL_LINES);
	for(std::vector<Vector2>::const_iterator vertexIter = vertices.begin(); vertexIter != vertices.end(); ++vertexIter){
		const Vector2& vertex = (*vertexIter);
		glVertex2f(vertex.x, vertex.y);
	}
	glEnd();

	}
}

///----------------------------------------------------------------------------------------------------------
///draw line segment 3d
void OpenGLRenderer::DrawLineSegment3D(const LineSegment3& linesegToDraw, Rgba viewStartColor, Rgba viewEndColor ){
	
	glBegin( GL_LINES);
	SetModelViewColor(viewStartColor);
	glVertex3f(linesegToDraw.startPoint.x, linesegToDraw.startPoint.y, linesegToDraw.startPoint.z );
	SetModelViewColor(viewEndColor);
	glVertex3f(linesegToDraw.endPoint.x, linesegToDraw.endPoint.y, linesegToDraw.endPoint.z );
	glEnd();
}

void OpenGLRenderer::DrawLineSegment3D(const LineSegment3& linesegToDraw, Rgba viewColor ){
	SetModelViewColor(viewColor);
	glBegin( GL_LINES);
	glVertex3f(linesegToDraw.startPoint.x, linesegToDraw.startPoint.y, linesegToDraw.startPoint.z );
	glVertex3f(linesegToDraw.endPoint.x, linesegToDraw.endPoint.y, linesegToDraw.endPoint.z );
	glEnd();
}

//this is still wrong, the arrow branches are at odd angles
void OpenGLRenderer::DrawArrow3D(LineSegment3 linesegToDraw, const Rgba& viewStartColor, const Rgba& viewEndColor ){
	glBegin( GL_LINES);
	SetModelViewColor(viewStartColor);
	glVertex3f(linesegToDraw.startPoint.x, linesegToDraw.startPoint.y, linesegToDraw.startPoint.z );
	SetModelViewColor(viewEndColor);
	glVertex3f(linesegToDraw.endPoint.x, linesegToDraw.endPoint.y, linesegToDraw.endPoint.z );
	glEnd();
	SetModelViewColor(viewStartColor);
	
	//draw arrowhead
	Vector3s arrowheadVertices;
	const float arrowheadLength = 0.7f;
	UNUSED(arrowheadLength);
	const float arrowheadWidth = 0.1f;

	Vector3 arrowTip = linesegToDraw.GetPointAtParametricValue(1.2f);
	
	//triangle from above
	arrowheadVertices.push_back(arrowTip);	
	arrowheadVertices.push_back(Vector3(linesegToDraw.endPoint.x, linesegToDraw.endPoint.y+arrowheadWidth, linesegToDraw.endPoint.z )); //2
	arrowheadVertices.push_back(Vector3(linesegToDraw.endPoint.x, linesegToDraw.endPoint.y-arrowheadWidth , linesegToDraw.endPoint.z )); //1
	//triangle from below
 	arrowheadVertices.push_back(Vector3(linesegToDraw.endPoint.x, linesegToDraw.endPoint.y-arrowheadWidth , linesegToDraw.endPoint.z )); //1
 	arrowheadVertices.push_back(Vector3(linesegToDraw.endPoint.x, linesegToDraw.endPoint.y+arrowheadWidth, linesegToDraw.endPoint.z )); //2
	arrowheadVertices.push_back(arrowTip);
	//perpendicular triangle south
	arrowheadVertices.push_back(arrowTip);
	arrowheadVertices.push_back(Vector3(linesegToDraw.endPoint.x, linesegToDraw.endPoint.y, linesegToDraw.endPoint.z + arrowheadWidth)); //2
	arrowheadVertices.push_back(Vector3(linesegToDraw.endPoint.x, linesegToDraw.endPoint.y, linesegToDraw.endPoint.z - arrowheadWidth)); //1
	//perpendicular triangle north
	arrowheadVertices.push_back(Vector3(linesegToDraw.endPoint.x, linesegToDraw.endPoint.y, linesegToDraw.endPoint.z - arrowheadWidth)); //2
	arrowheadVertices.push_back(Vector3(linesegToDraw.endPoint.x, linesegToDraw.endPoint.y, linesegToDraw.endPoint.z + arrowheadWidth)); //1
	arrowheadVertices.push_back(arrowTip);

	DrawTriangle3D(arrowheadVertices[0], arrowheadVertices[1], arrowheadVertices[2] );
	DrawTriangle3D(arrowheadVertices[3], arrowheadVertices[4], arrowheadVertices[5] );

	DrawTriangle3D(arrowheadVertices[6], arrowheadVertices[7], arrowheadVertices[8]);

	DrawTriangle3D(arrowheadVertices[9], arrowheadVertices[10], arrowheadVertices[11]);
}

///----------------------------------------------------------------------------------------------------------
///draws a polygon
void OpenGLRenderer::DrawPolygon(const Vector2s& vertices ){
	if(vertices.size() < 3){
		return;
	}
	glBegin(GL_LINE_LOOP);

	for(std::vector<Vector2>::const_iterator vertexIter = vertices.begin(); vertexIter != vertices.end(); ++vertexIter){
		const Vector2& vertex = (*vertexIter);
		glVertex2f(vertex.x, vertex.y);
	}
	glEnd();
}

void OpenGLRenderer::DrawPolygon(const Vector2s& vertices , Rgba& viewColor ){
	SetModelViewColor(viewColor );

	DrawPolygon(vertices );
}
void OpenGLRenderer::DrawSolidPolygon(const Vector2s& vertices){

	glBegin(GL_POLYGON);

	for(std::vector<Vector2>::const_iterator vertexIter = vertices.begin(); vertexIter != vertices.end(); ++vertexIter){
		const Vector2& vertex = (*vertexIter);
		glVertex2f(vertex.x, vertex.y);
	}
	glEnd();
}

void OpenGLRenderer::DrawSolidPolygon(const Vector2s& vertices , Rgba& viewColor ){
	SetModelViewColor(viewColor );

	glBegin(GL_POLYGON);

	for(std::vector<Vector2>::const_iterator vertexIter = vertices.begin(); vertexIter != vertices.end(); ++vertexIter){
		const Vector2& vertex = (*vertexIter);
		glVertex2f(vertex.x, vertex.y);
	}
	glEnd();
}

///----------------------------------------------------------------------------------------------------------
///draw a math Disc2
void OpenGLRenderer::DrawDisc2D(Disc2 DiscToDraw ){
	std::vector<Vector2>DiscVertices;
	//calculate disc points
	Vector2 DiscVertex;
	DiscVertex.SetLengthAndHeadingRadians(DiscToDraw.radius , 0.0f);
	for (int i = 1; i < 360; i++){
		DiscVertex.RotateDegrees(1.0f);
		DiscVertices.push_back(DiscVertex );
	}

	DrawPolygon(DiscVertices);
}
void OpenGLRenderer::DrawDisc2D(Disc2 DiscToDraw , const Rgba& viewColor ){
	SetModelViewColor(viewColor );
	DrawDisc2D(DiscToDraw );
}



///----------------------------------------------------------------------------------------------------------
///draw a solid math Disc2
void OpenGLRenderer::DrawSolidDisc2D(Disc2 DiscToDraw ){
	std::vector<Vector2>DiscVertices;
	//calculate disc points
	Vector2 DiscVertex;
	DiscVertex.SetLengthAndHeadingRadians(DiscToDraw.radius , 0.0f);
	for (int i = 1; i < 360; i++){
		DiscVertex.RotateDegrees(1.0f);
		DiscVertices.push_back(DiscVertex );
	}

	DrawSolidPolygon(DiscVertices);
}
void OpenGLRenderer::DrawSolidDisc2D(Disc2 DiscToDraw , Rgba& viewColor ){
	std::vector<Vector2>DiscVertices;
	//calculate disc points
	Vector2 DiscVertex;
	DiscVertex.SetLengthAndHeadingRadians(DiscToDraw.radius , 0.0f);
	for (int i = 1; i < 360; i++){
		DiscVertex.RotateDegrees(1.0f);
		DiscVertices.push_back(DiscVertex );
	}
	DrawSolidPolygon(DiscVertices, viewColor );
}

///----------------------------------------------------------------------------------------------------------
///draw a wire sphere3
void OpenGLRenderer::DrawSphere3D(Sphere3 sphereToDraw, const int& wedges, const int& slices, const Rgba& viewColor ){

std::vector<Vector3>SphereVertices;
// const float inverseTwoPI = 1 / TWO_PI;
// const float inversePI = 1 / PI;

float wedgeStep = TWO_PI / (float)(wedges);
float sliceStep = PI / (float)(slices);

//std::vector<Vector3>SphereVertices;
for (float w = 0.0f; w < TWO_PI; w += wedgeStep){
	float sineOfWedges = sin(w);
	float cosineOfWedges = cos(w);

	float sineOfWedgesNext = sin(w + wedgeStep);
	float cosineOfWedgesNext = cos(w + wedgeStep);
	//float s = 0.0f;
	for (float s = 0.0f; s <= PI + sliceStep; s += sliceStep){
		float sineOfSlices = sin(s);
		float cosineOfSlices = cos(s);

		float sineOfSlicesNext = sin(s + sliceStep);
		float cosineOfSlicesNext = cos(s + sliceStep);

		//0
		float sphereDrawX = sineOfSlices*cosineOfWedges; // sinS * cosW
		float sphereDrawY = cosineOfSlices; // cosS
		float sphereDrawZ = sineOfSlices*sineOfWedges; // sinS * sinW	
		Vector3 nextPointOnSphere = Vector3(
			sphereDrawX * sphereToDraw.radius + sphereToDraw.center.x,
			sphereDrawZ * sphereToDraw.radius + sphereToDraw.center.y,
			sphereDrawY * sphereToDraw.radius + sphereToDraw.center.z
			);
		SphereVertices.push_back(nextPointOnSphere);

		//1
		sphereDrawX = sineOfSlicesNext*cosineOfWedges; // sinS * cosW
		sphereDrawY = cosineOfSlicesNext; // cosS
		sphereDrawZ = sineOfSlicesNext*sineOfWedges; // sinS * sinW
		nextPointOnSphere = Vector3(
			sphereDrawX * sphereToDraw.radius + sphereToDraw.center.x,
			sphereDrawZ * sphereToDraw.radius + sphereToDraw.center.y,
			sphereDrawY * sphereToDraw.radius + sphereToDraw.center.z
			);
		SphereVertices.push_back(nextPointOnSphere);

		//2
		sphereDrawX = sineOfSlicesNext*cosineOfWedgesNext; // sinS * cosW
		sphereDrawY = cosineOfSlicesNext; // cosS
		sphereDrawZ = sineOfSlicesNext*sineOfWedgesNext; // sinS * sinW
		nextPointOnSphere = Vector3(
			sphereDrawX * sphereToDraw.radius + sphereToDraw.center.x,
			sphereDrawZ * sphereToDraw.radius + sphereToDraw.center.y,
			sphereDrawY * sphereToDraw.radius + sphereToDraw.center.z
			);
		SphereVertices.push_back(nextPointOnSphere);

		//3
		sphereDrawX = sineOfSlices*cosineOfWedgesNext; // sinS * cosW
		sphereDrawY = cosineOfSlices; // cosS
		sphereDrawZ = sineOfSlices*sineOfWedgesNext; // sinS * sinW
		nextPointOnSphere = Vector3(
			sphereDrawX * sphereToDraw.radius + sphereToDraw.center.x,
			sphereDrawZ * sphereToDraw.radius + sphereToDraw.center.y,
			sphereDrawY * sphereToDraw.radius + sphereToDraw.center.z
			);
		SphereVertices.push_back(nextPointOnSphere);
	}
}
//SetModelViewTranslation(sphereToDraw.center.x,sphereToDraw.center.y, sphereToDraw.center.y );
SetModelViewColor(viewColor);
glBegin(GL_LINE_STRIP);
	
	for(std::vector<Vector3>::const_iterator vertexIter = SphereVertices.begin(); vertexIter != SphereVertices.end(); ++vertexIter){
		const Vector3& vertex = (*vertexIter);
		glVertex3f(vertex.x, vertex.y, vertex.z);
	}
glEnd();

}

void OpenGLRenderer::DrawAABB2(AABB2 BoxToDraw ){
	Vector2s boxVertices;

	boxVertices.push_back(BoxToDraw.mins );
	boxVertices.push_back(Vector2( BoxToDraw.maxs.x , BoxToDraw.mins.y) );
	boxVertices.push_back(BoxToDraw.maxs );
	boxVertices.push_back(Vector2( BoxToDraw.mins.x , BoxToDraw.maxs.y) );
	
	DrawPolygon(boxVertices );
}

void OpenGLRenderer::DrawSolidAABB2(AABB2 BoxToDraw ){
	Vector2s boxVertices;

	boxVertices.push_back(BoxToDraw.mins );
	boxVertices.push_back(Vector2( BoxToDraw.maxs.x , BoxToDraw.mins.y) );
	boxVertices.push_back(BoxToDraw.maxs );
	boxVertices.push_back(Vector2( BoxToDraw.mins.x , BoxToDraw.maxs.y) );

	DrawSolidPolygon(boxVertices );
}

void OpenGLRenderer::DrawAABB3(AABB3 BoxToDraw ){
	//SetModelViewColor(Rgba::BLUE);
	Vector3s boxVertices;
	//bottom verts
	boxVertices.push_back(BoxToDraw.mins );
	boxVertices.push_back(Vector3( BoxToDraw.maxs.x , BoxToDraw.mins.y, BoxToDraw.mins.z ) );
	boxVertices.push_back(Vector3( BoxToDraw.maxs.x , BoxToDraw.maxs.y, BoxToDraw.mins.z ) );
	boxVertices.push_back(Vector3( BoxToDraw.mins.x , BoxToDraw.maxs.y, BoxToDraw.mins.z ) );
	//top verts
	boxVertices.push_back(Vector3( BoxToDraw.mins.x , BoxToDraw.mins.y, BoxToDraw.maxs.z ) );
	boxVertices.push_back(Vector3( BoxToDraw.maxs.x , BoxToDraw.mins.y, BoxToDraw.maxs.z ) );
	boxVertices.push_back(BoxToDraw.maxs );
	boxVertices.push_back(Vector3( BoxToDraw.mins.x , BoxToDraw.maxs.y, BoxToDraw.maxs.z ) );
	//front verts
	boxVertices.push_back(BoxToDraw.mins ); //1
	boxVertices.push_back(Vector3( BoxToDraw.mins.x , BoxToDraw.mins.y, BoxToDraw.maxs.z ) ); //4	
	boxVertices.push_back(Vector3( BoxToDraw.mins.x , BoxToDraw.maxs.y, BoxToDraw.maxs.z ) ); //3
	boxVertices.push_back(Vector3( BoxToDraw.mins.x , BoxToDraw.maxs.y, BoxToDraw.mins.z ) ); //2
	//back verts
	boxVertices.push_back(Vector3( BoxToDraw.maxs.x , BoxToDraw.mins.y, BoxToDraw.mins.z ) ); //4	
	boxVertices.push_back(Vector3( BoxToDraw.maxs.x , BoxToDraw.maxs.y, BoxToDraw.mins.z ) ); //2
	boxVertices.push_back(BoxToDraw.maxs ); //1
	boxVertices.push_back(Vector3( BoxToDraw.maxs.x , BoxToDraw.mins.y, BoxToDraw.maxs.z ) ); //3
	glBegin( GL_QUADS);
	for(std::vector<Vector3>::const_iterator vertexIter = boxVertices.begin(); vertexIter != boxVertices.end(); ++vertexIter){
		const Vector3& vertex = (*vertexIter);
		glVertex3f(vertex.x, vertex.y, vertex.z);
	}
	glEnd();

	boxVertices.clear();
	//SetModelViewColor(GetRandomColor());
	//bottom verts
	boxVertices.push_back(BoxToDraw.mins );
	boxVertices.push_back(Vector3( BoxToDraw.maxs.x , BoxToDraw.mins.y, BoxToDraw.mins.z ) );
	boxVertices.push_back(Vector3( BoxToDraw.maxs.x , BoxToDraw.maxs.y, BoxToDraw.mins.z ) );
	boxVertices.push_back(Vector3( BoxToDraw.mins.x , BoxToDraw.maxs.y, BoxToDraw.mins.z ) );
	//top verts
	boxVertices.push_back(Vector3( BoxToDraw.mins.x , BoxToDraw.mins.y, BoxToDraw.maxs.z ) );
	boxVertices.push_back(Vector3( BoxToDraw.maxs.x , BoxToDraw.mins.y, BoxToDraw.maxs.z ) );
	boxVertices.push_back(BoxToDraw.maxs );
	boxVertices.push_back(Vector3( BoxToDraw.mins.x , BoxToDraw.maxs.y, BoxToDraw.maxs.z ) );
	//front verts
	boxVertices.push_back(BoxToDraw.mins ); //1
	boxVertices.push_back(Vector3( BoxToDraw.mins.x , BoxToDraw.maxs.y, BoxToDraw.mins.z ) ); //2
	boxVertices.push_back(Vector3( BoxToDraw.mins.x , BoxToDraw.maxs.y, BoxToDraw.maxs.z ) ); //3
	boxVertices.push_back(Vector3( BoxToDraw.mins.x , BoxToDraw.mins.y, BoxToDraw.maxs.z ) ); //4

	boxVertices.push_back(BoxToDraw.mins ); //1
	boxVertices.push_back(Vector3( BoxToDraw.mins.x , BoxToDraw.mins.y, BoxToDraw.maxs.z ) ); //4	
	boxVertices.push_back(Vector3( BoxToDraw.mins.x , BoxToDraw.maxs.y, BoxToDraw.maxs.z ) ); //3
	boxVertices.push_back(Vector3( BoxToDraw.mins.x , BoxToDraw.maxs.y, BoxToDraw.mins.z ) ); //2
	//back verts
	boxVertices.push_back(Vector3( BoxToDraw.maxs.x , BoxToDraw.mins.y, BoxToDraw.mins.z ) ); //4	
	boxVertices.push_back(Vector3( BoxToDraw.maxs.x , BoxToDraw.maxs.y, BoxToDraw.mins.z ) ); //2
	boxVertices.push_back(Vector3( BoxToDraw.maxs.x , BoxToDraw.mins.y, BoxToDraw.maxs.z ) ); //3
	boxVertices.push_back(BoxToDraw.maxs ); //1
	boxVertices.push_back(Vector3( BoxToDraw.maxs.x , BoxToDraw.maxs.y, BoxToDraw.mins.z ) ); //2
	boxVertices.push_back(BoxToDraw.maxs ); //1
	boxVertices.push_back(Vector3( BoxToDraw.maxs.x , BoxToDraw.mins.y, BoxToDraw.maxs.z ) ); //3
	boxVertices.push_back(Vector3( BoxToDraw.maxs.x , BoxToDraw.mins.y, BoxToDraw.mins.z ) ); //4	
	glBegin( GL_LINES);
	for(std::vector<Vector3>::const_iterator vertexIter = boxVertices.begin(); vertexIter != boxVertices.end(); ++vertexIter){
		const Vector3& vertex = (*vertexIter);
		glVertex3f(vertex.x, vertex.y, vertex.z);
	}
	glEnd();

	//DrawQuad3D(boxVertices);
}

void OpenGLRenderer::DrawAABB3(AABB3 BoxToDraw, Rgba viewColor ){
	SetModelViewColor(viewColor);
	DrawAABB3(BoxToDraw);
}

void OpenGLRenderer::DrawCoordinateAxes(float length ){
	glDisable(GL_TEXTURE_2D);

	Vector3s vertices;
	vertices.push_back(Vector3( 0.0f , 0.0f , 0.0f ) );
	vertices.push_back(Vector3( length , 0.0f ,0.0f ) );
	vertices.push_back(Vector3( 0.0f , 0.0f ,0.0f ) );
	vertices.push_back(Vector3( 0.0f , length ,0.0f ) );
	vertices.push_back(Vector3( 0.0f , 0.0f ,0.0f ) );
	vertices.push_back(Vector3( 0.0f , 0.0f , length ) );

	glBegin( GL_LINES);
	SetModelViewColor(Rgba::RED); //x axis
	glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z );
	glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z );
	SetModelViewColor(Rgba::GREEN); //y axis
	glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z );
	glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z );
	SetModelViewColor(Rgba::BLUE); //z axis
	glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z );
	glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z );
	glEnd();

	
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///draw texture with specified texture coordinates
void OpenGLRenderer::DrawTextureQuad( const Texture& texture, const Vector2s& vertices, const Vector2s& textureCoordinates, bool isOpaque ){
	if(vertices.size() != 4)
		return;

	if(vertices.size() != textureCoordinates.size() )
		return;

	glEnable( GL_TEXTURE_2D );
// 	if(&texture == nullptr){
// 		std::string errorMessageTitle = "ERROR: texture is nullptr!";
// 		std::string errorMessageTexture = "DEBUG: "+texture.GetName() +" is missing a TexturePath!";
// 		ConsoleBeep();
// 		ConsoleGenericMessageBox( errorMessageTexture.c_str(),  errorMessageTitle.c_str() );
// 	}

	glBindTexture( GL_TEXTURE_2D, texture.GetPlatformHandle() );

	if(!isOpaque){
	   SetTextureViewTransparent();
	}
	
	glBegin( GL_QUADS);
		Vector2s::const_iterator vertexIter = vertices.begin();
		Vector2s::const_iterator textureCoordIter = textureCoordinates.begin();
		for(; vertexIter != vertices.end(); ++vertexIter, ++textureCoordIter ){
			const Vector2 vertex = (*vertexIter );
			const Vector2 textureCoordinate = (*textureCoordIter );
			
			glTexCoord2d( textureCoordinate.x, textureCoordinate.y );
			glVertex2f( vertex.x, vertex.y );
		}
	glEnd();
	glDisable( GL_TEXTURE_2D );
}
//this uses a basic texture square;
void OpenGLRenderer::DrawTextureQuad( const Texture& texture, const Vector2s& vertices, bool isOpaque ){
	//create basic set of texture coordinates
	Vector2s basicTextureCoordinates;

	basicTextureCoordinates.push_back(Vector2(0.0f, 1.0f) );
	basicTextureCoordinates.push_back(Vector2(1.0f, 1.0f) );
	basicTextureCoordinates.push_back(Vector2(1.0f, 0.0f) );
	basicTextureCoordinates.push_back(Vector2(0.0f, 0.0f) );
	
	DrawTextureQuad(texture, vertices, basicTextureCoordinates, isOpaque );
}

void OpenGLRenderer::DrawTextureQuadColor( const Texture& texture, const Vector2s& vertices, Rgba& viewColor ){
	SetModelViewColor(viewColor );

	DrawTextureQuad(texture, vertices );
}

//draws 3d textures
void OpenGLRenderer::DrawTextureQuad3D( const Texture& texture, const Vector3s& vertices, const Vector2s& textureCoordinates){
	if(vertices.size() != 4)
		return;

	if(vertices.size() != textureCoordinates.size() )
		return;

	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, texture.GetPlatformHandle() );

	SetTextureViewTransparent();

	glBegin( GL_QUADS);
	Vector3s::const_iterator vertexIter = vertices.begin();
	Vector2s::const_iterator textureCoordIter = textureCoordinates.begin();
	for(; vertexIter != vertices.end(); ++vertexIter, ++textureCoordIter ){
		const Vector3 vertex = (*vertexIter );
		const Vector2 textureCoordinate = (*textureCoordIter );

		glTexCoord2d( textureCoordinate.x, textureCoordinate.y );
		glVertex3f( vertex.x, vertex.y, vertex.z );
	}
	glEnd();
	glDisable( GL_TEXTURE_2D );
}

void OpenGLRenderer::DrawTextureQuad3D( const Texture& texture, const Vector3s& vertices ){
	//create basic set of texture coordinates
	Vector2s basicTextureCoordinates;
	basicTextureCoordinates.push_back(Vector2(0.0f, 1.0f) );
	basicTextureCoordinates.push_back(Vector2(1.0f, 1.0f) );
	basicTextureCoordinates.push_back(Vector2(1.0f, 0.0f) );
	basicTextureCoordinates.push_back(Vector2(0.0f, 0.0f) );

	DrawTextureQuad3D(texture, vertices, basicTextureCoordinates);
}

void OpenGLRenderer::DrawTextureQuad3DColor( const Texture& texture, const Vector3s& vertices, const Vector2s& textureCoordinates, Rgba& viewColor ){
	SetModelViewColor(viewColor );

	DrawTextureQuad3D(texture, vertices, textureCoordinates );
}

///----------------------------------------------------------------------------------------------------------
///push/ pop matrix
void OpenGLRenderer::PushMatrix(){
	glPushMatrix();
}
void OpenGLRenderer::PopMatrix(){
	glPopMatrix();
}

//===========================================================================================================

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///VBO && VAO Methods

void OpenGLRenderer::GenerateVertexArrayAndVBO(){
	//create a vertex array
	Vertex3Ds vertexArray;
	PopulateVertexArray(vertexArray);

	//Create buffer
	glGenBuffers(1, &m_vboID);
	GL_CHECK_ERROR();

	//BindBufferData(m_vboID, vertexArray.data(), vertexArray.size() * sizeof(Vertex3D) );
	//Copy Data to Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	GL_CHECK_ERROR();

	m_numOFVBOVertexes = sizeof(Vertex3D) * vertexArray.size();
	glBufferData(GL_ARRAY_BUFFER, m_numOFVBOVertexes, vertexArray.data(), GL_DYNAMIC_DRAW);
	GL_CHECK_ERROR();

	//m_isVboDirty = false;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///Vertex Arrays for Basic Drawing

void OpenGLRenderer::GenerateVertexArrayUnitCube(Vertex3Ds& out_vertexArray){
	out_vertexArray.clear();
	out_vertexArray.reserve(10000);
	//push stuff back to the out vertex
	//Z+ Up X+ Fwd Y+ Rt
	//AABB3 boxCoords(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f));
	//front
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 1.0f), Rgba::RED, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), Rgba::GREEN, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 1.0f, 0.0f), Rgba::BLUE, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 1.0f), Rgba::GOLD, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 1.0f, 1.0f), Rgba::MAGENTA, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 1.0f, 0.0f), Rgba::PURPLE, Vector2::ZERO));
	//back
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 0.0f, 1.0f), Rgba::RED, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 0.0f, 0.0f), Rgba::GREEN, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 1.0f, 0.0f), Rgba::BLUE, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 0.0f, 1.0f), Rgba::GOLD, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 1.0f, 1.0f), Rgba::MAGENTA, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 1.0f, 0.0f), Rgba::PURPLE, Vector2::ZERO));
	//right
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 1.0f, 0.0f), Rgba::RED, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 1.0f, 1.0f), Rgba::GREEN, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 1.0f, 1.0f), Rgba::BLUE, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 1.0f, 1.0f), Rgba::GOLD, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 1.0f, 0.0f), Rgba::MAGENTA, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 1.0f, 0.0f), Rgba::PURPLE, Vector2::ZERO));
	//left
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), Rgba::RED, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 1.0f), Rgba::GREEN, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 0.0f, 1.0f), Rgba::BLUE, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 0.0f, 1.0f), Rgba::GOLD, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 0.0f, 0.0f), Rgba::MAGENTA, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), Rgba::PURPLE, Vector2::ZERO));
	//top
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 1.0f), Rgba::RED, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 1.0f, 1.0f), Rgba::GREEN, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 1.0f, 1.0f), Rgba::BLUE, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 1.0f, 1.0f), Rgba::GOLD, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 0.0f, 1.0f), Rgba::MAGENTA, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 1.0f), Rgba::PURPLE, Vector2::ZERO));
	//bottom
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), Rgba::RED, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 1.0f, 0.0f), Rgba::GREEN, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 1.0f, 0.0f), Rgba::BLUE, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 1.0f, 0.0f), Rgba::GOLD, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(1.0f, 0.0f, 0.0f), Rgba::MAGENTA, Vector2::ZERO));
	out_vertexArray.push_back(Vertex3D(Vector3(0.0f, 0.0f, 0.0f), Rgba::PURPLE, Vector2::ZERO));
	
}

void OpenGLRenderer::GenerateVertexArrayAABB3(Vertex3Ds& out_vertexArray, AABB3 boxCoords, const Rgba& viewColor ){
	//this one uses draw triangles
	//out_vertexArray.clear();
	out_vertexArray.reserve(24);

	Vector2s samplerDefaultTextureCoords = GetTextureCoordinatesFromAABB2(AABB2::ZERO_TO_ONE);
	
	//push stuff back to the out vertex
	//Z+ Up X+ Fwd Y+ Rt
	//AABB3 draw trianglesBox
	//AABB3 boxCoords(Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f));
	//front
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, boxCoords.maxs.z), viewColor, samplerDefaultTextureCoords[0]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, boxCoords.mins.z), viewColor, samplerDefaultTextureCoords[1]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.maxs.y, boxCoords.mins.z), viewColor, samplerDefaultTextureCoords[2]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, boxCoords.maxs.z), viewColor, samplerDefaultTextureCoords[3]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.maxs.y, boxCoords.maxs.z), viewColor, samplerDefaultTextureCoords[0]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.maxs.y, boxCoords.mins.z), viewColor, samplerDefaultTextureCoords[1]));
	//back
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.mins.y, boxCoords.maxs.z), viewColor, samplerDefaultTextureCoords[2]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.mins.y, boxCoords.mins.z), viewColor, samplerDefaultTextureCoords[3]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, boxCoords.mins.z), viewColor, samplerDefaultTextureCoords[0]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.mins.y, boxCoords.maxs.z), viewColor, samplerDefaultTextureCoords[1]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, boxCoords.maxs.z), viewColor, samplerDefaultTextureCoords[2]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, boxCoords.mins.z), viewColor, samplerDefaultTextureCoords[3]));
	//right
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.maxs.y, boxCoords.mins.z), viewColor, samplerDefaultTextureCoords[0]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.maxs.y, boxCoords.maxs.z), viewColor, samplerDefaultTextureCoords[1]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, boxCoords.maxs.z), viewColor, samplerDefaultTextureCoords[2]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, boxCoords.maxs.z), viewColor, samplerDefaultTextureCoords[3]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, boxCoords.mins.z), viewColor, samplerDefaultTextureCoords[0]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.maxs.y, boxCoords.mins.z), viewColor, samplerDefaultTextureCoords[1]));
	//left
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, boxCoords.mins.z), viewColor, samplerDefaultTextureCoords[2]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, boxCoords.maxs.z), viewColor, samplerDefaultTextureCoords[3]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.mins.y, boxCoords.maxs.z), viewColor, samplerDefaultTextureCoords[0]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.mins.y, boxCoords.maxs.z), viewColor, samplerDefaultTextureCoords[1]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.mins.y, boxCoords.mins.z), viewColor, samplerDefaultTextureCoords[2]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, boxCoords.mins.z), viewColor, samplerDefaultTextureCoords[3]));
	//top
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, boxCoords.maxs.z), viewColor, samplerDefaultTextureCoords[0] ));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.maxs.y, boxCoords.maxs.z), viewColor, samplerDefaultTextureCoords[1] ));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, boxCoords.maxs.z), viewColor, samplerDefaultTextureCoords[2] ));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, boxCoords.maxs.z), viewColor, samplerDefaultTextureCoords[3] ));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.mins.y, boxCoords.maxs.z), viewColor, samplerDefaultTextureCoords[0] ));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, boxCoords.maxs.z), viewColor, samplerDefaultTextureCoords[1] ));
	//bottom
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, boxCoords.mins.z), viewColor, samplerDefaultTextureCoords[2]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.maxs.y, boxCoords.mins.z), viewColor, samplerDefaultTextureCoords[3]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, boxCoords.mins.z), viewColor, samplerDefaultTextureCoords[0]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, boxCoords.mins.z), viewColor, samplerDefaultTextureCoords[1]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.mins.y, boxCoords.mins.z), viewColor, samplerDefaultTextureCoords[2]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, boxCoords.mins.z), viewColor, samplerDefaultTextureCoords[3]));

}

void OpenGLRenderer::GenerateVertexArrayAABB2(Vertex3Ds& out_vertexArray, AABB2 boxCoords, const Rgba& boxColor ){
	out_vertexArray.clear();
	out_vertexArray.reserve(4);

	Vector2s samplerDefaultTextureCoords = GetTextureCoordinatesFromAABB2(AABB2::ZERO_TO_ONE);
	
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, 0.0f), boxColor, samplerDefaultTextureCoords[0]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.maxs.y, 0.0f), boxColor, samplerDefaultTextureCoords[1]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, 0.0f), boxColor, samplerDefaultTextureCoords[2]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.mins.y, 0.0f), boxColor, samplerDefaultTextureCoords[3]));
	
}

void OpenGLRenderer::GenerateVertexArrayTextureQuad(Vertex3Ds& out_vertexArray, AABB2 boxCoords, AABB2 textureCoords, const Rgba& vertexColor){
	//out_vertexArray.clear();
	out_vertexArray.reserve(4);

	Vector2s samplerDefaultTextureCoords = GetTextureCoordinatesFromAABB2(textureCoords);

	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, 0.0f), vertexColor, samplerDefaultTextureCoords[0]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.maxs.y, 0.0f), vertexColor, samplerDefaultTextureCoords[3]));
	
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.maxs.y, 0.0f), vertexColor, samplerDefaultTextureCoords[2]));
	out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.maxs.x, boxCoords.mins.y, 0.0f), vertexColor, samplerDefaultTextureCoords[1]));
	
	//out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, 0.0f), vertexColor, samplerDefaultTextureCoords[0]));
	//out_vertexArray.push_back(Vertex3D(Vector3(boxCoords.mins.x, boxCoords.mins.y, 0), Rgba::WHITE, samplerDefaultTextureCoords[0]));
}

void OpenGLRenderer::PopulateVertexArray(Vertex3Ds& out_vertexArray){
	out_vertexArray.clear();
	out_vertexArray.reserve(10000);
	//push stuff back to the out vertex
	//Z+ Up X+ Fwd Y+ Rt

	GenerateVertexArrayUnitCube(out_vertexArray);
	//GenerateVertexArrayCoordinateAxes(out_vertexArray);

	//GenerateSphere3DVertexArray(out_vertexArray, Sphere3(Vector3::ZERO, 1.0f));

}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///old vbo function
void OpenGLRenderer::DrawVBO(){
	//glDisable(GL_CULL_FACE);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex3D), (const GLvoid*)offsetof(Vertex3D, m_position));
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex3D), (const GLvoid*)offsetof(Vertex3D, m_color));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex3D), (const GLvoid*)offsetof(Vertex3D, m_texCoords));

	glDrawArrays(GL_TRIANGLES, 0, m_numOFVBOVertexes);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///matrix methods

///----------------------------------------------------------------------------------------------------------
///creates the default perspective matrix
Matrix4 OpenGLRenderer::MakeDefaultPerspectiveProjectionMatrix(){

	float topOfScreen = NEAR_DEPTH * tan(FOV_VERTICAL * 0.5f * RadiansPerDegree);
	float bottomOfScreen = -topOfScreen;
	float rightOfScreen = topOfScreen * ASPECT_RATIO;
	float leftOfScreen = -rightOfScreen;

	const float ZoomFactor = 2.0f;
	float ZoomNear = ZoomFactor * NEAR_DEPTH; //zoom 0.5 further, 2.0f closer

	float rightMinusleft = rightOfScreen - leftOfScreen;
	float topMinusBottom = topOfScreen - bottomOfScreen;
	float farMinusNear = FAR_DEPTH - NEAR_DEPTH;

	float rightLeftProjectionCalculation = (rightOfScreen + leftOfScreen) / rightMinusleft;
	float topBottomProjectionCalculation = (topOfScreen + bottomOfScreen) / topMinusBottom;
	float farNearProjectionCalculation = -((FAR_DEPTH + NEAR_DEPTH) / farMinusNear);
	Matrix4 matrixToModify;
	//this one works for Z fwd Y up
	matrixToModify.m_iBasis = Vector4(ZoomNear / rightMinusleft, 0.0f, rightLeftProjectionCalculation, 0.0f);
	matrixToModify.m_jBasis = Vector4(0.0f, ZoomNear / topMinusBottom, topBottomProjectionCalculation, 0.0f);
	matrixToModify.m_kBasis = Vector4(0.0f, 0.0f, farNearProjectionCalculation, ((-2.0f * FAR_DEPTH * NEAR_DEPTH) / farMinusNear));
	matrixToModify.m_translation = Vector4(0.0f, 0.0f, -1.0f, 0.0f); //bottom row for perspective OGL always 0,0,-1,0

	return matrixToModify * basisYZX;

	//trivial perspective projection
	// 	matrixToModify = Matrix4();
	// 	matrixToModify.m_translation = Vector4(0, 0, 0, 0);
	// 	matrixToModify.m_kBasis = Vector4(0, 0, 1, 1);

	//matrixToModify.Transpose();
}

///----------------------------------------------------------------------------------------------------------
///creates the default orthographic matrix
Matrix4 OpenGLRenderer::MakeDefaultOrthographicProjectionMatrix(){
	//L, R, B, T
	//0, W, 0, H
	const float topOfScreen = m_displayHeight;
	const float bottomOfScreen = 0.0f;
	const float rightOfScreen = m_displayWidth;
	const float leftOfScreen = 0.0f;

	const float orthoFarDepth = 1.0f;
	const float orthoNearDepth = -1.0f;
	const float farMinusNear = (orthoFarDepth - orthoNearDepth);
	const float farNearProjectionCalculation = -(orthoFarDepth + orthoNearDepth) / farMinusNear;

	const float rightMinusleft = rightOfScreen - leftOfScreen;
	const float topMinusBottom = topOfScreen - bottomOfScreen;

	const float rightLeftProjectionCalculation = -(rightOfScreen + leftOfScreen) / rightMinusleft;
	const float topBottomProjectionCalculation = -(topOfScreen + bottomOfScreen) / topMinusBottom;

	//float ZoomFactor = ASPECT_RATIO * (m_displayHeight / m_displayWidth );

	//look at the book for the orthographic matrix
	Matrix4 matrixToModify;
	//create orthographic here
	matrixToModify.m_iBasis = Vector4(2.0f / rightMinusleft, 0.0f, 0.0f, rightLeftProjectionCalculation);
	matrixToModify.m_jBasis = Vector4(0.0f, 2.0f / topMinusBottom, 0.0f, topBottomProjectionCalculation);
	matrixToModify.m_kBasis = Vector4(0.0f, 0.0f, -2.0f / farMinusNear, farNearProjectionCalculation);
	matrixToModify.m_translation = Vector4( 0.0f, 0.0f, 0.0f, 1.0f); //bottom row for orthographic OGL always 0,0,0,1

	//matrixToModify.Transpose();

	return matrixToModify;
}

//===========================================================================================================
