//==============================================================================================================
//StringUtils.hpp
//by Albert Chen Jul-14-2016.
//==============================================================================================================

#pragma once

#ifndef _included_StringUtils__
#define _included_StringUtils__

#include "StringTable.hpp"

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///queries

bool CompareByteArrays(const unsigned char* a, const unsigned char* b, const int& size);
int CompareCStr(const char* a, const char* b);

///----------------------------------------------------------------------------------------------------------
///inline queries

inline bool CompareByteArrays(const unsigned char* a, const unsigned char* b, const int& size) {
	for (int i = 0; i < size; i++) {
		if (a[i] != b[i])return false;
	}
	return true;
}

inline int CompareCStr(const char* a, const char* b) {
	int compareResult = strcmp(a, b);

	return compareResult;
}

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///getters

int GetCStrLength(const char* c);
int GetUCStrLength(const unsigned char* c);

//get file length stuff
int GetFileLength(const std::string& filePath);
int GetFileLength(FILE* fileHandle);


///----------------------------------------------------------------------------------------------------------
///inline getters

inline int GetCStrLength(const char* c) {
	return (int)strlen((char*)c); //gets the loc of the /0
}

inline int GetUCStrLength(const unsigned char* c) {

	//it seems there is no real way to do this

	return (int)strlen((char*)c);
}

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///converter methods

const char* IntToCStr(const int& i, int numberBase = 10);
const std::string IntToString(const int& i, int numberBase = 10);
const std::string UIntToString(const unsigned int& i, int numberBase = 10);

inline const std::string ToString(const int& i, int numberBase = 10) {
	return IntToString(i, numberBase);
}

const std::string ShortToString(const short& t);
const std::string UShortToString(const unsigned short& t);

const std::string FloatToString(const float& i, const char* floatFormat = "%.2f");
const std::string DoubleToString(const double& d, const char* doubleFormat = "%.2f");

inline const std::string ToString(const float& i, const char* floatFormat = "%.2f") {
	return FloatToString(i, floatFormat);
}

const float CStringToFloat(const char* c);
const int   CStringToInt(const char* c);

const int StringToInt(const std::string& s);
const float StringToFloat(const std::string& s);

const std::string CharToString(char c);

char* StringToWritableCStr(const std::string& str);

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///string utilities

void ConvertThisCharToThatCharInString(std::string& stringToConvert, char& thisChar, char& thatChar);
Strings SplitString(std::string str, const std::string& delimiter);
int SplitString(std::string str, const std::string& delimiter, Strings& out_splitStrings);
void SetFromString(float& out_value, const std::string& attrString);

///----------------------------------------------------------------------------------------------------------
///inline string utilities

inline void SetFromString(float& out_value, const std::string& attrString) {
	out_value = StringToFloat(attrString);
}


//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///conversion utilities

inline const char* IntToCStr(const int& i, int numberBase) {
	char* stringIntValue = new char[33];
	_itoa_s(i, stringIntValue, 33, numberBase);

	return stringIntValue;
}

inline const std::string IntToString(const int& i, int numberBase) {
	char stringIntValue[33];
	_itoa_s(i, stringIntValue, 33, numberBase);

	return std::string(stringIntValue);

}

inline const std::string UIntToString(const unsigned int& i, int numberBase) {
	char stringIntValue[33];

	_itoa_s(i, stringIntValue, 33, numberBase);

	std::string s = stringIntValue;
	return s;
}

inline const std::string ShortToString(const short& t) {
	return IntToString((int)t);
}

inline const std::string UShortToString(const unsigned short& t) {
	return UIntToString((unsigned int)t);
}

inline const std::string FloatToString(const float& i, const char* floatFormat) {
	char floatBuffer[2048];
	sprintf_s(floatBuffer, sizeof(floatBuffer), floatFormat, i);
	std::string s = floatBuffer;

	return s;
}

inline const std::string DoubleToString(const double& d, const char* doubleFormat) {
	char doubleBuffer[2048];
	sprintf_s(doubleBuffer, sizeof(doubleBuffer), doubleFormat, d);
	std::string s = doubleBuffer;

	return s;
}

inline const float CStringToFloat(const char* c) {
	return (float)atof(c);
}

inline const int CStringToInt(const char* c) {
	return (int)atof(c);
}

inline const int StringToInt(const std::string& s) {
	if (s.size() == 0)return 0;

	return CStringToInt(s.c_str());
}

inline const int StringToUInt(const std::string& s) {
	if (s.size() == 0)return 0;

	//use string to uint
	return  std::stoul(s, nullptr, 0);
}

inline const float StringToFloat(const std::string& s) {
	if (s.size() == 0)return 0;

	return CStringToFloat(s.c_str());
}

inline const std::string CharToString(char c) {
	std::string s(1, c);

	return s;
}

inline const std::string UCharToString(unsigned char c) {
	std::string s(1, c);

	return s;
}

//===========================================================================================================

#endif //__includedStringUtils__

