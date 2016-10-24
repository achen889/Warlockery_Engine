//==============================================================================================================
//DropDownWidget.cpp
//by Albert Chen May-7-2016.
//==============================================================================================================

#include "DropDownWidget.hpp"
#include "..\UISystem.hpp"
#include "..\..\Input\InputSystem.hpp"

//===========================================================================================================

WidgetRegistration DropDownWidget::s_DropDownWidgetRegistration("DropDown", &DropDownWidget::CreateWidget, &DropDownWidget::CreateWidgetXML);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors

DropDownWidget::DropDownWidget(){
	//do nothing

}

DropDownWidget::DropDownWidget(const std::string& name) : GroupWidget(name){
	
}

DropDownWidget::DropDownWidget(const XMLNode& node) : GroupWidget(node){
	m_resizeToFitChildren = true;
}

DropDownWidget::~DropDownWidget(){
	//do nothing
}

///----------------------------------------------------------------------------------------------------------
///virtual overrides

void DropDownWidget::StartUp() {
	BaseWidget::StartUp();

	CreateAllChildren();

	//DoResizeToFitChildren();

	m_currentOption = m_children[0];
	m_currentOption->ShowWidget();

	//set components
	SetRenderComponent();
	SetUICollider2DComponent();

}

//-----------------------------------------------------------------------------------------------------------

void DropDownWidget::CreateAllChildren() {

	Vector2 currentOffset = Vector2::ZERO;
	BaseWidget* newChild;
	BaseWidget* prevChild = NULL;
	
	AABB2 dropDownBounds = GetRenderBounds();
	
	for (ChildDefinition& childDef : m_childDefs) {

		std::string childName = childDef.name;
		Vector2 childOffset = childDef.offset;
		UIAnchorID childAnchor = UI_ANCHOR_TOP_CENTER;
		std::string childText = childDef.text;

		//create a new one and stuff it in m_children

		//adjust based on prev child
		if (prevChild) {
			float yAdjustment = prevChild->GetRenderBounds().CalcLengthOfSides().y;
			currentOffset.y -= yAdjustment;
		}
		newChild = theUISystem->CreateWidget(childName, currentOffset);
		newChild->SetParent(this);
		newChild->SetAnchorID(childAnchor);

		dropDownBounds.StretchToIncludePoint(newChild->GetPosition());
		
		//override text attrib
		if (childText != "") {
			
			for (int i = 0; i < NUM_WIDGET_STATES; i++) {
				WidgetStateID stateID = (WidgetStateID)i;

				WidgetStateProperties& myStateProperties = newChild->GetWidgetStateProperties(stateID);

				if (myStateProperties.m_defaultProperties->HasProperty("text")) {
					myStateProperties.m_defaultProperties->Set("text", childText);
				}
			}
		}

		newChild->HideWidget();

		m_children.push_back(newChild);

		prevChild = newChild;
	}

	//this stuff with the drop down still doesn't work :(
// 	float boxYpadding = abs(currentOffset.y);
// 
// 	dropDownBounds.AddPadding(0.0f, boxYpadding);

	Vector2 myResizedDimensions = dropDownBounds.CalcLengthOfSides();

	Vector2 myResizedOffset = dropDownBounds.CalcCenter();

	m_widgetDefaultProperties.Set("offset", myResizedOffset);

	m_widgetDefaultProperties.Set("dimensions", myResizedDimensions);

}

void DropDownWidget::SetDefaultPropertiesFromXML(const XMLNode& node) {
	GroupWidget::SetDefaultPropertiesFromXML(node);
}

//-----------------------------------------------------------------------------------------------------------

void DropDownWidget::LoadBaseAttributesFromXML(const XMLNode& node) {
	GroupWidget::LoadBaseAttributesFromXML(node);
}

//-----------------------------------------------------------------------------------------------------------

void DropDownWidget::ProcessInput(double deltaSeconds) {
	
	m_currentOption->ProcessInput(deltaSeconds);

	UICollider2DComponent& currentOptionUICollider = m_currentOption->GetUICollider2DComponent();
	
	if (!m_UIColliderComponent.OnHover()) {
		HideDropDownOptions();
	}
	else {
		if (currentOptionUICollider.OnHover()) {
			ShowDropDownOptions();
		}//end of outer if

		for (BaseWidget* child : m_children) {
			child->ProcessInput(deltaSeconds);

			UICollider2DComponent& childUICollider = child->GetUICollider2DComponent();
			if (childUICollider.OnHover()) {
				if (child->GetUICollider2DComponent().OnClick(MOUSE_BUTTON_LEFT)) {

					m_currentOption = child;

					break;
				}//end of nested if
			}
		}//end of for

	}//end of outer if/else

}

//-----------------------------------------------------------------------------------------------------------

void DropDownWidget::UpdateRenderComponent(double deltaSeconds) {

	if (!m_currentOption)
		return;

	Vector2 currentOffset = Vector2::ZERO;

	m_currentOption->SetOffset(currentOffset);
	m_currentOption->UpdateRenderComponent(deltaSeconds);

	BaseWidget* prevChild = m_currentOption;

	for (BaseWidget* child : m_children) {
		if (child != m_currentOption) {

			//adjust based on prev child
			if (prevChild) {
				float yAdjustment = prevChild->GetRenderBounds().CalcLengthOfSides().y;
				currentOffset.y -= yAdjustment;
			}

			child->SetOffset(currentOffset);
			child->UpdateRenderComponent(deltaSeconds);

			prevChild = child;
		}
	}

	GroupWidget::UpdateRenderComponent(deltaSeconds);

}

//-----------------------------------------------------------------------------------------------------------

//upkeep
void DropDownWidget::Update(double deltaSeconds) {

	GroupWidget::Update(deltaSeconds);

}

//render
void DropDownWidget::Render(OpenGLRenderer* renderer) {

	BaseWidget::Render(renderer);

	m_currentOption->Render(renderer);

	for (BaseWidget* child : m_children) {
		if(child != m_currentOption)
		   child->Render(renderer);
	}

}

//-----------------------------------------------------------------------------------------------------------

void DropDownWidget::ShowDropDownOptions() {
	for (BaseWidget* child : m_children) {
		if(m_currentOption != child)
			child->ShowWidget();
	}
}

//-----------------------------------------------------------------------------------------------------------

void DropDownWidget::HideDropDownOptions() {
	for (BaseWidget* child : m_children) {
		if (m_currentOption != child)
			child->HideWidget();
	}
}


//===========================================================================================================

