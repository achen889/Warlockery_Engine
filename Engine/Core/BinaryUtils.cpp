//==============================================================================================================
//BinaryUtils.cpp
//by Albert Chen Aug-17-2015.
//==============================================================================================================

#include "BinaryUtils.hpp"

#include "Engine/Renderer/Vertex3D.hpp"

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Binary buffer parser methods

///----------------------------------------------------------------------------------------------------------
///constructors
BinaryBufferParser::BinaryBufferParser(const unsigned char* buffer, size_t bufferSize){
	UNUSED(bufferSize);

	*m_buffer = *buffer;
	
}

BinaryBufferParser::BinaryBufferParser(std::vector<unsigned char> bytes){
	m_bufferSize = bytes.size();
	m_currentIndex = 0;
	m_buffer = new unsigned char[m_bufferSize];
	//m_buffer.reserve(m_bufferSize);
	//creates unsigned char* from bytes
	size_t vectorIndex = 0;
	for (std::vector<unsigned char>::iterator it = bytes.begin(); it != bytes.end(); ++it){
		unsigned char bitChar = (*it);

		//m_buffer.push_back(bitChar);// = bitChar;
		m_buffer[vectorIndex] = bitChar;
		vectorIndex++;
	}

}

///----------------------------------------------------------------------------------------------------------
///read primitive methods

//will do crazy things if it isn't really what you think it is

int BinaryBufferParser::ReadNextInt(){
	unsigned char* integerBytes = new unsigned char[SIZE_OF_INT];
	
	for (int i = 0; i < SIZE_OF_INT; i++){
		integerBytes[i] = m_buffer[m_currentIndex];

		m_currentIndex++;
	}

	//fancy way of getting an int
	return *(int*)integerBytes;
	
}

//-----------------------------------------------------------------------------------------------------------

unsigned int BinaryBufferParser::ReadNextUInt(){
	unsigned char* integerBytes = new unsigned char[SIZE_OF_INT];

	for (int i = 0; i < SIZE_OF_INT; i++){
		integerBytes[i] = m_buffer[m_currentIndex];

		m_currentIndex++;
	}

	//fancy way of getting an int
	return *(unsigned int*)integerBytes;
}

//-----------------------------------------------------------------------------------------------------------

short BinaryBufferParser::ReadNextShort(){
	unsigned char* shortBytes = new unsigned char[SIZE_OF_SHORT];

	for (int i = 0; i < SIZE_OF_SHORT; i++){
		shortBytes[i] = m_buffer[m_currentIndex];

		m_currentIndex++;
	}

	//fancy way of getting an int
	return *(short*)shortBytes;
}

//-----------------------------------------------------------------------------------------------------------

unsigned short BinaryBufferParser::ReadNextUShort(){
	unsigned char* shortBytes = new unsigned char[SIZE_OF_SHORT];

	for (int i = 0; i < SIZE_OF_SHORT; i++){
		shortBytes[i] = m_buffer[m_currentIndex];

		m_currentIndex++;
	}

	//fancy way of getting an int
	return *(unsigned short*)shortBytes;
}

//-----------------------------------------------------------------------------------------------------------

float BinaryBufferParser::ReadNextFloat(){
	unsigned char* floatBytes = new unsigned char[SIZE_OF_FLOAT];

	for (int i = 0; i < SIZE_OF_FLOAT; i++){
		floatBytes[i] = m_buffer[m_currentIndex];

		m_currentIndex++;
	}

	//fancy way of getting a float
	return *(float*)floatBytes;

}

//-----------------------------------------------------------------------------------------------------------

double BinaryBufferParser::ReadNextDouble(){
	unsigned char* doubleBytes = new unsigned char[SIZE_OF_DOUBLE];

	for (int i = 0; i < SIZE_OF_DOUBLE; i++){
		doubleBytes[i] = m_buffer[m_currentIndex];

		m_currentIndex++;
	}

	//fancy way of getting a float
	return *(double*)doubleBytes;

}

//-----------------------------------------------------------------------------------------------------------

const Vector2 BinaryBufferParser::ReadNextVec2(){
	Vector2 vectorFromBytes;

	vectorFromBytes.x = ReadNextFloat();
	vectorFromBytes.y = ReadNextFloat();

	return vectorFromBytes;
}

//-----------------------------------------------------------------------------------------------------------

const Vector3 BinaryBufferParser::ReadNextVec3(){
	Vector3 vectorFromBytes;

	vectorFromBytes.x = ReadNextFloat();
	vectorFromBytes.y = ReadNextFloat();
	vectorFromBytes.z = ReadNextFloat();

	return vectorFromBytes;
}

//-----------------------------------------------------------------------------------------------------------

 const Vector4 BinaryBufferParser::ReadNextVec4(){
	Vector4 vectorFromBytes;

	vectorFromBytes.x = ReadNextFloat();
	vectorFromBytes.y = ReadNextFloat();
	vectorFromBytes.z = ReadNextFloat();
	vectorFromBytes.w = ReadNextFloat();

	return vectorFromBytes;
}

 //-----------------------------------------------------------------------------------------------------------

 Rgba BinaryBufferParser::ReadNextRgba(){
	 Rgba tempColor;

	 tempColor.r = ReadNextUChar();
	 tempColor.g = ReadNextUChar();
	 tempColor.b = ReadNextUChar();
	 tempColor.a = ReadNextUChar();

	 return tempColor;
 }

 //-----------------------------------------------------------------------------------------------------------

 const Vertex3D BinaryBufferParser::ReadNextVertex3D(){
	 Vertex3D tempVertex3D;

	 tempVertex3D.m_position = ReadNextVec3();

	 tempVertex3D.m_color = ReadNextRgba();

	 tempVertex3D.m_texCoords = ReadNextVec2();
	
	 tempVertex3D.m_normal = ReadNextVec3();
	
	 tempVertex3D.m_tangent = ReadNextVec3();
	 if (tempVertex3D.m_tangent == Vector3::ZERO){
		 tempVertex3D.m_tangent = Vector3::COMMON_WORLD_NORMAL; //tempVertex3D.m_normal;
	 }

	 tempVertex3D.m_biTangent = ReadNextVec3();
	 if (tempVertex3D.m_biTangent == Vector3::ZERO){
		 tempVertex3D.m_biTangent = Vector3::COMMON_WORLD_NORMAL; //tempVertex3D.m_normal;
	 }

	 return tempVertex3D;

 }


//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Binary file parser methods

///----------------------------------------------------------------------------------------------------------
///constructors
BinaryFileParser::BinaryFileParser(const std::string& filePath){
	//LoadBinaryFileToExistingByteVector(filePath, m_buffer);
	m_buffer = LoadBinaryFileToNewBuffer(filePath, m_bufferSize);
	m_bufferSize = GetUCStrLength(m_buffer);
	m_currentIndex = 0;
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Binary buffer builder methods

//constructors

BinaryBufferBuilder::BinaryBufferBuilder(const std::string& filePath):
m_filePath(filePath)
{
	

	BinaryBufferBuilder();

}

BinaryBufferBuilder::BinaryBufferBuilder(const unsigned char* existingBuffer){
	BinaryBufferBuilder();
	//load existing buffer to byte vector
	LoadExistingBufferToByteVector(existingBuffer);

}

BinaryBufferBuilder::BinaryBufferBuilder(const std::string& filePath, const unsigned char* existingBuffer){
	
	m_filePath = filePath;
	BinaryBufferBuilder();

	LoadExistingBufferToByteVector(existingBuffer);

}

///----------------------------------------------------------------------------------------------------------
///output methods
std::string BinaryBufferBuilder::ByteVectorToString(bool useHex){
	std::string bufferString = "";
	for (std::vector<unsigned char>::iterator it = m_bufferDataBytes.begin(); it != m_bufferDataBytes.end(); ++ it ){
		if (!useHex)bufferString += UCharToString(*it);
		else bufferString += IntToString(*it,16)+" ";
	}

	return bufferString;
}

///----------------------------------------------------------------------------------------------------------
///write methods

//filename must include extension, assumes internal buffer
void BinaryBufferBuilder::WriteBufferToFile(const std::string& filePath){

	//WriteExistingByteVectorToFile(m_bufferDataBytes, filePath);

	//old version used a temp buffer
	unsigned char* bufferToCreate = new unsigned char[m_currentBufferSize];

	//copy byte data to buffer
	for (unsigned int i = 0; i < m_currentBufferSize; i++){
		bufferToCreate[i] = m_bufferDataBytes[i];
	}

	//write to a .warlock file
	WriteExistingUCharBufferToFile(bufferToCreate, m_currentBufferSize, filePath);

}

//uses internal file path
void BinaryBufferBuilder::WriteBufferToFile(){

	WriteBufferToFile(m_filePath);

}

//appends the buffer
void BinaryBufferBuilder::LoadExistingBufferToByteVector(const unsigned char* buffer, bool clearByteVector){

	if (clearByteVector)m_bufferDataBytes.clear();

	int bufferLength = GetUCStrLength(buffer);
	m_currentBufferSize = m_currentBufferSize + bufferLength;
	m_bufferDataBytes.reserve(m_currentBufferSize);

	//copy byte data to buffer
	for (int i = 0; i < bufferLength; i++){
		m_bufferDataBytes.push_back(buffer[i]);
	}

}

///----------------------------------------------------------------------------------------------------------
///write primitive methods

void BinaryBufferBuilder::WriteIntToBuffer(const int& intToWrite){

	m_bufferDataBytes.reserve(m_currentBufferSize + SIZE_OF_INT);
	m_currentBufferSize += SIZE_OF_INT;
	unsigned char* integerBytes = (unsigned char*)(&intToWrite);

	for (int i = 0; i < SIZE_OF_INT; i++){
		m_bufferDataBytes.push_back(integerBytes[i]);

		m_currentIndex++;
	}
	
}

//-----------------------------------------------------------------------------------------------------------

void BinaryBufferBuilder::WriteUIntToBuffer(const unsigned int& intToWrite){

	m_bufferDataBytes.reserve(m_currentBufferSize + SIZE_OF_INT);
	m_currentBufferSize += SIZE_OF_INT;
	unsigned char* integerBytes = (unsigned char*)(&intToWrite);

	for (int i = 0; i < SIZE_OF_INT; i++){
		m_bufferDataBytes.push_back(integerBytes[i]);

		m_currentIndex++;
	}

}

//-----------------------------------------------------------------------------------------------------------

void BinaryBufferBuilder::WriteShortToBuffer(const short& shortToWrite){

	m_bufferDataBytes.reserve(m_currentBufferSize + SIZE_OF_SHORT);
	m_currentBufferSize += SIZE_OF_SHORT;
	unsigned char* shortBytes = (unsigned char*)(&shortToWrite);

	for (int i = 0; i < SIZE_OF_SHORT; i++){
		m_bufferDataBytes.push_back(shortBytes[i]);

		m_currentIndex++;
	}

}

//-----------------------------------------------------------------------------------------------------------

void BinaryBufferBuilder::WriteUShortToBuffer(const unsigned short& shortToWrite){

	m_bufferDataBytes.reserve(m_currentBufferSize + SIZE_OF_SHORT);
	m_currentBufferSize += SIZE_OF_SHORT;
	unsigned char* shortBytes = (unsigned char*)(&shortToWrite);

	for (int i = 0; i < SIZE_OF_SHORT; i++){
		m_bufferDataBytes.push_back(shortBytes[i]);

		m_currentIndex++;
	}

}

//-----------------------------------------------------------------------------------------------------------

void BinaryBufferBuilder::WriteFloatToBuffer(const float& floatToWrite){
	m_bufferDataBytes.reserve(m_currentBufferSize + SIZE_OF_FLOAT);
	m_currentBufferSize += SIZE_OF_FLOAT;
	unsigned char* floatBytes = (unsigned char*)(&floatToWrite);

	for (int i = 0; i < SIZE_OF_FLOAT; i++){
		m_bufferDataBytes.push_back(floatBytes[i]);

		m_currentIndex++;
	}

}

//-----------------------------------------------------------------------------------------------------------

void BinaryBufferBuilder::WriteDoubleToBuffer(const double& doubleToWrite){
	m_bufferDataBytes.reserve(m_currentBufferSize + SIZE_OF_DOUBLE);
	m_currentBufferSize += SIZE_OF_DOUBLE;
	unsigned char* doubleBytes = (unsigned char*)(&doubleToWrite);

	for (int i = 0; i < SIZE_OF_DOUBLE; i++){
		m_bufferDataBytes.push_back(doubleBytes[i]);

		m_currentIndex++;
	}

}


//-----------------------------------------------------------------------------------------------------------

void BinaryBufferBuilder::WriteVectorToBuffer(Vector2 vectorToWrite){

	WriteFloatToBuffer(vectorToWrite.x);
	WriteFloatToBuffer(vectorToWrite.y);

}

void BinaryBufferBuilder::WriteVectorToBuffer(Vector3 vectorToWrite){

	WriteFloatToBuffer(vectorToWrite.x);
	WriteFloatToBuffer(vectorToWrite.y);
	WriteFloatToBuffer(vectorToWrite.z);

}

void BinaryBufferBuilder::WriteVectorToBuffer(Vector4 vectorToWrite){

	WriteFloatToBuffer(vectorToWrite.x);
	WriteFloatToBuffer(vectorToWrite.y);
	WriteFloatToBuffer(vectorToWrite.z);
	WriteFloatToBuffer(vectorToWrite.w);

}

void BinaryBufferBuilder::WriteMatrix4ToBuffer(Matrix4 matrixToWrite){

	WriteVectorToBuffer(matrixToWrite.m_iBasis);
	WriteVectorToBuffer(matrixToWrite.m_jBasis);
	WriteVectorToBuffer(matrixToWrite.m_kBasis);
	WriteVectorToBuffer(matrixToWrite.m_translation);

}
