//================================================================================================================================
// Texture.hpp
//Texture Class by Ken Harward
//substantial code changes by Albert Chen 
//================================================================================================================================

#pragma once
#ifndef __included_Texture__
#define __included_Texture__

#include "Engine/Math/Vector2.hpp"
#include <string>
#include <map>

class Texture;
typedef std::map< std::string, Texture* > TextureMap;
typedef std::pair< std::string, Texture* > TextureMapEntry;
typedef std::map< std::string, Texture* >::iterator TextureMapIterator;
typedef std::map< int , Texture* >	IntTextureMap;

///----------------------------------------------------------------------------------------------------------
///constants

const std::string COMMON_TEXTURE_DIFFUSE =  "CommonDiffuseTexel";
const std::string COMMON_TEXTURE_NORMAL =	"CommonNormalTexel";
const std::string COMMON_TEXTURE_SPECULAR = "CommonSpecularTexel";
const std::string COMMON_TEXTURE_EMISSIVE = "CommonEmissiveTexel";

//================================================================================================================================
//Class Definition
//================================================================================================================================
class Texture{
public:
	virtual ~Texture();
	///-----------------------------------------------------------------------------------
	///getters and setters
	const std::string&		GetName() const															{ return m_pathName ; }
	unsigned int			GetPlatformHandle() const												{ return m_platformHandle ; }
	///-----------------------------------------------------------------------------------
	///texture manipulation methods
	static Texture*	CreateOrGetTexture( const std::string& texturePath, bool allowNullTexture = false );

	static void ShutDown(){

		//Texture::s_globalTextures.clear();

		TextureMapIterator textureIt = s_globalTextures.begin();
		while (textureIt != s_globalTextures.end()){

			if (textureIt->second){
				delete textureIt->second;
				textureIt->second = NULL;

				textureIt = s_globalTextures.erase(textureIt);
				continue;
			}

			++textureIt;
		}

		//ConsolePrintf("deleting global textures!\n");
	}

protected:
	///-----------------------------------------------------------------------------------
	///constructor
	Texture(){
		//do nothing
	}
	Texture( const std::string& pathName, bool allowNullTexture = false );

	void CreateTextureFromData(const std::string& texturePath, unsigned char* pixelData, const int& x, const int& y, const int& numberOfColorComponents);
	
	friend void CreateCommonEngineTextures();
	friend Texture* GetTextureInMap(const std::string& texName);

	friend bool VerifyTexturePath(const std::string& texturePath );
	
	//vars
	std::string	m_pathName;
	Vector2	m_size;
	unsigned int m_platformHandle;

	static TextureMap s_globalTextures;
	
};

typedef std::vector<Texture*>Textures;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline Texture::~Texture(){
	//do nothing
}

inline Texture* GetTextureInMap(const std::string& texName){
	TextureMapIterator texIt = Texture::s_globalTextures.find(texName);
	if (texIt != Texture::s_globalTextures.end()){
		return (texIt->second);
	}
	return NULL;
}


//===========================================================================================================

#endif // __included_Texture__