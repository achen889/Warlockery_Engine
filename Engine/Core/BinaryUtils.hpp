//==============================================================================================================
//BinaryUtils.hpp
//by Albert Chen Aug-17-2015.
//==============================================================================================================

#pragma once

#ifndef _included_BinaryUtils__
#define _included_BinaryUtils__

//includes
#include <vector>
#include "Utilities.hpp"
#include "Engine/Math/Matrix4.hpp"

//===========================================================================================================

//consts
static const unsigned int SIZE_OF_INT = sizeof(int);
static const unsigned int SIZE_OF_CHAR = sizeof(char);
static const unsigned int SIZE_OF_SHORT = sizeof(short);
static const unsigned int SIZE_OF_FLOAT = sizeof(float);
static const unsigned int SIZE_OF_DOUBLE = sizeof(double);
static const unsigned int SIZE_OF_BOOL = sizeof(bool);

class Matrix4;
struct Vertex3D;
class Rgba;

typedef unsigned char Byte;

//===========================================================================================================

//data is a value not a ptr
#define WRITE_BYTES(data) WriteBytes((void*)&data, SIZE_OF(data))

struct ByteBuffer{
	Byte* buffer = NULL;
	size_t curSize = 0;
	size_t maxSize;
	size_t writeIndex = 0;
	size_t readIndex = 0;

	~ByteBuffer() {
		//delete the buffer
	}

	void Set(Byte* newBuffer, size_t max_size) {
		buffer = newBuffer;
		maxSize = max_size;
	}

	const Byte* GetBuffer() const { return buffer; }

	const size_t GetBufferLength() const { return curSize;  }

	//writes to end of buffer, true means was enough room, false: not enough room
	bool WriteBytes(void* data, size_t dataSize);

	//reads from buffer copying data up to size into data
	//number of bytes read, should equal size 
	size_t ReadBytes(void* data, size_t dataSize);

	//gets the buffer as a series of hex values
	std::string ToStringAsBase(int numberBase ) {
		if (buffer) {
			std::string bufferDebugString = "";
			for (size_t i = 0; i < curSize; i++) {
				int bufferVal = buffer[i];
				bufferDebugString += IntToString(bufferVal, numberBase) + ":";
			}
			bufferDebugString += "";

			return bufferDebugString;
		}
		return "";
	}


};

inline std::string GetBufferAsNumbers(const Byte* buffer, size_t bufferLen , int numBase) {
	if (buffer) {
		std::string bufferDebugString = "";
		for (size_t i = 0; i < bufferLen; i++) {
			int bufferVal = buffer[i];
			bufferDebugString += IntToString(bufferVal, numBase) + ":";
		}
		bufferDebugString += "";

		return bufferDebugString;
	}
	return "";
}


//===========================================================================================================

class BinaryBufferParser{
public:
	//constructors
	BinaryBufferParser();
	~BinaryBufferParser();
	
	BinaryBufferParser(const Byte* buffer, size_t bufferSize);
	BinaryBufferParser(std::vector<Byte> bytes);

	//Read primitive methods
	int ReadNextInt();
	unsigned int ReadNextUInt();
	
	short ReadNextShort();
	unsigned short ReadNextUShort();
	
	char ReadNextChar();
	unsigned char ReadNextUChar();
	
	float ReadNextFloat();	
	double ReadNextDouble();

	bool ReadNextBool();

	std::string ReadNextString();

	const Vector2 ReadNextVec2();
	const Vector3 ReadNextVec3();
	const Vector4 ReadNextVec4();
	
	Matrix4 ReadNextMatrix4();

	Rgba ReadNextRgba();
	const Vertex3D ReadNextVertex3D();

	//read by ref
	int ReadNextInt(int& i);
	unsigned int ReadNextUInt(unsigned int& i);

	short ReadNextShort(short& i);
	unsigned short ReadNextUShort(unsigned short& i);

	char ReadNextChar(char& c);
	unsigned char ReadNextUChar(unsigned char& c);

	float ReadNextFloat(float& f);
	double ReadNextDouble(double& d);

	bool ReadNextBool(bool& b);

	std::string ReadNextString(std::string& s);
	
	Vector2 ReadNextVec2(Vector2& v);
	Vector3 ReadNextVec3(Vector3& v);
	Vector4 ReadNextVec4(Vector4& v);
	Matrix4 ReadNextMatrix4(Matrix4& m);
	
	//getters
	bool IsEndOfBuffer();
	unsigned char* GetBuffer();
	float GetBufferReadPercent();
	
	//vars
	size_t m_bufferSize;
	unsigned char* m_buffer;
	size_t m_currentIndex;

};

///----------------------------------------------------------------------------------------------------------
///inline methods

inline BinaryBufferParser::BinaryBufferParser(){
	//do nothing
}

inline BinaryBufferParser::~BinaryBufferParser(){
	//do nothing
	//delete m_buffer;
}

//getters

inline unsigned char* BinaryBufferParser::GetBuffer(){
// 	unsigned char* outBuffer = new unsigned char[m_bufferSize];
// 	
// 	size_t vectorIndex = 0;
// 	for (std::vector<unsigned char>::iterator it = m_buffer.begin(); it != m_buffer.end(); ++it){
// 		unsigned char bitChar = (*it);
// 		outBuffer[vectorIndex] = bitChar;
// 
// 		vectorIndex++;
// 	}

	return m_buffer;

	//return outBuffer;
}

inline bool BinaryBufferParser::IsEndOfBuffer(){
	if (m_currentIndex == m_bufferSize){
		return true;
	}
	return false;
}

inline float BinaryBufferParser::GetBufferReadPercent(){
	return m_currentIndex / (float)m_bufferSize;
}

//read primitive methods

inline char BinaryBufferParser::ReadNextChar(){

	char c = (char)m_buffer[m_currentIndex];

	m_currentIndex++;

	return c;
}

inline unsigned char BinaryBufferParser::ReadNextUChar(){

	unsigned char c = (unsigned char)m_buffer[m_currentIndex];

	m_currentIndex++;

	return c;
}

inline bool BinaryBufferParser::ReadNextBool(){
	unsigned char boolByte = (char)m_buffer[m_currentIndex];

	if (boolByte == (unsigned char)01)return true;

	return false;
}

inline std::string BinaryBufferParser::ReadNextString(){
	
	std::string stringFromBytes = "";

	//while not at null terminator, this is inefficient
	while (m_buffer[m_currentIndex] != (unsigned char)'\0'){
		stringFromBytes += ReadNextChar();
	}

	m_currentIndex++;

	return stringFromBytes;
}

inline Matrix4 BinaryBufferParser::ReadNextMatrix4(){
	Matrix4 matrixFromBytes;

	matrixFromBytes.m_iBasis = ReadNextVec4();
	matrixFromBytes.m_jBasis = ReadNextVec4();
	matrixFromBytes.m_kBasis = ReadNextVec4();
	matrixFromBytes.m_translation = ReadNextVec4();

	return matrixFromBytes;
}

///----------------------------------------------------------------------------------------------------------
///read by ref

inline int BinaryBufferParser::ReadNextInt(int& i){
	i = ReadNextInt();
}

inline unsigned int BinaryBufferParser::ReadNextUInt(unsigned int& i){
	i = ReadNextUInt();
}

inline short BinaryBufferParser::ReadNextShort(short& i){
	i = ReadNextShort();

	return i;
}

inline unsigned short BinaryBufferParser::ReadNextUShort(unsigned short& i){
	i = ReadNextUShort();

	return i;
}

inline char BinaryBufferParser::ReadNextChar(char& c){
	c = ReadNextChar();

	return c;
}

inline unsigned char BinaryBufferParser::ReadNextUChar(unsigned char& c){
	c = ReadNextUChar();

	return c;
}

inline float BinaryBufferParser::ReadNextFloat(float& f){
	f = ReadNextFloat();

	return f;
}

inline double BinaryBufferParser::ReadNextDouble(double& d){
	d = ReadNextDouble();

	return d;
}

inline bool BinaryBufferParser::ReadNextBool(bool& b){
	b = ReadNextBool();

	return b;
}

inline std::string BinaryBufferParser::ReadNextString(std::string& s){
	s = ReadNextString();

	return s;
}

inline Vector2 BinaryBufferParser::ReadNextVec2(Vector2& v){
	v = ReadNextVec2();
}

inline Vector3 BinaryBufferParser::ReadNextVec3(Vector3& v){
	v = ReadNextVec3();
}

inline Vector4 BinaryBufferParser::ReadNextVec4(Vector4& v){
	v = ReadNextVec4();
}

inline Matrix4 BinaryBufferParser::ReadNextMatrix4(Matrix4& m){
	m = ReadNextMatrix4();
}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================

class BinaryFileParser : public BinaryBufferParser {
public:
	//constructors
	BinaryFileParser(const std::string& filePath );
	
	BinaryFileParser();
	~BinaryFileParser();


private:
	//std::vector<unsigned char> m_buffer;
};

///----------------------------------------------------------------------------------------------------------
///inline methods

inline BinaryFileParser::BinaryFileParser(){
	//do nothing
}

inline BinaryFileParser::~BinaryFileParser(){
	//call inherited destructor
	BinaryBufferParser::~BinaryBufferParser();
}

//===========================================================================================================
class BinaryBufferBuilder{
public:
	//constructors
	BinaryBufferBuilder();

	~BinaryBufferBuilder();

	BinaryBufferBuilder(const std::string& filePath);
	BinaryBufferBuilder(const unsigned char* existingBuffer);
	BinaryBufferBuilder(const std::string& filePath, const unsigned char* existingBuffer);
	
	//output

	std::string ByteVectorToString(bool useHex = false);
	void CopyByteVectorToByteBuffer(Byte* bufferToCopyTo, size_t bufferToCopyToSize);

	//write buffer
	void WriteBufferToFile(const std::string& filePath );
	void WriteBufferToFile();
	//load existing, generally appends
	void LoadExistingBufferToByteVector(const unsigned char* buffer, bool clearByteVector = false);

	//Write primitive types methods
	
	void WriteIntToBuffer(const int& intToWrite);
	void WriteUIntToBuffer(const unsigned int& intToWrite);

	void WriteShortToBuffer(const short& shortToWrite);
	void WriteUShortToBuffer(const unsigned short& shortToWrite);

	void WriteCharToBuffer(const char& charToWrite);
	void WriteUCharToBuffer(const unsigned char& charToWrite);
	void WriteByteToBuffer(const Byte& byteToWrite);

	void WriteFloatToBuffer(const float& floatToWrite);
	void WriteDoubleToBuffer(const double& doubleToWrite);

	void WriteBoolToBuffer(const bool& boolToWrite);

	void WriteStringToBuffer(const std::string& stringToWrite);

	void WriteVectorToBuffer(Vector2 vectorToWrite);
	void WriteVectorToBuffer(Vector3 vectorToWrite);
	void WriteVectorToBuffer(Vector4 vectorToWrite);

	void WriteMatrix4ToBuffer(Matrix4 matrixToWrite);

	//vars
	std::vector<Byte> m_bufferDataBytes;
	std::string m_filePath;
	size_t m_currentIndex = 0;
	size_t m_currentBufferSize = 0;
};

///----------------------------------------------------------------------------------------------------------
///inline methods

inline BinaryBufferBuilder::BinaryBufferBuilder(){

}

inline BinaryBufferBuilder::~BinaryBufferBuilder(){
	//do nothing
}

//---------------------------------------------------------------------------------------------------------- -

inline void BinaryBufferBuilder::WriteCharToBuffer(const char& charToWrite){
	m_bufferDataBytes.reserve(m_currentBufferSize + SIZE_OF_CHAR);
	m_currentBufferSize += SIZE_OF_CHAR;

	unsigned char* charBytes = (unsigned char*)(&charToWrite);

	m_bufferDataBytes.push_back(charBytes[0]);
	m_currentIndex++;

}

inline void BinaryBufferBuilder::WriteUCharToBuffer(const unsigned char& charToWrite){
	m_bufferDataBytes.reserve(m_currentBufferSize + SIZE_OF_CHAR);
	m_currentBufferSize += SIZE_OF_CHAR;
	unsigned char* charBytes = (unsigned char*)(&charToWrite);

	m_bufferDataBytes.push_back(charBytes[0]);
	m_currentIndex++;
}

inline void BinaryBufferBuilder::WriteByteToBuffer(const Byte& byteToWrite) {

	WriteUCharToBuffer(byteToWrite); 

}

inline void BinaryBufferBuilder::WriteBoolToBuffer(const bool& boolToWrite){
	m_bufferDataBytes.reserve(m_currentBufferSize + SIZE_OF_BOOL);
	m_currentBufferSize += SIZE_OF_BOOL;

	unsigned char* boolBytes = (unsigned char*)(&boolToWrite);

	m_bufferDataBytes.push_back(boolBytes[0]);
	m_currentIndex++;
}

inline void BinaryBufferBuilder::WriteStringToBuffer(const std::string& stringToWrite){
	size_t stringSize = stringToWrite.size();

	for (unsigned int i = 0; i < stringSize; i++){
		WriteUCharToBuffer((unsigned char)stringToWrite[i]);
	}

	//add null terminator
	WriteUCharToBuffer((unsigned char)'\0');
}


//===========================================================================================================


#endif