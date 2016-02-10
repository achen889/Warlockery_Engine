//==============================================================================================================
//DevConsole.cpp
//by Albert Chen Jun-16-2015.
//==============================================================================================================

#include "DevConsole.hpp"

#define FIND_COMMAND(name) ConsoleCommand::g_ConsoleCommandMap.find(commandName)
#define COMMAND_MAP_END ConsoleCommand::g_ConsoleCommandMap.end()

ConsoleCommandMap ConsoleCommand::g_ConsoleCommandMap;
std::string ConsoleCommand::g_helpText;

void AppendHelpText(const std::string& info){
	ConsoleCommand::g_helpText += info;
	ConsoleCommand::g_helpText += "\n";

	for(int i = 0; i < 70; i++){
		ConsoleCommand::g_helpText += "-";
	}

	ConsoleCommand::g_helpText += "\n";
}

//-----------------------------------------------------------------------------------------------------------

//make this split argList into arg tokens
void ConsoleArguments::TokenizeArgList(){
	char* tokenInBuffer;
	char* currentValue = currentValue = strtok_s(argList, " \0", &tokenInBuffer);

	
	while (currentValue != NULL){
		if (currentValue == NULL)break;

		argTokens.push_back(currentValue);

		currentValue = strtok_s(NULL, " ", &tokenInBuffer);
	}
}

//-----------------------------------------------------------------------------------------------------------

void RunCommandFromBufferText(std::string& bufferText) {
	AdjustBufferTextForExecution(bufferText);
	if (bufferText != "") {
		char* cStringBufferText = StringToWritableCString(bufferText);
		char* currentValue;
		char* tokenInBuffer;
		currentValue = strtok_s(cStringBufferText, " ", &tokenInBuffer);
		//ConsoleGenericMessageBox(currentValue, "loading token, current value = ");
		std::string commandName = currentValue;
		currentValue = strtok_s(NULL, "\0", &tokenInBuffer);

		printf("Running Command: %s Args: %s\n", commandName.c_str(), currentValue);

		if (currentValue != NULL) {
			RunCommand(commandName, new ConsoleArguments(currentValue));
		}
		else {
			RunCommand(commandName, NULL);
		}
	}
}

//-----------------------------------------------------------------------------------------------------------

void RunCommand(const std::string& commandName, ConsoleArguments* args ){
		if (FIND_COMMAND(commandName) != COMMAND_MAP_END){
			ConsoleCommand* commandToRun = ConsoleCommand::g_ConsoleCommandMap[commandName];
			if (commandToRun){
				commandToRun->m_command(args);
			}
		}
}

//-----------------------------------------------------------------------------------------------------------


//===========================================================================================================

void AdjustBufferTextForExecution(std::string& bufferText){
	if (bufferText.size() > 0 && bufferText[0] < 0)
		bufferText = bufferText.substr(1, bufferText.length());
}