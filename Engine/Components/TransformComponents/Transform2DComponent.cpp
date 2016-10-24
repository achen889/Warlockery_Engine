//==============================================================================================================
//Transform2DComponent.cpp
//by Albert Chen Feb-25-2016.
//==============================================================================================================

#include "Transform2DComponent.hpp"

ComponentRegistration Transform2DComponent::s_TransformComponentRegistration("Transform2D", &Transform2DComponent::CreateComponent);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Transform Component

Transform2DComponent::Transform2DComponent(const XMLNode& node): BaseComponent(node){

}

Transform2DComponent::Transform2DComponent(const std::string& name) : BaseComponent(name){
	
}

//-----------------------------------------------------------------------------------------------------------

//use this to get the matrix for stuff
ModelViewMatrix Transform2DComponent::CreateTransformMatrix() {
	//PROFILE_SECTION();

	ModelViewMatrix m_modelViewTransform;

	m_modelViewTransform.SetTranslation2D(m_position);
	m_modelViewTransform.SetScale(m_scale);
	m_modelViewTransform.SetRotationDegrees2D(m_orientation);

	return m_modelViewTransform;
}

//-----------------------------------------------------------------------------------------------------------

void Transform2DComponent::OnCreate(){
	//can specify something on create here
}

//-----------------------------------------------------------------------------------------------------------

void Transform2DComponent::OnDestroy(){
	//can specify something on destroy here
}

//-----------------------------------------------------------------------------------------------------------

void Transform2DComponent::Update(double deltaSeconds){
	UNUSED(deltaSeconds);
	


}

//===========================================================================================================


