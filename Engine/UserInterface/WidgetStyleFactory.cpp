//==============================================================================================================
//WidgetStyleFactory.cpp
//by Albert Chen Mar-23-2016.
//==============================================================================================================

#include "WidgetStyleFactory.hpp"

WidgetStyleFactoryMap WidgetStyleFactory::s_WidgetStyleFactories;
std::vector<std::string> WidgetStyleFactory::s_WidgetStyleFactoryNames;
//===========================================================================================================

bool WidgetStyleFactory::LoadAllWidgetStyleFactories(){	
	std::vector<std::string> WidgetStyleFilePaths;
	
	if (FindAllFilesOfType("Data/UI/", ".style.xml", WidgetStyleFilePaths, true)){

		//parse all WidgetStyle files
		XMLParsers WidgetStyleFileParsers;
		WidgetStyleFileParsers.reserve(WidgetStyleFilePaths.size() * sizeof(XMLParsers));

		WidgetStyleFactory::LoadXMLParsersFromWidgetStyleFiles(WidgetStyleFileParsers, WidgetStyleFilePaths);

		//load an instance of all the factories to the static map
		int WidgetStyleParserIndex = 0;
		for (StringsIterator it = WidgetStyleFilePaths.begin(); it != WidgetStyleFilePaths.end(); ++ it ){
			//std::string& filePath = (*it);

			XMLFileParser& myWidgetStyleParser = WidgetStyleFileParsers[WidgetStyleParserIndex];
			
			std::string WidgetStyleName = std::string( myWidgetStyleParser.m_xmlNode.getAttribute("name") );

			s_WidgetStyleFactoryNames.push_back(WidgetStyleName);

			WidgetStyleFactory::AddWidgetStyleFactoryToStaticFactoryMap(WidgetStyleName, myWidgetStyleParser );

			WidgetStyleParserIndex++;
			
		}

	}

	return true;
	
}

//-----------------------------------------------------------------------------------------------------------

void WidgetStyleFactory::LoadXMLParsersFromWidgetStyleFiles(XMLParsers& WidgetStyleFileParsers, std::vector<std::string> WidgetStyleFilePaths){
	XMLFileParser newWidgetStyleFileParser;
	int WidgetStyleParserIndex = 0;
	for (StringsIterator it = WidgetStyleFilePaths.begin(); it != WidgetStyleFilePaths.end(); ++it){
		std::string& filePath = (*it);

		LoadXMLFileToXMLParser(newWidgetStyleFileParser, filePath, "WidgetStyle");

		WidgetStyleFileParsers.push_back(newWidgetStyleFileParser);

		WidgetStyleParserIndex++;

	}

	//ConsoleGenericMessageBox("Loaded XML files");
}

WidgetStyleFactory* WidgetStyleFactory::FindFactoryByName(const std::string& name){
	return (WidgetStyleFactory::s_WidgetStyleFactories[(name)]);
}

//-----------------------------------------------------------------------------------------------------------

void WidgetStyleFactory::AddWidgetStyleFactoryToStaticFactoryMap(const std::string& factoryName, XMLFileParser& WidgetStyleParser){

	s_WidgetStyleFactories[factoryName] = new WidgetStyleFactory(WidgetStyleParser.m_xmlNode);

}

//-----------------------------------------------------------------------------------------------------------

WidgetStyleFactory* WidgetStyleFactory::GetRandomWidgetStyleFactoryFromFactoryMap(){
	int WidgetStyleRandomIndex = GetRandomIntInRange(0, WidgetStyleFactory::s_WidgetStyleFactoryNames.size()-1);

	StringsIterator randomWidgetStyleFactoryName = WidgetStyleFactory::s_WidgetStyleFactoryNames.begin() + WidgetStyleRandomIndex;

	return ( WidgetStyleFactory::s_WidgetStyleFactories[ *(randomWidgetStyleFactoryName)] );
	
}

