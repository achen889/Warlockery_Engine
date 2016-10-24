//==============================================================================================================
//InputSystem.hpp
//by Albert Chen Feb-2-2015.
//==============================================================================================================

#pragma once

#ifndef _included_InputSystem__
#define _included_InputSystem__

#include "Engine/Math/Vector2.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Math/Math2D.hpp"

#include "Engine/Core/EventCallback.hpp"

//macro key defines
#define IF_INPUT_SYSTEM_EXISTS if(theInputSystem != NULL)
#define IF_INPUT_IS_KEY_PRESSED(name) if(theInputSystem->IsKeyDown((int)name))
#define IF_INPUT_IS_KEY_RELEASED(name) if(theInputSystem->WasKeyJustReleased((int)name))
#define INPUT_IS_KEY_RELEASED(name)theInputSystem->WasKeyJustReleased((int)name)
#define INPUT_IS_KEY_PRESSED(name)theInputSystem->IsKeyDown((int)name)

#define INPUT_GET_MOUSE_SCREEN_POS() theInputSystem->GetMouseScreenPosition()
#define INPUT_WAS_MOUSE_BUTTON_RELEASED(name) theInputSystem->WasMouseButtonJustReleased((int)name)
#define INPUT_IS_MOUSE_BUTTON_PRESSED(name) theInputSystem->IsMouseButtonDown((int)name)
#define IF_INPUT_WAS_MOUSE_BUTTON_RELEASED(name) if(theInputSystem->WasMouseButtonJustReleased((int)name) )
#define IF_INPUT_IS_MOUSE_BUTTON_PRESSED(name) if(theInputSystem->IsMouseButtonDown((int)name))

#include "InputCommon.hpp"

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
	bool WasKeyJustPressed(int keyData);
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
	bool m_mouseCursorHidden;
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
		return m_KeyState[keyData].m_wasJustChanged && !m_KeyState[keyData].m_isDown;
	else
		return false;
}

inline bool InputSystem::WasKeyJustPressed(int keyData) {
	if (keyData >= 0 && keyData < NUMBER_OF_VIRTUAL_KEYS)
		return m_KeyState[keyData].m_wasJustChanged && m_KeyState[keyData].m_isDown;
	else
		return false;
}

inline void InputSystem::SetLatestKeyPressed(int charID){
	m_latestKeyPressed = charID;
}

///----------------------------------------------------------------------------------------------------------
///mouse queries

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

inline void InputSystem::SetScreenMousePosition(const float& mouseX, const float& mouseY ){

	m_mouseScreenPosition = Vector2(mouseX, mouseY );

}

inline bool InputSystem::IsMouseHoverOnAABB2(const AABB2& renderBounds){

	Vector2 mouseScreenPoint = theInputSystem->GetMouseScreenPosition();
	if (DoAABBAndPointOverlap(renderBounds, mouseScreenPoint)){
		return true;
	}
	return false;

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




