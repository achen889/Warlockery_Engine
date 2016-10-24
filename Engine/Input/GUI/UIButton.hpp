//==============================================================================================================
//Button.hpp
//by Albert Chen Sep-1-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Button__
#define _included_Button__

#include "UIElement.hpp"

#include "UIText.hpp"
#include "..\..\Core\EventCallback.hpp"

//===========================================================================================================

//===========================================================================================================
class UIButton : public UIElement{
public:
	
	//data
	UIText* m_buttonText;
	UIText* m_buttonTooltipText;
	Rgba m_textColor; //tooltip color is the same

	EventCallback* m_buttonEventCallback;

	//constructors
	UIButton(const Vector2& screenPosition, const Vector2& newSize = DEFAULT_ELEMENT_SIZE, const Vector2& scaleFactor = Vector2::ONE, const Rgba& elementColor = Rgba::WHITE);
	UIButton();
	~UIButton();

	//accessors
	void SetText(const std::string& buttonText);
	void SetTooltipText(const std::string& buttonText);
	void SetTooltipTextScreenCoords(const Vector2& newscreenCoords){
		m_buttonTooltipText->SetScreenCoords(newscreenCoords);
	}

	bool OnHover(){
		//if we use the tooltip
		if (m_buttonTooltipText->m_renderBounds == m_renderBounds){
			return m_buttonTooltipText->OnHover();
		}
		else{
			return UIElement::OnHover();
		}
	}

	bool OnClick(MouseButtonID mouseButtonClicked){
		//if we use the tooltip

		if (m_buttonTooltipText->m_renderBounds == m_renderBounds){

			return m_buttonTooltipText->OnClick(mouseButtonClicked);
		}
		else{
			return UIElement::OnClick(mouseButtonClicked);
		}
	}

	void ExecuteButtonEvent(){
		if (m_buttonEventCallback)
			m_buttonEventCallback->Execute();
	}
	
	void SetEventCallback(EventCallbackFunc* func, const std::string& name, void* data){
		
		m_buttonEventCallback = new EventCallback(name, func, data);
	}

	//render

};

typedef std::vector<UIButton*> UIButtons;
typedef std::vector<UIButton*>::iterator UIButtonsIterator;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods
inline UIButton::UIButton(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------
inline UIButton::~UIButton(){

	delete m_buttonText;
	delete m_buttonTooltipText;
	delete m_buttonEventCallback;

	UIElement::~UIElement();
}
//-----------------------------------------------------------------------------------------------------------


#endif