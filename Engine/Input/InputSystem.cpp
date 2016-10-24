//==============================================================================================================
//InputSystem.cpp
//by Albert Chen Feb-3-2015.
//==============================================================================================================

#include "InputSystem.hpp"
#include "Engine\Console\DevConsole.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <Windowsx.h>
#include <Xinput.h> // include the Xinput API
#pragma comment( lib, "xinput" ) // Link in the xinput.lib static library

InputSystem* theInputSystem = NULL;

///----------------------------------------------------------------------------------------------------------
///window message processing
LRESULT _stdcall WindowMessageProcessingFunction( HWND windowHandle, UINT MessageId, WPARAM WParam, LPARAM LParam ){
	if(theInputSystem){
		switch( MessageId ){
			//keyboard messages
			case WM_CHAR:
				if (theInputSystem){
					theInputSystem->SetLatestKeyPressed((int)WParam);
				}
				return 0;
			case WM_KEYDOWN:
				if(theInputSystem ){
					theInputSystem->SetKeyState((int)WParam, true );
				}
				//ConsolePrintf("key down!");
				return 0;

			case WM_KEYUP:
				if(theInputSystem ){
					theInputSystem->SetKeyState((int)WParam, false );
				}
				//ConsolePrintf("key up!");
				return 0;
			case WM_CLOSE:
				theInputSystem->SetSystemQuit();
			//mouse messages
			case WM_MOUSEMOVE:
					//client window coordinates adjusted from screen coordinates
					theInputSystem->SetScreenMousePosition((float)GET_X_LPARAM(LParam), (float)theInputSystem->m_clientDisplaySize.y - GET_Y_LPARAM(LParam) );
					theInputSystem->SetMouseMoveDirection();
					return 0;
			case WM_MOUSEWHEEL:
				//insert mouse wheel controls here
				theInputSystem->SetMouseWheelScrollDirection(GET_WHEEL_DELTA_WPARAM(WParam));
				return 0;
			case WM_LBUTTONDBLCLK:
				theInputSystem->SetMouseState(MOUSE_BUTTON_LEFT, true, true );
				return 0;
			case WM_RBUTTONDBLCLK:
				theInputSystem->SetMouseState(MOUSE_BUTTON_RIGHT, true, true );
				return 0;
			case WM_LBUTTONDOWN:
				theInputSystem->SetMouseState(MOUSE_BUTTON_LEFT, true);
				//ConsolePrintf("mouse left down!");
				return 0;
			case WM_RBUTTONDOWN:
				theInputSystem->SetMouseState(MOUSE_BUTTON_RIGHT, true);
				return 0;
			case WM_LBUTTONUP:
				theInputSystem->SetMouseState(MOUSE_BUTTON_LEFT, false);
				//ConsolePrintf("mouse left up!");
				return 0;
			case WM_RBUTTONUP:
				theInputSystem->SetMouseState(MOUSE_BUTTON_RIGHT, false);
				return 0;
			default: 
				break;
		}//end of switch

	}
	return DefWindowProc(windowHandle, MessageId, WParam, LParam );
}//end of GMPF

//===========================================================================================================

CONSOLE_COMMAND(mouseDat){
	UNUSED_COMMAND_ARGS
	std::string mouseDataText = "==========Mouse Data==========\n";
	const unsigned int mouseDataTextLineSkipValue = 500;
	if (theInputSystem){
		Vector2 mouseScreenPos = theInputSystem->GetMouseScreenPosition();
		Vector2 mouseAbsPos = theInputSystem->GetMousePosition();
		bool hasMouseWheel = theInputSystem->IsMouseWheelPresent();

		mouseDataText += "Mouse Screen Position (" + FloatToString(mouseScreenPos.x) + "," + FloatToString(mouseScreenPos.y) + ")\n";
		mouseDataText += "Mouse Absolute Position (" + FloatToString(mouseAbsPos.x) + "," + FloatToString(mouseAbsPos.y) + ")\n";
		if (hasMouseWheel){
			mouseDataText += "Mouse Wheel Detected.\n";
		}
		else{
			mouseDataText += "No Mouse Wheel Detected.\n";
		}
	}
	theTextSystem->SetVAOStringTextureCoords(theTextSystem->m_textIn, DEV_CONSOLE_OUTPUT_POSITION, mouseDataText, DEV_CONSOLE_OUTPUT_COLOR, mouseDataTextLineSkipValue);
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors
InputSystem::InputSystem()
	:m_platformHandle(NULL),
	m_prevAddress(0L){
	
	if(!theInputSystem ){
		theInputSystem = this;
	}
	m_isReadyToQuit = false;

	REGISTER_CONSOLE_COMMAND(mouseDat, "Display Mouse Data.");
}


InputSystem::~InputSystem(){
	//do nothing
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///upkeep methods
void InputSystem::StartUp(void* platformHandle, const Vector2& clientDisplaySize ){
	m_platformHandle = platformHandle;
	m_clientDisplaySize = clientDisplaySize;
	m_prevAddress = SetWindowLong((HWND)m_platformHandle, GWL_WNDPROC, (long)& WindowMessageProcessingFunction);
	
	//set defaults for mouse
	isMouseDragging = false;
	mouseDragBox = AABB2::ZERO;
	for (int i = 0; i < NUMBER_OF_MOUSE_BUTTONS; i++){
		m_MouseState[i].m_didDoubleClick= false;
		m_MouseState[i].m_wasJustReleased = false;
		m_MouseState[i].m_isDown = false;
	}


	//set up keyboard
	for (int i = 0; i < NUMBER_OF_VIRTUAL_KEYS; i++){
		m_KeyState[i].m_isDown = false;
		m_KeyState[i].m_wasJustChanged = false;
	}

	//set up xbox controllers
	for (int controllerIndex = 0; controllerIndex < MAX_NUMBER_OF_XBOX_CONTROLLERS; controllerIndex++){

		XBoxControllerState& controllerState = m_xbox_Controller_States[controllerIndex];
		controllerState.isPluggedIn = false;
		controllerState.m_controllerID = controllerIndex;

		for (int buttonIndex = 0; buttonIndex < NUMBER_OF_XBOX_BUTTONS; buttonIndex++){
			controllerState.m_buttonDownStates[buttonIndex] = false;
		}

		for(int stickIndex = 0; stickIndex < NUMBER_OF_XBOX_JOYSTICKS; stickIndex++){
			controllerState.m_stickPositions[stickIndex] = Vector2(0.0f,0.0f ) ;
		}

	}

}

void InputSystem::Update(){
	for(int i = 0; i< NUMBER_OF_VIRTUAL_KEYS; i++){
		m_KeyState[i].m_wasJustChanged = false;
	}

	for(int i = 0; i< NUMBER_OF_MOUSE_BUTTONS; i++){
		m_MouseState[i].m_wasJustReleased = false;
		//m_MouseState[i].m_isDown = false; //drag box only works with this off
		m_MouseState[i].m_didDoubleClick = false;
	}

	//m_mouseWheelDeltaPrev = m_mouseWheelDelta;
	m_isMouseWheelStopped = true;
	m_mouseWheelDelta = 0;

	ProcessKeyboardInput();
	
	ProcessXBoxControllerInput(m_xbox_Controller_States[0]);
	ProcessXBoxControllerInput(m_xbox_Controller_States[1]);
}

void InputSystem::ShutDown(){
	m_prevAddress = SetWindowLong((HWND)m_platformHandle, GWL_WNDPROC, (long)& m_prevAddress );
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///keyboard queries

void InputSystem::SetKeyState(int keyData, bool isPressedDown ){
	if( keyData >=0 && keyData < NUMBER_OF_VIRTUAL_KEYS ){

		if(m_KeyState[keyData ].m_isDown && !isPressedDown){
		   m_KeyState[keyData ].m_wasJustChanged = true;
		}
		else if (!m_KeyState[keyData].m_isDown && isPressedDown) {
			m_KeyState[keyData].m_wasJustChanged = true;
		}
		else{
		   m_KeyState[keyData ].m_wasJustChanged = false;
		}

		m_KeyState[keyData ].m_isDown = isPressedDown;

	}
}

///----------------------------------------------------------------------------------------------------------
///mouse queries

void InputSystem::SetMouseState(int mouseData, bool isPressedDown, bool diddoubleclick ){
	if (mouseData >= 0 && mouseData < NUMBER_OF_MOUSE_BUTTONS){
		//did the user double click
		m_MouseState[mouseData].m_didDoubleClick = diddoubleclick;

		if (m_MouseState[mouseData].m_isDown && !isPressedDown){
			m_MouseState[mouseData].m_wasJustReleased = true;
		}
		else{
			m_MouseState[mouseData].m_wasJustReleased = false;
		}

		//set button down
		m_MouseState[mouseData].m_isDown = isPressedDown;
	}
}

//-----------------------------------------------------------------------------------------------------------



///----------------------------------------------------------------------------------------------------------
///xbox queries

float InputSystem::NormalizeXBoxControllerStickValue(short incomingValue ){
	static const int XBOX_CONTROLLER_STICK_AXIS_OFFSET = 32768;
	static const float XBOX_CONTROLLER_STICK_RANGE = 65535.0f;//do I have an extra 5
	static const float ONE_OVER_XBOX_CONTROLLER_STICK_RANGE = (1 / XBOX_CONTROLLER_STICK_RANGE);

	int rawStickAbsolutePosition = incomingValue + XBOX_CONTROLLER_STICK_AXIS_OFFSET;
	float normalizedStickAxisPosition = (float) rawStickAbsolutePosition * ONE_OVER_XBOX_CONTROLLER_STICK_RANGE;

	normalizedStickAxisPosition *=2.0f;
	normalizedStickAxisPosition -=1.0f;

	return normalizedStickAxisPosition;
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///processing methods
void InputSystem::ProcessKeyboardInput(){
	HWND myPlatformHandle = (HWND)(m_platformHandle);
	MSG messageHandler;

	for(;;){
		BOOL messageWasFound = PeekMessage(&messageHandler, myPlatformHandle, 0, 0, PM_REMOVE );

		if( !messageWasFound )
			break;

	TranslateMessage( &messageHandler);
	DispatchMessage( &messageHandler );
	}
}

void InputSystem::ProcessXBoxControllerInput(XBoxControllerState& controllerState ){
	//const button IDS
	const unsigned short BIT_A_BUTTON = 0x1000;
	const unsigned short BIT_B_BUTTON = 0x2000;
	const unsigned short BIT_X_BUTTON = 0x4000;
	const unsigned short BIT_Y_BUTTON = 0x8000;

	XINPUT_STATE xboxControllerState;
	memset( &xboxControllerState, 0, sizeof( xboxControllerState ) );

	DWORD errorStatus = XInputGetState( controllerState.m_controllerID, &xboxControllerState );
	if( errorStatus == ERROR_SUCCESS ){
		controllerState.isPluggedIn = true;
		
		//get controller states
		//for buttons
		controllerState.m_buttonDownStates[XBOX_BUTTON_X ] = (BIT_X_BUTTON & xboxControllerState.Gamepad.wButtons) != 0;
		controllerState.m_buttonDownStates[XBOX_BUTTON_Y ] = (BIT_Y_BUTTON & xboxControllerState.Gamepad.wButtons) != 0;
		controllerState.m_buttonDownStates[XBOX_BUTTON_A ] = (BIT_A_BUTTON & xboxControllerState.Gamepad.wButtons) != 0;
		controllerState.m_buttonDownStates[XBOX_BUTTON_B ] = (BIT_B_BUTTON & xboxControllerState.Gamepad.wButtons) != 0;
		//for triggers
		controllerState.m_buttonDownStates[XBOX_BUTTON_LEFT_TRIGGER ] = xboxControllerState.Gamepad.bLeftTrigger > 127;
		controllerState.m_buttonDownStates[XBOX_BUTTON_RIGHT_TRIGGER ] = xboxControllerState.Gamepad.bRightTrigger > 127;
		//get joystick	
		float lStickX = NormalizeXBoxControllerStickValue(xboxControllerState.Gamepad.sThumbLX);
		float lStickY = NormalizeXBoxControllerStickValue(xboxControllerState.Gamepad.sThumbLY);
		float rStickX = NormalizeXBoxControllerStickValue(xboxControllerState.Gamepad.sThumbRX);
		float rStickY = NormalizeXBoxControllerStickValue(xboxControllerState.Gamepad.sThumbRY);

		controllerState.m_stickPositions[XBOX_JOYSTICK_LEFT] = Vector2(lStickX,lStickY );
		controllerState.m_stickPositions[XBOX_JOYSTICK_RIGHT] = Vector2(rStickX,rStickY );

	}else if( errorStatus == ERROR_DEVICE_NOT_CONNECTED ){
		controllerState.isPluggedIn = false;
	}
}

///----------------------------------------------------------------------------------------------------------
///mouse helpers

//uses windows.h...move this
bool InputSystem::IsMouseWheelPresent() {
	return (GetSystemMetrics(SM_MOUSEWHEELPRESENT) != 0);
}

//-----------------------------------------------------------------------------------------------------------

//uses windows.h...move this
void InputSystem::ShowMouseCursor() {
	if (m_mouseCursorHidden)
		ShowCursor(true);
}

//-----------------------------------------------------------------------------------------------------------

//uses windows.h...move this
void InputSystem::HideMouseCursor() {
	if (!m_mouseCursorHidden)
		ShowCursor(false);
}

//-----------------------------------------------------------------------------------------------------------

//uses windows.h...move this
const Vector2& InputSystem::GetMousePosition() {
	POINT cursorPos;
	GetCursorPos(&cursorPos);

	float mousePosX = (float)cursorPos.x;
	float mousePosY = (float)cursorPos.y;
	//cursorPos.y

	m_mousePosition = Vector2(mousePosX, mousePosY);

	return m_mousePosition;
}

//-----------------------------------------------------------------------------------------------------------

//uses windows.h...move this
const Vector2 InputSystem::SnapMousePosition(const int& snapmouseX, const int& snapmouseY) {

	SetCursorPos(snapmouseX, snapmouseY);
	m_mousePosition = GetMousePosition();

	return Vector2((float)snapmouseX, (float)snapmouseY);
}

//===========================================================================================================
