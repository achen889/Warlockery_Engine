//==============================================================================================================
//KeyBindingSystem.cpp
//by Albert Chen Mar-22-2016.
//==============================================================================================================

#include "KeyBindingSystem.hpp"

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///Get the input query to use for the type of input
bool GetInputQueryForInputKeyType(const int& key, const InputType& inputKeyType) {

	switch (inputKeyType) {
	case INPUT_TYPE_WAS_RELEASED:
		return (theInputSystem->WasKeyJustReleased(key));
	case INPUT_TYPE_WAS_PRESSED:
		return (theInputSystem->WasKeyJustPressed(key));
	case INPUT_TYPE_PRESSED:
		return (INPUT_IS_KEY_PRESSED(key) && !INPUT_IS_KEY_RELEASED(key));
	case INPUT_TYPE_PRESSED_OR_RELEASED:
		return (INPUT_IS_KEY_RELEASED(key) || INPUT_IS_KEY_PRESSED(key));
	}

	return false;
}


//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///add key binding helper func
void AddKeyBinding(InputActionMap& actionKeyBindingMap,
	const std::string& actionName, const int& key, const InputType& inputKeyType,
	EventCallbackFunc* func, void* args) {

	KeyBindingEventCallback newKeyBindingEntry(key, inputKeyType, func, args);
	newKeyBindingEntry.actionName = actionName;
	newKeyBindingEntry.eventCallback->eventName = actionName;

	//consider doing a find in map first...

	//adds to key binding map
	actionKeyBindingMap.insert(InputActionMapEntry(GetStringID(actionName), newKeyBindingEntry));

}

//-----------------------------------------------------------------------------------------------------------
//this one only calls an event

void AddKeyBinding(InputActionMap& actionKeyBindingMap,
	const std::string& actionName, 
	const int& key, const InputType& inputKeyType, 
	const int& button, const ButtonType& buttonType, 
	NamedProperties& params ) {

	KeyBindingEventCallback newKeyBindingEntry(key, inputKeyType, actionName, params);
	newKeyBindingEntry.button = button;
	newKeyBindingEntry.buttonType = buttonType;
	newKeyBindingEntry.actionName = actionName;
	newKeyBindingEntry.eventCallback->eventName = actionName;

	//consider doing a find in map first...

	//adds to key binding map
	actionKeyBindingMap.insert(InputActionMapEntry(GetStringID(actionName), newKeyBindingEntry));

}

//-----------------------------------------------------------------------------------------------------------

void ProcessInputActionMap(InputActionMap& m_actionBindingMap, void* customArgs){
	//loop through all the actions
	for (InputActionMapIterator it = m_actionBindingMap.begin(); it != m_actionBindingMap.end(); ++it) {
		 KeyBindingEventCallback& actionEvent = (it->second);
		 if (actionEvent.eventCallback) {
			 //optionally change args
			 if (customArgs) {
				 actionEvent.eventCallback->args = customArgs;
			 }
			 //process the event
			 actionEvent.ProcessActionEvent();
		}
	}
}


//===========================================================================================================

