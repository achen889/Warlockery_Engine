//==============================================================================================================
//WidgetFactory.hpp
//by Albert Chen Mar-23-2016.
//==============================================================================================================

#pragma once

#ifndef _included_WidgetFactory__
#define _included_WidgetFactory__

#include "BaseWidget.hpp"
#include "Engine/Core/XML/XMLUtils.hpp"
#include "Engine/Core/FileWizard.hpp"

//===========================================================================================================

class WidgetFactory;

typedef std::map<std::string, WidgetFactory*> WidgetFactoryMap;
typedef std::map<std::string, WidgetFactory*>::iterator WidgetFactoryMapIterator;

typedef BaseWidget Widget;

class WidgetFactory{
public:

	WidgetFactory(XMLNode& widgetNode);

	//randomly spawn one of the Widgets

	template<typename T>
	T* SpawnWidget(){
		return new T(*(T*)(m_prototype));
	}

	const std::string& GetWidgetType() { return m_type; }

	static bool LoadAllWidgetFactories();

	static void LoadXMLParsersFromWidgetFiles(XMLParsers& WidgetFileParsers, std::vector<std::string> WidgetFilePaths);

	static WidgetFactory* FindFactoryByName(const std::string& name);

	static void AddWidgetFactoryToStaticFactoryMap(const std::string& factoryName, XMLFileParser& WidgetParser);

	static WidgetFactoryMap GetAllFactories(){ return WidgetFactory::s_WidgetFactories;  }

	static WidgetFactory* GetRandomWidgetFactoryFromFactoryMap();

	Widget* GetWidgetPrototype(){ return m_prototype;  }

	static WidgetFactoryMap s_WidgetFactories;

protected:
	
	static std::vector<std::string> s_WidgetFactoryNames;
	std::string m_type;
	Widget* m_prototype;
	
};

//===========================================================================================================

#endif //__includedWidgetFactory__
