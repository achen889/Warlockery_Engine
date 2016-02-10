//==============================================================================================================
//Console.cpp
//by Albert Chen Feb-17-2015.
//==============================================================================================================

#include "Console.hpp"

#include <stdio.h>
#include <stdarg.h>

#include "Engine/Input/InputSystem.hpp"

//===========================================================================================================

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///prints to console
void ConsolePrintString(const std::string& messageString){
	ConsolePrintf(messageString.c_str());
}

void ConsolePrintf( const char* messageFormat, ... ){
	//static initializations
	static const size_t BUFFER_SIZE = 2048;
	static char message[BUFFER_SIZE];

	va_list argumentList;
	va_start(argumentList, messageFormat );
		_vsnprintf_s(message, BUFFER_SIZE, BUFFER_SIZE -1, messageFormat, argumentList );
	va_end(argumentList);
	//windows specific
	OutputDebugStringA(message);
	
}

//-----------------------------------------------------------------------------------------------------------

//creates a message box with the given message title and text
//reference for windows: https://msdn.microsoft.com/en-us/library/windows/desktop/ms645505%28v=vs.85%29.aspx
//convert string to char*
int ConsoleGenericMessageBox(const char* messageText, const char* messageTitle, const UINT& messageBoxProperties){
	return MessageBoxA(nullptr, messageText, messageTitle, messageBoxProperties );
}

//-----------------------------------------------------------------------------------------------------------

int ConsoleGenericMessageBox(const std::string& messageText, const std::string& messageTitle, const UINT& messageBoxProperties){
	return MessageBoxA(nullptr, messageText.c_str(), messageTitle.c_str(), messageBoxProperties );
}

//-----------------------------------------------------------------------------------------------------------

//should output to vs output window
void ConsoleOutputDebugString(const std::string& messageText, const char* file, const size_t line){
	std::string messageString = messageText;
	messageString += "\nFile: ";
	messageString += file;
	messageString += "\nLine: " + IntToString(line);

	std::wstring stemp = std::wstring(messageString.begin(), messageString.end());
	LPCWSTR sw = stemp.c_str();

	OutputDebugString(sw);
}

//-----------------------------------------------------------------------------------------------------------

//makes windows play a beep sound, frequency ranges from 0 - 1000 ?_?
void ConsoleBeep(unsigned int frequency, unsigned int durationSeconds){
	Beep(frequency, durationSeconds * 100);
}

//-----------------------------------------------------------------------------------------------------------

std::string BuildError(const std::string& errorText, const char* fileName, const char* functionName, int lineNum){

	std::string fileNameStr = fileName;
	std::string functionNameStr = functionName;

	std::string errorString = "Note: "+errorText +"\n";
	errorString += "==========================================\n";
	errorString += "Error Details\n";
	errorString += "In File: " + fileNameStr + "\n";
	errorString += "In Function: " + functionNameStr + "\n";
	errorString += "On Line: " + IntToString(lineNum) + "\n";

	return errorString;
}

//-----------------------------------------------------------------------------------------------------------

void ExecuteErrorAndDie(const std::string& errorText){
	std::string fullErrorString;
	fullErrorString += "==========================================\n";
	fullErrorString += errorText +"\n";
	fullErrorString += "==========================================\n";
	fullErrorString += "The Game will now Quit.";

	int outParam = ConsoleGenericMessageBox(fullErrorString, "===FATAL ERROR===", MB_OK | MB_ICONERROR );
	//quit on ok
	if (outParam == IDOK){
		//quit game
		exit(1);
		//IF_INPUT_SYSTEM_EXISTS{ theInputSystem->SetSystemQuit(); }
	}
}

//-----------------------------------------------------------------------------------------------------------

void ExecuteRecoverableError(const std::string& errorText){
	std::string fullErrorString;
	fullErrorString += "==========================================\n";
	fullErrorString += errorText +"\n";
	fullErrorString += "==========================================\n";
	fullErrorString += "Do you wish to continue? (Y/N)";

	int outParam = ConsoleGenericMessageBox(fullErrorString, "===RECOVERABLE ERROR===", MB_YESNO | MB_ICONWARNING );
	if (outParam == IDNO){
		//quit game
		exit(1);
		//IF_INPUT_SYSTEM_EXISTS{ theInputSystem->SetSystemQuit(); }
	}
}

//-----------------------------------------------------------------------------------------------------------

void ExecuteAssertAndDie(const std::string& assertText, const std::string& assertConditionString, bool assertCondition){
	if (!assertCondition){
		std::string fullAssertString;
		fullAssertString += "==========================================\n";
		fullAssertString += assertText + "\n";
		fullAssertString += "Condition( "+assertConditionString+")\n";
		fullAssertString += "==========================================\n";
		fullAssertString += "The Game will now Quit.";

		//print stuff
		int outParam = ConsoleGenericMessageBox(fullAssertString, "===FATAL Assert===", MB_OK | MB_ICONERROR);
		if (outParam == IDOK){
			//quit game
			exit(1);
			//IF_INPUT_SYSTEM_EXISTS{ theInputSystem->SetSystemQuit(); }
		}
	}
}

//-----------------------------------------------------------------------------------------------------------

void ExecuteAssertRecoverable(const std::string& assertText, const std::string& assertConditionString, bool assertCondition){
	if (!assertCondition){
		std::string fullAssertString;
		fullAssertString += "==========================================\n";
		fullAssertString += assertText + "\n";
		fullAssertString += "Condition( " + assertConditionString + ")\n";
		fullAssertString += "==========================================\n";
		fullAssertString += "Do you wish to continue? (Y/N)";

		int outParam = ConsoleGenericMessageBox(fullAssertString, "===RECOVERABLE ASSERT===", MB_YESNO | MB_ICONWARNING);
		if (outParam == IDNO){
			//quit game
			exit(1);
			//IF_INPUT_SYSTEM_EXISTS{ theInputSystem->SetSystemQuit(); }
		}

	}
}

//===========================================================================================================