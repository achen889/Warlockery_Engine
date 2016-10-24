//==============================================================================================================
//ObjectFactory.cpp
//by Albert Chen Mar-28-2016.
//==============================================================================================================

#include "ObjectFactory.hpp"

#include "Engine/Multithreading/Job.hpp"
#include "../Multithreading/JobManager.hpp"

BaseObjectFactoryMapRegistry BaseObjectFactory::s_ObjectFactoryRegistry;
ObjectFactoryNamesRegistry BaseObjectFactory::s_ObjectFactoryNamesRegistry;

Strings BaseObjectFactory::s_ObjectFactoryNames;
Strings BaseObjectFactory::s_ObjectFactoryTypeNames;

//===========================================================================================================
// 
// struct LoadXMLFileToXMLParserJob : Job {
// 
// 	//vars
// 	XMLFileParser& xmlParserRef;
// 	std::string filePath;
// 	std::string rootNodeStr;
// 	XMLParsers& xmlParsers;
// 
// 	//methods
// 	LoadXMLFileToXMLParserJob(XMLFileParser& xmlParser, const std::string& file_path, 
// 		const char* rootNode, XMLParsers& xml_parsers) :
// 		xmlParserRef(xmlParser),
// 		filePath(file_path),
// 		rootNodeStr(rootNode),
// 		xmlParsers(xml_parsers)
// 	{
// 		
// 	}
// 
// 	void Execute() {
// 		if (LoadXMLFileToXMLParser(xmlParserRef, filePath, rootNodeStr.c_str())) {
// 			xmlParsers.push_back(xmlParserRef);
// 		}
// 
// 	}
// protected:
// 	//Assignment Operator
// 	const LoadXMLFileToXMLParserJob& operator=(const LoadXMLFileToXMLParserJob& jobToAssign) {
// 		*this = jobToAssign;
// 
// 		return *this;
// 	}
// 
// };

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///base object factory

bool BaseObjectFactory::LoadAllObjectXMLFiles(Strings& ObjectFilePaths, const std::string& objectFileDir, 
	const std::string& objectFileExt, bool recurseFolders) {

	return FindAllFilesOfType(objectFileDir, objectFileExt , ObjectFilePaths, recurseFolders);
}

//-----------------------------------------------------------------------------------------------------------

void BaseObjectFactory::LoadXMLParsersFromObjectFiles(
	XMLParsers& ObjectFileParsers,
	std::vector<std::string> ObjectFilePaths,
	const std::string& objectTypeXML) {

	XMLFileParser newObjectFileParser;
	int ObjectParserIndex = 0;
	for (StringsIterator it = ObjectFilePaths.begin(); it != ObjectFilePaths.end(); ++it) {
		std::string& filePath = (*it);
		
		LoadXMLFileToXMLParser(newObjectFileParser, filePath, objectTypeXML.c_str());
		ObjectFileParsers.push_back(newObjectFileParser);
// 		if (theJobManager) {
// 			LoadXMLFileToXMLParserJob* newJob = new LoadXMLFileToXMLParserJob(newObjectFileParser, filePath,
// 				objectTypeXML.c_str(), ObjectFileParsers);
// 			theJobManager->AddPendingJob(newJob, PRIORITY_HIGH);
// 		}

		ObjectParserIndex++;
	}

	//ConsoleGenericMessageBox("Loaded XML files");
}

//-----------------------------------------------------------------------------------------------------------

void ClearAllObjectFactories() {
	BaseObjectFactoryMapRegistry::iterator it;
	for (it = BaseObjectFactory::s_ObjectFactoryRegistry.begin(); it != BaseObjectFactory::s_ObjectFactoryRegistry.end(); ) {
		BaseObjectFactoryMap& map = it->second;
		BaseObjectFactoryMapIterator mapIt;
		for (mapIt = map.begin(); mapIt != map.end(); ) {
			BaseObjectFactory* factory = (mapIt->second);
			if (factory) {
				//factory->FreePrototype();
				delete factory;
				factory = NULL;
			}
			mapIt = map.erase(mapIt);
		}//end of inner for
		it = BaseObjectFactory::s_ObjectFactoryRegistry.erase(it);
	}//end of outer for

}

//-----------------------------------------------------------------------------------------------------------


//===========================================================================================================