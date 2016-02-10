//==============================================================================================================
//TextSystem.cpp
//by Albert Chen Jun-16-2015.
//==============================================================================================================

#include "Engine\Renderer\Text\TextSystem.hpp"
#include "Engine\Core\Time.hpp"
#include "Engine\Console\DevConsole.hpp"
#include "Engine\Math\MathCommands.hpp"
#include "Engine\Core\BinaryUtils.hpp"
#include "..\OpenGLRenderer.hpp"

TextSystem* theTextSystem = NULL;

static Vector2 cursorPosition = DEV_CONSOLE_CURSOR_START_POSITION;

//default commands

CONSOLE_COMMAND(messageBox){
	std::string argText;
	if (COMMAND_HAS_ARGS){
		argText += "==========Arg Tokens==========\n";
		//const unsigned int argTextLineSkipValue = 500;
		for (int i = 0; i < NUMBER_OF_ARG_TOKENS; i++){
			argText += "ArgToken[" + IntToString(i) + "] = " + ARG_TOKENS[i] + "\n";
		}
		ConsoleGenericMessageBox(argText, "ConsoleCommand: Message Box");
	}


	//theTextSystem->SetVAOStringTextureCoords(theTextSystem->m_textVAO, DEV_CONSOLE_OUTPUT_POSITION, argText, DEV_CONSOLE_OUTPUT_COLOR, argTextLineSkipValue);
}

CONSOLE_COMMAND(printArgs){
	std::string argText;

	argText += "==========Arg Tokens==========\n";
	const unsigned int argTextLineSkipValue = 500;
	for (int i = 0; i < NUMBER_OF_ARG_TOKENS; i++){
		argText += "ArgToken[" + IntToString(i) + "] = " + ARG_TOKENS[i] +"\n";
	}

	theTextSystem->SetVAOStringTextureCoords(theTextSystem->m_textIn, DEV_CONSOLE_OUTPUT_POSITION, argText, DEV_CONSOLE_OUTPUT_COLOR, argTextLineSkipValue);
}

CONSOLE_COMMAND(help){
	UNUSED_COMMAND_ARGS
	
 	const unsigned int helpTextLineSkipValue = 9999;
	
	OUTPUT_STRING_TO_CONSOLE("=====HELP=====\n"+ConsoleCommand::g_helpText, helpTextLineSkipValue);
	//theTextSystem->SetVAOStringTextureCoords(theTextSystem->m_textIn, DEV_CONSOLE_OUTPUT_POSITION, helpText, DEV_CONSOLE_OUTPUT_COLOR, helpTextLineSkipValue);
}

CONSOLE_COMMAND(clear){
	UNUSED_COMMAND_ARGS
	theTextSystem->m_textIn.m_vertexArray.clear();
}

CONSOLE_COMMAND(quit){
	UNUSED_COMMAND_ARGS
	 	if (theInputSystem){
	 		theInputSystem->SetSystemQuit();
	 	}
}

CONSOLE_COMMAND(logConsole){
	UNUSED_COMMAND_ARGS
	std::string consoleLogText = "";
	for (int i = 0; i < (int)theTextSystem->m_textHistory.size(); i++){
		consoleLogText += theTextSystem->m_textHistory[i] + "\n";
	}
	//consoleLogText += theTextSystem->textInputBuffer+"\n";

	WriteExistingBufferToTextFile(consoleLogText , "DevConsoleLog.txt");
}

CONSOLE_COMMAND(calcTextWidth){
	if (COMMAND_HAS_ARGS){
		std::string argText = "";
		for (int i = 0; i < NUMBER_OF_ARG_TOKENS; i++){
			argText += ARG_TOKEN_S(i) + " ";
		}
		

		float textWidthResult = 0.0f;

		if (theTextSystem){
			textWidthResult = theTextSystem->m_fontSystem.CalcTextWidth(argText);
			
			std::string textWidthMessage = "The Argument:\n" + argText + "\n rendered using font: " +
				theTextSystem->m_fontSystem.m_FontSummary.m_infoFace + "\n"+
				" has a text width of " + FloatToString(textWidthResult) + " pixels\n";

			ConsoleGenericMessageBox(textWidthMessage);
		}//end of text system
	}
}

CONSOLE_COMMAND(profile) {
	UNUSED_COMMAND_ARGS

	ProfileSection::s_doDebugProfiling = !ProfileSection::s_doDebugProfiling;
	if (ProfileSection::s_doDebugProfiling) {
		OUTPUT_STRING_TO_CONSOLE("Debug Profiling ON.", 1000);
	}
	else {
		OUTPUT_STRING_TO_CONSOLE("Debug Profiling OFF.", 1000);
	}
	
}

///----------------------------------------------------------------------------------------------------------
///constructor
TextSystem::TextSystem():m_IsDevelopmentConsoleActive(false){

	RegisterDefaultCommands();

	m_mathCommands = MathCommands();

	textInputBuffer = "";
	
	if (!theTextSystem){
		theTextSystem = this;
	}

	m_textCamera = Camera3D(Vector3::ZERO, EulerAngles(0.0f, 0.0f, 0.0f));

}

//===========================================================================================================

//starts up with a default font texture
void TextSystem::StartUp(const std::string& defaultFontFilePath, OpenGLRenderer* renderer){
	LoadFontSystem(defaultFontFilePath);

	m_OGLRenderer = renderer;

	InitializeQuads();
	InitializeFontVAO(m_textIn);
	InitializeFontVAO(m_textOut);
	InitializeFontMeshRenderer();
}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::ShutDown(){
	
	m_fontSystem.ShutDown();

}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::RegisterDefaultCommands(){
	//registering commands
	REGISTER_CONSOLE_COMMAND(help, "Displays Console Commands.");
	REGISTER_CONSOLE_COMMAND(clear, "Clears Console.");
	REGISTER_CONSOLE_COMMAND(quit, "Quit Application.");
	REGISTER_CONSOLE_COMMAND(logConsole, "Outputs Console To DevConsoleLog.txt");
	REGISTER_CONSOLE_COMMAND(printArgs, "Print Console Args to Output.");
	REGISTER_CONSOLE_COMMAND(messageBox, "Creates a MessageBox for the Console Args.");
	REGISTER_CONSOLE_COMMAND(calcTextWidth, "Calc the text width with the current font.");
	REGISTER_CONSOLE_COMMAND(profile, "Toggles debug profiling.");
}

//===========================================================================================================

void TextSystem::LoadFontSystem(const std::string& fontFilePath){
	m_fontSystem.LoadFontDataToBuffer(fontFilePath);
	m_fontSystem.LoadFontSystemFromBuffer();
}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::LoadFontSystem(FontSystem& fontSystem, const std::string& fontFilePath){
	fontSystem.LoadFontDataToBuffer(fontFilePath);
	fontSystem.LoadFontSystemFromBuffer();
}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::InitializeFontMeshRenderer(){
	//m_textRenderer = MeshRenderer();
	//m_textRenderer.m_material = new Material();
	//m_textRenderer.m_mesh = new Mesh();

	std::string texturePath = "Data/Fonts/" + m_fontSystem.m_FontSummary.m_fileName;

	//m_textRenderer.m_material->m_glSampler.SetTexture(texturePath);
	m_textRenderer.m_material->SetTextureInMap("gTexture", texturePath);
	m_textRenderer.m_mesh->SetDrawMode(GL_QUADS);
	m_textRenderer.m_material->InitializeMaterial( "Data/Shaders/basicSampler.vert", "Data/Shaders/basicSampler.frag"); //load prog

	m_textRenderer.BindVertexArray();
}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::InitializeFontMeshRenderer(FontSystem& fontSystem){
	m_textRenderer = MeshRenderer();
	m_textRenderer.m_material = new Material();
	m_textRenderer.m_mesh = new Mesh();

	std::string texturePath = "Data/Fonts/" + fontSystem.m_FontSummary.m_fileName;

	//m_textRenderer.m_material->m_glSampler.SetTexture(texturePath);
	m_textRenderer.m_material->SetTextureInMap("gTexture", texturePath);
	m_textRenderer.m_mesh->SetDrawMode(GL_QUADS);
	m_textRenderer.m_material->InitializeMaterial("Data/Shaders/basicSampler.vert", "Data/Shaders/basicSampler.frag"); //load prog

	m_textRenderer.BindVertexArray();
}


//===========================================================================================================

void TextSystem::InitializeFontVAO(VertexArrayObject& myVAO){
	std::string texturePath = "Data/Fonts/" + m_fontSystem.m_FontSummary.m_fileName;

	myVAO.m_glSampler.SetTextureInMap("gTexture",texturePath);
	myVAO.SetDrawMode(GL_QUADS);
	m_OGLRenderer->InitializeVAO(myVAO, "Data/Shaders/basicSampler.vert", "Data/Shaders/basicSampler.frag"); //load prog
	m_OGLRenderer->CreateVAOSampler(myVAO, GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);
	
}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::InitializeQuads(){
	static const AABB2 BACKGROUND_QUAD(Vector2::ZERO, Vector2(m_OGLRenderer->GetDisplayWidth(), m_OGLRenderer->GetDisplayHeight()));
	m_textBackgroundQuads.SetDrawMode(GL_QUADS);
	m_OGLRenderer->GenerateVertexArrayAABB2(m_textBackgroundQuads.m_vertexArray, BACKGROUND_QUAD, DEV_CONSOLE_BACKGROUND_COLOR);
	m_OGLRenderer->InitializeVAO(m_textBackgroundQuads, "Data/Shaders/basic.vert", "Data/Shaders/basic.frag");
	
	m_textCursor.SetDrawMode(GL_QUADS);
	m_OGLRenderer->InitializeVAO(m_textCursor, "Data/Shaders/basic.vert", "Data/Shaders/basic.frag");
}

//===========================================================================================================

void TextSystem::Update(double deltaSeconds){
	UNUSED(deltaSeconds);

	ToggleDevelopmentConsole();
	if (m_IsDevelopmentConsoleActive){

		InputTextToBuffer(textInputBuffer);

		ProcessDevConsoleHotkeys(textInputBuffer);
	}
}

//-----------------------------------------------------------------------------------------------------------

//does not use VAO at all
std::string TextSystem::InputTextToBuffer(std::string& bufferText){
		//text input
		if (theInputSystem){
			unsigned int keyPressedID = theInputSystem->GetLatestKeyPressed();

			if (!IsKeyPressedDevConsoleHotkey(keyPressedID)){
				//do something to prevent serious repeat chars
				if (keyPressedID != 0 ){
					bufferText += (unsigned char)keyPressedID;
				}
				return bufferText;
			}
		}
	return "";
}

//-----------------------------------------------------------------------------------------------------------

bool TextSystem::IsKeyPressedDevConsoleHotkey(const unsigned int& keyPressedID ){
	if (keyPressedID == KEY_ENTER){
		return true;
	}
	if (keyPressedID == KEY_BACKSPACE){
		return true;
	}
	if (keyPressedID == KEY_CTRL){
		return true;
	}
	if (keyPressedID == KEY_PAGE_UP){
		return true;
	}
	if (keyPressedID == KEY_PAGE_DOWN){
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::ProcessDevConsoleHotkeys(std::string& bufferText){
	if (theInputSystem){
		
		//UP, DOWN to cycle text history
		ProcessTextHistoryHotkeys(bufferText);

		//press ENTER to use command
		ConfirmDevConsoleCommand(bufferText);

		//press BACKSPACE to delete chars
		DeleteDevConsoleChar(bufferText);
		
		//Use Mouse Wheel to Scroll
		ProcessDevConsoleScrolling(bufferText);

	}//end of is inputSystem

}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///dev console hotkey helpers

//-----------------------------------------------------------------------------------------------------------

void TextSystem::ConfirmDevConsoleCommand(std::string& bufferText) {
	//skip a line with enter
	if (theInputSystem->WasKeyJustReleased(KEY_ENTER)) {
		m_textHistory.push_back(bufferText);
		ProcessConsoleCommands(bufferText); //may use vao
	}
}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::ProcessTextHistoryHotkeys(std::string& bufferText) {
	static unsigned int textHistoryOffset = 1;
	//can get last command, but not before that, well the correct stuff is in the buffer but I can't get it to display
	if (theInputSystem->WasKeyJustReleased(KEY_ARROW_UP)) {
		bufferText = "";
		//textHistoryOffset++;
		unsigned int indexToScan = m_textHistory.size() - textHistoryOffset;
		bufferText = m_textHistory[indexToScan];

		if (textHistoryOffset < m_textHistory.size())textHistoryOffset++;
	}
	if (theInputSystem->WasKeyJustReleased(KEY_ARROW_DOWN)) {
		bufferText = "";

		unsigned int indexToScan = m_textHistory.size() - textHistoryOffset;
		bufferText = m_textHistory[indexToScan];
		if (textHistoryOffset > 0)textHistoryOffset--;
	}

}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::DeleteDevConsoleChar(std::string& bufferText) {
	//deleting chars
	if (theInputSystem->WasKeyJustReleased(KEY_BACKSPACE)) {
		DeleteDevConsoleChar(m_textIn, bufferText); //uses textIn
	}
}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::ProcessDevConsoleScrolling(std::string& bufferText){
	static int consoleScrollAmount;
	if (theInputSystem->IsKeyDown(KEY_CTRL)){
		consoleScrollAmount = 10;
	}
	else{
		consoleScrollAmount = 1;
	}
	
	//scrolling through stuff, sorta works but likely causes extra mem allocated
	if (theInputSystem->IsMouseWheelPresent()){
		if (!theInputSystem->m_isMouseWheelStopped){
			if (theInputSystem->IsMouseWheelScrollingUp()){
				DevConsoleScrollUp(bufferText, consoleScrollAmount);
			}
			else{
				DevConsoleScrollDown(bufferText, consoleScrollAmount);
			}//end of if/else
		}//end of is stopped
	}//end of mouse wheel

	if (theInputSystem->WasKeyJustReleased(KEY_PAGE_UP)){
		DevConsoleScrollUp(bufferText, consoleScrollAmount);
	}
	else if (theInputSystem->WasKeyJustReleased(KEY_PAGE_DOWN)){
		DevConsoleScrollDown(bufferText, consoleScrollAmount);
	}

	//std::string scrollLV = "scroll level: " + FloatToString(DEV_CONSOLE_SCROLL_LEVEL) + "\n";
	//ConsolePrintf(scrollLV.c_str());

}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::ResetDevConsoleScrollHeight(){
	//for (int i = 0; i < (int)myVAO.m_vertexArray.size()/*textInputBuffer.length()*/; i++){
}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::DevConsoleScrollUp(std::string& bufferText, const int& scrollAmount){
	for (int i = 0; i < scrollAmount; i++){
		SkipConsoleLine(m_textIn, true); //uses VAO
		bufferText = "";
		//SetVAOStringTextureCoords(m_textVAO, DEV_CONSOLE_CURSOR_START_POSITION, bufferText, DEV_CONSOLE_INPUT_COLOR);
	}

}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::DevConsoleScrollDown(std::string& bufferText, const int& scrollAmount){
	for (int i = 0; i < scrollAmount; i++){
		SkipConsoleLine(m_textIn, false); //uses vao
		bufferText = "";
		//SetVAOStringTextureCoords(m_textVAO, DEV_CONSOLE_CURSOR_START_POSITION, bufferText, DEV_CONSOLE_INPUT_COLOR);
	}
}

//-----------------------------------------------------------------------------------------------------------

static int indexOfCurrentTextBuffer = 0;

void TextSystem::DeleteDevConsoleChar(VertexArrayObject& myVAO, std::string& bufferText){
	UNUSED(myVAO);

	//for (int i = indexOfCurrentTextBuffer; i < (int)m_textVAO.m_vertexArray.size(); i++){

	if (!m_textIn.m_vertexArray.empty() && m_textIn.m_vertexArray.size() > 4 ){
		m_textIn.m_vertexArray.pop_back();
		m_textIn.m_vertexArray.pop_back();
		m_textIn.m_vertexArray.pop_back();
		m_textIn.m_vertexArray.pop_back();
	}
		if (bufferText.length() > 0){
			unsigned int charBeingDeleted = bufferText[bufferText.length() - 1];

			float charAdvance = (float)m_fontSystem.GetCharXAdvance(charBeingDeleted);
			cursorPosition.x -= charAdvance * TEXT_SCALE_FACTOR;

			bufferText.resize(bufferText.size() - 1);

			indexOfCurrentTextBuffer -= 4;
		}


}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::SkipConsoleLine(VertexArrayObject& myVAO, bool skipDown){
	//skip everything down a line
	SkipConsoleLine(myVAO.m_vertexArray, skipDown);
}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::SkipConsoleLine(Vertex3Ds& in_vertexArray, bool skipDown) {
	//skip everything down a line
	for (int i = 0; i < (int)in_vertexArray.size()/*textInputBuffer.length()*/; i++) {
		if (skipDown) {
			DEV_CONSOLE_SCROLL_LEVEL -= DEV_CONSOLE_LINE_SPACING;
			float Yoffset = in_vertexArray[i].m_position.y - DEV_CONSOLE_LINE_SPACING;

			if (in_vertexArray[i].m_position.y > Yoffset)//using const values for this is neat
				in_vertexArray[i].m_position.y -= DEV_CONSOLE_LINE_SPACING;

		}
		else {
			DEV_CONSOLE_SCROLL_LEVEL += DEV_CONSOLE_LINE_SPACING;
			float Yoffset = in_vertexArray[i].m_position.y + DEV_CONSOLE_LINE_SPACING;

			if (in_vertexArray[i].m_position.y < Yoffset)//using const values for this is neat
				in_vertexArray[i].m_position.y += DEV_CONSOLE_LINE_SPACING;

		}//end of if/else

	}
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///create text Vertex array

AABB2 TextSystem::CalcCharTextBox(const Vector2& cursorPos, const unsigned int& charID){
	AABB2 textBox;
	textBox.mins = cursorPos;
	//apply char offset
	textBox.mins += m_fontSystem.GetCharOffset(charID) * TEXT_SCALE_FACTOR;

	Vector2 charGlyphSize = m_fontSystem.GetCharGlyphSize(charID);

	textBox.maxs = textBox.mins + Vector2(TEXT_SCALE_FACTOR * charGlyphSize.x, TEXT_SCALE_FACTOR * charGlyphSize.y);

	return textBox;
}

//-----------------------------------------------------------------------------------------------------------

AABB2 TextSystem::CalcCharTextBox(FontSystem& fontSystem, const Vector2& cursorPos, const unsigned int& charID){
	AABB2 textBox;
	textBox.mins = cursorPos;
	//apply char offset
	textBox.mins += fontSystem.GetCharOffset(charID) * TEXT_SCALE_FACTOR;

	Vector2 charGlyphSize = fontSystem.GetCharGlyphSize(charID);

	textBox.maxs = textBox.mins + Vector2(TEXT_SCALE_FACTOR * charGlyphSize.x, TEXT_SCALE_FACTOR * charGlyphSize.y);

	return textBox;
}


//-----------------------------------------------------------------------------------------------------------

void TextSystem::SetVAOCharTextureCoords(VertexArrayObject& myVAO, const Vector2& cursorPos, const unsigned int& charID, const Rgba& textColor){
	AABB2 charTextureCoords = m_fontSystem.GetCharTextureCoords(charID);
	AABB2 textBox = CalcCharTextBox(cursorPos, charID);
	
	m_OGLRenderer->GenerateVertexArrayTextureQuad(myVAO.m_vertexArray, textBox, charTextureCoords, textColor);
}

//-----------------------------------------------------------------------------------------------------------

///----------------------------------------------------------------------------------------------------------
///define starting position as bottom left corner and then it draws to the right
Vector2 TextSystem::SetVAOStringTextureCoords(VertexArrayObject& myVAO, const Vector2& startingPosition, const std::string& myText, const Rgba& textColor, const unsigned int& lineSkipValue ){

	float startPointMinsX = startingPosition.x;

	static std::string prevText = "";

	if (startingPosition == DEV_CONSOLE_OUTPUT_POSITION){
		cursorPosition = DEV_CONSOLE_OUTPUT_POSITION;
		prevText = "";
	}
	
	if (myText != prevText){
		for (int i = prevText.length(); i < (int)myText.length(); i++){
			unsigned int nextCharID = (unsigned int)myText[i];
			SetVAOCharTextureCoords(myVAO, cursorPosition, nextCharID, textColor);
			indexOfCurrentTextBuffer += 4;
			float nextCharAdvance = (float)m_fontSystem.GetCharXAdvance(nextCharID);
			cursorPosition.x += nextCharAdvance * TEXT_SCALE_FACTOR;
			//skip line here
			if (i % lineSkipValue == 0 && i > 0.0f){
				cursorPosition.x = startPointMinsX;
				cursorPosition.y -= DEV_CONSOLE_LINE_SPACING;
			}
			else if ((unsigned char)nextCharID == '\n'){
				cursorPosition.x = startPointMinsX;
				cursorPosition.y -= DEV_CONSOLE_LINE_SPACING;
			}
		}//end of for

		CURSOR_QUAD.mins = cursorPosition;
		CURSOR_QUAD.maxs.x = CURSOR_QUAD.mins.x + DEV_CONSOLE_CURSOR_WIDTH;

	}

	prevText = myText;

	myVAO.m_vboID = m_OGLRenderer->CreateBuffer(myVAO.m_vboID, myVAO.m_vertexArray.data(), myVAO.CalcVAOSize());
	m_OGLRenderer->CreateVertexArrayWithShader(myVAO.m_Program, myVAO.m_vboID, myVAO.m_vaoID);

	return cursorPosition;
}

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///mesh renderer methods

Vector2 TextSystem::SetFontSystemStringTextureCoords(FontSystem& fontSystem, const Vector2& startingPosition, const std::string& myText, const Rgba& textColor, const unsigned int& lineSkipValue){
	UNUSED(lineSkipValue);

	float startPointMinsX = startingPosition.x;

	MeshRenderer& myTextRenderer = *fontSystem.m_fontRenderer;

	static Vector2 tempCursorPos = Vector2::ZERO;
	tempCursorPos = startingPosition;

	Vertex3Ds textQuads;
	textQuads.clear();
	//textQuads.reserve(myText.size() * 4);

	for (int i = 0; i < (int)myText.length(); i++){
		unsigned int nextCharID = (unsigned int)myText[i];
		SetFontSystemCharTextureCoords(fontSystem,textQuads, tempCursorPos, nextCharID, textColor);
		indexOfCurrentTextBuffer += 4;
		float nextCharAdvance = (float)fontSystem.GetCharXAdvance(nextCharID) + fontSystem.GetCharOffset(nextCharID).x;
		tempCursorPos.x += nextCharAdvance * TEXT_SCALE_FACTOR;
		if ((unsigned char)nextCharID == '\n'){
			tempCursorPos.x = startPointMinsX;
			tempCursorPos.y -= DEV_CONSOLE_LINE_SPACING;
		}
	}//end of for

	myTextRenderer.m_mesh->CopyMeshVertexData(textQuads);


	return tempCursorPos;
}

//-----------------------------------------------------------------------------------------------------------


void TextSystem::SetFontSystemCharTextureCoords(FontSystem& fontSystem, Vertex3Ds& textQuads, const Vector2& cursorPos, const unsigned int& charID, const Rgba& textColor, bool clearOtherChars){

	AABB2 charTextureCoords = fontSystem.GetCharTextureCoords(charID);

	AABB2 textBox = CalcCharTextBox(cursorPos, charID);

	if (clearOtherChars)
		textQuads.clear();

	m_OGLRenderer->GenerateVertexArrayTextureQuad(textQuads, textBox, charTextureCoords, textColor);
}


//===========================================================================================================

Vector2 TextSystem::SetMeshRendererStringTextureCoords(MeshRenderer& myTextRenderer, const Vector2& startingPosition, const std::string& myText, const Rgba& textColor, const unsigned int& lineSkipValue ){
	UNUSED(lineSkipValue);
	
	float startPointMinsX = startingPosition.x;
	
	static Vector2 tempCursorPos = Vector2::ZERO;
	tempCursorPos = startingPosition;

	Vertex3Ds textQuads;
	textQuads.clear();
	//textQuads.reserve(myText.size() * 4);

		for (int i = 0; i < (int)myText.length(); i++){
			unsigned int nextCharID = (unsigned int)myText[i];
			SetMeshRendererCharTextureCoords(textQuads, tempCursorPos, nextCharID, textColor);
			indexOfCurrentTextBuffer += 4;
			float nextCharAdvance = (float)m_fontSystem.GetCharXAdvance(nextCharID) + m_fontSystem.GetCharOffset(nextCharID).x;
			tempCursorPos.x += nextCharAdvance * TEXT_SCALE_FACTOR;
			if ((unsigned char)nextCharID == '\n'){
				tempCursorPos.x = startPointMinsX;
				tempCursorPos.y -= DEV_CONSOLE_LINE_SPACING;
			}
		}//end of for
	
	myTextRenderer.m_mesh->CopyMeshVertexData(textQuads);


	return tempCursorPos;
}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::SetMeshRendererCharTextureCoords(Vertex3Ds& textQuads, const Vector2& cursorPos, const unsigned int& charID, const Rgba& textColor, bool clearOtherChars){

	AABB2 charTextureCoords = m_fontSystem.GetCharTextureCoords(charID);

	AABB2 textBox = CalcCharTextBox(cursorPos, charID);

	if (clearOtherChars)
		textQuads.clear();

	m_OGLRenderer->GenerateVertexArrayTextureQuad(textQuads, textBox, charTextureCoords, textColor);
}

//-----------------------------------------------------------------------------------------------------------

AABB2 TextSystem::GetTextureCoordinatesForChar(const char& charID ){

	return m_fontSystem.GetCharTextureCoords((unsigned int)charID);

}

//-----------------------------------------------------------------------------------------------------------

AABB2 TextSystem::GetTextureCoordinatesForChar(FontSystem& fontSystem, const char& charID){

	return fontSystem.GetCharTextureCoords((unsigned int)charID);

}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::RenderTextString(const std::string& myString){
	m_OGLRenderer->SetTextureViewTransparent();
	//m_OGLRenderer->DisableModelViewDepthTest();

	SetMeshRendererStringTextureCoords(m_textRenderer, Vector2(50.0f, 50.0f), myString, Rgba::WHITE, 10000);
	//m_textRenderer.BindVertexArray();
	RenderTextMesh2D(m_textRenderer);
}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::RenderTextString(const std::string& myString, const Vector2& startingPosition, const Rgba& textColor, const float& localTextScaleFactor ){
	UNUSED(localTextScaleFactor);
	//m_OGLRenderer->SetTextureViewTransparent();
	m_OGLRenderer->DisableModelViewDepthTest();
	SetMeshRendererStringTextureCoords(m_textRenderer, startingPosition, myString, textColor, 10000);
	//m_textRenderer.BindVertexArray();

	RenderTextMesh2D(m_textRenderer);
}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::RenderTextString(FontSystem& fontSystem, const std::string& myString, const Vector2& startingPosition, const Rgba& textColor, const float& localTextScaleFactor){
	UNUSED(localTextScaleFactor);

	SetFontSystemStringTextureCoords(fontSystem, startingPosition, myString, textColor, 10000);
	theTextSystem->RenderTextMesh2D(*fontSystem.m_fontRenderer);
}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::RenderChar(const char& myChar, const Vector2& startingPosition, const Rgba& textColor, const float& localTextScaleFactor){
	UNUSED(localTextScaleFactor);
	//m_OGLRenderer->SetTextureViewTransparent();
	Vertex3Ds charQuad;
	SetMeshRendererCharTextureCoords(charQuad, startingPosition, myChar, textColor, true);

	m_textRenderer.m_mesh->CopyMeshVertexData(charQuad);
	//m_textRenderer.BindVertexArray();

	RenderTextMesh2D(m_textRenderer);
}

//===========================================================================================================

void TextSystem::Render(){
		
		Render(m_textIn);

}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::Render(VertexArrayObject& myVAO){
	if (m_IsDevelopmentConsoleActive){
		m_OGLRenderer->DisableModelViewDepthTest();
		m_OGLRenderer->DisableModelCullFaceMode();
		//m_OGLRenderer->SetTextureViewTransparent();

		//static std::string prevTextBuffer = "";

		currentCursorPos = DEV_CONSOLE_CURSOR_START_POSITION;
		currentCursorPos = SetVAOStringTextureCoords(myVAO, DEV_CONSOLE_CURSOR_START_POSITION, textInputBuffer, DEV_CONSOLE_INPUT_COLOR);
		
		//SetVAOStringTextureCoords(m_textOut, DEV_CONSOLE_OUTPUT_POSITION, textOutputBuffer, DEV_CONSOLE_OUTPUT_COLOR);

		RenderDevConsoleBackground(m_textBackgroundQuads);

		RenderCursorQuad(m_textCursor);
		
		RenderText(m_textIn);
		
		//RenderText(m_textOut);
	}
}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================

void TextSystem::RenderText(VertexArrayObject& myVAO){
	//render text
	GLuint& programToRenderWith = myVAO.m_Program;
	m_OGLRenderer->SetShaderProgramToUse(programToRenderWith);
	
	m_OGLRenderer->BindViewMatricesToProgram(programToRenderWith, IDENTITY_MATRIX, m_OGLRenderer->MakeDefaultOrthographicProjectionMatrix());
	m_OGLRenderer->ProgramBindFloat(programToRenderWith, "gTime", (float)GetCurrentSeconds());
	
// 	GLuint texIndex = 0;
// 	m_OGLRenderer->ProgramBindSamplerIndex(programToRenderWith, "gTexture", texIndex);
// 	glActiveTexture(GL_TEXTURE0 + texIndex);
// 
// 	glEnable(GL_TEXTURE_2D);
// 
// 	glBindTexture(GL_TEXTURE_2D, myVAO.m_glSampler.m_texture->GetPlatformHandle());
// 	glBindSampler(texIndex, myVAO.m_samplerID);

	myVAO.m_glSampler.BindTextureMapToShader(programToRenderWith);

	m_OGLRenderer->DrawVertexArray(myVAO.m_drawMode, myVAO.m_vaoID, myVAO.m_vertexArray.size());

	glDisable(GL_TEXTURE_2D);

	m_OGLRenderer->DisableShaderProgram();
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_OGLRenderer->DestroyVAO(myVAO);
}

//===========================================================================================================

void TextSystem::RenderTextMesh2D(MeshRenderer& myTextRenderer){
	if (theOGLRenderer)
		theOGLRenderer->SetTextureViewTransparent();

	myTextRenderer.RenderMesh2D();// (m_textCamera, false);
}

///----------------------------------------------------------------------------------------------------------
///render helpers

void TextSystem::RenderCursorQuad(VertexArrayObject& myVAO){
	static float cursorAlphaBlinkValue = 0.0f;
	const float alphaBlinkBlendValA = 80.0f;
	cursorAlphaBlinkValue = alphaBlinkBlendValA + abs( (255.0f -alphaBlinkBlendValA) * sin((float)GetCurrentSeconds() * 12.0f) );
	//std::string cursorBlinkText = "\nAlpha Val = " + FloatToString(cursorAlphaBlinkValue);
	//ConsolePrintf(cursorBlinkText.c_str());
	DEV_CONSOLE_CURSOR_COLOR.a = (unsigned char)(cursorAlphaBlinkValue);// *255.0f);

	//render cursor
	myVAO.m_vertexArray.clear();
	m_OGLRenderer->GenerateVertexArrayAABB2(myVAO.m_vertexArray, CURSOR_QUAD, DEV_CONSOLE_CURSOR_COLOR);

	myVAO.m_vboID = m_OGLRenderer->CreateBuffer(myVAO.m_vboID, myVAO.m_vertexArray.data(), myVAO.CalcVAOSize());
	m_OGLRenderer->CreateVertexArrayWithShader(myVAO.m_Program, myVAO.m_vboID, myVAO.m_vaoID);

	GLuint& programToRenderWithC = myVAO.m_Program;
	m_OGLRenderer->SetShaderProgramToUse(programToRenderWithC);

	m_OGLRenderer->BindViewMatricesToProgram(programToRenderWithC, IDENTITY_MATRIX, m_OGLRenderer->MakeDefaultOrthographicProjectionMatrix());
	m_OGLRenderer->ProgramBindFloat(programToRenderWithC, "gTime", (float)GetCurrentSeconds());

	m_OGLRenderer->DrawVertexArray(myVAO.m_drawMode, myVAO.m_vaoID, myVAO.m_vertexArray.size());

	m_OGLRenderer->DisableShaderProgram();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_OGLRenderer->DestroyVAO(myVAO);
}

//-----------------------------------------------------------------------------------------------------------

//render BG quad
void TextSystem::RenderDevConsoleBackground(VertexArrayObject& myVAO){
	GLuint& programToRenderWithT = myVAO.m_Program;
	m_OGLRenderer->SetShaderProgramToUse(programToRenderWithT);

	m_OGLRenderer->BindViewMatricesToProgram(programToRenderWithT, IDENTITY_MATRIX, m_OGLRenderer->MakeDefaultOrthographicProjectionMatrix());
	m_OGLRenderer->ProgramBindFloat(programToRenderWithT, "gTime", (float)GetCurrentSeconds());

	m_OGLRenderer->DrawVertexArray(myVAO.m_drawMode, myVAO.m_vaoID, myVAO.m_vertexArray.size());

	m_OGLRenderer->DisableShaderProgram();
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

//===========================================================================================================
//console commands

void TextSystem::ProcessConsoleCommands(std::string& bufferText){

	AdjustBufferTextForExecution(bufferText);
	if (bufferText != ""){

		char* cStringBufferText = StringToWritableCString(bufferText);
		char* currentValue;
		char* tokenInBuffer;
		currentValue = strtok_s(cStringBufferText, " ", &tokenInBuffer);
		//ConsoleGenericMessageBox(currentValue, "loading token, current value = ");
		std::string commandName = currentValue;

		currentValue = strtok_s(NULL, "\0", &tokenInBuffer);
		//ConsoleGenericMessageBox(currentValue, "loading token, current value = ");
		if (currentValue != NULL){
			RunCommand(commandName, new ConsoleArguments(currentValue));
		}
		else{
			RunCommand(bufferText, NULL);
		}
	}

	cursorPosition = DEV_CONSOLE_CURSOR_START_POSITION;

	DevConsoleScrollDown(bufferText);
}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::ExecuteConsoleString(std::string& bufferText){
	ProcessConsoleCommands(bufferText);
}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::ClearCommand(std::string& bufferText){
	unsigned int commandLength = bufferText.length();

	for (int i = 0; i < (int)(commandLength * 4); i++){
		m_textIn.m_vertexArray.pop_back();
	}

}

void TextSystem::ProcessCommandLine(std::string& commandLineBuffer){
	//assume buffer is -command arg... -command arg...
	Strings commandBuffers = SplitString(commandLineBuffer, "-");
	//assuming first thing is -command, first entry is empty string
	for (unsigned int i = 1; i < commandBuffers.size(); i++){

		ProcessCommandBuffer(commandBuffers[i]);

	}

}

//-----------------------------------------------------------------------------------------------------------

void TextSystem::ProcessCommandBuffer(const std::string& commandBuffer){
	char* cStringBufferText = StringToWritableCString(commandBuffer);
	char* currentValue;
	char* tokenInBuffer;
	currentValue = strtok_s(cStringBufferText, " ", &tokenInBuffer);
	std::string commandName = currentValue;

	currentValue = strtok_s(NULL, "\0", &tokenInBuffer);
	if (currentValue != NULL){
		RunCommand(commandName, new ConsoleArguments(currentValue));
	}
	else{
		RunCommand(commandBuffer, NULL);
	}
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///friend methods

void InitializeTextSystem(TextSystem*& textSystemObject, const std::string& bitmapFontFilePath, OpenGLRenderer* renderer){
	if (textSystemObject){
		//initialize text renderer
		textSystemObject = new TextSystem();
		textSystemObject->LoadFontSystem(bitmapFontFilePath);
		textSystemObject->m_OGLRenderer = renderer;
		textSystemObject->InitializeQuads();
		textSystemObject->InitializeFontVAO(textSystemObject->m_textIn);
		textSystemObject->InitializeFontMeshRenderer();
	}
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///global helpers

void OutputStringToConsole(const std::string& name, const unsigned int& lineLength){

	if (theTextSystem){
		theTextSystem->SkipConsoleLine(theTextSystem->m_textIn, false);
		theTextSystem->SetVAOStringTextureCoords(theTextSystem->m_textIn, DEV_CONSOLE_OUTPUT_POSITION, name, DEV_CONSOLE_OUTPUT_COLOR, lineLength);
		//theTextSystem->SkipConsoleLine(theTextSystem->m_textIn, false);
		theTextSystem->ResetCursorPosition();
	}

}

//===========================================================================================================
