//==============================================================================================================
//OpenGLRenderer.hpp
//by Albert Chen Jan-15-2015.
//==============================================================================================================

#pragma once

#ifndef _included_OpenGLRenderer__
#define _included_OpenGLRenderer__

#include <windows.h>
#include <vector>
#include <gl\GL.h>
#include <gl\GLU.h>

#include "Engine\Math\EngineMath.hpp"
#include "Engine\Math\Vector3.hpp"
#include "Engine\Renderer\Texture.hpp"
#include "Engine\Math\Disc2.hpp"
#include "Engine\Math\AABB2.hpp"
#include "Engine\Core\Rgba.hpp"
#include "Engine\Math\EulerAngles.hpp"
#include "Engine\Renderer\Camera3D.hpp"
#include "Engine\Renderer\glext.h"
#include "Engine\Math\Sphere3.hpp"
#include "Engine\Math\LineSegment3.hpp"
#include "Engine\Math\AABB3.hpp"
#include "Engine\Math\MatrixUtils.hpp"

static const Matrix4 basisYZX(
	Vector4(0, 1, 0, 0), //xRIGHT -> yRIGHT //basis for shader
	Vector4(0, 0, 1, 0), //yUP -> zUP // -zUP, changing this doesn't fix that
	Vector4(-1, 0, 0, 0), //zFWD -> xFWD
	Vector4(0, 0, 0, 1)); //no translation

//default is Y up Z fwd, basis fixes that //basis for world
static const Matrix4 basisZXY(
	Vector4(0, 0, 1, 0), //x -> z
	Vector4(1, 0, 0, 0), //y -> x
	Vector4(0, 1, 0, 0), //z -> y
	Vector4(0, 0, 0, 1)); //no translation

//const char* defaultStaticShader = "";

//const char* defaultTextureShader = "";

//ConsoleGenericMessageBox("In LoadShader: Shader Created!");

// can take multiple strings - internally these will all be 
// concatenated end-to-end

#define GL_CHECK_ERROR() GLCheckError(__FILE__, __LINE__-1)

static const float FOV_VERTICAL = 45.0f;
static const float ASPECT_RATIO = (16.0f / 9.0f);
static const float NEAR_DEPTH = 0.1f;
static const float FAR_DEPTH = 1000.0f;

//---------------------------------------------------------------------------
// Silly OpenGL stuff you have to do in Windows to use "advanced" OpenGL
//	features (OpenGL 1.5 and beyond), such as those for VBOs:

// Put externs for these globals in some header that everyone (who cares about drawing) can see
extern PFNGLGENBUFFERSPROC		glGenBuffers;
extern PFNGLBINDBUFFERPROC		glBindBuffer;
extern PFNGLBUFFERDATAPROC		glBufferData;
extern PFNGLGENERATEMIPMAPPROC	glGenerateMipmap;
extern PFNGLDELETEBUFFERSPROC   glDeleteBuffers;

extern PFNGLCREATESHADERPROC     glCreateShader;
extern PFNGLSHADERSOURCEPROC     glShaderSource;
extern PFNGLDELETESHADERPROC     glDeleteShader;
extern PFNGLGETSHADERIVPROC      glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLCOMPILESHADERPROC    glCompileShader;

extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLDETACHSHADERPROC glDetachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLPROGRAMPARAMETERIPROC glProgramParameteri;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;

extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLGETUNIFORMINDICESPROC glGetUniformIndices;
extern PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
extern  PFNGLUSEPROGRAMPROC glUseProgram;

extern PFNGLUNIFORM1FVPROC glUniform1fv;
extern PFNGLUNIFORM1IVPROC glUniform1iv;
extern PFNGLUNIFORM2FVPROC glUniform2fv;
extern PFNGLUNIFORM3FVPROC glUniform3fv;
extern PFNGLUNIFORM4FVPROC glUniform4fv;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;

extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

extern PFNGLGENSAMPLERSPROC glGenSamplers;
extern PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri;
extern PFNGLDELETESAMPLERSPROC glDeleteSamplers;
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLBINDSAMPLERPROC glBindSampler;

struct VertexArrayObject;
struct Vertex3D;
class Material;
struct GLSampler;

typedef std::vector<Vertex3D> Vertex3Ds;

class OpenGLRenderer{
protected:
	HWND m_windowHandle;
	HDC m_deviceContext;
	HGLRC m_renderingContext;
	float m_displayWidth;
	float m_displayHeight;
	size_t m_frameCount;

	//ModelViewMatrixStack* m_matrixStack;
//protected:
	void CreateOpenGLRenderingContext();
	void DestroyOpenGLRenderingContext();
public:
	OpenGLRenderer();
	~OpenGLRenderer();

	//-----------------------------------------------------------------------------------------------------------
	//general functions
	void InitializeAdvancedOpenGLFunctions();
	bool CheckCompatability();
	void GLCheckError();
	void GLCheckError(const char* file, int line );
	std::string GLTranslateErrorCode(const GLenum& errorCode);
	//-----------------------------------------------------------------------------------------------------------
	//GLuint gProgram;
	//GLuint gProgram1;

	//Initialize Shader Methods
	void InitializeShaders();
	//legacy
	void InitializeVAO(VertexArrayObject& myVAO, const char* vertFile, const char* fragFile);
	void InitializeVAOWithVertexArrayBuffer(VertexArrayObject& myVAO);
	void InitializeVAOWithShader(VertexArrayObject& myVAO, const char* vertFile, const char* fragFile);
	void DestroyVAO(VertexArrayObject& myVAO);
	//void DestroyMeshVertexArray(MeshRenderer& myMeshRenderer);
	void CreateVAOVertexArrayWithShader(VertexArrayObject& myVAO);
	void CreateVAOBuffer(VertexArrayObject& myVAO);

	int GetBufferCount();
	
	//VBO shader drawing helper methods
	GLuint InitializeVertexBuffer(GLuint& out_vboID);
	GLuint InitializeIndexBuffer(GLuint& out_iboID);
	void CopyVertexBufferData(GLuint vboID, const void* bufferData, size_t bufferSize);
	void CopyIndexBufferData(GLuint iboID, const void* bufferData, size_t bufferSize);

	GLuint CreateBuffer(GLuint vboID, const void* bufferData, size_t bufferSize);
	void CreateVertexArrayWithShader(GLuint programID, GLuint& bufferID, GLuint& vertexArrayID);
	bool ProgramBindAttribute(GLuint program_id, char const *name, GLint count, GLenum type, GLboolean normalize, GLsizei stride, GLsizei offset);
	//program loading methods
	GLuint LoadProgram(const char* vert_file, const char* frag_file);
	bool LinkProgram(GLuint program_id, GLuint vert_shader, GLuint frag_shader);

	//program bindings
	bool ProgramBindMatrix(GLuint prog_id, const char* name, const Matrix4& mat, bool doTranspose = false);
	bool ProgramBindFloat(GLuint programID, char const *name, const float& floatToPass);
	bool ProgramBindFloatArray(GLuint programID, char const *name, float* floatArray, int arraySize);
	bool ProgramBindVec2(GLuint programID, char const *name, const Vector2& vecToPass);
	bool ProgramBindVec2Array(GLuint programID, char const *name, Vector2* vecArray, int arraySize);
	bool ProgramBindVec3(GLuint programID, char const *name, const Vector3& vecToPass);
	bool ProgramBindVec3Array(GLuint programID, char const *name, Vector3* vecArray, int arraySize);
	bool ProgramBindVec4(GLuint programID, char const *name, const Vector4& vecToPass);
	bool ProgramBindVec4Array(GLuint programID, char const *name, Vector4* vecToPass, int arraySize);
	bool ProgramBindSamplerIndex(GLuint programID, char const *name, GLint index);
	
	bool CheckProgramStatus(GLuint program_id);
	//shader loading methods
	GLuint LoadShader(const char* filename, GLenum shader_type);

	void FreeShader(GLuint shader_id);
	bool CheckShaderStatus(GLuint shader_id, const char* filename);
	void CreateVAOSampler(VertexArrayObject& myVAO, GLenum minFilter, GLenum magFilter, GLenum uWrap, GLenum vWrap);
	GLuint CreateSampler(GLSampler& myGLSampler);
	void DestroySampler(GLuint sampler_id);
	//shader rendering
	void SetShaderProgramToUse(GLuint& programUse);
	void DisableShaderProgram();
	//program binding wrappers
	void BindViewMatricesToProgram(GLuint& programBinding, const Matrix4& viewMatrix, const Matrix4& perspectiveMatrix);
	void BindCameraTransformToProgram(GLuint& programBinding, Camera3D& camera);

	void DrawVertexArray(GLenum drawMode, GLint vaoID, size_t vaoSize);
	void DrawVertexElements(GLenum drawMode, GLint iboID, size_t iboSize, size_t indexStartDraw = 0 );
	void RenderWithVBOs(Camera3D& camera);
	void PrepareVAOForRendering(VertexArrayObject& myVAO, Camera3D& camera, bool isPerspective );
	void RenderVAO(VertexArrayObject& myVAO, Camera3D& camera, bool isPerspective = true);

	void RenderParticlesWithVAO(VertexArrayObject& myVAO, Camera3D& camera);
	//===========================================================================================================
	
	//-----------------------------------------------------------------------------------------------------------

	///Getters
	float GetDisplayWidth() const{ return m_displayWidth; }
	size_t GetFrameCount() const{ return m_frameCount; }
	float GetDisplayHeight() const{ return m_displayHeight; }
	Vector2 GetDisplaySize() const{ return Vector2(m_displayWidth, m_displayHeight); }
	//Setters
	void SetModelViewDepthTest();
	void DisableModelViewDepthTest();
	//these views are deprecated
	void SetOrthographicView();
	void SetOrthographicView(float setLeft, float setRight, float setBottom, float setTop );
	void SetPerspectiveView();
	void SetPerspectiveView(float fieldOfViewDegreesVertical, float aspectRatio, float nearDepth, float farDepth);
	
	void SetModelViewAlphaTest();
	void SetModelCullFaceMode(GLenum cullFaceMode);
	void DisableModelCullFaceMode();

	void SetModelLineWidth(const int& lineWidth = 1);
	void SetModelViewColor(const Rgba& viewColor);
	void SetModelViewTranslation(float x, float y, float z=0.0f);
	void SetModelViewScale(float x, float y, float z=0.0f);
	void SetTextureViewTransparent();
	void SetTextureBlendFunction(GLenum sourceFactor ,GLenum destFactor );
	void SetModelViewMatrixMode();
	void SetModelViewRotation(float angleDegrees, float amountAroundX, float amountAroundY, float amountAroundZ );

	void SetModelPolygonMode(bool isLineMode = false);

	void SwapBuffers();
	void ClearBuffer();
	void ClearBuffer(Rgba clearColor, float clearDepth = 1.0f );

	//stencil methods
	void SetModelViewStencilTest();
	void DisableModelViewStencilTest();

	void EnableStencilBufferDrawing();
	void DisableStencilBufferDrawing();

	//color mask
	void SetModelColorMask(const unsigned char& maskAll ){
		
		glColorMask(maskAll, maskAll, maskAll, maskAll);
	}
	void SetModelColorMask(const unsigned char& maskR, const unsigned char& maskG, const unsigned char& maskB, const unsigned char& maskA){
		glColorMask(maskR, maskG, maskB, maskA);
	}
	

	//camera methods
	void ApplyCameraTransform(const Vector3& cameraPosition, EulerAngles cameraEulerAngles );
	void ApplyCameraTransform(const Camera3D& camera);

	void ClearColor(Rgba clearedColor );
	void ClearDepth(float clearedDepth );

	void StartUp( HWND windowHandle);
	void ShutDown();
	
	//draw functions
	void DrawTriangle(const Vector2& v0, const Vector2& v1, const Vector2& v2 );
	void DrawTriangle3D(const Vector3& v0, const Vector3& v1, const Vector3& v2 );

	void DrawQuad(const Vector2s& vertices );
	void DrawQuad3D(const Vector3s& vertices );

	void DrawPoints(const Vector2s& vertices );
	void DrawPoints3D(const Vector3s& vertices );
	/*DEBUG*/
	void DrawPoint3D(const Vector3& vertexPt);

	void DrawLines(const Vector2s& vertices );
	
	/*DEBUG*/
	void DrawLineSegment3D(const LineSegment3& linesegToDraw, Rgba viewStartColor );
	void DrawLineSegment3D(const LineSegment3& linesegToDraw, Rgba viewStartColor, Rgba viewEndColor );
	/*DEBUG*/
	void DrawArrow3D(LineSegment3 linesegToDraw, const Rgba& viewStartColor = Rgba::MAGENTA, const Rgba& viewEndColor = Rgba::GOLD );
	
	void DrawPolygon(const Vector2s& vertices );
	void DrawPolygon(const Vector2s& vertices , Rgba& viewColor );

	void DrawSolidPolygon(const Vector2s& vertices, Rgba& viewColor );
	void DrawSolidPolygon(const Vector2s& vertices);

	void DrawDisc2D(Disc2 DiscToDraw );
	
	void DrawDisc2D(Disc2 DiscToDraw, const Rgba& viewColor);
	/*DEBUG*/
	void DrawSphere3D(Sphere3 sphereToDraw, const int& wedges = 16, const int& slices = 16, const Rgba& viewColor = Rgba::MAGENTA );

	void DrawSolidDisc2D(Disc2 DiscToDraw );
	void DrawSolidDisc2D(Disc2 DiscToDraw, Rgba& viewColor );

	void DrawAABB2(AABB2 BoxToDraw );
	void DrawSolidAABB2(AABB2 BoxToDraw );
	/*DEBUG*/
	void DrawAABB3(AABB3 BoxToDraw );
	void DrawAABB3(AABB3 BoxToDraw, Rgba viewColor );

	void DrawCoordinateAxes(float length);

	//texture handlers
	void DrawTextureQuad( const Texture& texture, const Vector2s& vertices, const Vector2s& textureCoordinates, bool isOpaque = false );
	void DrawTextureQuad( const Texture& texture, const Vector2s& vertices, bool isOpaque = false );
	void DrawTextureQuadColor( const Texture& texture, const Vector2s& vertices, Rgba& viewColor );

	void DrawTextureQuad3D( const Texture& texture, const Vector3s& vertices, const Vector2s& textureCoordinates );
	void DrawTextureQuad3D( const Texture& texture, const Vector3s& vertices );
	void DrawTextureQuad3DColor( const Texture& texture, const Vector3s& vertices, const Vector2s& textureCoordinates, Rgba& viewColor );

	void PushMatrix();
	void PopMatrix();

	//-----------------------------------------------------------------------------------------------------------
	//VBO && VAO drawing
	GLuint m_vboID;
	GLuint m_vaoID;
	size_t m_numOFVBOVertexes;
	size_t m_numOFVBOVertexes1;
	GLuint m_vboID1;
	GLuint m_vaoID1;
	//Vertex3Ds m_VertexAttributes;

	void GenerateVertexArrayAndVBO();
	//some VAO drawing methods
	void GenerateVertexArrayUnitCube(Vertex3Ds& out_vertexArray);
	void GenerateVertexArrayAABB3(Vertex3Ds& out_vertexArray, AABB3 boxCoords, const Rgba& viewColor = Rgba::WHITE );
	void GenerateVertexArrayAABB2(Vertex3Ds& out_vertexArray, AABB2 boxCoords, const Rgba& boxColor = Rgba::WHITE);
	void GenerateVertexArrayTextureQuad(Vertex3Ds& out_vertexArray, AABB2 boxCoords, AABB2 textureCoords, const Rgba& vertexColor = Rgba::WHITE );
	
	void GenerateVertexArraySphere3D(Vertex3Ds& out_vertexArray, Sphere3 sphereToDraw, const int& wedges = 32, const int& slices = 32, const Rgba& viewColor = Rgba::MAGENTA);

	void PopulateVertexArray(Vertex3Ds& out_vertexArray);

	

	void DrawVBO();
	
	Matrix4 MakeDefaultPerspectiveProjectionMatrix();
	Matrix4 MakeDefaultOrthographicProjectionMatrix();

};
//===========================================================================================================

extern OpenGLRenderer* theOGLRenderer;

///----------------------------------------------------------------------------------------------------------
///inline functions

inline void OpenGLRenderer::SetModelViewDepthTest(){
	glEnable(GL_DEPTH_TEST);
}

inline void OpenGLRenderer::DisableModelViewDepthTest(){
	glDisable(GL_DEPTH_TEST);
}

inline void OpenGLRenderer::SetModelLineWidth(const int& lineWidth){
	glLineWidth((float)lineWidth);
}


inline void OpenGLRenderer::SetModelViewAlphaTest(){
	glAlphaFunc( GL_GREATER, 0.5f );
	glEnable( GL_ALPHA_TEST );
}

//cull face modes = GL_BACK , GL_FRONT
inline void OpenGLRenderer::SetModelCullFaceMode(GLenum cullFaceMode){
	glCullFace(cullFaceMode);
}

inline void OpenGLRenderer::DisableModelCullFaceMode(){
	glDisable(GL_CULL_FACE);
}

///----------------------------------------------------------------------------------------------------------
///blend function
//source , destination do cool things
inline void OpenGLRenderer::SetTextureBlendFunction(GLenum sourceFactor ,GLenum destFactor ){
	glEnable(GL_BLEND);
	glBlendFunc(sourceFactor, destFactor ); //black magic method
}

inline void OpenGLRenderer::ClearColor(Rgba clearedColor ){
	float viewColorR = (float) clearedColor.r * g_oneOverColorBits;
	float viewColorG = (float) clearedColor.g * g_oneOverColorBits;
	float viewColorB = (float) clearedColor.b * g_oneOverColorBits;
	float viewColorA = (float) clearedColor.a * g_oneOverColorBits;

	glClearColor( viewColorR , viewColorG, viewColorB , viewColorA );
}

inline void OpenGLRenderer::ClearDepth(float clearedDepth ){
	glClearDepth(clearedDepth );
}

//===========================================================================================================


#endif