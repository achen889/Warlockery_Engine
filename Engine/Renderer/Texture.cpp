//================================================================================================================================
// Texture.cpp
//================================================================================================================================

#include "Engine/Renderer/Texture.hpp"
#define STBI_HEADER_FILE_ONLY
#include "Engine/Renderer/stb_image.c"
#undef STBI_HEADER_FILE_ONLY

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#define GL_CLAMP_TO_EDGE 0x812F

//================================================================================================================================
//global texture declarations
//================================================================================================================================
std::map< std::string, Texture* >	Texture::s_globalTextures ;
///-----------------------------------------------------------------------------------
///Constructors
///-----------------------------------------------------------------------------------
Texture::Texture(const std::string& texturePath, bool allowNullTexture ){
	int				x ;
	int				y ; 
	int				numberOfColorComponents ;
	int				requiredNumberOfColorComponents ;
	UNUSED(requiredNumberOfColorComponents);
	
	unsigned char*	pixelData	= stbi_load( texturePath.c_str(), &x, &y, &numberOfColorComponents, 0 );
	if(pixelData == nullptr && !allowNullTexture ){
		std::string errorMessageTitle = "ERROR: pixelData not set, texture not found!";
		std::string errorMessageTexture = "DEBUG: "+texturePath +" is Missing or Broken!";

		ConsoleGenericMessageBox( errorMessageTexture.c_str(),  errorMessageTitle.c_str() );
	}

	CreateTextureFromData(texturePath, pixelData, x, y, numberOfColorComponents);
	
	stbi_image_free( pixelData );
}

//===========================================================================================================

void Texture::CreateTextureFromData(const std::string& texturePath, unsigned char* pixelData, const int& x, const int& y, const int& numberOfColorComponents){
	if (!pixelData)
		return;

	m_size.x = (float)x;
	m_size.y = (float)y;
	m_pathName = texturePath;

	// Enable texturing
	glEnable(GL_TEXTURE_2D);

	// Tell OpenGL that our pixel data is single-byte aligned
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Ask OpenGL for an unused texName (ID number) to use for this texture
	glGenTextures(1, (GLuint*)&m_platformHandle);

	// Tell OpenGL to bind (set) this as the currently active texture
	glBindTexture(GL_TEXTURE_2D, m_platformHandle);

	// Set texture clamp vs. wrap (repeat)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // one of: GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT, GL_MIRROR_CLAMP_TO_EDGE, ...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // one of: GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT, GL_MIRROR_CLAMP_TO_EDGE, ...

	// Set magnification (texel > pixel) and minification (texel < pixel) filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // one of: GL_NEAREST, GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // one of: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR

	GLenum bufferFormat = GL_RGBA; // the format our source pixel data is currently in; any of: GL_RGB, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA, ...
	if (numberOfColorComponents == 3)
		bufferFormat = GL_RGB;
	if (numberOfColorComponents == 1)
		bufferFormat = GL_RED; //gotta be a way to get a white font color with this

	// Todo: What happens if numComponents is neither 3 nor 4?

	GLenum internalFormat = bufferFormat; // the format we want the texture to me on the card; allows us to translate into a different texture format as we upload to OpenGL

	glTexImage2D(				// Upload this pixel data to our new OpenGL texture
		GL_TEXTURE_2D,			// Creating this as a 2d texture
		0,						// Which mipmap level to use as the "root" (0 = the highest-quality, full-res image), if mipmaps are enabled
		internalFormat,			// Type of texel format we want OpenGL to use for this texture internally on the video card
		(int)m_size.x,			// Texel-width of image; for maximum compatibility, use 2^N + 2^B, where N is some integer in the range [3,10], and B is the border thickness [0,1]
		(int)m_size.y,			// Texel-height of image; for maximum compatibility, use 2^M + 2^B, where M is some integer in the range [3,10], and B is the border thickness [0,1]
		0,						// Border size, in texels (must be 0 or 1)
		bufferFormat,			// Pixel format describing the composition of the pixel data in buffer
		GL_UNSIGNED_BYTE,		// Pixel color components are unsigned bytes (one byte per color/alpha channel)
		pixelData);	// Location of the actual pixel data bytes/buffer


}

//===========================================================================================================

///-----------------------------------------------------------------------------------
///creates or get textures NOTE: Use Backslashes '/' for the file path!
///-----------------------------------------------------------------------------------
Texture* Texture::CreateOrGetTexture(const std::string& texturePath, bool allowNullTexture ){

	if(texturePath.find("\\") != std::string::npos ){
		std::string errorMessageTitle = "ERROR: Forward Slash Detected in File Path!";
		std::string errorMessageTexture = "DEBUG: "+texturePath +" contains a forward slash //\n";
		ERROR_RECOVERABLE(errorMessageTexture+errorMessageTitle);
	}
	
	TextureMap::iterator textureIter = s_globalTextures.find( texturePath );
	if (textureIter != s_globalTextures.end()){
		return textureIter->second;
	}	
	Texture* newTexture = new Texture(texturePath, allowNullTexture); //mem leak here
	//if not null add to list
	if (!(newTexture->GetName() == "")){
			s_globalTextures[texturePath] = newTexture;
	}
	return newTexture;

}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///friend methods

void CreateCommonEngineTextures(){
	

	const int defaultColorComponents = 3;

	unsigned char defaultDiffuseTexel [4] = { 255, 255, 255, 255 }; //opaque white
	unsigned char defaultNormalTexel  [defaultColorComponents] = { 127, 127, 255 }; //normal facing out
	unsigned char defaultSpecularTexel[defaultColorComponents] = { 50, 127 , 0  }; //dark yellow
	unsigned char defaultEmissiveTexel[defaultColorComponents] = { 127, 127, 127 }; //grey

	const int TEXEL_SIZE_X = 1;
	const int TEXEL_SIZE_Y = 1;

	Texture* defaultDiffuseTexture =  new Texture();
	Texture* defaultNormalTexture =   new Texture();
	Texture* defaultSpecularTexture = new Texture();
	Texture* defaultEmissiveTexture = new Texture();

	int platformHandleID = 0;

	//make diffuse
 	defaultDiffuseTexture->m_platformHandle = platformHandleID;
 	defaultDiffuseTexture->CreateTextureFromData(COMMON_TEXTURE_DIFFUSE, defaultDiffuseTexel, TEXEL_SIZE_X, TEXEL_SIZE_Y, 4);
	Texture::s_globalTextures[COMMON_TEXTURE_DIFFUSE] = defaultDiffuseTexture;
	platformHandleID++;

	//make normal
	defaultNormalTexture->m_platformHandle = platformHandleID;
	defaultNormalTexture->CreateTextureFromData(COMMON_TEXTURE_NORMAL, defaultNormalTexel, TEXEL_SIZE_X, TEXEL_SIZE_Y, defaultColorComponents);
	Texture::s_globalTextures[COMMON_TEXTURE_NORMAL] = defaultNormalTexture;
	platformHandleID++;

	//make specular
	defaultSpecularTexture->m_platformHandle = platformHandleID;
	defaultSpecularTexture->CreateTextureFromData(COMMON_TEXTURE_SPECULAR, defaultSpecularTexel, TEXEL_SIZE_X, TEXEL_SIZE_Y, defaultColorComponents);
	Texture::s_globalTextures[COMMON_TEXTURE_SPECULAR] = defaultSpecularTexture;
	platformHandleID++;

	//make emissive
	defaultEmissiveTexture->m_platformHandle = platformHandleID;
	defaultEmissiveTexture->CreateTextureFromData(COMMON_TEXTURE_EMISSIVE, defaultEmissiveTexel, TEXEL_SIZE_X, TEXEL_SIZE_Y, defaultColorComponents);
	Texture::s_globalTextures[COMMON_TEXTURE_EMISSIVE] = defaultEmissiveTexture;
	platformHandleID++;
	
// 	defaultDiffuseTexture = NULL;
// 	defaultNormalTexture = NULL;
// 	defaultSpecularTexture = NULL;
// 	defaultEmissiveTexture = NULL;

}

//-----------------------------------------------------------------------------------------------------------

//checks if path has an image
bool VerifyTexturePath(const std::string& texturePath ){
	Texture* tempTexture = Texture::CreateOrGetTexture(texturePath, true);
	if (tempTexture->GetName() == ""){
		return false;
	}
	return true;
}

//===========================================================================================================

