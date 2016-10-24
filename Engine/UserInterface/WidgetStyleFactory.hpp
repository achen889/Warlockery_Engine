//==============================================================================================================
//WidgetStyleFactory.hpp
//by Albert Chen Mar-23-2016.
//==============================================================================================================

#pragma once


#ifndef _included_WidgetStyleFactory__
#define _included_WidgetStyleFactory__

#include "WidgetStyle.hpp"
#include "Engine/Core/XML/XMLUtils.hpp"
#include "Engine/Core/FileWizard.hpp"

//===========================================================================================================

class WidgetStyleFactory;

typedef std::map<std::string, WidgetStyleFactory*> WidgetStyleFactoryMap;
typedef std::map<std::string, WidgetStyleFactory*>::iterator WidgetStyleFactoryMapIterator;

class WidgetStyleFactory{
public:
	WidgetStyleFactory(XMLNode& WidgetStyleNode){
		m_prototype = new WidgetStyle(WidgetStyleNode);

		//pull out factory specific


	}

	//randomly spawn one of the WidgetStyles

	WidgetStyle* SpawnWidgetStyle(){
		return new WidgetStyle(*(m_prototype));
	}

	static bool LoadAllWidgetStyleFactories();

	static void LoadXMLParsersFromWidgetStyleFiles(XMLParsers& WidgetStyleFileParsers, std::vector<std::string> WidgetStyleFilePaths);

	static WidgetStyleFactory* FindFactoryByName(const std::string& name);

	static void AddWidgetStyleFactoryToStaticFactoryMap(const std::string& factoryName, XMLFileParser& WidgetStyleParser);

	static WidgetStyleFactoryMap GetAllFactories(){ return WidgetStyleFactory::s_WidgetStyleFactories;  }

	static WidgetStyleFactory* GetRandomWidgetStyleFactoryFromFactoryMap();

	WidgetStyle* GetWidgetStylePrototype(){ return m_prototype;  }

	static WidgetStyleFactoryMap s_WidgetStyleFactories;

protected:
	
	static std::vector<std::string> s_WidgetStyleFactoryNames;

	WidgetStyle* m_prototype;
	
};



//-----------------------------------------------------------------------------------------------------------


//===========================================================================================================

#endif //__includedWidgetStyleFactory__
