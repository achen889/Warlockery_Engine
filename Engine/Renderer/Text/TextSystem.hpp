//==============================================================================================================
//TextSystem.hpp
//by Albert Chen Jun-16-2015.
//==============================================================================================================

#pragma once

#ifndef _included_TextRenderer__
#define _included_TextRenderer__


#include "Engine\Core\Utilities.hpp"
#include "Engine\Renderer\Text\FontSystem.hpp"

#include "Engine\Input\InputSystem.hpp"
#include "Engine\Math\MathCommands.hpp"

#include "Engine\Renderer\OpenGLRenderer.hpp"

//===========================================================================================================

const float TEXT_SCALE_FACTOR = 1.0f;
const unsigned int TEXT_CHARACTERS_PER_LINE = 120;
const float TEXT_LINE_SPACING_FACTOR = 32.0f;
const float TEXT_CHAR_SPACING_FACTOR = 1.0f;

//dev console consts
static const Vector2 DEV_CONSOLE_CURSOR_START_POSITION = Vector2(0.0f, 64.0f);
static const Vector2 DEV_CONSOLE_OUTPUT_POSITION = Vector2(DEV_CONSOLE_CURSOR_START_POSITION.x + 700.0f, 64.0f + DEV_CONSOLE_CURSOR_START_POSITION.y - (1.0f * TEXT_SCALE_FACTOR * TEXT_LINE_SPACING_FACTOR ));
static const float DEV_CONSOLE_CURSOR_WIDTH = TEXT_LINE_SPACING_FACTOR * 0.25f;

static AABB2 CURSOR_QUAD(Vector2::ZERO, Vector2(DEV_CONSOLE_CURSOR_WIDTH, TEXT_LINE_SPACING_FACTOR));
//something's wrong with these static colors
static Rgba DEV_CONSOLE_INPUT_COLOR = Rgba::VIOLET;
static Rgba DEV_CONSOLE_OUTPUT_COLOR = Rgba::GOLD;
static Rgba DEV_CONSOLE_CURSOR_COLOR = Rgba::NEON_GREEN;
static Rgba DEV_CONSOLE_BACKGROUND_COLOR = Rgba(0, 0, 0, 180);
static float DEV_CONSOLE_SCROLL_LEVEL = 0;
static const float DEV_CONSOLE_LINE_SPACING = TEXT_SCALE_FACTOR * TEXT_LINE_SPACING_FACTOR;

#define OUTPUT_STRING_TO_CONSOLE(name, lineLength) OutputStringToConsole(name, lineLength)

#define OUTPUT_STRING_TO_SCREEN(name, screenPosX, screenPosY ) if(theTextSystem){ theTextSystem->RenderTextString(name, Vector2((float)screenPosX, (float)screenPosY ) ); }

#define OUTPUT_STRING_TO_SCREEN_VEC2(name, textPosVec2 ) if(theTextSystem){ theTextSystem->RenderTextString(name, textPosVec2); }

#define OUTPUT_COLOR_STRING_TO_SCREEN(name, screenPosX, screenPosY, color ) if(theTextSystem){ theTextSystem->RenderTextString(name, Vector2((float)screenPosX, (float)screenPosY ), Rgba(color), 1.0f); }

#define OUTPUT_COLOR_CHAR_TO_SCREEN(name, screenPosX, screenPosY, color ) if(theTextSystem){ theTextSystem->RenderChar(name, Vector2((float)screenPosX, (float)screenPosY ), Rgba(color) ); }

//===========================================================================================================

class TextSystem{
public:
	FontSystem m_fontSystem;
	//VertexArrayObject m_fontVAO;

	MeshRenderer m_textRenderer; //general purpose text rendering
	MeshRenderer m_devConsoleTextRenderer; //dev console text only
	MeshRenderer m_devConsoleBgRenderer; //dev console bg
	MeshRenderer m_devConsoleCursorRenderer; //renders blinking cursor quad
	Vertex3Ds m_devConsoleTextVerts;

	Camera3D m_textCamera;

	//legacy code
	VertexArrayObject m_textIn;
	VertexArrayObject m_textOut;
	VertexArrayObject m_textBackgroundQuads;
	
	//legacy stuff
	VertexArrayObject m_textCursor;
	
	OpenGLRenderer* m_OGLRenderer;
	bool m_IsDevelopmentConsoleActive;
	std::string textInputBuffer;

	//std::string textOutputBuffer;
	std::vector<std::string>m_textHistory;

	MathCommands m_mathCommands;
	Vector2 currentCursorPos;

	//constructors
	TextSystem();

	void StartUp(const std::string& defaultFontFilePath, OpenGLRenderer* renderer);
	void ShutDown();

	void RegisterDefaultCommands();

	~TextSystem(){

	}

	void Update(double deltaSeconds);	
	
	//dev console helpers
	void ToggleDevelopmentConsole();
	
	std::string InputTextToBuffer(std::string& bufferText);

	bool IsKeyPressedDevConsoleHotkey(const unsigned int& keyPressedID);

	void ProcessDevConsoleHotkeys(std::string& bufferText);

	//dev console hotkey helpers
	void ConfirmDevConsoleCommand(std::string& bufferText);
	void ProcessTextHistoryHotkeys(std::string& bufferText);
	void ProcessDevConsoleScrolling(std::string& bufferText);
	void ResetDevConsoleScrollHeight();

	void DevConsoleScrollUp(std::string& bufferText, const int& scrollAmount = 1);
	void DevConsoleScrollDown(std::string& bufferText, const int& scrollAmount = 1);

	void ProcessDeleteDevConsoleCharInput(std::string& bufferText);

	void DeleteDevConsoleChar(VertexArrayObject& myVAO, std::string& bufferText);
	void DeleteDevConsoleChar(Vertex3Ds& in_vertexArray, std::string& bufferText);

	void ProcessDevConsoleScrollingKeyInput(std::string& bufferText, int& consoleScrollAmount);
	void ProcessDevConsoleScrollingMouseInput(std::string& bufferText, int& consoleScrollAmount);

	void SkipConsoleLine(VertexArrayObject& myVAO, bool skipDown = true);
	void SkipConsoleLine(Vertex3Ds& m_vertexArray, bool skipDown = true);
	
	void ResetCursorPosition() { currentCursorPos = DEV_CONSOLE_CURSOR_START_POSITION; }

	//queries
	float CalcTextWidth(const std::string& textToCalc);

	//loading helpers
	void LoadFontSystem(const std::string& fontFilePath);
	void LoadFontSystem(FontSystem& fontSystem, const std::string& fontFilePath);

	void InitializeFontMeshRenderer();
	void InitializeFontMeshRenderer(FontSystem& fontSystem);

	//rendering helpers
	void RenderTextString(const std::string& myString);
	void RenderTextString(const std::string& myString, const Vector2& startingPosition = Vector2(50, 50), 
		const Rgba& textColor = Rgba::WHITE, const float& localTextScaleFactor = 1.0f);
	void RenderTextString(FontSystem& fontSystem, const std::string& myString, const Vector2& startingPosition, const Rgba& textColor, const float& localTextScaleFactor = 1.0f);
	void RenderChar(const char& myChar, const Vector2& startingPosition = Vector2(50, 50), const Rgba& textColor = Rgba::WHITE, const float& localTextScaleFactor = 1.0f);
	
	void InitializeFontVAO(VertexArrayObject& myVAO);
	void InitializeQuads();

	void ClearTextMesh() {
		m_textRenderer.m_mesh->ClearMesh();
	}

	//text setters
	AABB2 CalcCharTextBox(const Vector2& cursorPos, const unsigned int& charID, const float& localTextScaleFactor = 1.0f);
	AABB2 CalcCharTextBox(FontSystem& fontSystem, const Vector2& cursorPos, const unsigned int& charID);

	void SetVAOCharTextureCoords(VertexArrayObject& myVAO, const Vector2& cursorPos, const unsigned int& charID, const Rgba& textColor = Rgba::WHITE);
	
	Vector2 SetVAOStringTextureCoords(VertexArrayObject& myVAO, const Vector2& startingPosition, const std::string& myText, 
		const Rgba& textColor = Rgba::WHITE, const unsigned int& lineSkipValue = TEXT_CHARACTERS_PER_LINE);
	
	Vector2 SetFontSystemStringTextureCoords(FontSystem& fontSystem, const Vector2& startingPosition, const std::string& myText, 
		const Rgba& textColor = Rgba::WHITE, const unsigned int& lineSkipValue = TEXT_CHARACTERS_PER_LINE);
	void SetFontSystemCharTextureCoords(FontSystem& fontSystem, Vertex3Ds& textQuads, const Vector2& cursorPos, const unsigned int& charID, 
		const Rgba& textColor = Rgba::WHITE, bool clearOtherChars = false);
	
	Vector2 SetMeshRendererStringTextureCoords(MeshRenderer& myTextRenderer, const Vector2& startingPosition, const std::string& myText, 
		const Rgba& textColor = Rgba::WHITE, const unsigned int& lineSkipValue = TEXT_CHARACTERS_PER_LINE, bool clearVerts = true, 
		float localTextScaleFactor = 1.0f);
	
	Vector2 SetDevConsoleMeshStringTextureCoords(MeshRenderer& myTextRenderer, const Vector2& startingPosition, 
		const std::string& myText, const Rgba& textColor = DEV_CONSOLE_INPUT_COLOR, 
		const unsigned int& lineSkipValue = TEXT_CHARACTERS_PER_LINE, bool clearVerts = false);
	
	void SetMeshRendererCharTextureCoords(Vertex3Ds& textQuads, const Vector2& cursorPos, const unsigned int& charID,
		const Rgba& textColor = Rgba::WHITE, bool clearOtherChars = false, 
		float localTextScaleFactor = 1.0f);

	AABB2 GetTextureCoordinatesForChar(const char& charID);

	AABB2 GetTextureCoordinatesForChar(FontSystem& fontSystem, const char& charID);

	//render methods
	void RenderDevConsole();
	void RenderDevConsole(VertexArrayObject& myVAO);
	
	//render helpers
	void RenderTextMesh2D(MeshRenderer& myTextRenderer);
	void RenderText(VertexArrayObject& myVAO);
	void RenderCursorQuad(VertexArrayObject& myVAO);
	void RenderCursorQuad(MeshRenderer& myCursorRenderer);
	void RenderDevConsoleBackground(VertexArrayObject& myVAO);
	void RenderDevConsoleBackground(MeshRenderer& backgroundRenderer);
	//-----------------------------------------------------------------------------------------------------------
	//console command helpers
	void ProcessConsoleCommands(std::string& bufferText);

	void ExecuteConsoleString(std::string bufferText);

	void ClearCommand(std::string& bufferText);
	
	void ProcessCommandLine(std::string& commandLineBuffer);
	void ProcessCommandBuffer(const std::string& commandBuffer);
	///----------------------------------------------------------------------------------------------------------
	///event helpers

	void EventRenderText(NamedProperties& params) {
		std::string textStr = "";
		params.Get("text", textStr);

		Rgba textCol = Rgba::WHITE;
		params.Get("textColor", textCol);

		Vector2 textPos;
		params.Get("textPosition", textPos);

		float textScale;
		PropertyGetResult resultScale = params.Get("textScale", textScale);
		if (resultScale != RESULT_SUCCESS) {
			textScale = 1.0f;
		}
// 		std::string textFont = "";
// 		params.Get("textFont", textFont);
// 		if (textFont != "") {
// 			FontSystem tempFont;
// 			LoadFontSystem(tempFont, textFont);
// 			tempFont.InitializeFontMeshRenderer();
// 			RenderTextString(tempFont, textStr, textPos, textCol);
// 		}
// 		else {
//			RenderTextString(textStr, textPos, textCol);
//		}

		RenderTextString(textStr, textPos, textCol, textScale);

	}

	///----------------------------------------------------------------------------------------------------------
	///friend methods

	friend void InitializeTextSystem(TextSystem*& textSystemObject, const std::string& bitmapFontFilePath, OpenGLRenderer* renderer);


};

extern TextSystem* theTextSystem;

//===========================================================================================================

inline void TextSystem::ToggleDevelopmentConsole(){
	if (theInputSystem){
		if (theInputSystem->WasKeyJustReleased(KEY_TILDE)){
			m_IsDevelopmentConsoleActive = !m_IsDevelopmentConsoleActive;
		}

		if (m_IsDevelopmentConsoleActive){
			theInputSystem->ShowMouseCursor();
		}
		else{
			theInputSystem->HideMouseCursor();
		}
	}
}

//-----------------------------------------------------------------------------------------------------------

inline float TextSystem::CalcTextWidth(const std::string& textToCalc){

	return m_fontSystem.CalcTextWidth(textToCalc);
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///global helpers

void OutputStringToConsole(const std::string& name, const unsigned int& lineLength);

//-----------------------------------------------------------------------------------------------------------


//===========================================================================================================

#endif