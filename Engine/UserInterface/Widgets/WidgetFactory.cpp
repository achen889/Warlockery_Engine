//==============================================================================================================
//WidgetFactory.cpp
//by Albert Chen Mar-23-2016.
//==============================================================================================================

#include "WidgetFactory.hpp"
#include "ButtonWidget.hpp"

WidgetFactoryMap WidgetFactory::s_WidgetFactories;
std::vector<std::string> WidgetFactory::s_WidgetFactoryNames;
//===========================================================================================================

WidgetFactory::WidgetFactory(XMLNode& widgetNode) {

 	std::string widgetType = ReadXMLAttributeAsString(widgetNode, "type", "");
	m_type = widgetType;

	WidgetRegistration* registration = FindWidgetByName(m_type);
	if (registration) {
		m_prototype = registration->CreateWidget(widgetNode);
	}
	
	
	//pull out factory specific

}

//-----------------------------------------------------------------------------------------------------------

bool WidgetFactory::LoadAllWidgetFactories(){	
	std::vector<std::string> WidgetFilePaths;
	
	if (FindAllFilesOfType("Data/UI/", ".widget.xml", WidgetFilePaths, true)){

		//parse all Widget files
		XMLParsers WidgetFileParsers;
		WidgetFileParsers.reserve(WidgetFilePaths.size() * sizeof(XMLParsers));

		WidgetFactory::LoadXMLParsersFromWidgetFiles(WidgetFileParsers, WidgetFilePaths);

		//load an instance of all the factories to the static map
		int WidgetParserIndex = 0;
		for (StringsIterator it = WidgetFilePaths.begin(); it != WidgetFilePaths.end(); ++ it ){
			//std::string& filePath = (*it);

			XMLFileParser& myWidgetParser = WidgetFileParsers[WidgetParserIndex];
			
			std::string WidgetName = std::string(myWidgetParser.m_xmlNode.getAttribute("name"));

			s_WidgetFactoryNames.push_back(WidgetName);

			WidgetFactory::AddWidgetFactoryToStaticFactoryMap(WidgetName, myWidgetParser );

			WidgetParserIndex++;
			
		}

	}

	return true;
	
}

//-----------------------------------------------------------------------------------------------------------

void WidgetFactory::LoadXMLParsersFromWidgetFiles(XMLParsers& WidgetFileParsers, std::vector<std::string> WidgetFilePaths){
	XMLFileParser newWidgetFileParser;
	int WidgetParserIndex = 0;
	for (StringsIterator it = WidgetFilePaths.begin(); it != WidgetFilePaths.end(); ++it){
		std::string& filePath = (*it);

		LoadXMLFileToXMLParser(newWidgetFileParser, filePath, "Widget");

		WidgetFileParsers.push_back(newWidgetFileParser);

		WidgetParserIndex++;

	}

	//ConsoleGenericMessageBox("Loaded XML files");
}

WidgetFactory* WidgetFactory::FindFactoryByName(const std::string& name){
	return (WidgetFactory::s_WidgetFactories[(name)]);
}

//-----------------------------------------------------------------------------------------------------------

void WidgetFactory::AddWidgetFactoryToStaticFactoryMap(const std::string& factoryName, XMLFileParser& WidgetParser){

	s_WidgetFactories[factoryName] = new WidgetFactory(WidgetParser.m_xmlNode);

}

//-----------------------------------------------------------------------------------------------------------

WidgetFactory* WidgetFactory::GetRandomWidgetFactoryFromFactoryMap(){
	int WidgetRandomIndex = GetRandomIntInRange(0, WidgetFactory::s_WidgetFactoryNames.size()-1);

	StringsIterator randomWidgetFactoryName = WidgetFactory::s_WidgetFactoryNames.begin() + WidgetRandomIndex;

	return ( WidgetFactory::s_WidgetFactories[ *(randomWidgetFactoryName)] );
	
}

