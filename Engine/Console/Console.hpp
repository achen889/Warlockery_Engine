//==============================================================================================================
//Console.hpp
//by Albert Chen Feb-17-2015.
//==============================================================================================================
#pragma once

#ifndef _included_Console__
#define _included_Console__

#include <assert.h>
#include <wtypes.h>

#include "Engine/Console/LogFile.hpp"
#include "Engine/Core/Performance.hpp"

#define SIZE_OF(name) sizeof(name)
#define UNUSED(x) (void)(x)
#define STRINGIFY(name) (std::string)#name
#define TOKEN_PASTE(a, b) a ## b
#define CONCAT_TOKENS(a, b) TOKEN_PASTE(a, b)

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///#define macros

#define FUNCTION_NAME __FUNCTION__

#define FILE_NAME __FILE__

#define LINE_NUM __LINE__

#define BUILD_ERROR(errorText) BuildError(errorText, FILE_NAME, FUNCTION_NAME, LINE_NUM)

#define ERROR_AND_DIE(errorText) ExecuteErrorAndDie(BUILD_ERROR(errorText) )

#define ERROR_RECOVERABLE(errorText) ExecuteRecoverableError(BUILD_ERROR(errorText) )

#define ASSERT_AND_DIE( condition ,messageText)  ExecuteAssertAndDie(messageText, STRINGIFY(condition) , condition )
#define ASSERT_RECOVERABLE( condition ,messageText) ExecuteAssertRecoverable(messageText, STRINGIFY(condition) , condition )
#define ASSERT_PTR_VALID(name) ExecuteAssertRecoverable(STRINGIFY(name is a valid ptr), STRINGIFY(name != NULL) , name != NULL )

#define OUTPUT_DEBUG_STRING(errorText, FILE_NAME, LINE_NUM )

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///printf for console
void ConsolePrintString(const std::string& messageString);

void ConsolePrintf( const char* messageFormat, ... );

int ConsoleGenericMessageBox(const char* messageText, const char* messageTitle = "", const UINT& messageBoxProperties = MB_OK );

int ConsoleGenericMessageBox(const std::string& messageText, const std::string& messageTitle = "", const UINT& messageBoxProperties = MB_OK);

void ConsoleBeep(unsigned int frequency = 500, unsigned int duration = 1);

///----------------------------------------------------------------------------------------------------------
///error handling

std::string BuildError(const std::string& errorText, const char* fileName, const char* functionName, int lineNum);

void ExecuteErrorAndDie(const std::string& errorText);

void ExecuteRecoverableError(const std::string& errorText);

void ExecuteAssertAndDie(const std::string& assertText, const std::string& assertConditionString, bool assertCondition);

void ExecuteAssertRecoverable(const std::string& assertText, const std::string& assertConditionString, bool assertCondition);

//===========================================================================================================

#endif