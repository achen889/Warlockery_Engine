//==============================================================================================================
//FontSystem.hpp
//by Albert Chen Jun-11-2015.
//==============================================================================================================
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Core/Utilities.hpp"
#include <map>
#include "Engine/Math/Math2D.hpp"

#include "Engine/Renderer/MeshRenderer.hpp"
//#include "Engine/Renderer/OGLRenderingUtils.hpp"

#pragma once

#ifndef _included_FontSystem__
#define _included_FontSystem__

//===========================================================================================================

struct CharData{
	IntVec2 charUVs;
	IntVec2 glyphSize;
	IntVec2 offset;
	unsigned int xAdvance;
	unsigned int page;
	unsigned int chnl;

	CharData(){
		//do nothing
	}

	~CharData(){
		//do nothing
	}
	
};

typedef std::map<unsigned int, CharData> CharDataMap;
typedef std::pair<unsigned int, CharData> CharDataMapEntry;
typedef std::map<unsigned int, CharData>::iterator CharDataMapIterator;

//===========================================================================================================

struct FontSummary{
	std::string m_infoFace; //check
	float m_textureSize; //check
	float m_lineHeight; //check
	float m_baseHeight; //check
	IntVec2 m_textureScale; //check
	std::string m_fileName; //check
	unsigned int m_charCount; //check
	
	FontSummary(){
		//do nothing
	}

	~FontSummary(){
		//do nothing
	}
};

//===========================================================================================================

//struct MeshRenderer;

class FontSystem{
public:
	//vars
	char* m_fontDataBuffer;
	FontSummary m_FontSummary;
	
	CharDataMap m_fontCharDataMap;

	Texture* m_FontTextureAtlas = NULL;
	MeshRenderer* m_fontRenderer = NULL;

	Material m_fontMat;

	std::string m_fontTexturePath;
	bool m_isLoaded = false;

	//methods
	FontSystem();
	FontSystem(const std::string fontFilePath);
	~FontSystem();

	void ShutDown(){
		//clear font char data
		m_fontCharDataMap.clear();
		if (m_fontRenderer){
			delete m_fontRenderer;
			m_fontRenderer = NULL;
		}
		if (m_FontTextureAtlas){
			delete m_FontTextureAtlas;
			m_FontTextureAtlas = NULL;
		}
	}

	void LoadFontDataToBuffer(const std::string& filePath);

	void LoadFontSystemFromBuffer();

	void LoadFontSummaryFromBuffer(char*& currentValue, char*& tokenInBuffer, std::string& fontSummaryLoaded);
	
	void LoadFontCharDataFromBuffer(char*& currentValue, char*& tokenInBuffer, std::string& charDataLoaded);
	
	void GetFontTexturePath();

	//assumes font mat already exists
	const Material& GetFontMaterial() { return m_fontMat; }

	void CreateFontTexture();

	void InitializeFontMeshRenderer();

	AABB2 GetCharTextureCoords(const unsigned int& charID);

	Vector2 GetCharGlyphSize(const unsigned int& charID);

	Vector2 GetCharOffset(const unsigned int& charID);

	unsigned int GetCharXAdvance(const unsigned int& charID);

	float CalcTextWidth(const std::string& textToCheck, const float& textScale = 1.0f);

};



//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods


inline FontSystem::~FontSystem(){
	//do nothing

// 	if (m_fontRenderer != NULL) {
// 		delete m_fontRenderer;
// 		m_fontRenderer = NULL;
// 	}

}

//-----------------------------------------------------------------------------------------------------------

inline void FontSystem::LoadFontDataToBuffer(const std::string& filePath){
	
	char* fontData = LoadTextFileToNewBuffer(filePath);

	m_fontDataBuffer = fontData;
}

//-----------------------------------------------------------------------------------------------------------

inline void FontSystem::GetFontTexturePath(){
	//get font texture path
	std::string texturePath = "Data/Fonts/" + m_FontSummary.m_fileName;
	m_fontTexturePath = texturePath;
}


#endif