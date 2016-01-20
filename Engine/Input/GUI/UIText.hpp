//==============================================================================================================
//UIText.hpp
//by Albert Chen Sep-8-2015.
//==============================================================================================================

#pragma once

#ifndef _included_UIText__
#define _included_UIText__

#include "UIElement.hpp"

//===========================================================================================================

class FontSystem;

//===========================================================================================================

class UIText : public UIElement {
public:
	std::string m_text;
	bool m_isToolTip;

	//methods

	//constructors
	UIText();
	UIText(const Vector2& screenPosition, const std::string& textToSet, bool isToolTip = false);

	void SetBitmapFontTexture(const FontSystem& fontSystem);
	//setters
	void SetText(const std::string& newText);
	void SetUITextSize();
	void SetUITextRenderBounds();
	
	//render overloads
	void RenderMesh(OpenGLRenderer* renderer);
	void RenderText(OpenGLRenderer* renderer);

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline UIText::UIText(){
	//do nothing
}



//===========================================================================================================

#endif