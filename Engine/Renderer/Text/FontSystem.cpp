//==============================================================================================================
//FontSystem.cpp
//by Albert Chen Jun-11-2015.
//==============================================================================================================

#include "Engine/Renderer/Text/FontSystem.hpp"

#include "Engine/Renderer/OpenGLRenderer.hpp"
//===========================================================================================================

FontSystem::FontSystem(){
	//do nothing
}

FontSystem::FontSystem(const std::string fontFilePath){
	LoadFontDataToBuffer(fontFilePath);

	LoadFontSystemFromBuffer();

	InitializeFontMeshRenderer();
}

void FontSystem::InitializeFontMeshRenderer(){
	m_fontRenderer = new MeshRenderer();
	m_fontRenderer->m_material = new Material();
	m_fontRenderer->m_mesh = new Mesh();

	std::string texturePath = "Data/Fonts/" + m_FontSummary.m_fileName;

	//m_textRenderer.m_material->m_glSampler.SetTexture(texturePath);
	m_fontRenderer->m_material->SetTextureInMap("gTexture", texturePath);

	m_fontRenderer->m_mesh->SetDrawMode(GL_QUADS);
	m_fontRenderer->m_material->InitializeMaterial("Data/Shaders/basicSampler.vert", "Data/Shaders/basicSampler.frag"); //load prog

	m_fontRenderer->BindVertexArray();
}

///----------------------------------------------------------------------------------------------------------
///accessors

AABB2 FontSystem::GetCharTextureCoords(const unsigned int& charID){

	CharDataMapIterator charIt = m_fontCharDataMap.find(charID);
	if (charIt != m_fontCharDataMap.end()){
		CharData& charData = (charIt->second);

		Vector2 CharUVmins = ToVector2(charData.charUVs);
		Vector2 CharUVmaxs = CharUVmins + ToVector2(charData.glyphSize);

		float CharStuffV = CharUVmins.y;
		CharUVmins.y = CharUVmaxs.y;
		CharUVmaxs.y = CharStuffV;
		
		CharUVmins.x /= (float)m_FontSummary.m_textureScale.x;
		CharUVmins.y /= (float)m_FontSummary.m_textureScale.y;

		CharUVmaxs.x /= (float)m_FontSummary.m_textureScale.x;
		CharUVmaxs.y /= (float)m_FontSummary.m_textureScale.y;

		return AABB2(CharUVmins, CharUVmaxs);
	}

	return AABB2::ZERO_TO_ONE;
}

//-----------------------------------------------------------------------------------------------------------

Vector2 FontSystem::GetCharGlyphSize(const unsigned int& charID){

	CharDataMapIterator charIt = m_fontCharDataMap.find(charID);
	if (charIt != m_fontCharDataMap.end()){
		CharData& charData = (charIt->second);

		Vector2 charGlyphSize = ToVector2(charData.glyphSize);

// 		charGlyphSize.x /= (float)m_FontSummary.m_textureSize;
// 		charGlyphSize.y /= (float)m_FontSummary.m_textureSize;

		charGlyphSize.y *= -1.0f;

		return charGlyphSize;
	}

	return Vector2::ZERO;
}

//-----------------------------------------------------------------------------------------------------------

Vector2 FontSystem::GetCharOffset(const unsigned int& charID){

	CharDataMapIterator charIt = m_fontCharDataMap.find(charID);
	if (charIt != m_fontCharDataMap.end()){
		CharData& charData = (charIt->second);
		
		Vector2 charOffset = ToVector2(charData.offset);

		//adjusting char offset
// 		charOffset.InverseScaleNonUniform(ToVector2(m_FontSummary.m_textureScale));
// 		charOffset.x /= m_FontSummary.m_textureSize;
// 		charOffset.y /= m_FontSummary.m_textureSize;

		charOffset.y *= -1.0f;
		return charOffset;
	}

	return Vector2::ZERO;
}

//-----------------------------------------------------------------------------------------------------------

unsigned int FontSystem::GetCharXAdvance(const unsigned int& charID){

	CharDataMapIterator charIt = m_fontCharDataMap.find(charID);
	if (charIt != m_fontCharDataMap.end()){
		CharData& charData = (charIt->second);
		
		unsigned int charXAdvance = charData.xAdvance;
		//adjusting char offset
//		charXAdvance /= (float)m_FontSummary.m_textureSize;

		return charXAdvance;
	}
	return 0;
}

//-----------------------------------------------------------------------------------------------------------

void FontSystem::LoadFontSystemFromBuffer(){
	std::string fontSummaryLoaded = "Font Summary Data:";

	char* currentValue;
	char* tokenInBuffer;
	
	LoadFontSummaryFromBuffer(currentValue, tokenInBuffer, fontSummaryLoaded);
	
	std::string charDataLoaded = "Char Data Loaded:";

	int charCount = (int)m_FontSummary.m_charCount;

	for (int i = 0; i < charCount; i++){

		LoadFontCharDataFromBuffer(currentValue, tokenInBuffer, charDataLoaded);

	}

	GetFontTexturePath();

	m_isLoaded = true;
}

//-----------------------------------------------------------------------------------------------------------

void FontSystem::LoadFontSummaryFromBuffer(char*& currentValue, char*& tokenInBuffer, std::string& fontSummaryLoaded){
	//loading font info face

	//use this with a char* buffer
	m_FontSummary.m_infoFace = "";
	currentValue = strtok_s(m_fontDataBuffer, "=", &tokenInBuffer);
	currentValue = strtok_s(NULL, " ", &tokenInBuffer);
	//ConsoleGenericMessageBox(currentValue, "loading token, current value = ");
	m_FontSummary.m_infoFace += currentValue;
	fontSummaryLoaded += "\nInfoFace = " + m_FontSummary.m_infoFace;

	//loading texture size
	currentValue = strtok_s(NULL, "=", &tokenInBuffer);
	currentValue = strtok_s(NULL, " ", &tokenInBuffer);
	m_FontSummary.m_textureSize = CStringToFloat(currentValue);
	fontSummaryLoaded += "\nTextureSize = " + FloatToString(m_FontSummary.m_textureSize);

	//skip the rest of this part for now
	currentValue = strtok_s(NULL, "\n", &tokenInBuffer);
	currentValue = strtok_s(NULL, "=", &tokenInBuffer);
	currentValue = strtok_s(NULL, " ", &tokenInBuffer);

	//load line height
	m_FontSummary.m_lineHeight = CStringToFloat(currentValue);
	fontSummaryLoaded += "\nLineHeight = " + FloatToString(m_FontSummary.m_textureSize);

	//load baseHeight
	currentValue = strtok_s(NULL, "=", &tokenInBuffer);
	currentValue = strtok_s(NULL, " ", &tokenInBuffer);
	m_FontSummary.m_baseHeight = CStringToFloat(currentValue);
	fontSummaryLoaded += "\nBaseHeight = " + FloatToString(m_FontSummary.m_baseHeight);

	//load textureScale
	currentValue = strtok_s(NULL, "=", &tokenInBuffer);
	currentValue = strtok_s(NULL, " ", &tokenInBuffer);
	m_FontSummary.m_textureScale.x = CStringToInt(currentValue);
	currentValue = strtok_s(NULL, "=", &tokenInBuffer);
	currentValue = strtok_s(NULL, " ", &tokenInBuffer);
	m_FontSummary.m_textureScale.y = CStringToInt(currentValue);
	fontSummaryLoaded += "\nTextureScale = " + IntToString(m_FontSummary.m_textureScale.x) + ", " + IntToString(m_FontSummary.m_textureScale.y);

	//bunch of other stuff skipped here

	//load file name
	m_FontSummary.m_fileName = "";
	currentValue = strtok_s(NULL, "\n", &tokenInBuffer);
	currentValue = strtok_s(NULL, "=", &tokenInBuffer);
	currentValue = strtok_s(NULL, "=", &tokenInBuffer);
	currentValue = strtok_s(NULL, "\n", &tokenInBuffer);
	m_FontSummary.m_fileName += currentValue;
	m_FontSummary.m_fileName.erase(m_FontSummary.m_fileName.begin());
	m_FontSummary.m_fileName.erase(m_FontSummary.m_fileName.end() - 2);
	//m_FontSummary.m_fileName = m_FontSummary.m_fileName.substr(1, m_FontSummary.m_fileName.length() - 3) /*+ "\0"*/;
	m_FontSummary.m_fileName[m_FontSummary.m_fileName.length() - 1] = '\0';
	fontSummaryLoaded += "\nFileName = " + m_FontSummary.m_fileName;

	//load charCount
	currentValue = strtok_s(NULL, "=", &tokenInBuffer);
	currentValue = strtok_s(NULL, "\n", &tokenInBuffer);
	//ConsoleGenericMessageBox(currentValue, "loading token, current value = ");
	m_FontSummary.m_charCount = (unsigned int)CStringToFloat(currentValue);
	fontSummaryLoaded += "\nCharCount = " + IntToString(m_FontSummary.m_charCount);
	//got char count
	ConsolePrintf(fontSummaryLoaded.c_str());
	//ConsoleGenericMessageBox(fontSummaryLoaded);
}

//-----------------------------------------------------------------------------------------------------------

void FontSystem::LoadFontCharDataFromBuffer(char*& currentValue, char*& tokenInBuffer, std::string& charDataLoaded){
	//parse one line of the charData
	currentValue = strtok_s(NULL, "=", &tokenInBuffer);
	currentValue = strtok_s(NULL, " ", &tokenInBuffer);
	//load charID
	unsigned int charID = CStringToInt(currentValue);

	//m_fontCharDataMap[charID] = CharData(); //mem leak: 36 * size of charDatMap

	//m_fontCharDataMap.insert(CharDataMapEntry(charID, CharData()));

	CharData charData;

	charDataLoaded += "\ncharID = " + IntToString(charID);
	//load char uvs
	currentValue = strtok_s(NULL, "=", &tokenInBuffer);
	currentValue = strtok_s(NULL, " ", &tokenInBuffer);
	unsigned int uvX = CStringToInt(currentValue);
	currentValue = strtok_s(NULL, "=", &tokenInBuffer);
	currentValue = strtok_s(NULL, " ", &tokenInBuffer);
	unsigned int uvY = CStringToInt(currentValue);
	charData.charUVs = IntVec2(uvX, uvY);
	charDataLoaded += "\ncharUVs = ( " + IntToString(charData.charUVs.x) + ", " + IntToString(charData.charUVs.y) + ") ";
	
	//load glyphSize
	currentValue = strtok_s(NULL, "=", &tokenInBuffer);
	currentValue = strtok_s(NULL, " ", &tokenInBuffer);
	unsigned int glyphSizeX = CStringToInt(currentValue);
	currentValue = strtok_s(NULL, "=", &tokenInBuffer);
	currentValue = strtok_s(NULL, " ", &tokenInBuffer);
	unsigned int glyphSizeY = CStringToInt(currentValue);
	charData.glyphSize = IntVec2(glyphSizeX, glyphSizeY);
	charDataLoaded += "\nglyphSize = ( " + IntToString(charData.glyphSize.x) + ", " + IntToString(charData.glyphSize.y) + ") ";
	//load offset
	currentValue = strtok_s(NULL, "=", &tokenInBuffer);
	currentValue = strtok_s(NULL, " ", &tokenInBuffer);
	unsigned int offsetX = CStringToInt(currentValue);
	currentValue = strtok_s(NULL, "=", &tokenInBuffer);
	currentValue = strtok_s(NULL, " ", &tokenInBuffer);
	unsigned int offsetY = CStringToInt(currentValue);
	charData.offset = IntVec2(offsetX, offsetY);
	charDataLoaded += "\noffset = ( " + IntToString(charData.offset.x) + ", " + IntToString(charData.offset.y) + ") ";
	//load x advance
	currentValue = strtok_s(NULL, "=", &tokenInBuffer);
	currentValue = strtok_s(NULL, " ", &tokenInBuffer);
	charData.xAdvance = (unsigned int)CStringToFloat(currentValue);
	charDataLoaded += "\nxAdvance = " + IntToString(charData.xAdvance);

	//add charData to map
	m_fontCharDataMap.insert(CharDataMapEntry(charID, charData) );

	//load page and chnl?
	// 		currentValue = strtok_s(NULL, "=", &tokenInBuffer);
	// 		currentValue = strtok_s(NULL, " ", &tokenInBuffer);
	// 		m_fontCharDataMap[charID]->page = CStringToFloat(currentValue);
	// 		currentValue = strtok_s(NULL, "=", &tokenInBuffer);
	// 		currentValue = strtok_s(NULL, " ", &tokenInBuffer);
	// 		m_fontCharDataMap[charID]->chnl = CStringToFloat(currentValue);
	//bunch of stuff skipped here

	currentValue = strtok_s(NULL, "\n", &tokenInBuffer);
}

//-----------------------------------------------------------------------------------------------------------

void FontSystem::CreateFontTexture(){
	std::string texturePath = "Data/Fonts/" + m_FontSummary.m_fileName;
	m_fontTexturePath = texturePath;
	m_FontTextureAtlas = Texture::CreateOrGetTexture(texturePath);
}

//-----------------------------------------------------------------------------------------------------------

float FontSystem::CalcTextWidth(const std::string& textToCheck, const float& textScale ){
	float totalTextWidth = 0.0f;
	for (int i = 0; i < (int)textToCheck.length(); i++){
		char& charToCheck = (char)textToCheck[i];
		totalTextWidth += GetCharXAdvance(charToCheck) * textScale;
		totalTextWidth += GetCharOffset(charToCheck).x * textScale;
	}
	return totalTextWidth;
}

//-----------------------------------------------------------------------------------------------------------