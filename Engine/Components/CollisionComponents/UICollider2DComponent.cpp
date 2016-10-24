//==============================================================================================================
//UICollider2DComponent.cpp
//by Albert Chen Oct-19-2015.
//==============================================================================================================

#include "UICollider2DComponent.hpp"
#include "..\..\Input\InputSystem.hpp"

ComponentRegistration UICollider2DComponent::s_UICollider2DComponentRegistration("Collider", &UICollider2DComponent::CreateComponent);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Collider Component

UICollider2DComponent::UICollider2DComponent(const XMLNode& node): Collider2DComponent(node){

}

UICollider2DComponent::UICollider2DComponent(const std::string& name) : Collider2DComponent(name){

}

//-----------------------------------------------------------------------------------------------------------

void UICollider2DComponent::OnCreate(){
	//can specify something on create here
}

//-----------------------------------------------------------------------------------------------------------

void UICollider2DComponent::OnDestroy(){
	//can specify something on destroy here
}

//-----------------------------------------------------------------------------------------------------------

void UICollider2DComponent::Update(double deltaSeconds){
	UNUSED(deltaSeconds);
	


}

///----------------------------------------------------------------------------------------------------------
///helpers

bool UICollider2DComponent::OnHover() {

	if (theInputSystem) {
		AABB2 boxCollider = GetBox2DCollider();
		return theInputSystem->IsMouseHoverOnAABB2(boxCollider);
	}

	return false;

}

//-----------------------------------------------------------------------------------------------------------

bool UICollider2DComponent::OnHeld(MouseButtonID mouseButtonClicked) {
	if (theInputSystem->IsMouseButtonDown(mouseButtonClicked)) {
		return true;
	}
	return false;
}


//-----------------------------------------------------------------------------------------------------------

bool UICollider2DComponent::OnClick(MouseButtonID mouseButtonClicked) {

	if (theInputSystem->WasMouseButtonJustReleased(mouseButtonClicked)) {
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------------------------------

bool UICollider2DComponent::OnLeftClick() {
	return OnClick(MOUSE_BUTTON_LEFT);
}


bool UICollider2DComponent::OnRightClick() {
	return OnClick(MOUSE_BUTTON_RIGHT);
}

//===========================================================================================================


