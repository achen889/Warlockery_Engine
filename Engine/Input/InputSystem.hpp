//==============================================================================================================
//InputSystem.hpp
//by Albert Chen Feb-2-2015.
//==============================================================================================================

#pragma once



#ifndef _included_InputSystem__
#define _included_InputSystem__

#include "Engine\Math\Vector2.hpp"
#include "Engine\Console\Console.hpp"
#include "Engine\Math\Math2D.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define NUMBER_OF_VIRTUAL_KEYS 256
#define MAX_NUMBER_OF_XBOX_CONTROLLERS 4

//macro key defines
#define IF_INPUT_SYSTEM_EXISTS if(theInputSystem)
#define IF_INPUT_IS_KEY_PRESSED(name) if(theInputSystem->IsKeyDown((int)name))
#define IF_INPUT_IS_KEY_RELEASED(name) if(theInputSystem->WasKeyJustReleased((int)name))
#define INPUT_IS_KEY_RELEASED(name)theInputSystem->WasKeyJustReleased((int)name)
#define INPUT_IS_KEY_PRESSED(name)theInputSystem->IsKeyDown((int)name)
#define INPUT_WAS_MOUSE_BUTTON_RELEASED(name) theInputSystem->WasMouseButtonJustReleased((int)name)
#define INPUT_IS_MOUSE_BUTTON_PRESSED(name) theInputSystem->IsMouseButtonDown((int)name)
#define IF_INPUT_WAS_MOUSE_BUTTON_RELEASED(name) if(theInputSystem->WasMouseButtonJustReleased((int)name) )
#define IF_INPUT_IS_MOUSE_BUTTON_PRESSED(name) if(theInputSystem->IsMouseButtonDown((int)name))

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
#define KEY_LEFT_SHIFT VK_LSHIFT
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
enum MouseButtonID{
	MOUSE_BUTTON_LEFT,
	MOUSE_BUTTON_RIGHT,
	NUMBER_OF_MOUSE_BUTTONS
};

//xbox button enums
enum XBoxButtonID{
	XBOX_BUTTON_X, 
	XBOX_BUTTON_Y, 
	XBOX_BUTTON_A, 
	XBOX_BUTTON_B,
	XBOX_BUTTON_LEFT_TRIGGER,
	XBOX_BUTTON_RIGHT_TRIGGER,
	NUMBER_OF_XBOX_BUTTONS

};

enum XboxJoystickID{
	XBOX_JOYSTICK_LEFT,
	XBOX_JOYSTICK_RIGHT,
	NUMBER_OF_XBOX_JOYSTICKS
};

struct XBoxControllerState{
	bool isPluggedIn;
	unsigned int m_controllerID;
	bool m_buttonDownStates[NUMBER_OF_XBOX_BUTTONS ];
	Vector2 m_stickPositions[ NUMBER_OF_XBOX_JOYSTICKS ];
	///----------------------------------------------------------------------------------------------------------
	///constructors
	XBoxControllerState(){
		//do nothing
	}

	XBoxControllerState(int controllerNumber){
		m_controllerID = controllerNumber;
	}

};

struct KeyState{
	bool m_isDown;
	bool m_wasJustReleased;
};

struct MouseButtonState{
	bool m_isDown;
	bool m_wasJustReleased;
	bool m_didDoubleClick;
	//bool m_isCursorHidden;
};

class InputSystem{
public:
	 InputSystem();
	~InputSystem();

	void StartUp(void* platformHandle, const Vector2& clientDisplaySize );
	void Update();
	void ShutDown();
	void SetSystemQuit();
	//input queries
	bool IsKeyDown(int keyData );
	bool WasKeyJustReleased(int keyData );
	void SetKeyState(int keyData, bool isPressedDown);

	//mouse queries
	void SetMouseState(int mouseData, bool isPressedDown, bool diddoubleclick = false);
	bool IsMouseButtonDown(int mouseData);
	bool WasMouseButtonJustReleased(int mouseData );
	bool DidMouseButtonDoubleClick(int mouseData );
	const Vector2& GetMousePosition();
	const Vector2& GetMouseScreenPosition(){ return m_mouseScreenPosition; }
	void SetScreenMousePosition(const float& mouseX, const float& mouseY );
	bool IsMouseHoverOnAABB2(const AABB2& renderBounds);
	const Vector2 SnapMousePosition(const int& snapmouseX, const int& snapmouseY);
	void ShowMouseCursor();
	void HideMouseCursor();
	const Vector2& GetMouseMoveDirection();
	const float CalcMouseMoveAngleDegrees();
	void SetMouseMoveDirection();
	bool IsMouseWheelPresent();
	void SetMouseWheelScrollDirection(int mouseDeltaValue );
	int GetMouseWheelDelta();
	bool IsMouseWheelScrollingUp();
	
	bool IsMouseWheelScrollingDown();
	//xbox queries
	const Vector2& GetXBoxStickPosition(XboxJoystickID stickID, unsigned int controllerID ) const;
	float NormalizeXBoxControllerStickValue(short incomingValue );
	bool IsButtonDown(int buttonData );
	bool IsPluggedIn(int controllerID);

	//processing methods
	void ProcessKeyboardInput();
	void ProcessXBoxControllerInput(XBoxControllerState& controllerState );
	bool IsReadyToQuit();
	
	Vector2 m_clientDisplaySize;
	bool m_isMouseWheelStopped;

	//debug functions
	void ConsolePrintMousePosition();
	void SetLatestKeyPressed(int charID);

	unsigned int GetLatestKeyPressed(){ 
		unsigned int latestKeyID = m_latestKeyPressed;
		m_latestKeyPressed = 0;
		return latestKeyID;  
	}

	bool m_isReadyToQuit;

private:
	void* m_platformHandle;
	long m_prevAddress;

	KeyState m_KeyState[NUMBER_OF_VIRTUAL_KEYS];
	MouseButtonState m_MouseState[NUMBER_OF_MOUSE_BUTTONS];
	Vector2 m_mousePosition;
	Vector2 m_mouseScreenPosition;
	Vector2 m_lastMousePosition;
	Vector2 m_mouseMoveDirection;
	bool m_isMouseWheelScrollUp;
	int m_mouseWheelDelta;
	int m_mouseWheelDeltaPrev;
	//mouse drag vars
	bool isMouseDragging;
	AABB2 mouseDragBox;

	int m_latestKeyPressed;

	XBoxControllerState m_xbox_Controller_States[MAX_NUMBER_OF_XBOX_CONTROLLERS];

	
	
};

extern InputSystem* theInputSystem;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

///----------------------------------------------------------------------------------------------------------
///keyboard queries
inline bool InputSystem::IsKeyDown(int keyData ){
	if( keyData >=0 && keyData < NUMBER_OF_VIRTUAL_KEYS )
		return m_KeyState[keyData].m_isDown;
	else
		return false;
}

inline bool InputSystem::WasKeyJustReleased(int keyData ){
	if( keyData >=0 && keyData < NUMBER_OF_VIRTUAL_KEYS )
		return m_KeyState[keyData].m_wasJustReleased;
	else
		return false;
}

inline void InputSystem::SetLatestKeyPressed(int charID){
	m_latestKeyPressed = charID;
}

///----------------------------------------------------------------------------------------------------------
///mouse queries
inline bool InputSystem::IsMouseWheelPresent(){
	return (GetSystemMetrics(SM_MOUSEWHEELPRESENT) != 0);
}

inline void InputSystem::ShowMouseCursor(){
	ShowCursor(true);
}

inline void InputSystem::HideMouseCursor(){
	ShowCursor(false);
}

inline void InputSystem::SetMouseWheelScrollDirection(int mouseDeltaValue ){
	
	if(IsMouseWheelPresent()){	
		m_mouseWheelDelta = mouseDeltaValue;
		if(m_mouseWheelDelta != 0){
			m_isMouseWheelStopped = false;
		}
	}
		
}

inline int InputSystem::GetMouseWheelDelta(){
	return m_mouseWheelDelta;
}

inline bool InputSystem::IsMouseWheelScrollingUp(){
	if(theInputSystem->IsMouseWheelPresent() ){
		return (m_mouseWheelDelta > 0 );
	}
	return false;
}

inline bool InputSystem::IsMouseWheelScrollingDown(){
	if (theInputSystem->IsMouseWheelPresent()){
		return (m_mouseWheelDelta < 0);
	}
	return false;
}

inline bool InputSystem::IsMouseButtonDown(int mouseData ){
	return m_MouseState[mouseData].m_isDown;
}

inline bool InputSystem::WasMouseButtonJustReleased(int mouseData ){
	if (mouseData >= 0 && mouseData < NUMBER_OF_MOUSE_BUTTONS)

		return m_MouseState[mouseData].m_wasJustReleased;
	else
		return false;
}

inline bool InputSystem::DidMouseButtonDoubleClick(int mouseData ){
	bool didDoubleClick = m_MouseState[mouseData].m_didDoubleClick;
	m_MouseState[mouseData].m_didDoubleClick = false;
	return didDoubleClick;
}

inline const Vector2& InputSystem::GetMousePosition(){
	POINT cursorPos; 
	GetCursorPos(&cursorPos); 

	float mousePosX = (float)cursorPos.x;
	float mousePosY = (float)cursorPos.y;
	//cursorPos.y

	m_mousePosition = Vector2( mousePosX, mousePosY); 

	return m_mousePosition; 
}

inline void InputSystem::SetScreenMousePosition(const float& mouseX, const float& mouseY ){
	//m_lastMousePosition = m_mousePosition;

	m_mouseScreenPosition = Vector2(mouseX, mouseY );

}

inline bool InputSystem::IsMouseHoverOnAABB2(const AABB2& renderBounds){

	Vector2 mouseScreenPoint = theInputSystem->GetMouseScreenPosition();
	if (DoAABBAndPointOverlap(renderBounds, mouseScreenPoint)){
		return true;
	}
	return false;

}

inline const Vector2 InputSystem::SnapMousePosition(const int& snapmouseX, const int& snapmouseY ){

	SetCursorPos(snapmouseX, snapmouseY);
	m_mousePosition = GetMousePosition();

	return Vector2((float)snapmouseX, (float)snapmouseY);
}

inline const Vector2& InputSystem::GetMouseMoveDirection(){
	return m_mouseMoveDirection;
}

inline void InputSystem::SetMouseMoveDirection(){
	//try center for m last mouse pos
	m_mouseMoveDirection = Vector2( m_mouseScreenPosition - m_lastMousePosition );
}

//clamps angle between 0 and 360
inline const float InputSystem::CalcMouseMoveAngleDegrees(){
	float mouseMoveAngleDegrees = m_mouseMoveDirection.CalcHeadingDegrees();
		//if less than 0
		while(mouseMoveAngleDegrees < 0 ){
			  mouseMoveAngleDegrees += 360.0f;
		}
		while(mouseMoveAngleDegrees > 360 ){
			  mouseMoveAngleDegrees -= 360.0f;
		}

	return mouseMoveAngleDegrees;
}

///----------------------------------------------------------------------------------------------------------
///xbox queries
inline const Vector2& InputSystem::GetXBoxStickPosition(XboxJoystickID stickID, unsigned int controllerID ) const{
	if(controllerID < MAX_NUMBER_OF_XBOX_CONTROLLERS){

		const XBoxControllerState& controllerState = m_xbox_Controller_States[controllerID];
		if(stickID < NUMBER_OF_XBOX_JOYSTICKS)
		   return controllerState.m_stickPositions[stickID];

	}

	return Vector2::ZERO;
}

inline bool InputSystem::IsButtonDown(int buttonData ){
	if( buttonData >=0 && buttonData < NUMBER_OF_XBOX_BUTTONS )
		return m_xbox_Controller_States[0].m_buttonDownStates[buttonData];
	else
		return false;
}

inline bool InputSystem::IsPluggedIn(int controllerID){
	if( controllerID >=0 && controllerID < MAX_NUMBER_OF_XBOX_CONTROLLERS )
		return m_xbox_Controller_States[controllerID].isPluggedIn;
	else
		return false;

}

///----------------------------------------------------------------------------------------------------------
///quit queries
inline void InputSystem::SetSystemQuit(){
	m_isReadyToQuit = true;
}

inline bool InputSystem::IsReadyToQuit(){
	return m_isReadyToQuit;
}

///----------------------------------------------------------------------------------------------------------
///debug methods

inline void InputSystem::ConsolePrintMousePosition(){
	ConsolePrintf("MousePosition< ");
	ConsolePrintVector2(m_mousePosition);
	ConsolePrintf(" >\n");
}

#endif




