//==============================================================================================================
//BaseWidget.cpp
//by Albert Chen Apr-18-2016.
//==============================================================================================================

#include "BaseWidget2D.hpp"

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///constructors

BaseWidget2D::BaseWidget2D(){
	//sets default properties
}

BaseWidget2D::~BaseWidget2D(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///start up

void BaseWidget2D::SetRenderComponent() {

	WidgetStateProperties& currentStateProperties = GetCurrentStateProperties();
	
	Vector2 myOffset;
	currentStateProperties.GetProperty("offset", myOffset);
	
	Vector2 myDimensions;
	currentStateProperties.GetProperty("dimensions", myDimensions);
	
	Rgba myBGColor;
	currentStateProperties.GetProperty("bgcolor", myBGColor);

	Rgba myEdgeColor;
	currentStateProperties.GetProperty("edgecolor", myEdgeColor);

	float myOpacity;
	currentStateProperties.GetProperty("opacity", myOpacity);

	m_renderComponent.SetRenderMesh2D(myOffset, myDimensions, myBGColor,
			COMMON_TEXTURE_DIFFUSE, "basicSampler",
			false);


}

//-----------------------------------------------------------------------------------------------------------

void BaseWidget2D::SetUICollider2DComponent() {

	WidgetStateProperties& currentStateProperties = GetCurrentStateProperties();

	Vector2 myOffset;
	currentStateProperties.GetProperty("offset", myOffset);
	
	Vector2 myDimensions;
	currentStateProperties.GetProperty("dimensions", myDimensions);

	m_UIColliderComponent.SetPosition(myOffset);
	m_UIColliderComponent.SetRadius(myDimensions * 0.5f);

}


///----------------------------------------------------------------------------------------------------------
///upkeep 

void BaseWidget2D::ProcessInput(double deltaSeconds) {
	UNUSED(deltaSeconds);

	if (m_UIColliderComponent.OnHover()) {
		SetState(WIDGET_STATE_HIGHLIGHTED);
		if (m_UIColliderComponent.OnLeftClick()) {
			SetState(WIDGET_STATE_ACTIVATED);
		}
	}

}

void BaseWidget2D::Update(double deltaSeconds) {

	ProcessInput(deltaSeconds);
	
	//do nothing
	WidgetStateProperties& currentStateProperties = GetCurrentStateProperties();
	currentStateProperties.Update(deltaSeconds);

	//optimize: only update if it is dirty
	SetRenderComponent();
	SetUICollider2DComponent();

}


///----------------------------------------------------------------------------------------------------------
///render

void BaseWidget2D::Render(OpenGLRenderer* renderer) {

	//basic render component
	m_renderComponent.Render2D(renderer);

}



//===========================================================================================================

