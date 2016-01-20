//==============================================================================================================
//BaseComponent.hpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#pragma once

#ifndef _included_BaseComponent__
#define _included_BaseComponent__

#include "Engine/Core/Utilities.hpp"
#include <stack>
#include "Engine/Core/XML/XMLUtils.hpp"

class ComponentRegistration;
class BaseComponent;

typedef std::map<std::string, ComponentRegistration*> ComponentRegistry;
typedef std::map<std::string, ComponentRegistration*>::iterator ComponentRegistryIterator;
typedef BaseComponent* (ComponentCreationFunc)(const std::string& name );

//===========================================================================================================
//self-registering class
class ComponentRegistration{
	public:
		~ComponentRegistration(){
			//do nothing
		}

		ComponentRegistration(const std::string& name,  ComponentCreationFunc* creationFunction );

		//registry helpers
		const std::string& GetName() const { return m_name;  }
		static ComponentRegistry* GetComponentRegistry(){ return s_ComponentRegistryMap;  }
		BaseComponent* CreateComponent(){ return (*m_creationFunc)(m_name);  }

		//friend methods
		friend ComponentRegistration* FindComponentByName(const std::string& name);
		
private:
		//vars
		ComponentCreationFunc* m_creationFunc;
		std::string m_name;
		static ComponentRegistry* s_ComponentRegistryMap;
		
};


///----------------------------------------------------------------------------------------------------------
///inline methods

//means of registering stuff to the registry
//name must be lowercase
inline ComponentRegistration::ComponentRegistration(const std::string& name, ComponentCreationFunc* creationFunction):
m_name(name)
//m_creationFunc(creationFunction)
{
	m_creationFunc = creationFunction;
	//create a new registry
	if (!s_ComponentRegistryMap){
		s_ComponentRegistryMap = new ComponentRegistry();
	}
	//insert into that registry
	s_ComponentRegistryMap->insert(std::pair<std::string, ComponentRegistration*>(name, this));
}

//===========================================================================================================

class OpenGLRenderer;

static size_t s_componentID = 0;

class BaseComponent{
public:
	BaseComponent(){
		//do nothing
	}
	virtual ~BaseComponent(){
		//do nothing
	}
	BaseComponent(const std::string& name);

	BaseComponent(const XMLNode& node);

	//virtual methods //not all components use all these functions
	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void ProcessInput(double deltaSeconds);
	virtual void Update(double deltaSeconds);
	virtual void Render(OpenGLRenderer* renderer);

	//accessors
	std::string GetNameID(){ return m_name + " " + IntToString(m_id); }
	std::string GetName(){ return m_name; }
	
	//vars
	std::string m_name;
	size_t m_id;
};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline BaseComponent::BaseComponent(const std::string& name) :
m_name(name),
m_id(s_componentID)
{
	s_componentID++;
}


//===========================================================================================================


#endif