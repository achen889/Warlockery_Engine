//==============================================================================================================
//UIText.cpp
//by Albert Chen Sep-8-2015.
//==============================================================================================================

#include "UIText.hpp"
#include "Engine\Renderer\Text\TextSystem.hpp"

//===========================================================================================================

//constructor with text
UIText::UIText(const Vector2& screenPosition, const std::string& textToSet, bool isToolTip ) : 
UIElement(screenPosition)
{
	m_text = textToSet;
	m_isToolTip = isToolTip;
	
	SetUITextRenderBounds();

	if (theTextSystem){
		SetBitmapFontTexture(theTextSystem->m_fontSystem);
	}
	
}

void UIText::SetBitmapFontTexture(const FontSystem& fontSystem ){

	m_elementRenderer->m_material->SetTextureInMap("gTexture", fontSystem.m_fontTexturePath);

}

///----------------------------------------------------------------------------------------------------------
///setters

void UIText::SetUITextSize(){
	float textWidth = theTextSystem->CalcTextWidth(m_text);

	float textHeight = TEXT_LINE_SPACING_FACTOR * TEXT_CHAR_SPACING_FACTOR * TEXT_SCALE_FACTOR;

	m_size = Vector2(textWidth, textHeight);
}

//-----------------------------------------------------------------------------------------------------------

void UIText::SetText(const std::string& newText){
	m_text = newText;

	SetUITextRenderBounds();
	
}

void UIText::SetUITextRenderBounds(){
	SetUITextSize();

	SetRenderBounds();

}

//-----------------------------------------------------------------------------------------------------------

void UIText::RenderMesh(OpenGLRenderer* renderer){
	//do this for tooltips
	if (m_isToolTip){
		if (OnHover()){

			renderer->SetTextureViewTransparent();

			RenderText(renderer);

		}
	}
	else{

		renderer->SetTextureViewTransparent();

		RenderText(renderer);
	}//end of if/else

}

//-----------------------------------------------------------------------------------------------------------

void UIText::RenderText(OpenGLRenderer* renderer){
	UNUSED(renderer);

	if (theTextSystem){
		theTextSystem->SetMeshRendererStringTextureCoords(*m_elementRenderer, m_screenCoordinates, m_text, m_color);
	}

	m_elementRenderer->RenderMesh2D();
}

//===========================================================================================================