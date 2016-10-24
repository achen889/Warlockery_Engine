//==============================================================================================================
//Widget2D.cpp
//by Albert Chen Apr-21-2016.
//==============================================================================================================

#include "BaseWidget.hpp"
#include "..\UISystem.hpp"
#include "..\WidgetStyleFactory.hpp"

//===========================================================================================================

WidgetRegistry* WidgetRegistration::s_WidgetRegistryMap;

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///constructors

BaseWidget::BaseWidget(){

}

BaseWidget::BaseWidget(const std::string& name): 
	m_name(name){

}

BaseWidget::BaseWidget(const XMLNode& node) : 
	m_xmlNode(node){


}

BaseWidget::BaseWidget(NamedProperties& creationArgs):
	m_widgetDefaultProperties(creationArgs)
{
	


}

//-----------------------------------------------------------------------------------------------------------

void BaseWidget::CreateBlankWidgetStateProperties() {
	for (int i = 0; i < NUM_WIDGET_STATES; i++) {
		WidgetStateID stateID = (WidgetStateID)i;

		CreateWidgetStateProperties(stateID);
	}
}

//-----------------------------------------------------------------------------------------------------------

void BaseWidget::LoadBaseAttributesFromXML(const XMLNode& node) {
	//initialize widget state
	CreateBlankWidgetStateProperties();

	//set name
	std::string widgetName = ReadXMLAttributeAsString(node, "name", "");
	m_name = widgetName;

	m_renderComponent = Render2DComponent(m_name);
	m_edgeRenderComponent = Render2DComponent(m_name);

	std::string widgetType = ReadXMLAttributeAsString(node, "type", "");
	m_type = widgetType;

	Vector2 widgetDimensions = ReadXMLAttributeAsVec2(node, "dimensions", Vector2(256.0f, 64.0f));
	m_widgetDefaultProperties.Set("dimensions", widgetDimensions);

	std::string colorSchemeName = ReadXMLAttributeAsString(node, "colorScheme", "");
	m_colorScheme = colorSchemeName;

	std::string widgetStyleName = ReadXMLAttributeAsString(node, "style", "");
	m_style = widgetStyleName;
	if (m_style != "") {
		WidgetStyleFactory* myStyleFac = WidgetStyleFactory::FindFactoryByName(m_style);
		if (myStyleFac) {
			m_colorScheme = myStyleFac->GetWidgetStylePrototype()->GetColorSchemeName();
		}
	}
	
	std::string updateEventNameStr = ReadXMLAttributeAsString(node, "updateEvent");
	m_widgetDefaultProperties.Set("updateEvent", updateEventNameStr);

	std::string renderEventNameStr = ReadXMLAttributeAsString(node, "renderEvent");
	m_widgetDefaultProperties.Set("renderEvent", renderEventNameStr);

}

//-----------------------------------------------------------------------------------------------------------

void BaseWidget::LoadWidgetStatesFromXML(const XMLNode& node) {
	//set widget states
	int num = node.nChildNode("WidgetState");
	for (int i = 0; i < num; i++) {
		XMLNode tempNode = node.getChildNode("WidgetState", i);

		CreateMyWidgetStatePropertiesFromXML(tempNode);
	}
}

//-----------------------------------------------------------------------------------------------------------

void BaseWidget::CreateMyWidgetStatePropertiesFromXML(const XMLNode& node) {
	std::string stateName = ReadXMLAttributeAsString(node, "name", "");
	if (stateName == "Default" || stateName == "default") {

		SetDefaultPropertiesFromXML(node);

	}
	else if (stateName == "Disabled") {
		CreateWidgetStateProperties(WIDGET_STATE_DISABLED, node);
	}
	else if (stateName == "Idle") {
		CreateWidgetStateProperties(WIDGET_STATE_IDLE, node);
	}
	else if (stateName == "Highlighted") {
		CreateWidgetStateProperties(WIDGET_STATE_HIGHLIGHTED, node);
	}
	else if (stateName == "Activated") {
		CreateWidgetStateProperties(WIDGET_STATE_ACTIVATED, node);
	}
}


//-----------------------------------------------------------------------------------------------------------

void BaseWidget::SetDefaultPropertiesFromUISystem() {
	if (theUISystem)
		m_widgetDefaultProperties = theUISystem->GetDefaultNamedProperties();
}

//-----------------------------------------------------------------------------------------------------------

void BaseWidget::SetDefaultPropertiesFromXML(const XMLNode& node) {
	if(theUISystem)
		m_widgetDefaultProperties = theUISystem->GetDefaultNamedProperties();

	//base widget properties
	std::string bgColorStr = ReadXMLAttributeAsString(node, "bgColor");
	Rgba bgRgba = Rgba::WHITE;
	bgRgba = theUISystem->GetColorFromColorScheme(m_colorScheme, bgColorStr);
	//change stuff so it uses the specified color scheme

	std::string edgeColorStr = ReadXMLAttributeAsString(node, "edgeColor");
	Rgba edgeRgba = theUISystem->GetColorFromColorScheme(m_colorScheme, edgeColorStr);

	float newOpacity = ReadXMLAttributeAsFloat(node, "opacity", 1.0f);

	float edgeSize = ReadXMLAttributeAsFloat(node, "edgeSize", 4.0f);

	std::string texturePath = ReadXMLAttributeAsString(node, "texturePath", COMMON_TEXTURE_DIFFUSE);
	if (texturePath != COMMON_TEXTURE_DIFFUSE) {
		//ConsolePrintf("\nname: %s: tex: %s col: %s", m_name.c_str(), texturePath.c_str(), bgRgba.ToVector4().ToString().c_str());
	}
	m_widgetDefaultProperties.Set("texturePath", texturePath);

	//set state properties
	m_widgetDefaultProperties.Set("bgColor", bgRgba);
	m_widgetDefaultProperties.Set("edgeColor", edgeRgba);
	m_widgetDefaultProperties.Set("opacity", newOpacity);
	m_widgetDefaultProperties.Set("edgeSize", edgeSize);

}

//-----------------------------------------------------------------------------------------------------------

void BaseWidget::CreateWidgetStateProperties(WidgetStateID stateID) {
	m_properties[stateID] = WidgetStateProperties();
	//match IDs
	m_properties[stateID].m_widgetStateID = stateID;
	//match color schemes
	m_properties[stateID].m_colorScheme = m_colorScheme;

}

//-----------------------------------------------------------------------------------------------------------

void BaseWidget::CreateWidgetStateProperties(WidgetStateID stateID, const XMLNode& node) {

	m_properties[stateID] = WidgetStateProperties(m_type, node, m_colorScheme);
	//match IDs
	m_properties[stateID].m_widgetStateID = stateID;
	


}

//-----------------------------------------------------------------------------------------------------------

NamedProperties* BaseWidget::GetMyStyleDefaultProperties() {
	if (m_style != "") {
		WidgetStyleFactory* styleFactory = WidgetStyleFactory::FindFactoryByName(m_style);
		if (styleFactory) {
			WidgetStyle* prototype = styleFactory->GetWidgetStylePrototype();
			if (prototype) {
				NamedProperties* protoNP = &prototype->GetStyleDefaultProperties();
				return protoNP;
			}
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------------------------------------

void BaseWidget::SetStatePropertiesDefaults() {
	NamedProperties* styleDefaultNP = GetMyStyleDefaultProperties();

	for (int i = 0; i < NUM_WIDGET_STATES; i++) {
		WidgetStateID stateID = (WidgetStateID)i;

		m_properties[stateID].SetMyDefaultProperties(&m_widgetDefaultProperties);

		if (styleDefaultNP) {
			m_properties[stateID].SetMyStyleDefaultProperties(styleDefaultNP);
		}
	}
}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///start up

void BaseWidget::StartUp(bool allocMat, bool allocMesh) {

	//load data using internal node
	LoadBaseAttributesFromXML(m_xmlNode);
	LoadWidgetStatesFromXML(m_xmlNode);

	//set all state defaults
	SetStatePropertiesDefaults();
	
	//set components
	SetRenderComponent(allocMat, allocMesh);
	SetUICollider2DComponent();

}

//-----------------------------------------------------------------------------------------------------------

void BaseWidget::StartUp() {

	//load data using internal node
	LoadBaseAttributesFromXML(m_xmlNode);
	LoadWidgetStatesFromXML(m_xmlNode);

	//set all state defaults
	SetStatePropertiesDefaults();

	//set components
	SetRenderComponent();
	SetUICollider2DComponent();

}

//-----------------------------------------------------------------------------------------------------------

void BaseWidget::SetOffset(const Vector2& offsetVec2){
	m_widgetDefaultProperties.Set("offset", offsetVec2);
}

//-----------------------------------------------------------------------------------------------------------

void BaseWidget::SetDimensions(const Vector2& dimensionsVec2) {
	m_widgetDefaultProperties.Set("dimensions", dimensionsVec2);
}

//-----------------------------------------------------------------------------------------------------------

const Vector2 BaseWidget::GetPosition() {
	Vector2 myParentPosition;
	BaseWidget* myParent = GetParent();
	if (myParent) {	
		Vector2 parentPos = myParent->GetPosition();
		myParentPosition = parentPos;
	}
	else {
		myParentPosition = Vector2::ZERO;
	}

	WidgetStateProperties& currentStateProperties = GetCurrentStateProperties();

	Vector2 myOffset;
	currentStateProperties.GetProperty("offset", myOffset);

	if (myParent) {
		AABB2 parentBounds = myParent->GetRenderBounds();
		myParentPosition = GetPositionForUIAnchorInAABB2(m_anchorID, parentBounds);
	}

	return myParentPosition + myOffset;
}

//-----------------------------------------------------------------------------------------------------------

void BaseWidget::SetRenderComponent() {

	WidgetStateProperties& currentStateProperties = GetCurrentStateProperties();

	Vector2 myRenderPosition = GetPosition();

	float myEdgeSize;
	currentStateProperties.GetProperty("edgeSize", myEdgeSize);

	Vector2 myDimensions;
	currentStateProperties.GetProperty("dimensions", myDimensions);

	Rgba myBGColor;
	currentStateProperties.GetProperty("bgColor", myBGColor);

	Rgba myEdgeColor;
	currentStateProperties.GetProperty("edgeColor", myEdgeColor);

	float myOpacity;
	currentStateProperties.GetProperty("opacity", myOpacity);

	//alter the colors
	myBGColor.a = Rgba::cA(myOpacity);
	myEdgeColor.a = Rgba::cA(myOpacity);

	std::string texturePath = COMMON_TEXTURE_DIFFUSE;
	currentStateProperties.GetProperty("texturePath", texturePath);

	m_renderComponent.SetName(m_name);
	m_renderComponent.SetRenderMesh2D(myRenderPosition, myDimensions, myBGColor,
		texturePath, "basicSampler", false,
		false, false);

	Vector2 edgeDimensions = myDimensions + Vector2(myEdgeSize * 0.5f);

	m_edgeRenderComponent.SetName(m_name + "_Edge");
	m_edgeRenderComponent.SetRenderMesh2D(myRenderPosition, edgeDimensions, myEdgeColor,
		COMMON_TEXTURE_DIFFUSE, "basicSampler", false,
		false, false);

	m_edgeRenderComponent.GetMeshRenderer()->m_mesh->SetDrawMode(GL_LINE_LOOP);

}


//-----------------------------------------------------------------------------------------------------------

void BaseWidget::SetRenderComponent( bool allocMat, bool allocMesh) {

	WidgetStateProperties& currentStateProperties = GetCurrentStateProperties();
	
	Vector2 myRenderPosition = GetPosition();

	float myEdgeSize;
	currentStateProperties.GetProperty("edgeSize", myEdgeSize);
	
	Vector2 myDimensions;
	currentStateProperties.GetProperty("dimensions", myDimensions);
	
	Rgba myBGColor;
	currentStateProperties.GetProperty("bgColor", myBGColor);

	Rgba myEdgeColor;
	currentStateProperties.GetProperty("edgeColor", myEdgeColor);

	float myOpacity;
	currentStateProperties.GetProperty("opacity", myOpacity);

	//alter the colors
	myBGColor.a = Rgba::cA(myOpacity);
	myEdgeColor.a = Rgba::cA(myOpacity);

	std::string texturePath = COMMON_TEXTURE_DIFFUSE;
	currentStateProperties.GetProperty("texturePath", texturePath);

	m_renderComponent.SetName(m_name);
	m_renderComponent.SetRenderMesh2D(myRenderPosition, myDimensions, myBGColor,
		texturePath, "basicSampler", false,
		allocMat, allocMesh);

	Vector2 edgeDimensions = myDimensions + Vector2(myEdgeSize * 0.5f);

	m_edgeRenderComponent.SetName(m_name+"_Edge");
	m_edgeRenderComponent.SetRenderMesh2D(myRenderPosition, edgeDimensions, myEdgeColor,
		COMMON_TEXTURE_DIFFUSE, "basicSampler", false,
		allocMat, allocMesh);

	m_edgeRenderComponent.GetMeshRenderer()->m_mesh->SetDrawMode(GL_LINE_LOOP);

}

//-----------------------------------------------------------------------------------------------------------

void BaseWidget::SetUICollider2DComponent() {

	WidgetStateProperties& currentStateProperties = GetCurrentStateProperties();

	Vector2 myRenderPosition = GetPosition();
	
	Vector2 myDimensions;
	currentStateProperties.GetProperty("dimensions", myDimensions);

	m_UIColliderComponent.SetPosition(myRenderPosition);
	m_UIColliderComponent.SetRadius(myDimensions * 0.5f);

}

///----------------------------------------------------------------------------------------------------------
///upkeep 

void BaseWidget::ProcessInput(double deltaSeconds) {
	UNUSED(deltaSeconds);

	if (m_UIColliderComponent.OnHover()) {
		SetState(WIDGET_STATE_HIGHLIGHTED);
		if (m_UIColliderComponent.OnLeftClick()) {
			SetState(WIDGET_STATE_ACTIVATED);
		}
	}
	else {
		SetState(WIDGET_STATE_IDLE);
	}
}

//-----------------------------------------------------------------------------------------------------------


void BaseWidget::UpdateRenderComponent(double deltaSeconds) {

	UNUSED(deltaSeconds);

	WidgetStateProperties& currentStateProperties = GetCurrentStateProperties();
	
	Vector2 myRenderPosition = GetPosition();

	float myEdgeSize;
	currentStateProperties.GetProperty("edgeSize", myEdgeSize);

	Vector2 myDimensions;
	currentStateProperties.GetProperty("dimensions", myDimensions);

	Rgba myBGColor;
	currentStateProperties.GetProperty("bgColor", myBGColor);

	Rgba myEdgeColor;
	currentStateProperties.GetProperty("edgeColor", myEdgeColor);

	float myOpacity;
	currentStateProperties.GetProperty("opacity", myOpacity);

	//alter the colors
	myBGColor.a = Rgba::cA(myOpacity);
	myEdgeColor.a = Rgba::cA(myOpacity);

	m_renderComponent.UpdateMesh(myRenderPosition, myDimensions, myBGColor, false);

	Vector2 edgeDimensions = myDimensions + Vector2(myEdgeSize * 0.5f);
	m_edgeRenderComponent.UpdateMesh(myRenderPosition, edgeDimensions, myEdgeColor, false);

	m_edgeRenderComponent.GetMeshRenderer()->m_mesh->SetDrawMode(GL_LINE_LOOP);
}

//-----------------------------------------------------------------------------------------------------------

void BaseWidget::Update(double deltaSeconds) {
	
	//consider, not updating at all if hidden
	
	WidgetStateProperties& currentStateProperties = GetCurrentStateProperties();

	//calls this widget's update event
	std::string updateEventNameStr = "";
	currentStateProperties.GetProperty("updateEvent", updateEventNameStr);
	if (updateEventNameStr != "") {
		NamedProperties updateEventParams;
		updateEventParams.Set("widgetInstance", this);
		updateEventParams.Set("deltaSeconds", deltaSeconds);
		EventSystem::GetInstance().FireEvent(updateEventNameStr, updateEventParams);
	}

	ProcessInput(deltaSeconds);
	
	currentStateProperties.Update(deltaSeconds);

	//optimize: only update if it is rendering, or if it is dirty
	if (!m_isHidden) {
		UpdateRenderComponent(deltaSeconds);
		SetUICollider2DComponent();
	}
}

//-----------------------------------------------------------------------------------------------------------


///----------------------------------------------------------------------------------------------------------
///render

void BaseWidget::Render(OpenGLRenderer* renderer) {
	
	if (m_isHidden)
		return;
	
	WidgetStateProperties& currentStateProperties = GetCurrentStateProperties();

	std::string renderEventNameStr = "";
	currentStateProperties.GetProperty("renderEvent", renderEventNameStr);
	if (renderEventNameStr != "") {
		NamedProperties renderEventParams;
		renderEventParams.Set("widgetInstance", this);
		EventSystem::GetInstance().FireEvent(renderEventNameStr, renderEventParams);
	}

	float myEdgeSize;
	currentStateProperties.GetProperty("edgeSize", myEdgeSize);

	renderer->SetModelLineWidth((int)myEdgeSize);

	//basic render component
	m_edgeRenderComponent.Render2D(renderer);

	m_renderComponent.Render2D(renderer);

	//draw debug collider
	//renderer->DrawMeshQuad(m_UIColliderComponent.GetBox2DCollider(), Rgba::GREEN);
}


///----------------------------------------------------------------------------------------------------------
///friend methods

WidgetRegistration* FindWidgetByName(const std::string& name) {
	WidgetRegistryIterator myRegistration = WidgetRegistration::s_WidgetRegistryMap->find(name);

	if (myRegistration != WidgetRegistration::s_WidgetRegistryMap->end()) {
		return (myRegistration->second);
	}

	return NULL;
}

//-----------------------------------------------------------------------------------------------------------

void ClearWidgetRegistry() {
	if (WidgetRegistration::s_WidgetRegistryMap != NULL) {

		WidgetRegistry& widgetRegistryMap = (*WidgetRegistration::s_WidgetRegistryMap);

		for (WidgetRegistryIterator it = widgetRegistryMap.begin(); it != widgetRegistryMap.end(); ) {
			WidgetRegistration* registration = (it->second);

			if (registration) {

// 				if (registration->m_baseWidgetHandle) {
// 					delete registration->m_baseWidgetHandle;
// 					registration->m_baseWidgetHandle = NULL;
// 				}

				//delete registration;
				//registration = NULL;

				it = widgetRegistryMap.erase(it);
			}
			else {
				++it;
			}

		}//end of for

		widgetRegistryMap.clear();
		delete WidgetRegistration::s_WidgetRegistryMap;
		WidgetRegistration::s_WidgetRegistryMap = NULL;
	}
}


//===========================================================================================================