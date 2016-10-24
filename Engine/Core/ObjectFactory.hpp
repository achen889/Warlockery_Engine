//==============================================================================================================
//ObjectFactory.hpp
//by Albert Chen Jun-4-2016.
//==============================================================================================================

#pragma once

#ifndef _included_ObjectFactory__
#define _included_ObjectFactory__

//#include "Object.hpp"
#include "Engine/Core/XML/XMLUtils.hpp"
#include "Engine/Core/FileWizard.hpp"

//#include "Engine/Core/Memory.hpp"

/*
 An XML Object Factory for any class/struct with the proper constructors.
*/

//===========================================================================================================

class BaseObjectFactory;

typedef std::map<std::string, BaseObjectFactory*> BaseObjectFactoryMap;
typedef std::map<std::string, BaseObjectFactory*>::iterator BaseObjectFactoryMapIterator;

typedef std::map<std::string, BaseObjectFactoryMap> BaseObjectFactoryMapRegistry;
typedef std::map<std::string, Strings> ObjectFactoryNamesRegistry;

//===========================================================================================================

class BaseObjectFactory {
public:
	//methods

	BaseObjectFactory() {
		//do nothing
	}
	virtual ~BaseObjectFactory() {
		//do nothing
	}


	virtual void FreePrototype() = 0;

	static bool LoadAllObjectXMLFiles(Strings& ObjectFilePaths, const std::string& objectFileDir,
		const std::string& objectFileExt, bool recurseFolders = false);

	static void LoadXMLParsersFromObjectFiles(XMLParsers& ObjectFileParsers, std::vector<std::string> ObjectFilePaths,
		const std::string& objectTypeXML);

	static BaseObjectFactoryMapRegistry GetAllFactories() { return BaseObjectFactory::s_ObjectFactoryRegistry; }

	//vars
	static BaseObjectFactoryMapRegistry s_ObjectFactoryRegistry;
	static ObjectFactoryNamesRegistry s_ObjectFactoryNamesRegistry;

	static std::vector<std::string> s_ObjectFactoryNames;
	static std::vector<std::string> s_ObjectFactoryTypeNames;

	friend void ClearAllObjectFactories();
	
};

template<typename T_OBJECT_TYPE>
class ObjectFactory : public BaseObjectFactory{
public:

	typedef T_OBJECT_TYPE* (ObjectCreationFuncName)(const std::string& name);
	typedef T_OBJECT_TYPE* (ObjectCreationFuncXML)(const XMLNode& node);

	//constructors
	ObjectFactory() : BaseObjectFactory() {
		//do nothing
	}

	ObjectFactory(XMLNode& node, ObjectCreationFuncXML* creationFunc = NULL){
		
		SetType(node);

		AllocPrototypeFromXML(node, creationFunc);
	}
	~ObjectFactory(){
		//free prototype
		FreePrototype();
	}
	
	//helpers
	void SetType(XMLNode& node) {
		std::string myType = ReadXMLAttributeAsString(node, "type", "");
		m_type = myType;
	}

	//can specify a custom object creation func to use, otherwise use XML constructor
	void AllocPrototypeFromXML(XMLNode& node, ObjectCreationFuncXML* creationFunc = NULL) {
		if (m_prototype) return;

		if (!creationFunc) {
			m_prototype = new T_OBJECT_TYPE(node);
		}
		else {
			m_prototype = CreateObject(node, creationFunc);
		}
		
	}
	
	virtual void FreePrototype() override {
		if (m_prototype) {
			delete m_prototype; 
			m_prototype = NULL;
		}
	}
	
	T_OBJECT_TYPE* Spawn(){
		return new T_OBJECT_TYPE(*(m_prototype));
	}

	//spawn as any other type, use at own risk
	template<typename T_CUSTOM_TYPE>
	T_CUSTOM_TYPE* SpawnAsType() {
		return new T_CUSTOM_TYPE(*(T_CUSTOM_TYPE*)(m_prototype));
	}

	const std::string& GetType() { return m_type; }

	//creation func helpers
	
	T_OBJECT_TYPE* CreateObject(const std::string& name, ObjectCreationFuncName* creationFuncName) {
		return (*m_creationFunc)(m_name); 
	}

	T_OBJECT_TYPE* CreateObject(const XMLNode& node, ObjectCreationFuncXML* creationFuncXML) {
		T_OBJECT_TYPE* newObject = (*creationFuncXML)(node);
		return newObject;
	}

	//static helpers

	static bool LoadAllObjectFactories(const std::string& objectFilePath, const std::string& objectFileXMLType, 
		const std::string& objectTypeXML, 
		bool recurseFolders = false, ObjectCreationFuncXML* creationFunc = NULL);
	
	static void AddObjectFactoryToStaticFactoryMap(
		const std::string& factoryName, XMLFileParser& ObjectParser, ObjectCreationFuncXML* creationFuncXML = NULL);
	
	static ObjectFactory<T_OBJECT_TYPE>* FindFactoryByName(const std::string& name);

	static ObjectFactory<T_OBJECT_TYPE>* GetRandomObjectFactoryFromFactoryMapOfType(const std::string type);

	T_OBJECT_TYPE* GetPrototype(){ return m_prototype; }

protected:
	T_OBJECT_TYPE* m_prototype = NULL;
	std::string m_type;//other uses

	static std::string s_typeName;

};

//-----------------------------------------------------------------------------------------------------------


///----------------------------------------------------------------------------------------------------------
///inline methods

template<typename T_OBJECT_TYPE>
std::string ObjectFactory<T_OBJECT_TYPE>::s_typeName;

template<typename T_OBJECT_TYPE>
inline bool ObjectFactory<T_OBJECT_TYPE>::LoadAllObjectFactories(const std::string& objectFilePath, const std::string& objectFileXMLType, 
	const std::string& objectTypeXML, bool recurseFolders, ObjectCreationFuncXML* creationFuncXML) {

	std::vector<std::string> ObjectFilePaths;

	std::string fileExt = "." + objectFileXMLType + ".xml";

	if (BaseObjectFactory::LoadAllObjectXMLFiles(ObjectFilePaths, objectFilePath, fileExt, recurseFolders)){

		//parse all object files
		XMLParsers ObjectFileParsers;
		ObjectFileParsers.reserve(ObjectFilePaths.size() * sizeof(XMLParsers));

		BaseObjectFactory::LoadXMLParsersFromObjectFiles(ObjectFileParsers, ObjectFilePaths, objectTypeXML);

		ObjectFactory<T_OBJECT_TYPE>::s_typeName = objectTypeXML;

		//load an instance of all the factories to the static map
		int ObjectParserIndex = 0;
		for (StringsIterator it = ObjectFilePaths.begin(); it != ObjectFilePaths.end(); ++it) {
			XMLFileParser& myObjectParser = ObjectFileParsers[ObjectParserIndex];

			std::string ObjectName = std::string(myObjectParser.m_xmlNode.getAttribute("name"));

			BaseObjectFactory::s_ObjectFactoryNamesRegistry[ObjectFactory<T_OBJECT_TYPE>::s_typeName].push_back(ObjectName);

			ObjectFactory<T_OBJECT_TYPE>::AddObjectFactoryToStaticFactoryMap(ObjectName, myObjectParser, creationFuncXML);

			ObjectParserIndex++;
		}

	}

	return true;

}

//-----------------------------------------------------------------------------------------------------------

template<typename T_OBJECT_TYPE>
inline void ObjectFactory<T_OBJECT_TYPE>::AddObjectFactoryToStaticFactoryMap(
	const std::string& factoryName, XMLFileParser& ObjectParser, ObjectCreationFuncXML* creationFuncXML) {
	//PROFILE_SECTION();
	
	std::string myTypeName = ObjectFactory<T_OBJECT_TYPE>::s_typeName;

	BaseObjectFactoryMap& objectFactoryMap = s_ObjectFactoryRegistry[myTypeName];

	objectFactoryMap[factoryName] = new ObjectFactory<T_OBJECT_TYPE>(ObjectParser.m_xmlNode, creationFuncXML);
	
}

//-----------------------------------------------------------------------------------------------------------

template<typename T_OBJECT_TYPE>
inline ObjectFactory<T_OBJECT_TYPE>* ObjectFactory<T_OBJECT_TYPE>::FindFactoryByName(const std::string& name) {
	std::string myTypeName = ObjectFactory<T_OBJECT_TYPE>::s_typeName;

	BaseObjectFactoryMap& objectFactoryMap = s_ObjectFactoryRegistry[myTypeName];

	ObjectFactory<T_OBJECT_TYPE>* foundObjectFactory = (ObjectFactory<T_OBJECT_TYPE>*)objectFactoryMap[name];

	return foundObjectFactory;

	//return (BaseObjectFactory::s_ObjectFactories[(name)]);
}

//-----------------------------------------------------------------------------------------------------------

template<typename T_OBJECT_TYPE>
inline ObjectFactory<T_OBJECT_TYPE>* ObjectFactory<T_OBJECT_TYPE>::GetRandomObjectFactoryFromFactoryMapOfType(const std::string type) {
	int ObjectRandomIndex = GetRandomIntInRange(0, BaseObjectFactory::s_ObjectFactoryNamesRegistry[type].size() - 1);

	StringsIterator randomObjectFactoryName = BaseObjectFactory::s_ObjectFactoryNamesRegistry[type].begin() + ObjectRandomIndex;

	return (ObjectFactory<T_OBJECT_TYPE>*)FindFactoryByName(*randomObjectFactoryName);
}

//===========================================================================================================

#endif //__includedObjectFactory__
