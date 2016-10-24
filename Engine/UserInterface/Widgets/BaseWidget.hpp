//==============================================================================================================
//BaseWidget.hpp
//by Albert Chen Apr-21-2016.
//==============================================================================================================

#pragma once

#ifndef _included_BaseWidget__
#define _included_BaseWidget__

#include "Engine/Components/CollisionComponents/UICollider2DComponent.hpp"
#include "Engine/Components/RenderComponents/Render2DComponent.hpp"
#include "../WidgetStates/WidgetStateProperties.hpp"
#include "../UICommon.hpp"

//===========================================================================================================

class WidgetRegistration;
class BaseWidget;

typedef std::map<std::string, WidgetRegistration*> WidgetRegistry;
typedef std::map<std::string, WidgetRegistration*>::iterator WidgetRegistryIterator;
typedef BaseWidget* (WidgetCreationFunc)(const std::string& name);
typedef BaseWidget* (WidgetCreationFuncXML)(const XMLNode& node);

//===========================================================================================================
//self-registering class
class WidgetRegistration {
public:

	~WidgetRegistration() {
		//do nothing
	}

	WidgetRegistration(const std::string& name, WidgetCreationFunc* creationFunction, WidgetCreationFuncXML* creationFunctionXML);

	//registry helpers
	const std::string& GetName() const { return m_name; }
	static WidgetRegistry* GetWidgetRegistry() { return WidgetRegistration::s_WidgetRegistryMap; }
	BaseWidget* CreateWidget() { return (*m_creationFunc)(m_name); }
	BaseWidget* CreateWidget(const XMLNode& node) { 
		m_baseWidgetHandle = (*m_creationFuncXML)(node);
		return m_baseWidgetHandle;
	}

	//friend methods
	friend WidgetRegistration* FindWidgetByName(const std::string& name);

	friend void ClearWidgetRegistry();

private:
	//vars
	WidgetCreationFunc* m_creationFunc;
	WidgetCreationFuncXML* m_creationFuncXML;
	std::string m_name;
	static WidgetRegistry* s_WidgetRegistryMap;
	BaseWidget* m_baseWidgetHandle = NULL;
};

///----------------------------------------------------------------------------------------------------------
///inline methods

//means of registering stuff to the registry
//name must be lowercase
inline WidgetRegistration::WidgetRegistration(const std::string& name, WidgetCreationFunc* creationFunction, WidgetCreationFuncXML* creationFunctionXML) :
	m_name(name),
	m_creationFunc(creationFunction),
	m_creationFuncXML(creationFunctionXML)
{

	//create a new registry
	if (!s_WidgetRegistryMap) {
		s_WidgetRegistryMap = new WidgetRegistry(); //clear this registry at end of game
	}

	//insert into that registry
	s_WidgetRegistryMap->insert(std::pair<std::string, WidgetRegistration*>(name, this));
}



//===========================================================================================================
/*
properties
offset (Vec2) – positional displacement from Parent widget
dimensions(Vec2) – 2D widget dimensions
bgColor (Rgba) – fill color (texture tint) for widget area
edgeColor (Rgba) – edge (line) color for widget AABB2 perimeter
opacity (float) – multiplied by bgColor.alpha and edgeColor.alpha, to fade widget
*/

class BaseWidget : EventSubscriberObject{
public:
	BaseWidget();
	BaseWidget(const std::string& name);
	BaseWidget(const XMLNode& node);
	BaseWidget(NamedProperties& creationArgs);

	virtual ~BaseWidget() {

		//clean up parent ptr
		if(m_parent)
		   m_parent = NULL;

	}

	//start up
	void CreateBlankWidgetStateProperties();
	virtual void LoadBaseAttributesFromXML(const XMLNode& node);

	virtual void LoadWidgetStatesFromXML(const XMLNode& node);
	virtual void CreateMyWidgetStatePropertiesFromXML(const XMLNode& node);

	void SetDefaultPropertiesFromUISystem();

	template<typename T_PROPERTY_TYPE>
	void SetDefaultProperty(const std::string& propertyName, T_PROPERTY_TYPE value);

	virtual void SetDefaultPropertiesFromXML(const XMLNode& node);
	virtual NamedProperties& GetDefaultProperties() {
		return m_widgetDefaultProperties;
	}

	void CreateWidgetStateProperties(WidgetStateID stateID);
	void CreateWidgetStateProperties(WidgetStateID stateID, const XMLNode& node);
	virtual NamedProperties* GetMyStyleDefaultProperties();
	void SetStatePropertiesDefaults();
	virtual WidgetStateProperties& GetWidgetStateProperties(WidgetStateID stateID) {
		return m_properties[stateID];
	}

	//accessors
	virtual BaseWidget* GetParent() { 
		if (m_parent)
			return m_parent;
		else
			return NULL;
	}
	virtual void SetParent(BaseWidget* parent) { m_parent = parent; }
	virtual const std::string GetType() { return m_type; }
	virtual bool IsHidden() { return m_isHidden; }


	virtual const std::string GetName() { return m_name; }
	virtual void SetName(const std::string& name) { 
		m_name = name;
// 		m_renderComponent.SetName(m_name);
// 		m_edgeRenderComponent.SetName(m_name);
	}

	virtual void SetAnchorID(const UIAnchorID& anchorID) { m_anchorID = anchorID; }

	virtual WidgetStateID GetCurrentState() {
		return m_currentState;
	}
	virtual void SetState(WidgetStateID newState) {
		//m_nextState = newState;
		//do a thing

		m_prevState = m_currentState;
		m_currentState = newState;
	}

	virtual WidgetStateProperties& GetCurrentStateProperties() {
		return m_properties[m_currentState];
	}

	//queries
	virtual bool OnHover() {
		return m_UIColliderComponent.OnHover();
	}
	virtual bool OnClick(MouseButtonID mouseID ) {
		return m_UIColliderComponent.OnClick(mouseID);
	}
	virtual bool OnHeld(MouseButtonID mouseID) {
		return m_UIColliderComponent.OnHeld(mouseID);
	}

	virtual void ShowWidget() {
		m_isHidden = false;
	}
	virtual void HideWidget() {
		m_isHidden = true;
	}

	//init
	virtual void StartUp();
	virtual void StartUp(bool allocMat , bool allocMesh );

	virtual void SetRenderComponent();
	virtual void SetRenderComponent(bool allocMat, bool allocMesh);
	virtual void SetUICollider2DComponent();
	
	virtual void SetOffset(const Vector2& offsetVec2);
	virtual void SetDimensions(const Vector2& dimensionsVec2);
	virtual const Vector2 GetPosition();
	virtual const AABB2 GetRenderBounds() {
		return m_renderComponent.GetRenderBounds();
	}

	virtual UICollider2DComponent& GetUICollider2DComponent() {
		return m_UIColliderComponent;
	}

	//upkeep
	virtual void ProcessInput(double deltaSeconds);

	virtual void Update(double deltaSeconds);
	virtual void UpdateRenderComponent(double deltaSeconds);

	//render
	virtual void Render(OpenGLRenderer* renderer);

	//helpers

protected:

	//vars
	std::string m_name;
	std::string m_type;
	std::string m_style;

	std::string m_colorScheme = "MenuColors";

	bool m_isHidden = false;
	UIAnchorID m_anchorID = UI_ANCHOR_INVALID;

	XMLNode m_xmlNode;

	WidgetStateProperties m_properties[NUM_WIDGET_STATES];
	NamedProperties m_widgetDefaultProperties; //defaults for the widget

	//state transition
	WidgetStateID m_currentState;
	WidgetStateID m_prevState;
	WidgetStateID m_nextState;

	//components
	Render2DComponent m_renderComponent;
	Render2DComponent m_edgeRenderComponent;
	UICollider2DComponent m_UIColliderComponent; 

	BaseWidget* m_parent = NULL;

};

typedef std::vector<BaseWidget*> Widgets;
typedef std::vector<BaseWidget*>::iterator WidgetsIterator;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline

template<typename T_PROPERTY_TYPE>
inline void BaseWidget::SetDefaultProperty(const std::string& propertyName, T_PROPERTY_TYPE value) {
	m_widgetDefaultProperties.Set(propertyName, value);
}

//===========================================================================================================

#endif //__includedBaseWidget__

