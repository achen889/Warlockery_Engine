//==============================================================================================================
//Button.cpp
//by Albert Chen Sep-1-2015.
//==============================================================================================================

#include "UIButton.hpp"

#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Input/InputSystem.hpp"
#include <string>
#include "Engine/Renderer/Text/TextSystem.hpp"

//===========================================================================================================

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors

UIButton::UIButton(const Vector2& screenPosition, const Vector2& newSize, const Vector2& scaleFactor, const Rgba& elementColor) 
	: UIElement(screenPosition, newSize, scaleFactor, elementColor){

	m_buttonEventCallback = NULL;

	m_buttonText = new UIText(screenPosition, "", false);
	m_buttonTooltipText = new UIText(Vector2(screenPosition.x, screenPosition.y - TEXT_LINE_SPACING_FACTOR), "", true);

}

///----------------------------------------------------------------------------------------------------------
///accesors

void UIButton::SetText(const std::string& buttonText){
	m_buttonText->SetText(buttonText);
}

//-----------------------------------------------------------------------------------------------------------

void UIButton::SetTooltipText(const std::string& buttonText){
	m_buttonTooltipText->SetText(buttonText);
	m_buttonTooltipText->m_renderBounds = m_renderBounds;
}

//===========================================================================================================