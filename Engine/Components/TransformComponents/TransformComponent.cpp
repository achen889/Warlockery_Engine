//==============================================================================================================
//TransformComponent.cpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#include "TransformComponent.hpp"

ComponentRegistration TransformComponent::s_TransformComponentRegistration("Transform", &TransformComponent::CreateComponent);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Transform Component

TransformComponent::TransformComponent(const XMLNode& node): BaseComponent(node){

}

TransformComponent::TransformComponent(const std::string& name) : BaseComponent(name){

}

//-----------------------------------------------------------------------------------------------------------

//use this to get the matrix for stuff
ModelViewMatrix TransformComponent::CreateTransformMatrix() {
	ModelViewMatrix m_modelViewTransform;

	m_modelViewTransform.SetTranslation3D(m_position);
	m_modelViewTransform.SetScale(m_scale);
	m_modelViewTransform.SetRotationDegrees3D(m_orientation);

	return m_modelViewTransform;
}

//-----------------------------------------------------------------------------------------------------------

void TransformComponent::OnCreate(){
	//can specify something on create here
}

//-----------------------------------------------------------------------------------------------------------

void TransformComponent::OnDestroy(){
	//can specify something on destroy here
}

//-----------------------------------------------------------------------------------------------------------

void TransformComponent::Update(double deltaSeconds){
	UNUSED(deltaSeconds);
	


}

//===========================================================================================================


