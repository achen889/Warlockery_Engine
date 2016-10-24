//==============================================================================================================
//GroupWidget.cpp
//by Albert Chen Apr-24-2016.
//==============================================================================================================

#include "GroupWidget.hpp"
#include "..\UISystem.hpp"
#include "..\UICommon.hpp"

//===========================================================================================================

WidgetRegistration GroupWidget::s_GroupWidgetRegistration("Group", &GroupWidget::CreateWidget, &GroupWidget::CreateWidgetXML);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors

GroupWidget::GroupWidget(){
	//do nothing

}

GroupWidget::GroupWidget(const std::string& name) : BaseWidget(name){
	
}

GroupWidget::GroupWidget(const XMLNode& node) : BaseWidget(node){
	
}

GroupWidget::~GroupWidget(){
	//do nothing

	ClearAllChildren();

}

//-----------------------------------------------------------------------------------------------------------

void GroupWidget::ClearAllChildren() {
	for (Widgets::iterator it = m_children.begin(); it != m_children.end(); ) {
		BaseWidget* widget = (*it);
		if (widget) {
			delete widget;
			widget = NULL;
		}
		it = m_children.erase(it);
	}
}

//-----------------------------------------------------------------------------------------------------------

void GroupWidget::StartUp() {
	BaseWidget::StartUp();

	CreateAllChildren();

	DoResizeToFitChildren();
}

//-----------------------------------------------------------------------------------------------------------

void GroupWidget::DoResizeToFitChildren() {
	if (m_resizeToFitChildren) {
		Vector2 myResizedDimensions;

		AABB2 resizedGroupBounds = ResizeGroupToFitChildren();
		myResizedDimensions = resizedGroupBounds.CalcLengthOfSides();

		Vector2 myResizedOffset = resizedGroupBounds.CalcCenter();

		m_widgetDefaultProperties.Set("offset", myResizedOffset);

		m_widgetDefaultProperties.Set("dimensions", myResizedDimensions);

		//UpdateRenderComponent(0.0);
	}
}

//-----------------------------------------------------------------------------------------------------------

void GroupWidget::LoadBaseAttributesFromXML(const XMLNode& node) {

	BaseWidget::LoadBaseAttributesFromXML(node);

	XMLNode childrenNode = node.getChildNode("Children");

	LoadAllChildDefinitions(childrenNode);
	
}

//-----------------------------------------------------------------------------------------------------------

void GroupWidget::LoadAllChildDefinitions(const XMLNode& node) {

	int num = node.nChildNode("Child");
	for (int i = 0; i < num; i++) {
		XMLNode tempNode = node.getChildNode("Child", i);
		//get name and offset
		std::string childName = ReadXMLAttributeAsString(tempNode, "name");

		Vector2 childOffset = ReadXMLAttributeAsVec2(tempNode, "offset");

		std::string childAnchorStr = ReadXMLAttributeAsString(tempNode, "anchor");
		UIAnchorID childAnchor = GetUIAnchorForString(childAnchorStr);

		std::string childText = ReadXMLAttributeAsString(tempNode, "text");

		m_childDefs.push_back(ChildDefinition(childName, childOffset, childAnchor, childText));
	}

}

//-----------------------------------------------------------------------------------------------------------

void GroupWidget::CreateAllChildren() {	

	for(ChildDefinition& childDef : m_childDefs){

		std::string childName = childDef.name;
		Vector2 childOffset = childDef.offset;
		UIAnchorID childAnchor = childDef.anchorID;
		std::string childText = childDef.text;

		//create a new one and stuff it in m_children
		BaseWidget* childWidget = theUISystem->CreateWidget(childName, childOffset);
		childWidget->SetParent(this);
		childWidget->SetAnchorID(childAnchor);
		//override text attrib
		if (childText != "") {
			WidgetStateProperties& childProp = childWidget->GetCurrentStateProperties();
			if (childProp.m_defaultProperties->HasProperty("text")) {
				childProp.m_defaultProperties->Set("text", childText);
			}
		}

		m_children.push_back(childWidget);
	}

}

//-----------------------------------------------------------------------------------------------------------

//upkeep
void GroupWidget::Update(double deltaSeconds) {

	BaseWidget::Update(deltaSeconds);

	for (BaseWidget* child : m_children) {
		if (child) {
			child->Update(deltaSeconds);
		}   
	}

}

//render
void GroupWidget::Render(OpenGLRenderer* renderer) {

	BaseWidget::Render(renderer);

	if (m_isHidden)
		return;

	for (BaseWidget* child : m_children) {
		if(child)
		   child->Render(renderer);
	}

}

//===========================================================================================================

