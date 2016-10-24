//==============================================================================================================
//FrameBuffer.cpp
//by Albert Chen Jul-21-2016.
//==============================================================================================================

#include "FrameBuffer.hpp"
#include "OpenGLRenderer.hpp"
#include "Material.hpp"

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///constructors

FrameBuffer::FrameBuffer() :
	m_fbo_id(0),
	colorTex(NULL)
{
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

//constructor handles making an empty frame buffer
FrameBuffer::FrameBuffer(unsigned int width, unsigned int height) :
	m_fbo_id(theOGLRenderer->GenerateFrameBuffer()),
	colorTex(NULL)
{
	Bind((GLuint)width, (GLuint)height);
	InitTexs((GLuint)width, (GLuint)height);
	if (theOGLRenderer) {
		if (!theOGLRenderer->VerifyFrameBufferStatus()) {
			theOGLRenderer->DeleteFrameBuffer((GLuint)m_fbo_id);
		}
		theOGLRenderer->UnbindFrameBuffer();
	}
}

//-----------------------------------------------------------------------------------------------------------

FrameBuffer::~FrameBuffer() {
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

//helpers
void FrameBuffer::Bind() {
	if (theOGLRenderer) {
		theOGLRenderer->BindFrameBuffer((GLuint)m_fbo_id,
			(GLuint)theOGLRenderer->GetDisplayWidth(),
			(GLuint)theOGLRenderer->GetDisplayHeight());
	}	
}

//-----------------------------------------------------------------------------------------------------------

void FrameBuffer::Bind(unsigned int width, unsigned int height) {
	if (theOGLRenderer)
		theOGLRenderer->BindFrameBuffer((GLuint)m_fbo_id, (GLuint)width, (GLuint)height);
}

//-----------------------------------------------------------------------------------------------------------

void FrameBuffer::InitTexs(unsigned int width, unsigned int height) {
	if (!colorTex) {
		colorTex = new Texture();
	}
	if (theOGLRenderer) {
		theOGLRenderer->FrameBufferColorTexture(colorTex, (GLuint)width, (GLuint)height);
		//theOGLRenderer->FrameBufferDepthTexture(depthTex);
	}
}

//-----------------------------------------------------------------------------------------------------------

 void FrameBuffer::RenderToNewMaterial(Material*& mat, const char* shader_name ) {
	//Material* newMat = new Material();
	 if (mat) {
		 mat->InitializeMaterial(shader_name);
		 mat->SetTextureInMap("gTexture", colorTex); //colorTex is backwards apparently
	 }
}

//===========================================================================================================

