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

// 	//Assignment Operator
// 	const XMLFileParser& operator=(const XMLFileParser& parserToAssign) {
// 		*this = parserToAssign;
// 
// 		return *this;
// 	}


	void WriteXMLFile(const std::string& filePath, const char *encoding = NULL, char nFormat = 1);
	
	
	const std::string GetXMLError();

};

typedef std::vector<XMLFileParser> XMLParsers;
typedef std::vector<XMLFileParser>::iterator XMLParsersIterator;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///helpful xml parsing methods

bool LoadXMLFileToXMLParser(XMLFileParser& xmlParser, const std::string& filePath, const char* rootNode = NULL);

bool HasXMLAttribute(const XMLNode& node, const char* attributeName);

//readers

int ReadXMLAttributeAsInt(const XMLNode& node, const char* attributeName, int defaultValue = 0);

unsigned int ReadXMLAttributeAsUInt(const XMLNode& node, const char* attributeName, unsigned int defaultValue = 0);

float ReadXMLAttributeAsFloat(const XMLNode& node, const char* attributeName, const float& defaultValue = 0.0f);

Vector2 ReadXMLAttributeAsVec2(const XMLNode& node, const char* attributeName, const Vector2& defaultValue = Vector2::ZERO);

Rgba ReadXMLAttributeAsRgba(const XMLNode& node, const char* attributeName, const Rgba& defaultValue = Rgba::WHITE);

std::string ReadXMLAttributeAsString(const XMLNode& node, const char* attributeName, const std::string& defaultValue = "");

//writers

void WriteBoolToXMLNode(XMLNode& node, const char* attributeName, const bool& boolAttribute);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///additional global helpers

void LoadColorSchemeFromXML(ColorScheme& out_colorScheme, const XMLNode& node);

///----------------------------------------------------------------------------------------------------------
///inline helpers

inline bool HasXMLAttribute(const XMLNode& node, const char* attributeName) {

	char isSet = node.isAttributeSet(attributeName);
	if (isSet == 0) {
		return false;
	}
	return true;

}

//-----------------------------------------------------------------------------------------------------------

template<typename T>
inline void ReadXMLAttribute(const XMLNode& node, const char* attributeName, T& out_value) {
	
	std::string attrString = ReadXMLAttributeAsString(node, attributeName);

	SetFromString(out_value, attrString);

}

//-----------------------------------------------------------------------------------------------------------

inline void LoadColorSchemeFromXML(ColorScheme& out_colorScheme, const XMLNode& node) {

	//set name
	std::string schemeName = ReadXMLAttributeAsString(node, "name", "");
	out_colorScheme.name = schemeName;

	//load all colors
	int num = node.nChildNode("Color");
	XMLNode tempNode;
	for (int i = 0; i < num; i++) {
		tempNode = node.getChildNode("Color", i);

		std::string colorName = ReadXMLAttributeAsString(tempNode, "name", "");
		Rgba myColor = ReadXMLAttributeAsRgba(tempNode, "value", Rgba::WHITE);

		out_colorScheme.colors.insert(ColorMapEntry(colorName, myColor));
	}

}


//===========================================================================================================

#endif