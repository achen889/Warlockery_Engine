//==============================================================================================================
//LogFile.hpp
//by Squirrel Eiserloh
//==============================================================================================================

//Code by Squirrel Eiserloh, adapted and extended by Albert Chen

#pragma once

#ifndef _included_LogFile__
#define _included_LogFile__

#include <string>

//-----------------------------------------------------------------------------------------------
extern std::string g_logFileName;

///----------------------------------------------------------------------------------------------------------
///log methods
void SetLogFileName(const std::string& newFileName);

void AppendTextLineToLogFile(const std::string& textLine, const std::string& newLogFileName = g_logFileName );

void WipeLogFile();

void LogFilePrintf(const char* messageFormat, ...);

void LogFilePrintString(const std::string& messageFormat );

#endif //__includedLogFile__




