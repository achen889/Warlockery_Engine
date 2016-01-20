//==============================================================================================================
//BinaryIOSystem.hpp
//by Albert Chen Aug-18-2015.
//==============================================================================================================

#pragma once

#ifndef _included_BinaryIOSystem__
#define _included_BinaryIOSystem__

#include "BinaryUtils.hpp"

//===========================================================================================================

enum BinaryIOMode{
	BINARY_READ_MODE,
	BINARY_WRITE_MODE,
	NUMBER_OF_BINARY_MODES
};

///----------------------------------------------------------------------------------------------------------
///the full binary IO system
class BinaryIOSystem{
public:
	BinaryIOSystem();
	~BinaryIOSystem();
	BinaryIOSystem(std::string& filePath, BinaryIOMode binaryMode);
	
	//vars
	BinaryFileParser* m_binaryFileParser; //reader
	BinaryBufferBuilder* m_binaryBufferBuilder; //writer
}

///----------------------------------------------------------------------------------------------------------
///inline methods

inline BinaryIOSystem::BinaryIOSystem(){
	m_binaryFileParser = NULL;
	m_binaryBufferBuilder = NULL;
}

inline BinaryIOSystem::~BinaryIOSystem(){

	BinaryFileParser::~BinaryFileParser();

	BinaryBufferBuilder::~BinaryBufferBuilder();

}

inline BinaryIOSystem::BinaryIOSystem(std::string& filePath, BinaryIOMode binaryMode){
	if (binaryMode == BINARY_READ_MODE){
		m_binaryFileParser = new BinaryFileParser(filePath); //load the reader
	}
	else if (binaryMode == BINARY_WRITE_MODE){
		m_binaryBufferBuilder = new BinaryBufferBuilder(filePath); //load the writer
	}

}

//===========================================================================================================

#endif