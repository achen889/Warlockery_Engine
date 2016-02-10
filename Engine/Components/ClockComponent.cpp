//==============================================================================================================
//ClockComponent.cpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#include "ClockComponent.hpp"

ComponentRegistration ClockComponent::s_ClockComponentRegistration("Clock", &ClockComponent::CreateComponent);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Clock Component

ClockComponent::ClockComponent(const XMLNode& node): BaseComponent(node){

}

//-----------------------------------------------------------------------------------------------------------

ClockComponent::ClockComponent(const std::string& name) : BaseComponent(name){

}

//-----------------------------------------------------------------------------------------------------------

void ClockComponent::OnCreate(){
	//can specify something on create here
}

//-----------------------------------------------------------------------------------------------------------

void ClockComponent::OnDestroy(){
	//can specify something on destroy here
}

//-----------------------------------------------------------------------------------------------------------

void ClockComponent::Update(double deltaSeconds){
	UNUSED(deltaSeconds);
	


}

//===========================================================================================================

