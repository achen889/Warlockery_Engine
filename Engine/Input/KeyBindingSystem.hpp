//==============================================================================================================
//KeyBindingSystem.hpp
//by Albert Chen Mar-22-2016.
//==============================================================================================================

#pragma once

#ifndef _included_KeyBindingSystem__
#define _included_KeyBindingSystem__

#include "InputSystem.hpp"

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///key binding system is limited to keyboard inputs

//key input types
enum InputType {
	INPUT_TYPE_WAS_RELEASED,
	INPUT_TYPE_WAS_PRESSED,
	INPUT_TYPE_PRESSED,
	INPUT_TYPE_PRESSED_OR_RELEASED,
	NUM_INPUT_TYPES
};

enum ButtonType {
	BUTTON_TYPE_PRESSED,
	BUTTON_TYPE_RELEASED,
	NUM_BUTTON_TYPES
};

bool GetInputQueryForInputKeyType(const int& key, const InputType& inputKeyType);

//===========================================================================================================

struct KeyBindingEventCallback {
	//vars
	int key;
	int button;
	ButtonType buttonType;
	std::string actionName = "";
	InputType keyType;
	EventCallback* eventCallback;

	//methods

	//bind event name with params
	KeyBindingEventCallback(const int& newkey, const InputType& inputKeyType, const std::string& eventName, NamedProperties& params ) :
		key(newkey),
		keyType(inputKeyType),
		actionName(eventName),
		eventCallback(new EventCallback(eventName, params))
	{

	}

	//bind specific callback func
	KeyBindingEventCallback(const int& newkey, const InputType& inputKeyType,
		EventCallbackFunc* func, void* args) :
		key(newkey),
		keyType(inputKeyType),
		eventCallback(new EventCallback(actionName, func, args))
	{

	}

	~KeyBindingEventCallback() {
// 		if (eventCallback) {
// 			delete eventCallback;
// 			eventCallback = NULL;
// 		}
	}

	bool DidInputAction() {
		return GetInputQueryForInputKeyType(key, keyType);
	}

	bool DidButtonAction() {
		static bool lastFrameButtonState = false;
		bool currentFrameButtonState = theInputSystem->IsButtonDown(button);

		bool buttonStateChanged;
		if (lastFrameButtonState != currentFrameButtonState) {
			buttonStateChanged = true;
		}
		else {
			buttonStateChanged = false;
		}

		lastFrameButtonState = currentFrameButtonState;

	
		if(buttonType == BUTTON_TYPE_PRESSED){
			return currentFrameButtonState;
		}
		else if (buttonType == BUTTON_TYPE_RELEASED) {
			return buttonStateChanged;
		}
		return false;
	}

	void ProcessActionEvent() {
		//the action key must be hit to execute the event
		if (DidInputAction() || DidButtonAction()) {
			//call the fixed callback
			if (eventCallback) {
				eventCallback->Execute();
			}
		}//end of outer if
	}

};


//===========================================================================================================
///Input Action Bindings
typedef std::map<int, KeyBindingEventCallback > InputActionMap;
typedef std::map<int, KeyBindingEventCallback >::iterator InputActionMapIterator;
typedef std::pair<int, KeyBindingEventCallback > InputActionMapEntry;

///----------------------------------------------------------------------------------------------------------
///input action helpers

void AddKeyBinding(
	InputActionMap& actionKeyBindingMap,
	const std::string& actionName,
	const int& key,
	const InputType& inputKeyType,
	EventCallbackFunc* func, void* args);


void AddKeyBinding(
	InputActionMap& actionKeyBindingMap,
	const std::string& actionName,
	const int& key,
	const InputType& inputKeyType,
	const int& button = 0,
	const ButtonType& buttonType = BUTTON_TYPE_PRESSED,
	NamedProperties& params = NamedProperties::EMPTY);

//lets you process a map of input actions and optionally change the event callback args
void ProcessInputActionMap(InputActionMap& m_actionBindingMap, void* customArgs = NULL);

//===========================================================================================================

#endif //__includedKeyBindingSystem__

