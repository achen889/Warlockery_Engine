//==============================================================================================================
//InputCommon.hpp
//by Albert Chen May-8-2016.
//==============================================================================================================

#pragma once
#include "Engine/Math/Vector2.hpp"

#ifndef _included_InputCommon__
#define _included_InputCommon__

//===========================================================================================================

#define NUMBER_OF_VIRTUAL_KEYS 256
#define MAX_NUMBER_OF_XBOX_CONTROLLERS 4

//special key defines
#define KEY_BACKSPACE VK_BACK
#define KEY_ENTER VK_RETURN
#define KEY_TILDE 192
#define KEY_COMMA VK_OEM_COMMA
#define KEY_CAPSLOCK VK_CAPITAL
#define KEY_SPACEBAR VK_SPACE
#define KEY_HOME VK_HOME
#define KEY_PRINT_SCREEN VK_SNAPSHOT
#define KEY_ESC VK_ESCAPE
#define KEY_TAB VK_TAB
#define KEY_SHIFT VK_SHIFT
#define KEY_LEFT_SHIFT VK_SHIFT //this must be shift too
#define KEY_RIGHT_SHIFT VK_RSHIFT
#define KEY_CTRL VK_CONTROL
#define KEY_LEFT_CTRL VK_LCONTROL
#define KEY_RIGHT_CTRL VK_RCONTROL
#define KEY_PAGE_DOWN VK_PRIOR
#define KEY_PAGE_UP VK_NEXT
#define KEY_ARROW_UP VK_UP
#define KEY_ARROW_DOWN VK_DOWN
#define KEY_ARROW_LEFT VK_LEFT
#define KEY_ARROW_RIGHT VK_RIGHT
#define KEY_F1 VK_F1
#define KEY_F2 VK_F2
#define KEY_F3 VK_F3
#define KEY_F4 VK_F4
#define KEY_F5 VK_F5
#define KEY_F6 VK_F6
#define KEY_F7 VK_F7
#define KEY_F8 VK_F8
#define KEY_F9 VK_F9
#define KEY_F10 VK_F10
#define KEY_F11 VK_F11
#define KEY_F12 VK_F12

//mouse button enums
enum MouseButtonID {
	MOUSE_BUTTON_LEFT,
	MOUSE_BUTTON_RIGHT,
	NUMBER_OF_MOUSE_BUTTONS
};

//xbox button enums
enum XBoxButtonID {
	XBOX_BUTTON_X,
	XBOX_BUTTON_Y,
	XBOX_BUTTON_A,
	XBOX_BUTTON_B,
	XBOX_BUTTON_LEFT_TRIGGER,
	XBOX_BUTTON_RIGHT_TRIGGER,
	NUMBER_OF_XBOX_BUTTONS

};

enum XboxJoystickID {
	XBOX_JOYSTICK_LEFT,
	XBOX_JOYSTICK_RIGHT,
	NUMBER_OF_XBOX_JOYSTICKS
};

struct XBoxControllerState {
	bool isPluggedIn;
	unsigned int m_controllerID;
	bool m_buttonDownStates[NUMBER_OF_XBOX_BUTTONS];
	Vector2 m_stickPositions[NUMBER_OF_XBOX_JOYSTICKS];
	///----------------------------------------------------------------------------------------------------------
	///constructors
	XBoxControllerState() {
		//do nothing
	}

	XBoxControllerState(int controllerNumber) {
		m_controllerID = controllerNumber;
	}

};

struct KeyState {
	bool m_isDown;
	bool m_wasJustChanged;
};

struct MouseButtonState {
	bool m_isDown;
	bool m_wasJustReleased;
	bool m_didDoubleClick;
};


//===========================================================================================================

#endif //__includedInputCommon__

