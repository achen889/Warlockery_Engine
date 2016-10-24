//==============================================================================================================
//FrameBuffer.hpp
//by Albert Chen Jul-21-2016.
//==============================================================================================================

#pragma once

#ifndef _included_FrameBuffer__
#define _included_FrameBuffer__

//===========================================================================================================

class Texture;

class Material;

//===========================================================================================================

struct FrameBuffer {
	//vars
	unsigned int m_fbo_id = 0;
	Texture* colorTex;
	unsigned int depthTex = 0;

	//methods

	//constructor
	FrameBuffer();
	FrameBuffer(unsigned int width, unsigned int height);
	~FrameBuffer();

	//helpers
	void InitTexs(unsigned int width, unsigned int height);
	void Bind();
	void Bind(unsigned int width, unsigned int height);
	void RenderToNewMaterial(Material*& mat, const char* shader_name);

};



//===========================================================================================================

#endif //__includedFrameBuffer__

