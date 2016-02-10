//==============================================================================================================
//TransformComponent.hpp
//by Albert Chen Jan-26-2016.
//==============================================================================================================

#pragma once

#ifndef _included_TransformComponent__
#define _included_TransformComponent__

#include "Engine/Components/BaseComponent.hpp"

#include "Engine/Math/MatrixUtils.hpp"
#include "Engine/Math/Math3D.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/EulerAngles.hpp"

class TransformComponent : public BaseComponent{
public:

	TransformComponent() {
		//do nothing
	}
	~TransformComponent();

	
	TransformComponent(const std::string& name);

	TransformComponent(const XMLNode& node);
	
	static BaseComponent* CreateComponent(const std::string& name){ return new TransformComponent(name); }

	static ComponentRegistration s_TransformComponentRegistration;

	void AddChild();

	void SetPosition(const Vector2& position);
	void SetPosition(const Vector3& position);

	void SetScale(const float& scaleUniform ) { m_scale = Vector3(scaleUniform, scaleUniform, scaleUniform); }
	void SetScale(const Vector2& scaleNonUniform) { m_scale = ToVector3(scaleNonUniform); }
	void SetScale(const Vector3& scaleNonUniform) { m_scale = scaleNonUniform; }

	void SetRotationDegrees3D(const EulerAngles& rotationDegrees3D) { m_orientation = rotationDegrees3D;  }

	ModelViewMatrix CreateTransformMatrix();
	
	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Update(double deltaSeconds);

	//lock axes
	//lock coords methods
	void DoLockX(Vector3& vectorToLock) {
		if (lockX) {
			vectorToLock.x = 0.0f;
		}
	}
	void DoLockY(Vector3& vectorToLock) {
		if (lockY) {
			vectorToLock.y = 0.0f;
		}
	}
	void DoLockZ(Vector3& vectorToLock) {
		if (lockZ) {
			vectorToLock.z = 0.0f;
		}
	}
	void DoLockXYZ(Vector3& vectorToLock) {
		DoLockX(vectorToLock);
		DoLockY(vectorToLock);
		DoLockZ(vectorToLock);
	}

	void LockXY() {
		lockX = lockY = true;
	}
	void UnlockXY() {
		lockX = lockY = false;
	}
	void LockZ() {
		lockZ = true;
	}
	void UnlockZ() {
		lockZ = false;
	}
	
protected:
	//vars
	Vector3 m_position;
	Vector3 m_scale;
	EulerAngles m_orientation;

	//lock axes
	bool lockX = false;
	bool lockY = false;
	bool lockZ = false;

	TransformComponent* m_parent;

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline TransformComponent::~TransformComponent(){

}

//-----------------------------------------------------------------------------------------------------------

inline void TransformComponent::SetPosition(const Vector2& position) { 
	m_position = ToVector3(position); 

	DoLockXYZ(m_position);
}

//-----------------------------------------------------------------------------------------------------------

inline void TransformComponent::SetPosition(const Vector3& position) { 
	m_position = position;

	DoLockXYZ(m_position);
}

//-----------------------------------------------------------------------------------------------------------


//===========================================================================================================

#endif