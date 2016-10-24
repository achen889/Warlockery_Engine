//==============================================================================================================
//GroupWidget.hpp
//by Albert Chen Apr-24-2016.
//==============================================================================================================

#pragma once

#ifndef _included_GroupWidget__
#define _included_GroupWidget__

#include "BaseWidget.hpp"

//===========================================================================================================

struct ChildDefinition {
	std::string name;
	Vector2 offset;
	UIAnchorID anchorID;
	std::string text;

	ChildDefinition(const std::string& newName, const Vector2& newOffset, const UIAnchorID& newAnchorID, const std::string& newText):
		name(newName),
		offset(newOffset),
		anchorID(newAnchorID),
		text(newText)
	{

	}
};

typedef std::vector<ChildDefinition> ChildDefinitions;

//===========================================================================================================

class GroupWidget : public BaseWidget {
public:
	//constructors
	GroupWidget();
	GroupWidget(const std::string& name);
	GroupWidget(const XMLNode& node);
	virtual ~GroupWidget();


	//virtual overrides
	virtual void StartUp() override;
	void ClearAllChildren();

	virtual void DoResizeToFitChildren();

	virtual void LoadBaseAttributesFromXML(const XMLNode& node) override;


	virtual void ShowWidget() {
		m_isHidden = false;
		for (BaseWidget* child : m_children) {
			if (child)
				child->ShowWidget();
		}
	}
	virtual void HideWidget() {
		m_isHidden = true;
		for (BaseWidget* child : m_children) {
			if(child)
				child->HideWidget();
		}
	}

	//upkeep
	virtual void Update(double deltaSeconds);

	//render
	virtual void Render(OpenGLRenderer* renderer);

	//helpers
	virtual void LoadAllChildDefinitions(const XMLNode& node);
	virtual void CreateAllChildren();
	virtual void AddChild(BaseWidget* childWidget);

	virtual void RemoveChild(const std::string& widgetName);

	BaseWidget* GetChild(const std::string& widgetName);
	void HideChild(const std::string& widgetName);
	void ShowChild(const std::string& widgetName);

	virtual const AABB2 ResizeGroupToFitChildren() {
		AABB2 myBounds = GetRenderBounds();
		
		for (BaseWidget* child : m_children) {
			Vector2 myChildPos = child->GetPosition();
			myBounds.StretchToIncludePoint(myChildPos);
		}

		return myBounds;
	}

	//self register
	static BaseWidget* CreateWidget(const std::string& name) { return new GroupWidget(name); }
	static BaseWidget* CreateWidgetXML(const XMLNode& node) { return new GroupWidget(node); }
	static WidgetRegistration s_GroupWidgetRegistration;

protected:
	//vars
	Widgets m_children;
	ChildDefinitions m_childDefs;
	bool m_resizeToFitChildren = false;

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods


inline void GroupWidget::AddChild(BaseWidget* childWidget) {
	if (childWidget) {
		m_children.push_back(childWidget);
		m_children.back()->SetParent(this);
	}
}

//-----------------------------------------------------------------------------------------------------------

inline void GroupWidget::RemoveChild(const std::string& widgetName) {
	for (WidgetsIterator it = m_children.begin(); it != m_children.end();  ){
		BaseWidget*& child = (*it);
		if (child) {
			if (child->GetName() == widgetName) {
				child->SetParent(NULL);

				delete child;
				child = NULL;

				it = m_children.erase(it);

				break;
			}
			else {
				++it;
			}
		}
		else {
			++it;
		}
	}
}

//-----------------------------------------------------------------------------------------------------------


inline BaseWidget* GroupWidget::GetChild(const std::string& widgetName) {
	for (BaseWidget* widget : m_children) {
		if (widget && widget->GetName() == widgetName) {
			return widget;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------------------------------------

inline void GroupWidget::HideChild(const std::string& widgetName) {
	for (BaseWidget* widget : m_children) {
		if (widget && widget->GetName() == widgetName) {
			widget->HideWidget();
			break;
		}
	}
}

//-----------------------------------------------------------------------------------------------------------

inline void GroupWidget::ShowChild(const std::string& widgetName) {
	for (BaseWidget* widget : m_children) {
		if (widget && widget->GetName() == widgetName) {
			widget->ShowWidget();
			break;
		}
	}
}


//===========================================================================================================

#endif //__includedGroupWidget__

