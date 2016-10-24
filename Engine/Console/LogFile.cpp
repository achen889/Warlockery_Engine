//==============================================================================================================
//LogFile.cpp
//by Squirrel Eiserloh
//==============================================================================================================

//adapted for use by Albert Chen

#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include "LogFile.hpp"

//#include "Engine/Core/Utilities.hpp"

//-----------------------------------------------------------------------------------------------
std::string g_logFileName = "Warlockery_Game_LogFile.txt";

bool g_enableLog = false;

///----------------------------------------------------------------------------------------------------------
///sets the file name
void SetLogFileName(const std::string& newFileName){
	g_logFileName = newFileName;
}

//-----------------------------------------------------------------------------------------------
// Opens, writes, flushes, and closes the file so the log is guaranteed to be up-to-date, even
//	if the very next line is a crash.
void AppendTextLineToLogFile(const std::string& textLine, const std::string& newLogFileName){
	if (!g_enableLog)return;
	FILE* file = nullptr;
	errno_t errorCode = fopen_s(&file, newLogFileName.c_str(), "at");
	if( !errorCode )
	{
		fputs( textLine.c_str(), file );
		fflush( file );
		fclose( file );
	}
}

//-----------------------------------------------------------------------------------------------
// Resets (wipes clean) the log file
void WipeLogFile()
{
	FILE* file = nullptr;
	errno_t errorCode = fopen_s( &file, g_logFileName.c_str(), "wb" );
	if( !errorCode )
	{
		fflush( file );
		fclose( file );
	}
}

//-----------------------------------------------------------------------------------------------
void LogFilePrintf(const char* messageFormat, ...){
	if (!g_enableLog)return;
	const int MESSAGE_MAX_LENGTH = 2048;
	char messageLiteral[ MESSAGE_MAX_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, messageFormat );
	vsnprintf_s( messageLiteral, MESSAGE_MAX_LENGTH, _TRUNCATE, messageFormat, variableArgumentList );
	va_end( variableArgumentList );
	messageLiteral[ MESSAGE_MAX_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)
	AppendTextLineToLogFile( messageLiteral);
}

//-----------------------------------------------------------------------------------------------------------

void LogFilePrintString(const std::string& messageFormat ){

	LogFilePrintf(messageFormat.c_str());

}

//===========================================================================================================