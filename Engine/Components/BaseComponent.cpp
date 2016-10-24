//==============================================================================================================
//BaseComponent.cpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#include "BaseComponent.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"

ComponentRegistry* ComponentRegistration::s_ComponentRegistryMap;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Base Game State Registration

///----------------------------------------------------------------------------------------------------------
///friend methods

ComponentRegistration* FindComponentByName(const std::string& name){
	ComponentRegistryIterator myRegistration = ComponentRegistration::s_ComponentRegistryMap->find(name);

	if (myRegistration != ComponentRegistration::s_ComponentRegistryMap->end()){
		return (myRegistration->second);
	}

	return NULL;
}

void ClearComponentRegistry() {
	
	if (ComponentRegistration::s_ComponentRegistryMap) {

		for (ComponentRegistryIterator it = (*ComponentRegistration::s_ComponentRegistryMap).begin(); 
									   it != (*ComponentRegistration::s_ComponentRegistryMap).end(); ){
			ComponentRegistration* registration = (it->second);
			
			if (registration) {
				it = (*ComponentRegistration::s_ComponentRegistryMap).erase(it);
			}
			else {
				++it;
			}
		}

		ComponentRegistration::s_ComponentRegistryMap->clear();
		delete ComponentRegistration::s_ComponentRegistryMap;
		ComponentRegistration::s_ComponentRegistryMap = NULL;
	}

}


//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Base Game State

BaseComponent::BaseComponent(const XMLNode& node){
	//get name of component
	m_name = StringToWritableCStr(ReadXMLAttributeAsString(node, "name"));

}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///virtual methods

void BaseComponent::OnCreate(){
	//can specify something on create here
}

//-----------------------------------------------------------------------------------------------------------

void BaseComponent::OnDestroy(){
	//can specify something on destroy here
}

//-----------------------------------------------------------------------------------------------------------

void BaseComponent::ProcessInput(double deltaSeconds){
	UNUSED(deltaSeconds);

}

//-----------------------------------------------------------------------------------------------------------

void BaseComponent::Update(double deltaSeconds){
	UNUSED(deltaSeconds);

}

//-----------------------------------------------------------------------------------------------------------

void BaseComponent::Render(OpenGLRenderer* renderer){
	UNUSED(renderer);

}

//===========================================================================================================