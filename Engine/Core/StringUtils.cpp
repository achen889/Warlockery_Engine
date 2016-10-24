//==============================================================================================================
//StringUtils.cpp
//by Albert Chen Jul-14-2016.
//==============================================================================================================

#include "StringUtils.hpp"

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
//string utilities

char* StringToWritableCStr(const std::string& str) {
	char * cstr = new char[str.length() + 1];
	strcpy_s(cstr, str.length() + 1, str.c_str());

	return cstr;
}

///----------------------------------------------------------------------------------------------------------
///goes through the string converting every instance of this char to that char
void ConvertThisCharToThatCharInString(std::string& stringToConvert, char& thisChar, char& thatChar) {
	//unsigned char forwardSlash = '\'';
	for (unsigned int i = 0; i < stringToConvert.length(); i++) {
		if (stringToConvert[i] == thisChar) {
			stringToConvert[i] = thatChar;
		}
	}
}

///----------------------------------------------------------------------------------------------------------
///splits string into tokens

Strings SplitString(std::string str, const std::string& delimiter) {

	Strings stringTokens;
	//stringTokens.reserve(10000);

	size_t found = str.find(delimiter);
	size_t strIndex = 0;
	while (found != std::string::npos) {
		strIndex = 0;
		stringTokens.push_back(str.substr(strIndex, found));

		strIndex = found + delimiter.size();

		str = str.substr(strIndex, str.size());

		found = str.find(delimiter);
	}

	strIndex = 0;
	stringTokens.push_back(str.substr(strIndex, found));

	return stringTokens;

}

//-----------------------------------------------------------------------------------------------------------

//returns the number of out_stringTokens
int SplitString(std::string str, const std::string& delimiter, Strings& out_stringTokens) {
	size_t found = str.find(delimiter);
	size_t strIndex = 0;
	while (found != std::string::npos) {
		strIndex = 0;
		out_stringTokens.push_back(str.substr(strIndex, found));

		strIndex = found + delimiter.size();

		str = str.substr(strIndex, str.size());

		found = str.find(delimiter);
	}

	strIndex = 0;
	out_stringTokens.push_back(str.substr(strIndex, found));

	return out_stringTokens.size();
}

//===========================================================================================================

