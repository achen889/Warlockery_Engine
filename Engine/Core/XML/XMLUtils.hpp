//==============================================================================================================
//XMLUtils.hpp
//by Albert Chen Aug-25-2015.
//==============================================================================================================

#pragma once

#ifndef _included_XMLUtils__
#define _included_XMLUtils__

#include "xmlParser.h"
#include "Engine/Core/Utilities.hpp"
#include "Engine/Math/Vector2.hpp"

typedef std::vector<XMLNode> XMLNodes;

//===========================================================================================================

//helper methods

struct XMLFileParser{
	bool m_isValid;

	XMLNode m_xmlNode;
	XMLNode m_rootNode;
	XMLResults m_xmlResults;

	XMLFileParser(){
		//do nothing
	}
	
	XMLFileParser(const std::string& filePath, const char* rootNode = NULL);


	void WriteXMLFile(const std::string& filePath, const char *encoding = NULL, char nFormat = 1);
	
	
	const std::string GetXMLError();

};

typedef std::vector<XMLFileParser> XMLParsers;
typedef std::vector<XMLFileParser>::iterator XMLParsersIterator;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///helpful xml parsing methods

bool LoadXMLFileToXMLParser(XMLFileParser& xmlParser, const std::string& filePath, const char* rootNode = NULL);

//readers

int ReadXMLAttributeAsInt(const XMLNode& node, const char* attributeName, int defaultValue = 0);

float ReadXMLAttributeAsFloat(const XMLNode& node, const char* attributeName, const float& defaultValue = 0.0f);

Vector2 ReadXMLAttributeAsVec2(const XMLNode& node, const char* attributeName, const Vector2& defaultValue = Vector2::ZERO);

Rgba ReadXMLAttributeAsRgba(const XMLNode& node, const char* attributeName, const Rgba& defaultValue);

std::string ReadXMLAttributeAsString(const XMLNode& node, const char* attributeName, const std::string& defaultValue = "");

//writers

void WriteBoolToXMLNode(XMLNode& node, const char* attributeName, const bool& boolAttribute);


//===========================================================================================================

#endif