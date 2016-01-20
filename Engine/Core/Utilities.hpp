
//==============================================================================================================
//Utilities.hpp
//by Albert Chen Feb-10-2015.
//==============================================================================================================

#ifndef _included_Utilities__
#define _included_Utilities__

#include "Engine\Console\Console.hpp"
//#include "Engine\Core\Memory.hpp"
#include "Engine\Core\EventCallback.hpp"
#include "Engine\Core\StringTable.hpp"
#include "Engine\Core\Performance.hpp"
#include <string>
#include <stdio.h>
#include <iosfwd>
#include <vector>

//-----------------------------------------------------------------------------------------------------------
//common file paths
const std::string COMMON_TEXTURE_FILE_PATH = "Data/Images/";
const std::string COMMON_MODEL_FILE_PATH = "Data/Models/";
const std::string COMMON_SHADER_FILE_PATH = "Data/Shaders/";

typedef std::vector<std::string> Strings;
typedef std::vector<std::string>::iterator StringsIterator;

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///queries

bool CompareByteArrays(const unsigned char* a, const unsigned char* b, const int& size);
int CompareCStr(const char* a, const char* b);

///----------------------------------------------------------------------------------------------------------
///getters

int GetCStrLength(const char* c);
int GetUCStrLength(const unsigned char* c);

//get file length stuff
int GetFileLength(const std::string& filePath);
int GetFileLength(FILE* fileHandle);

///----------------------------------------------------------------------------------------------------------
///converter methods

const char* IntToCStr(const int& i , int numberBase = 10);
const std::string IntToString(const int& i , int numberBase = 10);
const std::string UIntToString(const unsigned int& i, int numberBase = 10);

const std::string ShortToString(const short& t);
const std::string UShortToString(const unsigned short& t);

const std::string FloatToString(const float& i, const char* floatFormat = "%.2f" );
const std::string DoubleToString(const double& d);

const float CStringToFloat(const char* c); 
const int   CStringToInt(const char* c);

const int StringToInt(const std::string& s);
const float StringToFloat(const std::string& s);

const std::string CharToString(char c);

//string utilities
char* StringToWritableCString(const std::string& str);
void ConvertThisCharToThatCharInString(std::string& stringToConvert, char& thisChar, char& thatChar );
std::vector<std::string> SplitString(std::string s, const std::string& delimiter);


///----------------------------------------------------------------------------------------------------------
///i/o save/load

bool WriteExistingBufferToTextFile(const std::string& buffer, const std::string& filePath);
bool WriteExistingBufferToTextFile(const char* buffer, size_t bufferSize, const std::string& filePath);
bool WriteExistingUCharBufferToFile(unsigned char* buffer, size_t bufferSize, const std::string& filePath);
bool WriteExistingByteVectorToFile(std::vector<unsigned char> existingbyteVector, const std::string& filePath);

//existing buffers
bool LoadFileToExistingCStr(const std::string& filePath, char* existingBuffer, size_t existingBufferSize);
bool LoadFileToExistingUCharBuffer(const std::string& filePath, unsigned char* existingBuffer, size_t existingBufferSize );
bool LoadBinaryFileToExistingByteVector(const std::string& filePath, std::vector<unsigned char>& existingByteVector);
bool LoadFileToExistingString(const std::string& filePath, std::string& existingBuffer);
//new buffers
char* LoadTextFileToNewBuffer(const std::string& filePath );
unsigned char* LoadBinaryFileToNewBuffer(const std::string& filePath, size_t& out_bufferSize);


//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline utilities

inline bool CompareByteArrays(const unsigned char* a, const unsigned char* b, const int& size){
	for (int i = 0; i < size; i++){
		if (a[i] != b[i])return false;
	}
	return true;
}

inline int CompareCStr(const char* a, const char* b){
	int compareResult = strcmp(a, b);

	return compareResult;
}

///----------------------------------------------------------------------------------------------------------
///getters

inline int GetCStrLength(const char* c){
	return (int)strlen((char*)c);
}


inline int GetUCStrLength(const unsigned char* c){
	return (int)strlen((char*)c);
}

///----------------------------------------------------------------------------------------------------------
///conversion utilities
inline const char* IntToCStr(const int& i , int numberBase ){
	char* stringIntValue = new char [33]; 
	_itoa_s(i,stringIntValue,33, numberBase );

	return stringIntValue;
}

inline const std::string IntToString(const int& i , int numberBase ){
	char stringIntValue[33]; 
	_itoa_s(i,stringIntValue, 33 ,numberBase );

	std::string s = stringIntValue;
	return s;
}

inline const std::string UIntToString(const unsigned int& i, int numberBase){
	char stringIntValue[33];
	
	_itoa_s(i, stringIntValue, 33, numberBase);

	std::string s = stringIntValue;
	return s;
}

inline const std::string ShortToString(const short& t){
	return IntToString((int)t);
}

inline const std::string UShortToString(const unsigned short& t){
	return UIntToString((unsigned int)t);
}

inline const std::string FloatToString(const float& i, const char* floatFormat){
	char floatBuffer[2048];
	sprintf_s(floatBuffer,SIZE_OF(floatBuffer), floatFormat, i);
	std::string s = floatBuffer;

	return s;
}

inline const std::string DoubleToString(const double& d){
	char doubleBuffer[2048];
	sprintf_s(doubleBuffer, SIZE_OF(doubleBuffer), "%.4f", d);
	std::string s = doubleBuffer;

	return s;
}

inline const float CStringToFloat(const char* c){
	return (float)atof(c);
}

inline const int CStringToInt(const char* c){
	return (int)atof(c);
}

inline const int StringToInt(const std::string& s){
	if (s.size() == 0)return 0;

	return CStringToInt(s.c_str());
}

inline const float StringToFloat(const std::string& s){
	if (s.size() == 0)return 0;

	return CStringToFloat(s.c_str());
}

inline const std::string CharToString(char c){
	std::string s(1, c);

	return s;
}

inline const std::string UCharToString(unsigned char c){
	std::string s(1, c);

	return s;
}


#endif