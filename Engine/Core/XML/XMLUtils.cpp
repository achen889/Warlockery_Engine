//==============================================================================================================
//XMLUtils.cpp
//by Albert Chen Aug-25-2015.
//==============================================================================================================

#include "XMLUtils.hpp"

///----------------------------------------------------------------------------------------------------------
///xml file parser stuff

///----------------------------------------------------------------------------------------------------------
///constructors

//create from filePath and rootNode
XMLFileParser::XMLFileParser(const std::string& filePath, const char* rootNode){

	m_xmlNode = XMLNode::parseFile(filePath.c_str(), rootNode, &m_xmlResults);
	m_rootNode = m_xmlNode;
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///helper methods

bool LoadXMLFileToXMLParser(XMLFileParser& xmlParser , const std::string& filePath, const char* rootNode ){
	
	//m_xmlNode = new XMLNode();

	xmlParser = XMLFileParser(filePath, rootNode);
	//GetXMLError();

	return true;//IsXMLNodeValid();
}

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///queries


//-----------------------------------------------------------------------------------------------------------

const std::string XMLFileParser::GetXMLError(){
	std::string errorString = (std::string) XMLNode::getError(m_xmlResults.error);

	return errorString;
}

void XMLFileParser::WriteXMLFile(const std::string& filePath, const char *encoding, char nFormat){

	m_xmlResults.error = m_rootNode.writeToFile(filePath.c_str(), encoding, nFormat);

}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///readers



std::string ReadXMLAttributeAsString(const XMLNode& node, const char* attributeName, const std::string& defaultValue){
	XMLCSTR attrCStr = node.getAttribute(attributeName);
	if (attrCStr){
		std::string attrString = (std::string)attrCStr;

		return attrString;
	}

	return defaultValue;
}

//-----------------------------------------------------------------------------------------------------------

int ReadXMLAttributeAsInt(const XMLNode& node, const char* attributeName, int defaultValue){
	XMLCSTR attrCStr = node.getAttribute(attributeName);
	if (attrCStr){
		std::string attrString = (std::string)attrCStr;

		return StringToInt(attrString);
	}
	else
		return defaultValue;
}

//-----------------------------------------------------------------------------------------------------------

unsigned int ReadXMLAttributeAsUInt(const XMLNode& node, const char* attributeName, unsigned int defaultValue) {
	XMLCSTR attrCStr = node.getAttribute(attributeName);
	if (attrCStr) {
		std::string attrString = (std::string)attrCStr;

		return StringToUInt(attrString);
	}
	else
		return defaultValue;
}

//-----------------------------------------------------------------------------------------------------------

float ReadXMLAttributeAsFloat(const XMLNode& node, const char* attributeName, const float& defaultValue){
	XMLCSTR attrCStr = node.getAttribute(attributeName);
	if (attrCStr){
		std::string attrString = (std::string)attrCStr;

		return StringToFloat(attrString);
	}

	return defaultValue;
}

//-----------------------------------------------------------------------------------------------------------

Vector2 ReadXMLAttributeAsVec2(const XMLNode& node, const char* attributeName, const Vector2& defaultValue){
	XMLCSTR attrCStr = node.getAttribute(attributeName);
	if (attrCStr){
		std::string attrString = (std::string)attrCStr;

		return ToVector2(attrString);
	}
	else
		return defaultValue;
}

//-----------------------------------------------------------------------------------------------------------

//rgba as 0,0,0 alpha always 255
Rgba ReadXMLAttributeAsRgba(const XMLNode& node, const char* attributeName, const Rgba& defaultValue){
	Rgba color;
	std::string colorString = node.getAttribute(attributeName);
	if (colorString.size() > 0){
		std::vector<std::string> colorComponents = SplitString(colorString, ",");

		color.r = (unsigned char)CStringToInt(colorComponents[0].c_str());
		color.g = (unsigned char)CStringToInt(colorComponents[1].c_str());
		color.b = (unsigned char)CStringToInt(colorComponents[2].c_str());
		color.a = 255;//CStringToInt(colorComponents[3].c_str());

		return color;
	}
	return defaultValue;
}


//-----------------------------------------------------------------------------------------------------------

void WriteBoolToXMLNode(XMLNode& node, const char* attributeName, const bool& boolAttribute){
	if (boolAttribute){

		node.addAttribute(attributeName, "1");
		
	}
	else{

		node.addAttribute(attributeName, "0");
	}
}

//===========================================================================================================