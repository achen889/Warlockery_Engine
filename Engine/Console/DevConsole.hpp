//==============================================================================================================
//DevConsole.hpp
//by Albert Chen Jun-16-2015.
//==============================================================================================================
#pragma once

#ifndef _included_DevConsole__
#define _included_DevConsole__

#include <map>
//#include "Engine/Core/Utilities.hpp"
#include "Engine/Renderer/Text/TextSystem.hpp"

//defined macros for dev console
//Adapted from SD3 Chris Forseth Macro Trick
#define CONSOLE_COMMAND_MAP ConsoleCommand::g_ConsoleCommandMap

#define REGISTER_CONSOLE_COMMAND(name, info) AppendHelpText(STRINGIFY(name)+": "+info); static ConsoleCommand command_##name##(#name, ConsoleCommand_##name## ) //MUST BE USED IN CONSTRUCTOR

#define EXECUTE_CONSOLE_COMMAND(name) if (theTextSystem){ theTextSystem->ExecuteConsoleString(STRINGIFY(name)); }

#define EXECUTE_CONSOLE_COMMAND_STR(name) if (theTextSystem){ theTextSystem->ExecuteConsoleString(name); }

#define CONSOLE_COMMAND(name) static void ConsoleCommand_##name##(ConsoleArguments* args) //MUST BE USED IN SOURCE FILES

//===========================================================================================================

void AppendHelpText(const std::string& info);

//===========================================================================================================

//Argument definitions
#define ARG_TOKENS args->argTokens
#define NUMBER_OF_ARG_TOKENS (int)args->argTokens.size()
#define UNUSED_COMMAND_ARGS UNUSED(args);

#define COMMAND_HAS_ARGS args != NULL
#define ARG_TOKEN_F(i) args->GetTokenAsFloat(i)
#define ARG_TOKEN_I(i) args->GetTokenAsInt(i)
#define ARG_TOKEN_S(i) args->GetTokenAsString(i)
#define ARG_TOKEN_B(i) args->GetTokenAsByte(i)

struct ConsoleArguments{
	
	char* argList;

	std::vector<char*>argTokens;

	ConsoleArguments(char* args) :
		argList(args)
	{
		TokenizeArgList();
	}
	
	void TokenizeArgList();
	
	float GetTokenAsFloat(int i){
		return CStringToFloat(argTokens[i]);
	}


	int GetTokenAsInt(int i){
		return CStringToInt(argTokens[i]);
	}

	Byte GetTokenAsByte(int i) {
		return (Byte)argTokens[i][0];
	}

	std::string GetTokenAsString(int i){
		std::string tokenText = argTokens[i];
		return tokenText;
	}

// 	void GetArgListAs(const ArgType& argType){
// 
// 	}


};

//===========================================================================================================

typedef void(*ExecuteConsoleCommand)(ConsoleArguments* args);

struct ConsoleCommand{

	const char* m_name;
	ExecuteConsoleCommand m_command;

	ConsoleCommand(){
		//do nothing
	}

	ConsoleCommand(const char* myname):
	m_name(myname)
	{

		g_ConsoleCommandMap[m_name] = this;
	}

	ConsoleCommand(const char* myname, ExecuteConsoleCommand command ) :
		m_name(myname)
	{

		m_command = command;

		g_ConsoleCommandMap[m_name] = this;
	}



static std::map<std::string, ConsoleCommand*>  g_ConsoleCommandMap;
static std::string g_helpText;

};

typedef std::map<std::string, ConsoleCommand*> ConsoleCommandMap;


//===========================================================================================================

//-----------------------------------------------------------------------------------------------------------

void RunCommandFromBufferText(std::string& bufferText);

//-----------------------------------------------------------------------------------------------------------

void RunCommand(const std::string& commandName, ConsoleArguments* args );

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================

void AdjustBufferTextForExecution(std::string& bufferText);

//===========================================================================================================

#endif