//==============================================================================================================
//UserInterfaceCommon.hpp
//by Albert Chen Apr-18-2016.
//==============================================================================================================

#pragma once

#ifndef _included_UserInterfaceCommon__
#define _included_UserInterfaceCommon__

#include "Engine/Math/Vector2.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Input//InputCommon.hpp"
#include "Engine/Core/Clock.hpp"

//===========================================================================================================

extern Clock* g_UIClock;

void AllocUIClock();

double GetUIDeltaSeconds();

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///UI constants

//anchor points
static const Vector2 UI_SCREEN_SIZE = Vector2(1600, 900);

static const Vector2 UI_ANCHOR_SCREEN_CENTER = UI_SCREEN_SIZE * 0.5f;

static const Vector2 UI_ANCHOR_SCREEN_LEFT_CENTER = Vector2(0.0f, UI_SCREEN_SIZE.y * 0.5f);
static const Vector2 UI_ANCHOR_SCREEN_RIGHT_CENTER = Vector2(UI_SCREEN_SIZE.x * 0.5f, UI_SCREEN_SIZE.y * 0.5f);

static const Vector2 UI_ANCHOR_SCREEN_TOP_CENTER = Vector2(UI_SCREEN_SIZE.x * 0.5f, UI_SCREEN_SIZE.y);
static const Vector2 UI_ANCHOR_SCREEN_BOTTOM_CENTER = Vector2(UI_SCREEN_SIZE.x * 0.5f, 0.0f);

static const Vector2 UI_ANCHOR_SCREEN_TOP_LEFT = Vector2(0.0f, UI_SCREEN_SIZE.y);
static const Vector2 UI_ANCHOR_SCREEN_TOP_RIGHT = Vector2(UI_SCREEN_SIZE.x, UI_SCREEN_SIZE.y);

static const Vector2 UI_ANCHOR_SCREEN_BOTTOM_LEFT = Vector2(0.0f, 0.0f);
static const Vector2 UI_ANCHOR_SCREEN_BOTTOM_RIGHT = Vector2(UI_SCREEN_SIZE.x, 0.0f);

//===========================================================================================================

enum UIAnchorID {
	UI_ANCHOR_CENTER,
	UI_ANCHOR_LEFT_CENTER,
	UI_ANCHOR_RIGHT_CENTER,
	UI_ANCHOR_TOP_CENTER,
	UI_ANCHOR_BOTTOM_CENTER,
	UI_ANCHOR_TOP_LEFT,
	UI_ANCHOR_TOP_RIGHT,
	UI_ANCHOR_BOTTOM_LEFT,
	UI_ANCHOR_BOTTOM_RIGHT,
	NUM_UI_ANCHORS,
	UI_ANCHOR_INVALID
};

///----------------------------------------------------------------------------------------------------------
///global helpers

const Vector2 GetUIAnchorForScreen(const UIAnchorID& anchorEnum);

const Vector2 GetPositionForUIAnchorInAABB2(const UIAnchorID&, const AABB2& anchorBounds);

const UIAnchorID GetUIAnchorForString(const std::string& anchorString);

///----------------------------------------------------------------------------------------------------------
///inline globals

inline const Vector2 GetUIAnchorForScreen(const UIAnchorID& anchorEnum) {
	switch(anchorEnum){
	case UI_ANCHOR_CENTER: 
		return UI_ANCHOR_SCREEN_CENTER;
	case UI_ANCHOR_LEFT_CENTER: 
		return UI_ANCHOR_SCREEN_LEFT_CENTER;
	case UI_ANCHOR_RIGHT_CENTER: 
		return UI_ANCHOR_SCREEN_RIGHT_CENTER;
	case UI_ANCHOR_TOP_CENTER: 
		return UI_ANCHOR_SCREEN_TOP_CENTER;
	case UI_ANCHOR_BOTTOM_CENTER: 
		return UI_ANCHOR_SCREEN_BOTTOM_CENTER;
	case UI_ANCHOR_TOP_LEFT: 
		return UI_ANCHOR_SCREEN_TOP_LEFT;
	case UI_ANCHOR_TOP_RIGHT: 
		return UI_ANCHOR_SCREEN_TOP_RIGHT;
	case UI_ANCHOR_BOTTOM_LEFT: 
		return UI_ANCHOR_SCREEN_BOTTOM_LEFT;
	case UI_ANCHOR_BOTTOM_RIGHT: 
		return UI_ANCHOR_SCREEN_BOTTOM_RIGHT;
	}
	return Vector2::ZERO;
}

//-----------------------------------------------------------------------------------------------------------

inline const Vector2 GetPositionForUIAnchorInAABB2(const UIAnchorID& anchorID , const AABB2& anchorBounds) {

	switch (anchorID) {
	case UI_ANCHOR_CENTER:
		return anchorBounds.GetPointAtNormalizedPositionWithinBox(Vector2(0.5f, 0.5f));
	case UI_ANCHOR_LEFT_CENTER:
		return anchorBounds.GetPointAtNormalizedPositionWithinBox(Vector2(0.0f, 0.5f));
	case UI_ANCHOR_RIGHT_CENTER:
		return anchorBounds.GetPointAtNormalizedPositionWithinBox(Vector2(1.0f, 0.5f));
	case UI_ANCHOR_TOP_CENTER:
		return anchorBounds.GetPointAtNormalizedPositionWithinBox(Vector2(0.5f, 1.0f));
	case UI_ANCHOR_BOTTOM_CENTER:
		return anchorBounds.GetPointAtNormalizedPositionWithinBox(Vector2(0.5f, 0.0f));
	case UI_ANCHOR_TOP_LEFT:
		return anchorBounds.GetPointAtNormalizedPositionWithinBox(Vector2(0.0f, 1.0f));
	case UI_ANCHOR_TOP_RIGHT:
		return anchorBounds.GetPointAtNormalizedPositionWithinBox(Vector2(1.0f, 1.0f));
	case UI_ANCHOR_BOTTOM_LEFT:
		return anchorBounds.GetPointAtNormalizedPositionWithinBox(Vector2(0.0f, 0.0f));
	case UI_ANCHOR_BOTTOM_RIGHT:
		return anchorBounds.GetPointAtNormalizedPositionWithinBox(Vector2(1.0f, 0.0f));
	}
	return Vector2::ZERO;

}

//-----------------------------------------------------------------------------------------------------------

inline const UIAnchorID GetUIAnchorForString(const std::string& anchorString){
	if (anchorString == "center") {
		return UI_ANCHOR_CENTER;
	}
	else if (anchorString == "left center") {
		return UI_ANCHOR_LEFT_CENTER;
	}
	else if (anchorString == "right center") {
		return UI_ANCHOR_RIGHT_CENTER;
	}
	else if (anchorString == "top center") {
		return UI_ANCHOR_TOP_CENTER;
	}
	else if (anchorString == "bottom center") {
		return UI_ANCHOR_BOTTOM_CENTER;
	}

	else if (anchorString == "top left") {
		return UI_ANCHOR_TOP_LEFT;
	}
	else if (anchorString == "top right") {
		return UI_ANCHOR_TOP_RIGHT;
	}
	else if (anchorString == "bottom left") {
		return UI_ANCHOR_BOTTOM_LEFT;
	}
	else if (anchorString == "bottom right") {
		return  UI_ANCHOR_BOTTOM_RIGHT;
	}
	return UI_ANCHOR_INVALID;
}




//===========================================================================================================

#endif //__includedUserInterfaceCommon__

