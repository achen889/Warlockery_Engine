//==============================================================================================================
//Utilities.cpp
//by Albert Chen Feb-10-2015.
//==============================================================================================================

#include "Utilities.hpp"

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
//string utilities
char* StringToWritableCString(const std::string& str){
	char * cstr = new char[str.length() + 1];
	strcpy_s(cstr, str.length() + 1, str.c_str());

	return cstr;
}

///----------------------------------------------------------------------------------------------------------
///goes through the string converting every instance of this char to that char
void ConvertThisCharToThatCharInString(std::string& stringToConvert, char& thisChar, char& thatChar ){
	//unsigned char forwardSlash = '\'';
	for(unsigned int i = 0; i < stringToConvert.length(); i++ ){
		if(stringToConvert[i] == thisChar ){
			stringToConvert[i] = thatChar;
		}
	}
}

///----------------------------------------------------------------------------------------------------------
///splits string into tokens

std::vector<std::string> SplitString(std::string str, const std::string& delimiter){

	std::vector<std::string> stringTokens;
	//stringTokens.reserve(10000);

	size_t found = str.find(delimiter);
	size_t strIndex = 0;
	while (found != std::string::npos){
		strIndex = 0;
		stringTokens.push_back(str.substr(strIndex, found) );
		
		strIndex = found + delimiter.size();

		str = str.substr(strIndex, str.size());
		
		found = str.find(delimiter);
	}

	strIndex = 0;
	stringTokens.push_back(str.substr(strIndex, found));

	return stringTokens;
	
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///io utilities
bool WriteExistingBufferToTextFile(const char* buffer, size_t bufferSize, const std::string& filePath){
	FILE* file = NULL;
	fopen_s(&file, filePath.c_str(), "wb");//open c-string filepath to write binary.
	if(!file){
		std::string errorText = "Attempted to write buffer to file at path: ";
		errorText += filePath;
		errorText += " write failed!\n";
		ERROR_RECOVERABLE(errorText);
		return false;
	}

	fwrite(buffer, 1, bufferSize, file );

	fclose(file);

	return true;
}


bool WriteExistingUCharBufferToFile(unsigned char* buffer, size_t bufferSize, const std::string& filePath){
	FILE* file = NULL;
	fopen_s(&file, filePath.c_str(), "wb");//open c-string filepath to write binary.
	if (!file){
		std::string errorText = "Attempted to write buffer to file at path: ";
		errorText += filePath;
		errorText += " write failed!\n";
		ERROR_RECOVERABLE(errorText);
		return false;
	}

	fwrite(buffer, 1, bufferSize, file);

	fclose(file);

	return true;
}

//-----------------------------------------------------------------------------------------------------------

bool WriteExistingBufferToTextFile(const std::string& buffer, const std::string& filePath){
	FILE* file = NULL;
	fopen_s(&file, filePath.c_str(), "wb");//open c-string filepath to write binary.
	if (!file){
		// 		ConsolePrintf("attempted to write buffer to file at path: ");
		// 		ConsolePrintf(filePath.c_str());
		// 		ConsolePrintf(" write failed!\n");
		return false;
	}

	fwrite(buffer.c_str(), 1, (size_t)buffer.size(), file);

	fclose(file);

	return true;
}

//-----------------------------------------------------------------------------------------------------------

bool WriteExistingByteVectorToFile(std::vector<unsigned char> existingbyteVector, const std::string& filePath){
	FILE* file = NULL;
	fopen_s(&file, filePath.c_str(), "wb");//open c-string filepath to write binary.
	if (!file){
		// 		ConsolePrintf("attempted to write buffer to file at path: ");
		// 		ConsolePrintf(filePath.c_str());
		// 		ConsolePrintf(" write failed!\n");
		return false;
	}

	

	for (std::vector<unsigned char>::iterator it = existingbyteVector.begin(); it != existingbyteVector.end(); ++it){
		unsigned char c = (unsigned char)(*it);

		fwrite((unsigned char*)c, 1, 1, file);
	}

	fclose(file);

	return true;
}

//-----------------------------------------------------------------------------------------------------------

bool LoadFileToExistingCStr(const std::string& filePath, char* existingBuffer, size_t existingBufferSize ){
	FILE* file = NULL;
	fopen_s(&file, filePath.c_str(), "rb"); //open to read binary
	if(!file){
		// 		ConsolePrintf("attempted to load from file at path: \n");
		// 		ConsolePrintf(filePath.c_str());
		// 		ConsolePrintf(" load failed!\n");
		return false;
	}

	fread(existingBuffer, 1, existingBufferSize, file);

	existingBuffer[existingBufferSize] = '\0';

	fclose(file);


	return true;

}

//-----------------------------------------------------------------------------------------------------------

bool LoadFileToExistingUCharBuffer(const std::string& filePath, unsigned char* existingBuffer, size_t existingBufferSize ){
	FILE* file = NULL;
	fopen_s(&file,filePath.c_str(), "rb"); //open to read binary
	if(!file){
		//std::string loadErrorText = "ERROR: Attempted to load from file at path: " + filePath;
		//std::string loadErrorTitle = "*DEBUG* Load File To Existing Buffer Failed!";
		//ConsoleGenericMessageBox(loadErrorText, loadErrorTitle);
		return false;
	}
	fread(existingBuffer, 1, existingBufferSize, file);
	fclose(file);

	return true;

}

//-----------------------------------------------------------------------------------------------------------

bool LoadBinaryFileToExistingByteVector(const std::string& filePath, std::vector<unsigned char>& existingByteVector){
	FILE* file = NULL;
	fopen_s(&file, filePath.c_str(), "rb"); //open to read binary
	if (!file){
		//std::string loadErrorText = "ERROR: Attempted to load from file at path: " + filePath;
		//std::string loadErrorTitle = "*DEBUG* Load File To Existing Buffer Failed!";
		//ConsoleGenericMessageBox(loadErrorText, loadErrorTitle);
		return false;
	}
// 	//load to temp buffer
 	size_t fileSize = GetFileLength(file);

	fread(&existingByteVector.front(), 1, fileSize, file);

	//delete tempBuffer;

	fclose(file);

	return true;
}

//-----------------------------------------------------------------------------------------------------------

bool LoadFileToExistingString(const std::string& filePath, std::string& existingBuffer){
	FILE* file = NULL;
	fopen_s(&file, filePath.c_str(), "rb"); //open to read binary
	if (!file){
		//std::string loadErrorText = "ERROR: Attempted to load from file at path: " + filePath;
		//std::string loadErrorTitle = "*DEBUG* Load File To Existing Buffer Failed!";
		//ConsoleGenericMessageBox(loadErrorText, loadErrorTitle);
		return false;
	}
	//load to temp buffer
	size_t fileSize = GetFileLength(file);

	if (existingBuffer.size() < fileSize) existingBuffer.resize(fileSize, ' ');

	//std::stringstream ss;

	fread(&existingBuffer[0], 1, fileSize, file);

	fclose(file);

	return true;
}

//-----------------------------------------------------------------------------------------------------------

char* LoadTextFileToNewBuffer(const std::string& filePath ){
	FILE* file = NULL;
	fopen_s(&file, filePath.c_str(), "rb"); //open to read binary
	//debug
	if (!file){
		std::string loadErrorText = "ERROR: Attempted to load from file at path: " + filePath;
		const std::string loadErrorTitle = "*DEBUG* Load File To New Buffer Failed!";
		ConsoleGenericMessageBox(loadErrorText.c_str(), loadErrorTitle.c_str());
		return false;
	}
	//create buffer
	size_t fileLength = GetFileLength(file);
	char* newBuffer = new char[fileLength + 1];

	//buffer, size, count, handle
	fread(newBuffer, 1, fileLength, file);

	newBuffer[fileLength] = '\0';

	fclose(file);
	
	return newBuffer;
}

//-----------------------------------------------------------------------------------------------------------

unsigned char* LoadBinaryFileToNewBuffer(const std::string& filePath, size_t& out_bufferSize ){
	FILE* file = NULL;
	fopen_s(&file, filePath.c_str(), "rb"); //open to read binary
	//debug
	if (!file){
		std::string loadErrorText = "ERROR: Attempted to load from binary file at path: " + filePath;
		const std::string loadErrorTitle = "*DEBUG* Load File To New Buffer Failed!\n" +loadErrorText;
		ERROR_RECOVERABLE(loadErrorTitle);
		return false;
	}
	//create buffer
	size_t fileLength = GetFileLength(file);
	unsigned char* newBuffer = new unsigned char[fileLength];

	//buffer, size, count, handle
	out_bufferSize = fread(newBuffer, 1, fileLength, file);

	fclose(file);

	return newBuffer;
}

//-----------------------------------------------------------------------------------------------------------

int GetFileLength(const std::string& filePath){
	FILE* file = NULL;
	fopen_s(&file, filePath.c_str(), "rb"); //open to read binary

	fseek(file, 0, SEEK_END);
	int fileLength = ftell(file);
	fclose(file);

	return fileLength;
}

//-----------------------------------------------------------------------------------------------------------

//assumes file is already open and will be closed later
int GetFileLength(FILE* fileHandle){
	
	fseek(fileHandle, 0, SEEK_END);

	int fileLength = ftell(fileHandle);
	//fclose(file);
	rewind(fileHandle);

	return fileLength;
}

//-----------------------------------------------------------------------------------------------------------
