//==============================================================================================================
//Transform2DComponent.MPp
//by Albert Chen Feb-25-2016.
//==============================================================================================================

#pragma once

#ifndef _included_TransformComponent__
#define _included_TransformComponent__

#include "Engine/Components/BaseComponent.hpp"

#include "Engine/Math/MatrixUtils.hpp"
#include "Engine/Math/Math2D.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vector2.hpp"

class Transform2DComponent : public BaseComponent{
public:

	Transform2DComponent() {
		//do nothing
	}
	~Transform2DComponent();

	
	Transform2DComponent(const std::string& name);

	Transform2DComponent(const XMLNode& node);
	
	static BaseComponent* CreateComponent(const std::string& name){ return new Transform2DComponent(name); }

	static ComponentRegistration s_TransformComponentRegistration;

	virtual const Vector2& GetPosition() const { return m_position;  }

	virtual float GetOrientation() { return m_orientation;  }

	virtual void SetPosition(const Vector2& position);
	
	virtual void SetScale(const float& scaleUniform ) { m_scale = Vector2(scaleUniform, scaleUniform); }
	virtual void SetScale(const Vector2& scaleNonUniform) { m_scale = scaleNonUniform; }
	virtual Vector2 GetScale() { return m_scale; }

	virtual void SetOrientationDegrees(const float& degrees) { m_orientation = degrees; }

	virtual ModelViewMatrix CreateTransformMatrix();
	
	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Update(double deltaSeconds);

	//lock axes
	//lock coords methods
	void DoLockX(Vector2& vectorToLock) {	if (lockX) vectorToLock.x = 0.0f; }
	void DoLockY(Vector2& vectorToLock) {	if (lockY)vectorToLock.y = 0.0f; }
	void DoLockXY(Vector2& vectorToLock){
		DoLockX(vectorToLock);
		DoLockY(vectorToLock);
	}
	void LockXY() {	lockX = lockY = true; }
	void UnlockXY() {	lockX = lockY = false; }

protected:
	//vars
	Vector2 m_position = Vector2::ZERO;
	Vector2 m_scale = Vector2::ONE;
	float m_orientation = 0.0f;

	//lock axes
	bool lockX = false;
	bool lockY = false;

	Transform2DComponent* m_parent;

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline Transform2DComponent::~Transform2DComponent(){

}

//-----------------------------------------------------------------------------------------------------------

inline void Transform2DComponent::SetPosition(const Vector2& position) { 
	m_position = position; 

	DoLockXY(m_position);
}

//-----------------------------------------------------------------------------------------------------------


//===========================================================================================================

#endif