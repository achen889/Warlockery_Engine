//==============================================================================================================
//UIElement.hpp
//by Albert Chen Sep-1-2015.
//==============================================================================================================

#pragma once

#ifndef _included_UIElement__
#define _included_UIElement__

#include "Engine/Math/Math2D.hpp"
#include "Engine/Renderer/OGLRenderingUtils.hpp"

//===========================================================================================================

//UI constants
static const AABB2 DEFAULT_ELEMENT_BOUNDING_BOX = AABB2(Vector2(-16.0f, -16.0f), Vector2(16.0f, 16.0f)); //default 32x32 box about origin

static const Vector2 DEFAULT_ELEMENT_SIZE = Vector2(32.0f, 32.0f);

static int elementID = 0;

struct MeshRenderer;
class OpenGLRenderer;
enum MouseButtonID;

//UI anchor constants
// static const Vector2 UI_SCREEN_SIZE = Vector2(theOGLRenderer->GetDisplayWidth(), theOGLRenderer->GetDisplayHeight());
// static const Vector2 UI_ANCHOR_SCREEN_CENTER = UI_SCREEN_SIZE * 0.5f;
// static const Vector2 UI_ANCHOR_SCREEN_LEFT_CENTER = Vector2(0.0f, theOGLRenderer->GetDisplayHeight() * 0.5f);
// static const Vector2 UI_ANCHOR_SCREEN_RIGHT_CENTER = Vector2(theOGLRenderer->GetDisplayWidth(), theOGLRenderer->GetDisplayHeight() * 0.5f);

//===========================================================================================================

class UIElement{
public:
	//vars
	int m_id;
	Vector2 m_size = DEFAULT_ELEMENT_SIZE; //size in pixels
	AABB2 m_renderBounds = DEFAULT_ELEMENT_BOUNDING_BOX; //rendering box
	Vector2 m_scaleFactor = Vector2::ONE;

	Vector2 m_screenCoordinates; //screen position
	Rgba m_color = Rgba::WHITE;
	Rgba m_color1 = Rgba::BLACK;

	MeshRenderer* m_elementRenderer = NULL;
	
	//methods

	//constructors
	UIElement();

	UIElement(const Vector2& screenPosition, const Vector2& newSize = DEFAULT_ELEMENT_SIZE , const Vector2& scaleFactor = Vector2::ONE , const Rgba& elementColor = Rgba::WHITE );

	virtual ~UIElement();

	//setters
	void InitializeDefaultTexturedElementMeshRenderer();

	void SetDefaultElementTexture(const std::string& texturePath);
	
	void SetSize(const int& sizeX, const int& sizeY);
	void SetSize(const Vector2& newSize);

	void SetRenderBounds();
	void SetRenderBounds(const Vector2& screenCoords, const Vector2& elementSize);
	void SetRenderBounds(AABB2& renderBounds);

	void SetScreenCoords(const Vector2& newscreenCoords){
		m_screenCoordinates = newscreenCoords;
		
		//SetRenderBounds();
	}

	void SetElementScaleFactor(const Vector2& newScale);
	void SetElementScaleFactor(const float& newScale);

	void SetElementColor(const Rgba& elementColor);
	void SetElementColor1(const Rgba& elementColor1);

	//interaction methods
	bool OnHover();
	bool OnClick(MouseButtonID mouseButtonClicked);
	bool OnKeyDown(int keyData);
	bool OnWasKeyJustReleased(int keyData);

	//render
	virtual void RenderMesh(OpenGLRenderer* renderer, ModelViewMatrix* modelMat = NULL);
	virtual void RenderMesh(OpenGLRenderer* renderer, const std::string& texturePath );
	
};

typedef std::vector<UIElement*> UIElements;
typedef std::vector<UIElement*>::iterator UIElementsIterator;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline UIElement::~UIElement(){
	//do nothing
	if (m_elementRenderer){
		delete m_elementRenderer;
	}
	
}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///setters

inline void UIElement::SetSize(const int& sizeX, const int& sizeY){
	m_size.x = (float)sizeX;
	m_size.y = (float)sizeY;
}

inline void UIElement::SetSize(const Vector2& newSize){
	m_size = newSize;
}

inline void UIElement::SetRenderBounds(){
	Vector2 renderMins = Vector2(m_screenCoordinates);
	Vector2 renderMaxs = Vector2(m_screenCoordinates + m_size);

	m_renderBounds = AABB2(renderMins, renderMaxs);
}

inline void UIElement::SetRenderBounds(AABB2& renderBounds){
	Vector2 renderMins = Vector2(m_screenCoordinates);
	Vector2 renderMaxs = Vector2(m_screenCoordinates + m_size);

	renderBounds = AABB2(renderMins, renderMaxs);
}

inline void UIElement::SetElementScaleFactor(const float& newScale){
	m_scaleFactor = Vector2(newScale, newScale);
}

inline void UIElement::SetElementScaleFactor(const Vector2& newScale){
	m_scaleFactor = newScale;
}

inline void UIElement::SetElementColor(const Rgba& elementColor){
	m_color = elementColor;
}

inline void UIElement::SetElementColor1(const Rgba& elementColor1){
	m_color1 = elementColor1;
}

//-----------------------------------------------------------------------------------------------------------

#endif